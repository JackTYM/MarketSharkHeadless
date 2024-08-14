//
// Created by root on 8/13/24.
//

#ifndef MCLIB_QUEUEMANAGER_H
#define MCLIB_QUEUEMANAGER_H

#include <string>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

class DelayUtils {
public:
    static std::thread delayAction(int delay, std::function<void()> callback);
};

class QueueManager {
public:
    static void finishAction(const std::string& action);
    static void finishAction();
    static void addToQueue(std::function<void()> action);
    static void addToStartOfQueue(std::function<void()> action);

private:
    static void startNext();

    static std::queue<std::function<void()>> queue;
    static std::string currentAction;
    static std::thread finishFailsafe;
    static std::mutex mutex;
    static std::condition_variable cv;
};

#endif //MCLIB_QUEUEMANAGER_H
