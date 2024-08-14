#include "QueueManager.h"

std::queue<QueueManager::Action> QueueManager::actionQueue;
QueueManager::Action QueueManager::currentAction = {"", nullptr};

void QueueManager::addTaskToEnd(const std::string& actionName, const std::function<void()>& task) {
    actionQueue.push({actionName, task});
    if (currentAction.task == nullptr) {
        startNextAction();
    }
}

void QueueManager::addTaskToStart(const std::string& actionName, const std::function<void()>& task) {
    std::queue<Action> newQueue;
    newQueue.push({actionName, task});
    while (!actionQueue.empty()) {
        newQueue.push(actionQueue.front());
        actionQueue.pop();
    }
    actionQueue = std::move(newQueue);
    if (currentAction.task == nullptr) {
        startNextAction();
    }
}

bool QueueManager::endCurrentTask(const std::string& actionName) {
    if (currentAction.name == actionName) {
        currentAction = {"", nullptr};
        startNextAction();
        return true;
    }
    return false;
}

std::string QueueManager::getCurrentActionName() {
    return currentAction.name;
}

void QueueManager::startNextAction() {
    if (!actionQueue.empty()) {
        currentAction = actionQueue.front();
        actionQueue.pop();
        if (currentAction.task) {
            currentAction.task();
        }
    }
}
