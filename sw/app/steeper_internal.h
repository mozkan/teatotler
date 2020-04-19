#ifndef APP_STEEPER_INTERNAL_H
#define APP_STEEPER_INTERNAL_H

#include <cstdint>

#include <array>

#include "system/display.h"
#include "system/teatotler.h"

namespace application {
namespace steeper_internal {

constexpr int kSecondsPerSteepTimeIncrement = 30;

// The starting offset on the display in milliseconds.
constexpr int kDisplayTimeStartingOffsetMs = 60000;

constexpr int kMaxSteepTimeCounts = sys::LinearDisplay::kPixelCount;
constexpr int kMaxWholeNumberCounts = (sys::LinearDisplay::kPixelCount + 1) / 2;

constexpr uint32_t kBagLowerDurationMs = 2000;
constexpr uint32_t kBagRaiseDurationMs = 2200;

constexpr std::array<bool, kMaxSteepTimeCounts> kBlankDisplay {{
  false, false, false, false, false, false, false, false, false
}};

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
// the rotation if it goes outside of the displayable range. Knob counts
// correspond to 30 second increments on the silkscreen.
void UpdateDisplayTime(
    int* knob_rotation, std::array<bool, kMaxSteepTimeCounts>* buffer);

// Updates only the pixels corresponding to the whole counts on the front panel.
// Knob rotations corespond directly to the values "1", "2", "3", etc. on the
// silkscreen with the pixels in between them being skipped.
void UpdateDisplayWholeCounts(
  int* knob_rotation, std::array<bool, kMaxSteepTimeCounts>* buffer);

// Conversion functions to go to and from steep counts shown on the display and
// milliseconds.
uint32_t SteepCountsToMilliseconds(int counts);
int MillisecondsToSteepCounts(uint32_t steep_time_ms);

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
  enum class State {
    kReadUserInput,
    kLowerTeabag,
  };

  SteepState ReadUserInput(uint32_t time_ms);
  SteepState LowerTeabag(uint32_t time_ms);
  void ReadSteepTime();
  void DriveWinchWithButtons();

  SteepParameters* parameters_;

  State state_;

  sys::BasicHBridge* winch_drive_;
  sys::LinearDisplay* time_indicator_;
  sys::PanelButtons* buttons_;
  sys::RotarySwitch* knob_;

  int steep_time_counts_;
  uint32_t bag_lower_start_ms_;
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
  enum class State {
    kStartSteep,
    kSteeping,
    kDunkTeabagDown,
    kDunkTeabagUp,
    kSteepComplete
  };

  void StartSteep(uint32_t time_ms);
  void Steeping(uint32_t time_ms);
  void DunkTeabagDown(uint32_t time_ms);
  void DunkTeabagUp(uint32_t time_ms);
  void UpdateDisplay(uint32_t time_ms);

  SteepParameters* parameters_;

  sys::BasicHBridge* winch_drive_;
  sys::LinearDisplay* time_indicator_;
  sys::PanelButtons* buttons_;

  State state_;

  uint32_t time_between_dunks_ms_;
  uint32_t dunk_start_ms_;
  uint32_t dunk_interval_end_ms_;
  int dunks_remaining_;
  uint8_t toggle_blink_;
};

class SteepComplete {
 public:
  SteepComplete(
      sys::LinearDisplay* time_indicator, sys::BasicHBridge* winch_drive);
  ~SteepComplete() = default;

  SteepState Run(uint32_t time_ms);

 private:
  enum class State {
    kStart,
    kRaiseTeabag,
    kBlink,
    kEnd,
  };

  void Start(uint32_t time_ms);
  void RaiseTeabag(uint32_t time_ms);
  void Blink(uint32_t time_ms);
  void End();

  State state_;

  sys::LinearDisplay* time_indicator_;
  sys::BasicHBridge* winch_drive_;

  uint32_t raise_start_ms_;
  uint32_t blink_start_ms_;
  uint8_t toggle_blink_;
};

}  // namespace steeper_internal
}  // namespace application

#endif /* APP_STEEPER_INTERNAL_H */
