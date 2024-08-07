#include "Auth.h"
#include "../Server.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <iostream>
#include <mclib/common/Json.h>

static std::string session_id = "";
static ix::WebSocket webSocket;

using json = nlohmann::json;

void Auth::setupWebsocket() {
    webSocket.setUrl("wss://cofl.jacktym.dev");

    webSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr &msg) {
        switch (msg->type) {
            case ix::WebSocketMessageType::Open:
                std::cout << "Session ID " << session_id << std::endl;
                if (session_id.empty()) {
                    sendToWebsocket("Activating", "");
                } else {
                    sendToWebsocket("Reconnecting", "");
                }

                break;
            case ix::WebSocketMessageType::Close:
                std::cout << "Closed!" << std::endl;
                break;
            case ix::WebSocketMessageType::Error:
                std::cout << "Error!" << std::endl;
                break;
            case ix::WebSocketMessageType::Message:
                std::cout << "Received: " << msg->str << std::endl;

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
                    std::cout << parsed_json["message"] << std::endl;
                    sendToWebsocket("RequestSession", "");
                } else if (type == "Reconnected") {
                    std::cout << parsed_json["message"] << std::endl;
                    sendToWebsocket("RequestSession", "");
                } else if (type == "FailedActivation") {
                    std::cout << parsed_json["message"] << std::endl;
                } else if (type == "IncorrectSession") {
                    std::cout << "Incorrect Session!" << std::endl;
                    session_id = "";
                    webSocket.close();
                    setupWebsocket();
                } else if (type == "UpdateSession") {
                    std::cout << "Updating Session!" << std::endl;
                    Server::currentUsername = parsed_json["username"];
                    Server::currentUUID = parsed_json["uuid"];
                    Server::currentSSID = parsed_json["ssid"];

                    std::cout << "Connecting to Server!" << std::endl;
                    Server::connectToServer();
                }

                break;
        }
    });

    webSocket.start();
    //webSocket.send("Hello, WebSocket!");

    while (true) {

    }
    // Let the WebSocket run for a while
    //std::this_thread::sleep_for(std::chrono::seconds(30));
    //webSocket.stop();
}

void Auth::sendToWebsocket(const std::string &type, const std::string message) {
    // Using nlohmann::json for JSON construction
    nlohmann::json jsonObject;
    jsonObject["type"] = type;
    jsonObject["message"] = message;

    std::string jsonString = jsonObject.dump();

    std::cout << "Sending: " << jsonString << std::endl;

    sendNoLog(type, message);
}


void Auth::sendNoLog(const std::string &type, const std::string message) {
    // Using nlohmann::json for JSON construction
    nlohmann::json jsonObject;
    jsonObject["type"] = type;
    jsonObject["message"] = message;

    jsonObject["key"] = "b73bb1c6-aa42-453e-aa0b-ea22f8cb70dd";
    jsonObject["username"] = Server::currentUsername;
    jsonObject["hwid"] = "";
    jsonObject["version"] = "MEGALODON";
    jsonObject["modVersion"] = "1.0.0";

    if (!session_id.empty()) {
        jsonObject["session_id"] = session_id;
    }

    std::string jsonString = jsonObject.dump();

    webSocket.send(jsonString);
}
