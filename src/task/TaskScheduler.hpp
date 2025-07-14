#pragma once
#include <Arduino.h>

#include <functional>
#include <vector>

class TaskScheduler {
   public:
    using Callback = std::function<void(void)>;

    int every(unsigned long interval, Callback callback);
    int once(unsigned long delay, Callback callback);

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

        Task(int id, Callback cb, unsigned long intv, unsigned long last, bool rep)
            : id(id),
              callback(cb),
              interval(intv),
              lastRun(last),
              repeat(rep),
              completed(false),
              paused(false) {}
    };

    std::vector<Task> tasks;
    int nextId = 1;

    Task* findTask(int id);
};

// the global instance
extern TaskScheduler Scheduler;
