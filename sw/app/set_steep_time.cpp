#include <cstdint>

#include "app/steeper_internal.h"
#include "system/teatotler.h"

namespace application {
namespace steeper_internal {

SetSteepTime::SetSteepTime(
    SteepParameters* parameters, sys::BasicHBridge* winch_drive,
    sys::LinearDisplay* time_indicator, sys::PanelButtons* buttons,
    sys::RotarySwitch* knob)
    : parameters_(parameters), winch_drive_(winch_drive),
      time_indicator_(time_indicator), buttons_(buttons), knob_(knob),
      steep_time_counts_(0) {}

SteepState SetSteepTime::Run(uint32_t time_ms) {
  SteepState next_state = SteepState::kSetSteepTime;

  ReadSteepTime();
  DriveWinchWithButtons();

  if (buttons_->CheckPressedButton() == sys::PanelButtons::Button::kReset) {
    next_state = SteepState::kSetDunkCount;
  }

  if (buttons_->CheckPressedButton() == sys::PanelButtons::Button::kStart) {
    parameters_->steep_time_ms =
        steep_time_counts_ * 1000 * kMillisecondsPerSteepTimeIncrement;
    parameters_->steep_start_time_ms = time_ms;

    next_state = SteepState::kSteep;
  }

  return next_state;
}

// Reads the current state of the rotary knob and converts that to steep time in
// milliseconds.
void SetSteepTime::ReadSteepTime() {
  steep_time_counts_ += knob_->GetRotation();

  // Saturate the value if there are more knob rotations than valid counts
  // displayable on the indicator.
  if (steep_time_counts_ > 9) {
    steep_time_counts_ = 9;
  } else if (steep_time_counts_ < 0) {
    steep_time_counts_ = 0;
  }

  // Set the indicator's on pixels.
  int steep_time_increment;
  for (steep_time_increment = 0; steep_time_increment < steep_time_counts_;
       steep_time_increment++) {
    parameters_->steep_time_indication[steep_time_increment] = true;
  }

  // Clear the remaining pixels.
  for (steep_time_increment = steep_time_counts_;
       steep_time_increment < kMaxSteepTimeCounts;
       steep_time_increment++) {
    parameters_->steep_time_indication[steep_time_increment] = false;
  }

  time_indicator_->Update(parameters_->steep_time_indication);
}

// Reads the state of the Raise and Lower buttons and drives the winch
// appropriately.
void SetSteepTime::DriveWinchWithButtons() {
  if (buttons_->CheckPressedButton() == sys::PanelButtons::Button::kRaise) {
    winch_drive_->DriveForward();
  } else if (buttons_->CheckPressedButton() ==
             sys::PanelButtons::Button::kLower) {
    winch_drive_->DriveReverse();
  } else {
    winch_drive_->Stop();
  }
}

}  // namespace steeper_internal
}  // namespace application
