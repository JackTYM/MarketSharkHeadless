#include "Auth.h"
#include "../Server.h"
#include "../cofl/RawCommand.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <iostream>
#include <mclib/common/Json.h>

#include <ColorConfig.h>

using json = nlohmann::json;

void Auth::setupWebsocket() {
    Objects::msWebSocket.setUrl("wss://cofl.jacktym.dev");

    Objects::msWebSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr &msg) {
        switch (msg->type) {
            case ix::WebSocketMessageType::Open:
                if (Objects::msSession.empty()) {
                    Objects::sendToWebsocket("Activating", "");
                } else {
                    Objects::sendToWebsocket("Reconnecting", "");
                }

                break;
            case ix::WebSocketMessageType::Close:
                std::cout << ColorConfig::Disconnection << "Closed connection to MarketShark!" << Colors::End;
                break;
            case ix::WebSocketMessageType::Error:
                std::cout << ColorConfig::Error << "Error in MarketShark!" << Colors::End;
                break;
            case ix::WebSocketMessageType::Message:
                if (Objects::getDebug()) {
                    std::cout << ColorConfig::Debug << "Received: " << msg->str << Colors::End;
                }

                json parsed_json = json::parse(msg->str);

                /*if (parsed_json.contains("username")) {
                    std::string username = parsed_json["username"];
                    if (!username.empty()) {
                        return;
                    }
                }*/

                std::string type = parsed_json["type"];

                if (type == "Activated") {
                    Objects::msSession = parsed_json["session_id"];
                    std::cout << ColorConfig::Debug << parsed_json["message"] << Colors::End;
                    if (Objects::getCurrentUsername() == "") {
                        std::cout << ColorConfig::Important << "Please log in with Microsoft on Discord to proceed!" << Colors::End;
                    }
                    Objects::sendToWebsocket("RequestSession", "");
                } else if (type == "Reconnected") {
                    std::cout << ColorConfig::ServerStatus << parsed_json["message"] << Colors::End;
                    Objects::sendToWebsocket("RequestSession", "");
                } else if (type == "FailedActivation") {
                    std::cout << ColorConfig::Error << parsed_json["message"] << Colors::End;
                } else if (type == "IncorrectSession") {
                    std::cout << ColorConfig::Error << "Incorrect Session!" << Colors::End;
                    Objects::msSession = "";
                    Objects::msWebSocket.close();
                    Objects::msWebSocket.start();
                } else if (type == "UpdateSession") {
                    std::cout << ColorConfig::Debug << "Updating Session!" << Colors::End;
                    Objects::setCurrentUsername(parsed_json["username"]);
                    Objects::currentUUID = parsed_json["uuid"];
                    Objects::currentSSID = parsed_json["ssid"];

                    std::cout << ColorConfig::ServerStatus << "Connecting to Server!" << Colors::End;

                    if (Objects::m_Connection == nullptr) {
                        std::thread([]() {
                            Server::connectToServer();
                        }).detach();
                    }
                } else if (type == "Stats") {

                    Objects::sendRawCommand("ping", "");
                    Objects::sendRawCommand("delay", "");

                    Stats::pingTime = duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                    Objects::m_Connection->SendPacket(mc::protocol::packets::out::ClientStatusPacket(mc::protocol::packets::out::ClientStatusPacket::Action::RequestStats));

                    std::thread([]() {
                        std::this_thread::sleep_for(std::chrono::seconds(5));
                        nlohmann::json response;
                        response["purse"] = Stats::purse;
                        response["island"] = Stats::island;
                        response["visitors"] = 0;
                        response["status"] = true;
                        response["hypixel_ping"] = Stats::hypixelPing;
                        response["cofl_ping"] = Stats::coflPing;
                        response["cofl_delay"] = Stats::coflDelay;
                        response["paused"] = Objects::paused;

                        Objects::sendToWebsocket("Stats", response.dump());
                    }).detach();
                } else if (type == "Settings") {

                } else if (type == "SendChat") {
                    std::string msg = parsed_json["message"];
                    if (msg.starts_with("/cofl ")) {
                        std::string prefix = "/cofl ";
                        std::size_t start = prefix.length();
                        std::size_t end = msg.find(' ', start);

                        std::string var1 = msg.substr(start, end - start);
                        std::string var2 = end != std::string::npos ? msg.substr(end + 1) : "";

                        Objects::sendRawCommand(var1, var2);
                    } else {
                        mc::protocol::packets::out::ChatPacket packet(msg);
                        Objects::m_Connection->SendPacket(&packet);
                    }

                    std::thread([index = ChatLogger::chatList.size(), message = msg]() {
                        std::this_thread::sleep_for(std::chrono::seconds(10));

                        auto it = ChatLogger::chatList.begin();
                        std::advance(it, index);
                        std::list<std::string> sublist(it, ChatLogger::chatList.end());

                        nlohmann::json messages = nlohmann::json::array();

                        for (const auto& item : sublist) {
                            messages.push_back(item);
                        }

                        nlohmann::json response;
                        response["chat"] = message;
                        response["messages"] = messages.dump();

                        Objects::sendToWebsocket("ChatResponses", response.dump());
                    }).detach();
                } else if (type == "Chat") {
                    int amount = parsed_json["amount"];
                    auto totalMessages = ChatLogger::chatList.size();
                    auto startIndex = (amount >= totalMessages) ? 0 : totalMessages - amount;

                    auto it = ChatLogger::chatList.begin();
                    std::advance(it, startIndex);

                    std::list<std::string> sublist(it, ChatLogger::chatList.end());

                    nlohmann::json messages = nlohmann::json::array();

                    for (const auto& item : sublist) {
                        messages.push_back(item);
                    }

                    nlohmann::json response;
                    response["messages"] = messages.dump();

                    Objects::sendToWebsocket("ChatMessages", response.dump());
                } else if (type == "Inventory") {

                } else if (type == "AuctionHouse") {

                } else if (type == "Captcha") {
                    Objects::sendRawCommand("captcha", "vertical");
                } else if (type == "HorizontalCaptcha") {
                    Objects::sendRawCommand("captcha", "optifine");
                } else if (type == "CaptchaSolve") {
                    std::string msg = parsed_json["message"];
                    size_t type_start = msg.find(' ') + 1;
                    size_t type_end = msg.find(' ', type_start);

                    Objects::sendRawCommand(msg.substr(type_start, type_end - type_start), msg.substr(type_end + 1));
                } else if (type == "Pause") {
                    Objects::paused = true;
                    Objects::coflWebSocket.stop();
                } else if (type == "Unpause") {
                    Objects::paused = false;
                    if (Objects::coflWebSocket.getReadyState() != ix::ReadyState::Open) {
                        setupWebsocket();
                    }
                } else if (type == "BugLog") {

                }  else if (type == "ConfigSync") {

                } else if (type == "ConfigLoadMissing") {

                }

                break;
        }
    });

    Objects::msWebSocket.start();
    //Objects::msWebSocket.send("Hello, WebSocket!");

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    // Let the WebSocket run for a while
    //std::this_thread::sleep_for(std::chrono::seconds(30));
    //Objects::msWebSocket.stop();
}
