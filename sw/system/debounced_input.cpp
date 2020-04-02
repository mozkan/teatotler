#include "system/debounced_input.h"

namespace sys {

constexpr uint32_t DebouncedInput::kDebounceIntervalMs;

DebouncedInput::DebouncedInput(mcu::io::IDigitalInput* input_pin)
    : input_pin_(input_pin), current_state_(mcu::io::Value::kHigh),
      last_read_(mcu::io::Value::kHigh), time_at_last_read_ms_(0),
      duration_at_current_state_ms_(0) {
  input_pin_->SetResistorMode(mcu::io::ResistorMode::kPullUp);
}

void DebouncedInput::Sample(uint32_t time_ms) {
  mcu::io::Value read_state = input_pin_->Read();

  if (last_read_ != read_state) {
    duration_at_current_state_ms_ = 0;
  } else {
    duration_at_current_state_ms_ += time_ms - time_at_last_read_ms_;
  }

  if (duration_at_current_state_ms_ >= kDebounceIntervalMs) {
    current_state_ = read_state;
  }

  time_at_last_read_ms_ = time_ms;
  last_read_ = read_state;
}

mcu::io::Value DebouncedInput::GetValue() {
  return current_state_;
}

}  // namespace sys
