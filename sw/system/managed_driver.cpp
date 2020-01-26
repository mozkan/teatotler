#include "system/managed_driver.h"

namespace sys {

ManagedDriver::ManagedDriver(uint32_t run_period)
    : kRunPeriodMs(run_period), last_run_time_ms_(0) {}

void ManagedDriver::Run(uint32_t time_ms) {
  if ((time_ms - last_run_time_ms_) < kRunPeriodMs) {
    return;
  }

  // Set this first so that we capture when the iteration started (more
  // deterministic) rather than when it ended (implementation-dependent and
  // variable across iterations).
  last_run_time_ms_ = time_ms;

  DoRunIteration(time_ms);
}

}  // namespace sys
