//
// Created by root on 7/31/24.
//

#ifndef MARKETSHARKCPP_RAWCOMMAND_H
#define MARKETSHARKCPP_RAWCOMMAND_H

#include <string>

class RawCommand {
private:
    std::string Type;
    std::string Data;

public:
    RawCommand(const std::string& type, const std::string& data)
            : Type(type), Data(data) {}

    std::string getType() const {
        return Type;
    }

    void setType(const std::string& type) {
        Type = type;
    }

    std::string getData() const {
        return Data;
    }

    void setData(const std::string& data) {
        Data = data;
    }
};

#endif //MARKETSHARKCPP_RAWCOMMAND_H
