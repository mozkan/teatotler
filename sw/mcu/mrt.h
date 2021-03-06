#ifndef MCU_MRT_H
#define MCU_MRT_H

#include <cstdint>

#include "mcu/interval_timer.h"

namespace mcu {

// Multi-rate timer peripheral driver. One channel.
class Mrt : public IIntervalTimer {
 public:
  // Creates an MRT driver that starts counting from period down to zero at the
  // system clock frequency. Reloads the timer when zero is reached. Starts
  // immediately after construction.
  //
  // Valid range for period is 0 to 0x7FFFFFFF.
  Mrt(uint32_t period);
  virtual ~Mrt() = default;

  // Returns true when the counter reaches zero. Resets on read of a successful
  // expiration. Subsequent calls will return false until the counter expires
  // again.
  bool TimerExpired() override;

 private:
  int channel_;
};

}  // namespace mcu

#endif /* MCU_MRT_H */
