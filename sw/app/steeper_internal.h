#ifndef APP_STEEPER_INTERNAL_H
#define APP_STEEPER_INTERNAL_H

#include <cstdint>

#include <array>

#include "system/teatotler.h"

namespace application {
namespace steeper_internal {

constexpr int kMillisecondsPerSteepTimeIncrement = 30;
constexpr int kMaxSteepTimeCounts = sys::LinearDisplay::kPixelCount;

constexpr uint32_t kBagLowerDurationMs = 1000;
constexpr uint32_t kBagRaiseDurationMs = kBagLowerDurationMs;

enum class SteepState {
  kSetSteepTime,
  kSetDunkCount,
  kSteep,
  kSteepComplete,
};

struct SteepParameters {
  // The steep time in milliseconds. Default to 3 minutes 30 seconds.
  int steep_time_ms = 1000 * 210;

  // The start time of the steep process.
  int steep_start_time_ms = 0;

  // Default to two dunks per steep.
  int dunk_count = 2;

  // The number of counts on the display to show, representing time in 30 second
  // increments. This is the pixel buffer for the LinearDisplay.
  std::array<bool, 9> steep_time_indication = {false};
};

// Updates a given pixel buffer with a raw set of rotary knob counts. Saturates
// the rotation if it goes outside of the displayable range.
void UpdateDisplayParameters(
    int* knob_rotation, std::array<bool, kMaxSteepTimeCounts>* buffer);

class SetSteepTime {
 public:
  SetSteepTime(SteepParameters* parameters,
               sys::BasicHBridge* winch_drive,
               sys::LinearDisplay* time_indicator,
               sys::PanelButtons* buttons,
               sys::RotarySwitch* knob);
  ~SetSteepTime() = default;

  SteepState Run(uint32_t time_ms);

 private:
  void ReadSteepTime();
  void DriveWinchWithButtons();

  SteepParameters* parameters_;

  sys::BasicHBridge* winch_drive_;
  sys::LinearDisplay* time_indicator_;
  sys::PanelButtons* buttons_;
  sys::RotarySwitch* knob_;

  int steep_time_counts_;
};

class SetDunkCount {
 public:
  SetDunkCount(SteepParameters* parameters,
               sys::LinearDisplay* time_indicator,
               sys::PanelButtons* buttons,
               sys::RotarySwitch* knob);
  ~SetDunkCount() = default;

  SteepState Run(uint32_t time_ms);

 private:
  void UpdateDisplay(uint32_t time_ms);

  SteepParameters* parameters_;

  sys::LinearDisplay* time_indicator_;
  sys::PanelButtons* buttons_;
  sys::RotarySwitch* knob_;

  uint8_t toggle_blink_;
};

class Steep {
 public:
  Steep(SteepParameters* parameters,
        sys::BasicHBridge* winch_drive,
        sys::LinearDisplay* time_indicator,
        sys::PanelButtons* buttons);
  ~Steep() = default;

  SteepState Run(uint32_t time_ms);

 private:
  SteepParameters* parameters_;

  sys::BasicHBridge* winch_drive_;
  sys::LinearDisplay* time_indicator_;
  sys::PanelButtons* buttons_;
};
#if 0
class SteepComplete {
 public:
  SteepComplete(SteepParameters* parameters,
                sys::BasicHBridge* winch_drive);
  ~SteepComplete() = default;

  SteepState Run(uint32_t time_ms);

 private:
  SteepParameters* parameters_;
  sys::BasicHBridge* winch_drive_;
};
#endif
}  // namespace steeper_internal
}  // namespace application

#endif /* APP_STEEPER_INTERNAL_H */
