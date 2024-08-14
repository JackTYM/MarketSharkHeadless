//
// Created by root on 7/31/24.
//

#ifndef MARKETSHARKCPP_COMMAND_H
#define MARKETSHARKCPP_COMMAND_H

#include <string>
#include <sstream>
#include "CommandType.h"

template<typename T>
class Command {
private:
    CommandType Type;
    T Data;

public:
    Command() {}

    Command(CommandType type, const T& data)
            : Type(type), Data(data) {}

    CommandType getType() const {
        return Type;
    }

    void setType(CommandType type) {
        Type = type;
    }

    T getData() const {
        return Data;
    }

    void setData(const T& data) {
        Data = data;
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << "Command [Type=" << Type << ", data=" << Data << "]";
        return oss.str();
    }
};

#endif //MARKETSHARKCPP_COMMAND_H
