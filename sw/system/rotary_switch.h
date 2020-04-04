#ifndef SYSTEM_ROTARY_SWITCH_H
#define SYSTEM_ROTARY_SWITCH_H

#include <cstdint>

#include "mcu/digital_io.h"
#include "system/debounced_input.h"
#include "util/periodic_task.h"

namespace sys {

// Represents a rotary switch input which provides a reset-on-read rotation
// count.
class RotarySwitch : public util::PeriodicTask {
 public:
  RotarySwitch(uint32_t run_interval,
               mcu::io::IDigitalInput* channel_a,
               mcu::io::IDigitalInput* channel_b);
  virtual ~RotarySwitch() = default;

  // Returns how far the switch has rotated in ticks (detents) since thi
  // function was last called.
  int GetRotation();

 private:
  void DoRunIteration(uint32_t time_ms) override;
  int CalculateCurrentState();

  int rotation_;
  int last_encoder_state_;
  DebouncedInput channel_a_;
  DebouncedInput channel_b_;
};

}  // namespace sys

#endif /* SYSTEM_ROTARY_SWITCH_H */
