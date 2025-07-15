#pragma once

#include "Arduino.h"
#include "functional"
#include "utility"
#include "vector"

class TaskScheduler {
public:
    using Callback = std::function<void()>;

    int every(unsigned long interval, const Callback &callback);
    int once(unsigned long delay, const Callback &callback);
    void cancel(int id);
    void pause(int id);
    void resume(int id);
    void reschedule(int id, unsigned long newInterval);
    void loop();

private:
    struct Task {
        int id;
        Callback callback;
        unsigned long interval;
        unsigned long lastRun;
        bool repeat;
        bool completed;
        bool paused;

        Task(const int id, Callback cb, const unsigned long interval, const unsigned long last, const bool rep)
            : id(id),
              callback(std::move(cb)),
              interval(interval),
              lastRun(last),
              repeat(rep),
              completed(false),
              paused(false) {
        }
    };

    std::vector<Task> tasks;
    int nextId = 1;

    Task *findTask(int id);
};

// the global instance
extern TaskScheduler Scheduler;
