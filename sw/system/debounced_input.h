#ifndef SYSTEM_DEBOUNCED_INPUT_H
#define SYSTEM_DEBOUNCED_INPUT_H

#include <cstdint>

#include "mcu/digital_io.h"

namespace sys {

// Class to aggregate all front panel buttons. Provides a simple interface to
// detect one button press at a time.
class DebouncedInput {
 public:
  DebouncedInput(mcu::io::IDigitalInput* input_pin);
  ~DebouncedInput() = default;

  // Samples the pin and determines the current debounced state. Takes wall
  // clock time as time_ms.
  void Sample(uint32_t time_ms);

  // Gets the current deboucned state of the button.
  mcu::io::Value GetValue();

 private:
  // The amount of time in milliseconds that the pin has to stay at the same
  // state in order for the debounced state to change to that state.
  static constexpr uint32_t kDebounceIntervalMs = 10;

  mcu::io::IDigitalInput* input_pin_;
  mcu::io::Value current_state_;
  mcu::io::Value last_read_;
  uint32_t time_at_last_read_ms_;
  uint32_t duration_at_current_state_ms_;
};

}  // namespace sys

#endif /* SYSTEM_DEBOUNCED_INPUT_H */
