#include "Auth.h"
#include "../Server.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <iostream>
#include <mclib/common/Json.h>

static std::string session_id = "";

using json = nlohmann::json;

void Auth::setupWebsocket() {
    Objects::msWebSocket.setUrl("wss://cofl.jacktym.dev");

    Objects::msWebSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr &msg) {
        switch (msg->type) {
            case ix::WebSocketMessageType::Open:
                if (session_id.empty()) {
                    sendToWebsocket("Activating", "");
                } else {
                    sendToWebsocket("Reconnecting", "");
                }

                break;
            case ix::WebSocketMessageType::Close:
                std::cout << Colors::Black << "Closed!" << Colors::End;
                break;
            case ix::WebSocketMessageType::Error:
                std::cout << Colors::Red << "Error!" << Colors::End;
                break;
            case ix::WebSocketMessageType::Message:
                std::cout << Colors::Black << "Received: " << msg->str << Colors::End;

                json parsed_json = json::parse(msg->str);

                /*if (parsed_json.contains("username")) {
                    std::string username = parsed_json["username"];
                    if (!username.empty()) {
                        return;
                    }
                }*/

                std::string type = parsed_json["type"];

                if (type == "Activated") {
                    session_id = parsed_json["session_id"];
                    std::cout << Colors::Black << parsed_json["message"] << Colors::End;
                    sendToWebsocket("RequestSession", "");
                } else if (type == "Reconnected") {
                    std::cout << Colors::Black << parsed_json["message"] << Colors::End;
                    sendToWebsocket("RequestSession", "");
                } else if (type == "FailedActivation") {
                    std::cout << Colors::Black << parsed_json["message"] << Colors::End;
                } else if (type == "IncorrectSession") {
                    std::cout << Colors::Black << "Incorrect Session!" << Colors::End;
                    session_id = "";
                    Objects::msWebSocket.close();
                    setupWebsocket();
                } else if (type == "UpdateSession") {
                    std::cout << Colors::Black << "Updating Session!" << Colors::End;
                    Objects::currentUsername = parsed_json["username"];
                    Objects::currentUUID = parsed_json["uuid"];
                    Objects::currentSSID = parsed_json["ssid"];

                    std::cout << Colors::Black << "Connecting to Server!" << Colors::End;
                    Server::connectToServer();
                }

                break;
        }
    });

    Objects::msWebSocket.start();
    //Objects::msWebSocket.send("Hello, WebSocket!");

    while (true) {

    }
    // Let the WebSocket run for a while
    //std::this_thread::sleep_for(std::chrono::seconds(30));
    //Objects::msWebSocket.stop();
}

void Auth::sendToWebsocket(const std::string &type, const std::string message) {
    // Using nlohmann::json for JSON construction
    nlohmann::json jsonObject;
    jsonObject["type"] = type;
    jsonObject["message"] = message;

    std::string jsonString = jsonObject.dump();

    if (Objects::debug) {
        std::cout << Colors::Black << "Sending: " << jsonString << Colors::End;
    }

    sendNoLog(type, message);
}


void Auth::sendNoLog(const std::string &type, const std::string message) {
    // Using nlohmann::json for JSON construction
    nlohmann::json jsonObject;
    jsonObject["type"] = type;
    jsonObject["message"] = message;

    jsonObject["key"] = "b73bb1c6-aa42-453e-aa0b-ea22f8cb70dd";
    jsonObject["username"] = Objects::currentUsername;
    jsonObject["hwid"] = "";
    jsonObject["version"] = "MEGALODON";
    jsonObject["modVersion"] = "1.0.0";

    if (!session_id.empty()) {
        jsonObject["session_id"] = session_id;
    }

    std::string jsonString = jsonObject.dump();

    Objects::msWebSocket.send(jsonString);
}
