#include "TaskScheduler.hpp"

#include "algorithm"

// the global instance
TaskScheduler Scheduler;

int TaskScheduler::every(unsigned long interval, const Callback &callback) {
    int id = nextId++;
    tasks.emplace_back(id, callback, interval, millis(), true);
    return id;
}

int TaskScheduler::once(unsigned long delay, const Callback &callback) {
    int id = nextId++;
    tasks.emplace_back(id, callback, delay, millis(), false);
    return id;
}

void TaskScheduler::cancel(int id) {
    tasks.erase(
        std::remove_if(tasks.begin(), tasks.end(), [id](const Task &t) { return t.id == id; }),
        tasks.end());
}

void TaskScheduler::pause(const int id) {
    if (Task *task = findTask(id)) {
        task->paused = true;
    }
}

void TaskScheduler::resume(const int id) {
    if (Task *task = findTask(id)) {
        task->paused = false;
        task->lastRun = millis();
    }
}

void TaskScheduler::reschedule(const int id, const unsigned long newInterval) {
    if (Task *task = findTask(id)) {
        task->interval = newInterval;
        task->lastRun = millis();
    }
}

void TaskScheduler::loop() {
    const unsigned long now = millis();
    for (auto &task: tasks) {
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
        std::remove_if(tasks.begin(), tasks.end(), [](const Task &t) { return t.completed; }),
        tasks.end());
}

TaskScheduler::Task *TaskScheduler::findTask(const int id) {
    for (auto &task: tasks) {
        if (task.id == id) return &task;
    }
    return nullptr;
}
