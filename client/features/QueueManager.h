//
// Created by root on 8/13/24.
//

#ifndef MCLIB_QUEUEMANAGER_H
#define MCLIB_QUEUEMANAGER_H

#include <queue>
#include <string>
#include <functional>
#include <Colors.h>

class QueueManager {
public:
    static void addTaskToEnd(const std::string& actionName, const std::function<void()>& task);
    static void addTaskToStart(const std::string& actionName, const std::function<void()>& task);
    static bool endCurrentTask(const std::string& actionName);
    static std::string getCurrentActionName();

private:
    struct Action {
        std::string name;
        std::function<void()> task;
    };

    static std::queue<Action> actionQueue;
    static Action currentAction;

    static void startNextAction();
};

#endif //MCLIB_QUEUEMANAGER_H
