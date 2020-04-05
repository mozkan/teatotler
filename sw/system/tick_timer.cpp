#include "system/tick_timer.h"

namespace sys {

TickTimer::TickTimer(mcu::IIntervalTimer* timer)
    : time_(0), timer_(timer), error_handler_([](){}) {}

// TODO: Add statistics collection for how many iterations are done between
// ticks.
uint32_t TickTimer::WaitUntilNextTick() {
  if (timer_->TimerExpired()) {
    // If the timer has already expired we have missed our deadline.
    error_handler_();
  } else {
    // At least one call to TimerExpired() was false, meaning that the timer is
    // still running. We have met our deadline.

    // Wait until the timer expires.
    while (!timer_->TimerExpired()) {
    }
  }

  time_++;

  return time_;
}

void TickTimer::SetDealineMissedHandler(std::function<void()> handler) {
  error_handler_ = handler;
}

}  // namespace sys
