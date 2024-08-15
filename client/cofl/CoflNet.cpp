//
// Created by root on 7/31/24.
//

#include "CoflNet.h"

static std::string session_id = "";

using json = nlohmann::json;

std::map<CommandType, std::string> CommandTypeHelper::data;

void CoflNet::setupWebsocket() {
    CommandTypeHelper::initialize();
    Objects::coflWebSocket.setUrl(
            "wss://sky.coflnet.com/modsocket?version=1.5.6-Alpha&player=" + Objects::currentUsername + "&SId=" +
            GetCoflSession(Objects::currentUsername).id);

    Objects::coflWebSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr &msg) {
        switch (msg->type) {
            case ix::WebSocketMessageType::Open:
                break;
            case ix::WebSocketMessageType::Close:
                std::cout << Colors::Red << "Closed!" << Colors::End;
                break;
            case ix::WebSocketMessageType::Error:
                std::cout << Colors::Red << "Error!" << Colors::End;
                break;
            case ix::WebSocketMessageType::Message:
                if (Objects::debug) {
                    std::cout << Colors::Black << "Received: " << msg->str << Colors::End;
                }

                json j = json::parse(msg->str);

                std::string type = j["type"];
                std::string data = j["data"];

                JsonStringCommand cmd(type, data);

                if (Objects::debug) {
                    std::cout << Colors::Black << "Cofl Handling Command=" << cmd.toString() << Colors::End;
                }

                switch (cmd.getType()) {
                    case CommandType::WriteToChat:
                        std::cout << Colors::WhiteBackground << cmd.GetAs<ChatMessageData>().getData().Text
                                  << Colors::End;
                        break;
                    case CommandType::Execute: {
                        std::string msg = cmd.GetAs<std::string>().getData();
                        if (Objects::debug) {
                            std::cout << Colors::Black << "Sending Message - " << msg << Colors::End;
                        }

                        if (msg.starts_with("/cofl ")) {
                            size_t type_start = msg.find(' ') + 1;
                            size_t type_end = msg.find(' ', type_start);

                            RawCommand rc(msg.substr(type_start, type_end - type_start), msg.substr(type_end + 1));
                            json cmd;

                            cmd["Type"] = rc.getType();
                            cmd["Data"] = rc.getData();

                            Objects::coflWebSocket.send(cmd.dump());
                        } else {
                            mc::protocol::packets::out::ChatPacket packet(msg);
                            Objects::m_Connection->SendPacket(&packet);
                        }
                        break;
                    }
                    case CommandType::ChatMessage:
                        if (Objects::debug) {
                            std::cout << Colors::Black << "Adding Message - ";
                            for (const ChatMessageData &wcmd: cmd.GetAs<std::vector<ChatMessageData>>().getData()) {
                                std::cout << wcmd.Text;
                                if (!wcmd.OnClick.empty()) {
                                    std::cout << " onClick: " << wcmd.OnClick;
                                }
                                if (!wcmd.Hover.empty()) {
                                    std::cout << " hover: " << wcmd.Hover;
                                }
                                if (wcmd.Text.find("What do you want to do?") != std::string::npos) {
                                    RawCommand rc("flip", "true");
                                    json cmd;

                                    cmd["Type"] = rc.getType();
                                    cmd["Data"] = rc.getData();

                                    Objects::coflWebSocket.send(cmd.dump());
                                }
                            }
                            std::cout << Colors::End;
                        } else {
                            std::cout << Colors::WhiteBackground;
                            for (const ChatMessageData &wcmd: cmd.GetAs<std::vector<ChatMessageData>>().getData()) {
                                std::cout << wcmd.Text;

                                if (wcmd.Text.find("What do you want to do?") != std::string::npos) {
                                    RawCommand rc("flip", "true");
                                    json cmd;

                                    cmd["Type"] = rc.getType();
                                    cmd["Data"] = rc.getData();

                                    Objects::coflWebSocket.send(cmd.dump());
                                } else if (wcmd.Text.contains("so we can load your settings")) {
                                    if (!wcmd.OnClick.empty()) {
                                        std::cout << " onClick: " << wcmd.OnClick;
                                    }
                                }
                            }
                            std::cout << Colors::End;
                        }
                        break;
                    case CommandType::Flip: {
                        FlipData flip = cmd.GetAs<FlipData>().getData();
                        FlipItem item = FlipItem();
                        item.auctionId = flip.Id;
                        item.coflWorth = flip.Target;
                        //item.uuid = flip.auctionData.uuid;
                        //item.sellerUuid = flip.auctionData.sellerUuid;
                        //item.skyblockId = flip.auctionData.skyblockId;
                        item.finder = flip.Finder;
                        item.bed = false;

                        if (Objects::debug) {
                            std::cout << Colors::Cyan << "New Flip! Target - " << flip.Target << Colors::End;
                        }
                        AutoOpen::OpenAuction(item);
                        break;
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

                        RawCommand rc("foundMods", modListData.dump());
                        json cmd;

                        cmd["Type"] = rc.getType();
                        cmd["Data"] = rc.getData();

                        std::cout << cmd.dump() << Colors::End;

                        Objects::coflWebSocket.send(cmd.dump());
                        break;
                }

                break;
        }
    });

    Objects::coflWebSocket.start();
}