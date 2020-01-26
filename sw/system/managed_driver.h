#ifndef SYSTEM_MANAGED_DRIVER_H
#define SYSTEM_MANAGED_DRIVER_H

#include <cstdint>

namespace sys {

// A common interface for drivers that need to be externally managed. These
// drivers are not run within dedicated threads of execution. They are also not
// invoked as part of a dedicated thread of execution. Work is done via calls to
// Run() to drive one iteration through their control logic at a fixed interval
// set on construction. Run() can be called as fast or as slow as needed, with
// this base class managing the actual run interval timing. All other logic
// done within an iteration of Run needs to be defined by derived classes via
// DoRunIteration().
class ManagedDriver {
 public:
  ManagedDriver(uint32_t run_period);
  virtual ~ManagedDriver() = default;

  // Runs one iteration of the driver logic.
  //
  // time_ms represents the absolute wall clock time in milliseconds.
  virtual void Run(uint32_t time_ms);

 private:
  // Derived classes implement this to perform their periodic functions.
  virtual void DoRunIteration(uint32_t time_ms) = 0;
  //virtual void DoRunIteration(uint32_t /*time_ms*/) {};

  const uint32_t kRunPeriodMs;

  // The wall clock time at which the driver last run an iteration.
  uint32_t last_run_time_ms_;
};

}  // namespace sys

#endif /* SYSTEM_MANAGED_DRIVER_H */
