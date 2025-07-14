#include "TaskScheduler.hpp"

#include <algorithm>

// the global instance
TaskScheduler Scheduler;

int TaskScheduler::every(unsigned long interval, Callback callback) {
    int id = nextId++;
    tasks.push_back(Task(id, callback, interval, millis(), true));
    return id;
}

int TaskScheduler::once(unsigned long delay, Callback callback) {
    int id = nextId++;
    tasks.push_back(Task(id, callback, delay, millis(), false));
    return id;
}

void TaskScheduler::cancel(int id) {
    tasks.erase(
        std::remove_if(tasks.begin(), tasks.end(), [id](const Task& t) { return t.id == id; }),
        tasks.end());
}

void TaskScheduler::pause(int id) {
    if (Task* task = findTask(id)) {
        task->paused = true;
    }
}

void TaskScheduler::resume(int id) {
    if (Task* task = findTask(id)) {
        task->paused = false;
        task->lastRun = millis();
    }
}

void TaskScheduler::reschedule(int id, unsigned long newInterval) {
    if (Task* task = findTask(id)) {
        task->interval = newInterval;
        task->lastRun = millis();
    }
}

void TaskScheduler::loop() {
    unsigned long now = millis();
    for (auto& task : tasks) {
        if (task.completed || task.paused) continue;

        if (now - task.lastRun >= task.interval) {
            task.lastRun = now;
            task.callback();
            if (!task.repeat) {
                task.completed = true;
            }
        }
    }

    tasks.erase(
        std::remove_if(tasks.begin(), tasks.end(), [](const Task& t) { return t.completed; }),
        tasks.end());
}

TaskScheduler::Task* TaskScheduler::findTask(int id) {
    for (auto& task : tasks) {
        if (task.id == id) return &task;
    }
    return nullptr;
}
