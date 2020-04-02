#ifndef UTIL_PERIODIC_TASK_H
#define UTIL_PERIODIC_TASK_H

#include <cstdint>

namespace util {

// A common interface for tasks that run repeatedly after a fixed time interval.
// Work is done via calls to Run() to drive one iteration at a fixed interval
// set on construction. Run() can be called as fast or as slow as needed, with
// this base class managing the actual run interval timing. All other logic
// done within an iteration of Run needs to be defined by derived classes via
// DoRunIteration().
class PeriodicTask {
 public:
  PeriodicTask(uint32_t run_period);
  virtual ~PeriodicTask() = default;

  // Runs one iteration of the task's internal logic.
  //
  // time_ms represents the absolute wall clock time in milliseconds.
  virtual void Run(uint32_t time_ms);

 private:
  // Derived classes implement this to perform their periodic functions.
  virtual void DoRunIteration(uint32_t time_ms) = 0;

  const uint32_t kRunPeriodMs;

  // The wall clock time at which the task last ran an iteration.
  uint32_t last_run_time_ms_;
};

}  // namespace util

#endif /* UTIL_PERIODIC_TASK_H */
