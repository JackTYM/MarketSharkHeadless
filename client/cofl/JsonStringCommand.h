//
// Created by root on 7/31/24.
//

#ifndef MARKETSHARKCPP_JSONSTRINGCOMMAND_H
#define MARKETSHARKCPP_JSONSTRINGCOMMAND_H

#include "Command.h"
#include "CommandType.h"
#include <mclib/common/internal/json.hpp>

using json = nlohmann::json;

class JsonStringCommand : public Command<std::string> {
public:
    JsonStringCommand(const std::string& type, const std::string& data) {
        this->setType(CommandTypeHelper::fromJson(type));
        this->setData(data);
    }

    JsonStringCommand() : Command<std::string>() {}

    JsonStringCommand(CommandType type, const std::string& data)
            : Command<std::string>(type, data) {}

    template<typename T>
    Command<T> GetAs() {
        T t = json::parse(this->getData()).get<T>();
        return Command<T>(this->getType(), t);
    }
};

#endif //MARKETSHARKCPP_JSONSTRINGCOMMAND_H
