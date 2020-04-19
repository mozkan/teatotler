#include <cstdint>

#include "app/steeper_internal.h"
#include "system/teatotler.h"

namespace application {
namespace steeper_internal {

SetSteepTime::SetSteepTime(
    SteepParameters* parameters, sys::BasicHBridge* winch_drive,
    sys::LinearDisplay* time_indicator, sys::PanelButtons* buttons,
    sys::RotarySwitch* knob)
    : parameters_(parameters), state_(State::kReadUserInput),
      winch_drive_(winch_drive), time_indicator_(time_indicator),
      buttons_(buttons), knob_(knob), steep_time_counts_(),
      bag_lower_start_ms_(0) {
  steep_time_counts_ = MillisecondsToSteepCounts(parameters_->steep_time_ms);
}

SteepState SetSteepTime::Run(uint32_t time_ms) {
  // Stay in this state by default.
  SteepState next_state = SteepState::kSetSteepTime;

  switch (state_) {
    case State::kReadUserInput:
      next_state = ReadUserInput(time_ms);
    break;

    case State::kLowerTeabag:
      next_state = LowerTeabag(time_ms);
    break;
  }

  return next_state;
}

SteepState SetSteepTime::ReadUserInput(uint32_t time_ms) {
  ReadSteepTime();
  DriveWinchWithButtons();

  if (buttons_->CheckPressedButton() == sys::PanelButtons::Button::kReset) {
    return SteepState::kSetDunkCount;
  }

  if (buttons_->CheckPressedButton() == sys::PanelButtons::Button::kStart) {
    bag_lower_start_ms_ = time_ms;
    state_ = State::kLowerTeabag;
  }

  return SteepState::kSetSteepTime;
}

SteepState SetSteepTime::LowerTeabag(uint32_t time_ms) {
  winch_drive_->DriveReverse();

  if (time_ms > (bag_lower_start_ms_ + kBagLowerDurationMs)) {
    winch_drive_->Stop();

    // Capture the steeping parameters before moving on to the next SteepState.
    parameters_->steep_time_ms = SteepCountsToMilliseconds(steep_time_counts_);
    parameters_->steep_start_time_ms = time_ms;

    return SteepState::kSteep;
  }

  return SteepState::kSetSteepTime;
}

// Reads the current state of the rotary knob and converts that to steep time in
// milliseconds.
void SetSteepTime::ReadSteepTime() {
  steep_time_counts_ += knob_->GetRotation();

  UpdateDisplayParameters(&steep_time_counts_,
                          &parameters_->steep_time_indication);

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
