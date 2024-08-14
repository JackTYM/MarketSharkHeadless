#ifndef MARKETSHARKCPP_CHATMESSAGEDATA_H
#define MARKETSHARKCPP_CHATMESSAGEDATA_H

#include <string>
#include <mclib/common/internal/json.hpp>

using json = nlohmann::json;

class ChatMessageData {
public:
    std::string Text;
    std::string OnClick;
    std::string Hover;

    ChatMessageData() {}

    ChatMessageData(const std::string& text, const std::string& onClick, const std::string& hover)
            : Text(text), OnClick(onClick), Hover(hover) {}

    friend void to_json(json& j, const ChatMessageData& c) {
        j = json{{"text", c.Text}, {"onClick", c.OnClick}, {"hover", c.Hover}};
    }

    friend void from_json(const json& j, ChatMessageData& c) {
        if (j.find("text") != j.end() && !j.at("text").is_null()) {
            j.at("text").get_to(c.Text);
        } else {
            c.Text = "";
        }

        if (j.find("onClick") != j.end() && !j.at("onClick").is_null()) {
            j.at("onClick").get_to(c.OnClick);
        } else {
            c.OnClick = "";
        }

        if (j.find("hover") != j.end() && !j.at("hover").is_null()) {
            j.at("hover").get_to(c.Hover);
        } else {
            c.Hover = "";
        }
    }

    bool isTextNull() const {
        return Text.empty();
    }

    bool isOnClickNull() const {
        return OnClick.empty();
    }

    bool isHoverNull() const {
        return Hover.empty();
    }
};

#endif //MARKETSHARKCPP_CHATMESSAGEDATA_H