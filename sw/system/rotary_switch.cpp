#include "system/rotary_switch.h"

#include <array>

// This rotary switch implementation is for an encoder with two output, A and B,
// that assert and deassert pulses 90 degrees out of phase. The specific Encoder
// tested is a Bourns PEC16 type encoder. Asserted pulses short the channel to
// the common pin. Deasserted states open-circuit the channel with respect to
// the common pin.
//
// The pulse train is as follows:
//                             ______      ______ ----------- open
//                   A     ____      ______      ______ ----- closed
//
//                      ____      ______      ______ -------- open
//                   B      ______      ______      ______ -- closed
//  Binary Encoding         3  2  0  1  3  2  0  1  3

namespace sys {

namespace {

// The number of distinct encoding states per period.
constexpr int kEncodingStates = 4;

// The number of state changes seen on channels A and B between physical
// detents. Tested empirically.
constexpr int kStateChangesPerDetent = 4;

// Lookup table for the expected current state for a clockwise rotation, indexed
// by the previously observed state.
constexpr std::array<int, kEncodingStates> kClockwiseLookup {{ 1, 3, 0, 2 }};

// Lookup table for the expected current state for a counter clockwise rotation,
// indexed by the previously observed state.
constexpr std::array<int, kEncodingStates>
    kCounterClockwiseLookup {{ 2, 0, 3, 1 }};

}  // namespace

RotarySwitch::RotarySwitch(uint32_t run_interval,
                           mcu::io::IDigitalInput* channel_a,
                           mcu::io::IDigitalInput* channel_b)
    : PeriodicTask(run_interval), rotation_(0), last_encoder_state_(0),
      channel_a_(channel_a), channel_b_(channel_b) {}

// Resets rotation_ on read.
int RotarySwitch::GetRotation() {
  // One detent tick is four state changes in the encoder signal.
  int temp = rotation_ / kStateChangesPerDetent;

  // Keep the remainder so we don't misalign our output with respect to the
  // physical detents on partial rotations.
  rotation_ = rotation_ % kStateChangesPerDetent;

  return temp;
}

void RotarySwitch::DoRunIteration(uint32_t time_ms) {
  channel_a_.Sample(time_ms);
  channel_b_.Sample(time_ms);

  int current_state = CalculateCurrentState();

  if (kClockwiseLookup[last_encoder_state_] == current_state) {
    // We got a match with our current state when our previous state indexed
    // into the clockwise lookup table. Increment clockwise rotation.
    rotation_++;
  } else if (kCounterClockwiseLookup[last_encoder_state_] == current_state) {
    // We got a match with our current state when our previous state indexed
    // into the counter clockwise lookup table. Decrement clockwise rotation.
    rotation_--;
  }
  // else we skipped a phase. Don't set rotation either way.

  last_encoder_state_ = current_state;
}

// Encoder inputs are pulled up and the encoder drives drives them low or floats
// them high. Convert logic low values to set bits in the binary encoded state.
int RotarySwitch::CalculateCurrentState() {
  int binary_bit_0 = 0;
  int binary_bit_1 = 0;

  if (channel_a_.GetValue() == mcu::io::Value::kLow) {
    binary_bit_0 = 1;
  }

  if (channel_b_.GetValue() == mcu::io::Value::kLow) {
    binary_bit_1 = 1;
  }

  return (binary_bit_1 << 1) | binary_bit_0;
}

}  // namespace sys
