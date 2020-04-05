#ifndef SYSTEM_TICK_TIMER_H
#define SYSTEM_TICK_TIMER_H

#include <cstdint>

#include <functional>

#include "mcu/interval_timer.h"

namespace sys {

// Simple tick timing resource. Contains an appropriate timer to perform a busy
// wait until the next tick. Stops execution and takes a user-provided action if
// a deadline is missed.
class TickTimer {
 public:
  TickTimer(mcu::IIntervalTimer* timer);
  ~TickTimer() = default;

  // Busy waits on the start of the next tick.
  // TODO: Make this put the CPU in a fast wakeup sleep mode to save power.
  uint32_t WaitUntilNextTick();

  // Sets an optional handler for when the tick deadline is missed.
  void SetDealineMissedHandler(std::function<void()> handler);

 private:
  uint32_t time_;

  mcu::IIntervalTimer* timer_;
  std::function<void()> error_handler_;
};

}  // namespace sys

#endif /* SYSTEM_TICK_TIMER_H */
