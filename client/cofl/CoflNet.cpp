//
// Created by root on 7/31/24.
//

#include "CoflNet.h"

static std::string session_id = "";

using json = nlohmann::json;

std::map<CommandType, std::string> CommandTypeHelper::data;

void CoflNet::setupWebsocket() {
    CommandTypeHelper::initialize();
    Objects::coflWebSocket.setUrl((Objects::getUsSocket() ? "ws://sky-us" : "wss://sky") +
                                  std::string(".coflnet.com/modsocket?version=") +
                                  (Objects::getBafSocket() ? "1.5.0-af" : "1.5.6-Alpha") +
                                  "&player=" + Objects::getCurrentUsername() +
                                  "&SId=" + GetCoflSession(Objects::getCurrentUsername()).id
    );

    Objects::coflWebSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr &msg) {
        switch (msg->type) {
            case ix::WebSocketMessageType::Open:
                break;
            case ix::WebSocketMessageType::Close:
                Logger::log(ColorConfig::Disconnection + "Closed!" + Colors::End);
                break;
            case ix::WebSocketMessageType::Error:
                Logger::log(ColorConfig::Error + "Error!" + Colors::End);
                break;
            case ix::WebSocketMessageType::Message:
                if (Objects::getDebug()) {
                    Logger::log(ColorConfig::Debug + "Received: " + msg->str + Colors::End);
                }

                json j = json::parse(msg->str);

                std::string type = j["type"];
                std::string data = j["data"];

                JsonStringCommand cmd(type, data);

                if (Objects::getDebug()) {
                    Logger::log(ColorConfig::Debug + "Cofl Handling Command=" + cmd.toString() + Colors::End);
                }

                switch (cmd.getType()) {
                    case CommandType::WriteToChat: {
                        std::string text = cmd.GetAs<ChatMessageData>().getData().Text;
                        Logger::log(ColorConfig::CoflMessage + text + Colors::End);

                        // Captcha

                        if (text.contains("Click to get a letter captcha to prove you are not.") &&
                            !text.contains("You are currently delayed for likely being afk")) {
                            Objects::sendRawCommand("captcha", "vertical");
                        } else if (text.contains("Thanks for confirming that you are a real user")) {
                            Objects::sendToWebsocket("CaptchaSuccess", "");
                        } else if (text.contains("You solved the captcha, but you failed too many previously")) {
                            Objects::sendToWebsocket("CaptchaCorrect", "");
                        } else if (text.contains("Your answer was not correct")) {
                            Objects::sendToWebsocket("CaptchaIncorrect", "");
                        }

                        // Stats

                        if (text.contains("You are currently delayed by ")) {
                            Stats::coflDelay = Colors::stripColorCodes(text).substr(40).substr(0,Colors::stripColorCodes(text).substr(40).find(' '));
                        } else if (text.contains("You are currently not delayed at all")) {
                            Stats::coflDelay = "No Delay";
                        }
                    }
                        break;
                    case CommandType::Execute: {
                        std::string msg = cmd.GetAs<std::string>().getData();
                        msg = Colors::stripColorCodes(msg);
                        if (Objects::getDebug()) {
                            Logger::log(ColorConfig::Debug + "Sending Message - " + msg + Colors::End);
                        }

                        if (msg.starts_with("/cofl ")) {
                            size_t type_start = msg.find(' ') + 1;
                            size_t type_end = msg.find(' ', type_start);

                            Objects::sendRawCommand(msg.substr(type_start, type_end - type_start),
                                                    msg.substr(type_end + 1));
                        } else {
                            mc::protocol::packets::out::ChatPacket packet(msg);
                            Objects::m_Connection->SendPacket(&packet);
                        }
                        break;
                    }
                    case CommandType::ChatMessage: {
                        if (Objects::getDebug()) {
                            Logger::log(ColorConfig::Debug);
                        } else {
                            Logger::log(ColorConfig::CoflMessage);
                        }

                        for (const ChatMessageData &wcmd: cmd.GetAs<std::vector<ChatMessageData>>().getData()) {
                            Logger::log(Colors::stripColorCodes(wcmd.Text));

                            if (Objects::getDebug()) {
                                if (!wcmd.OnClick.empty()) {
                                    Logger::log(" onClick: " + wcmd.OnClick);
                                }
                                if (!wcmd.Hover.empty()) {
                                    Logger::log(" hover: " + wcmd.Hover);
                                }
                            }

                            if (wcmd.Text.find("What do you want to do?") != std::string::npos) {
                                Objects::sendRawCommand("flip", "true");
                            } else if (wcmd.Text.contains("so we can load your settings")) {
                                if (!wcmd.OnClick.empty()) {
                                    Logger::log(" onClick: " + wcmd.OnClick);
                                }
                            } else if (wcmd.Text.contains("Your settings could not be loaded, please relink again")) {
                                Objects::sendRawCommand("logout", "");

                                Objects::sendRawCommand("start", "");
                            } else if (wcmd.Text.contains("Your Ping to execute SkyCofl commands is: ")) {
                                Stats::coflPing = Colors::stripColorCodes(wcmd.Text).substr(42);
                            }
                        }
                        Logger::log(Colors::End);

                        // Captcha
                        std::string strippedData = data.substr(1, data.length() - 2);
                        size_t pos = 0;
                        while ((pos = strippedData.find("\\\"", pos)) != std::string::npos) {
                            strippedData.replace(pos, 2, "\"");
                            pos += 1;
                        }

                        strippedData = Colors::stripColorCodes(strippedData);

                        if (strippedData.contains("/cofl captcha ")) {
                            std::vector<std::string> clickRows;
                            std::vector<std::string> clickColumns;
                            int clickIndex = 0;
                            std::string captchaString;

                            std::vector<std::string> tempColumns;
                            for (const ChatMessageData &wcmd: cmd.GetAs<std::vector<ChatMessageData>>().getData()) {
                                std::string line = wcmd.Text;

                                std::size_t pos = line.find((char) 0xA7);
                                while (pos != std::string::npos) {
                                    line.erase(pos, 2);
                                    pos = line.find((char) 0xA7);
                                }

                                std::regex emoji_pattern("[\U0001F1E7\U0001F1FE]");
                                std::regex section_pattern("§.");

                                line = std::regex_replace(line, emoji_pattern, "");
                                line = std::regex_replace(line, section_pattern, "");

                                line = Objects::replaceAll(line, "\302", "");

                                line = Objects::replaceAll(line, "\\n", "\n");
                                Logger::log(line);

                                captchaString += line;

                                if (!wcmd.OnClick.empty()) {
                                    if (clickRows.size() < clickIndex) {
                                        clickRows.push_back(wcmd.OnClick);
                                    }

                                    for (char16_t character: line) {
                                        int length = 0;

                                        if (character == u'\uFFFD') {
                                            length = 8;
                                        } else if (character == u'⋅') {
                                            length = 4;
                                        } else if (character == u' ') {
                                            length = 8;
                                        }

                                        for (int i = 0; i < length; i++) {
                                            if (!wcmd.OnClick.contains("config")) {
                                                tempColumns.push_back(wcmd.OnClick);
                                            }
                                        }
                                    }
                                }

                                if (wcmd.Text.contains("\n")) {
                                    clickIndex++;

                                    if (tempColumns.size() > clickColumns.size()) {
                                        clickColumns.clear();
                                        clickColumns.insert(clickColumns.end(), tempColumns.begin(), tempColumns.end());
                                    }
                                    tempColumns.clear();
                                }
                            }

                            //Logger::log("Column Clicks Length " + clickColumns.size() + Colors::End);

                            std::map<std::string, int> characters;

                            for (char c: captchaString) {
                                if (characters.find(std::string(1, c)) == characters.end()) {
                                    // FIGURE ME OUT
                                    //characters[std::string(1, c)] = Minecraft::getMinecraft().fontRendererObj.getCharWidth(c);
                                }
                            }

                            nlohmann::json response;

                            std::stringstream jsonStream;
                            jsonStream << "[";

                            for (size_t i = 0; i < clickRows.size(); ++i) {
                                jsonStream << clickRows[i];
                                if (i < clickRows.size() - 1) {
                                    jsonStream << ", ";
                                }
                            }

                            jsonStream << "]";

                            response["captcha"] = captchaString;
                            response["onClicks"] = jsonStream.str();

                            response["clickColumns"] = nlohmann::json::array();
                            for (const auto &column: clickColumns) {
                                response["clickColumns"].push_back(column);
                            }
                            response["lengths"] = nlohmann::json::object();
                            for (const auto &character: characters) {
                                response["lengths"][character.first] = character.second;
                            }

                            Objects::sendToWebsocket("Captcha", response.dump());
                        }

                        break;
                    }
                    case CommandType::Flip: {
                        FlipData flip = cmd.GetAs<FlipData>().getData();
                        FlipItem item = FlipItem();
                        item.auctionId = flip.Id;
                        item.coflWorth = flip.Target;
                        item.uuid = flip.Uuid;
                        item.sellerUuid = flip.SellerUuid;
                        item.skyblockId = flip.SkyblockId;
                        item.finder = flip.Finder;
                        item.bed = false;
                        item.auctionStart = flip.AuctionStart;
                        item.purchaseAt = flip.PurchaseAt;
                        item.displayName = flip.ItemName;
                        item.strippedDisplayName = Colors::stripColorCodes(flip.ItemName);
                        item.buyPrice = flip.StartingBid;

                        try {
                            //item.auctionStart = std::chrono::milliseconds(std::stoll(flip.AuctionStart.count()));
                        } catch (const std::exception &e) {

                        }

                        if (Objects::getDebug()) {
                            Logger::log(ColorConfig::FlipInfo + "New Flip! Item Name - " + item.strippedDisplayName + " Target - " + std::to_string(flip.Target) + Colors::End);
                        }
                        AutoOpen::OpenAuction(item);
                        break;
                    }
                    case CommandType::GetInventory: {
                        json cmd;

                        cmd["Type"] = "uploadInventory";
                        cmd["Data"] = R"({"_events":{},"_eventsCount":0,"id":0,"type":"minecraft:inventory","title":"Inventory","slots":[null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null],"inventoryStart":9,"inventoryEnd":45,"hotbarStart":36,"craftingResultSlot":0,"requiresConfirmation":true,"selectedItem":null})";

                        Objects::coflWebSocket.send(cmd.dump());
                    }
                    case CommandType::set:
                    case CommandType::TokenLogin:
                    case CommandType::Clicked:
                    case CommandType::PlaySound:
                    case CommandType::PurchaseStart:
                    case CommandType::PurchaseConfirm:
                    case CommandType::Reset:
                    case CommandType::PrivacySettings:
                    case CommandType::Countdown:
                    case CommandType::updatePurse:
                    case CommandType::updateBits:
                    case CommandType::updateServer:
                    case CommandType::updateLocation:
                    case CommandType::chatBatch:
                    case CommandType::uploadTab:
                    case CommandType::uploadScoreboard:
                    case CommandType::GetScoreboard:
                    case CommandType::ProxyRequest:
                    case CommandType::Ignoring:
                        break;
                    case CommandType::GetMods:
                        json modListData;

                        std::vector<std::string> fileNames = {"SkyCofl-1.5.6-alpha.jar"};
                        modListData["fileNames"] = fileNames;
                        std::vector<std::string> modNames = {"CoflSky"};
                        modListData["modNames"] = modNames;
                        std::vector<std::string> fileHashes = {
                                "EC918C16437DC0082B1689CBE8E3DD6F4285B6C285687428B548D98C6CCE525F"};
                        modListData["fileHashes"] = fileHashes;

                        Objects::sendRawCommand("foundMods", modListData.dump());
                        break;
                }

                break;
        }
    });

    Objects::coflWebSocket.start();
}