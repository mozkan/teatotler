#include <cstdint>

#include "app/steeper_internal.h"
#include "system/teatotler.h"

namespace application {
namespace steeper_internal {

namespace {

constexpr int kBlinkPeriodMs = 500;

}  // namespace

Steep::Steep(
    SteepParameters* parameters, sys::BasicHBridge* winch_drive,
    sys::LinearDisplay* time_indicator, sys::PanelButtons* buttons)
    : parameters_(parameters),  winch_drive_(winch_drive),
      time_indicator_(time_indicator), buttons_(buttons),
      state_(State::kStartSteep), next_state_(State::kSteepComplete),
      time_between_dunks_ms_(0), dunk_start_ms_(0), toggle_blink_(0) {}

SteepState Steep::Run(uint32_t time_ms) {
  // Stay in this state by default. Exit when the steep process has completed.
  SteepState next_state = SteepState::kSteep;

  switch (state_) {
    case State::kStartSteep:
      StartSteep();
    break;

    case State::kSteeping:
      Steeping(time_ms);
    break;

    case State::kLowerTeabag:
      LowerTeabag(time_ms);
    break;

    case State::kRaiseTeabag:
      RaiseTeabag(time_ms);
    break;

    case State::kSteepComplete:
      next_state = SteepState::kSteepComplete;
    break;
  }

  UpdateDisplay(time_ms);

  return next_state;
}

void Steep::StartSteep() {
  time_between_dunks_ms_ =
      static_cast<uint32_t>(parameters_->steep_time_ms /
                            parameters_->dunk_count);
  state_ = State::kSteeping;
}

void Steep::Steeping(uint32_t time_ms) {
  if (time_ms > static_cast<uint32_t>(parameters_->steep_start_time_ms +
                                      parameters_->steep_time_ms)) {
    state_ = State::kSteepComplete;
  }
}

void Steep::LowerTeabag(uint32_t time_ms) {
  winch_drive_->DriveReverse();
  if (time_ms  > (dunk_start_ms_ + kBagLowerDurationMs)) {
    winch_drive_->Stop();
    state_ = next_state_;
  }
}

void Steep::RaiseTeabag(uint32_t time_ms) {
  winch_drive_->DriveForward();
  if (time_ms  > (dunk_start_ms_ + kBagLowerDurationMs)) {
    winch_drive_->Stop();
    state_ = next_state_;
  }
}

void Steep::UpdateDisplay(uint32_t time_ms) {
  uint32_t steep_time_remaining =
      static_cast<uint32_t>(parameters_->steep_time_ms) -
      static_cast<uint32_t>(time_ms - parameters_->steep_start_time_ms);

  int steep_time_display_counts =
      MillisecondsToSteepCounts(steep_time_remaining);

  UpdateDisplayParameters(
      &steep_time_display_counts,
      &parameters_->steep_time_indication);

  if ((time_ms % kBlinkPeriodMs) == 0) {
    toggle_blink_++;
  }

  // Only show the current dunk count on odd intervals.
  if (toggle_blink_ & 0x01) {
    time_indicator_->Update(parameters_->steep_time_indication);
  } else {
    time_indicator_->Update(kBlankDisplay);
  }
}

}  // namespace steeper_internal
}  // namespace application
