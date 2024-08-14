#include "QueueManager.h"
#include <iostream>
#include <chrono>

std::queue<std::function<void()>> QueueManager::queue;
std::string QueueManager::currentAction;
std::thread QueueManager::finishFailsafe;
std::mutex QueueManager::mutex;
std::condition_variable QueueManager::cv;

std::thread DelayUtils::delayAction(int delay, std::function<void()> callback) {
    return std::thread([delay, callback]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        callback();
    });
}

void QueueManager::finishAction(const std::string& action) {
    std::lock_guard<std::mutex> lock(mutex);
    if (currentAction == action) {
        finishAction();
    }
}

void QueueManager::finishAction() {
    std::lock_guard<std::mutex> lock(mutex);
    std::cout << "Finished " + currentAction << std::endl;
    currentAction.clear();
    if (finishFailsafe.joinable()) {
        finishFailsafe.join();
    }
    if (!queue.empty()) {
        auto r = queue.front();
        queue.pop();
        currentAction = "Runnable"; // Example placeholder
        r();
        std::cout << "1 Started " + currentAction << std::endl;

        finishFailsafe = DelayUtils::delayAction(1000, []() { finishAction(currentAction); });
    }
}

void QueueManager::addToQueue(std::function<void()> action) {
    std::lock_guard<std::mutex> lock(mutex);
    std::cout << "Added Runnable" << std::endl;
    queue.push(action);
    startNext();
}

void QueueManager::addToStartOfQueue(std::function<void()> action) {
    std::lock_guard<std::mutex> lock(mutex);
    std::cout << "Added Runnable" << std::endl;
    queue.emplace(action);
    startNext();
}

void QueueManager::startNext() {
    std::lock_guard<std::mutex> lock(mutex);
    if (currentAction.empty() && !queue.empty()) {
        auto r = queue.front();
        queue.pop();
        currentAction = "Runnable"; // Example placeholder
        r();
        std::cout << "2 Started " + currentAction << std::endl;

        finishFailsafe = DelayUtils::delayAction(1000, []() { finishAction(currentAction); });
    }
}