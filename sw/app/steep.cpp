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
      state_(State::kStartSteep), time_between_dunks_ms_(0), dunk_start_ms_(0),
      dunk_interval_end_ms_(0), dunks_remaining_(0), toggle_blink_(0) {}

SteepState Steep::Run(uint32_t time_ms) {
  // Stay in this state by default. Exit when the steep process has completed.
  SteepState next_state = SteepState::kSteep;

  switch (state_) {
    case State::kStartSteep:
      StartSteep(time_ms);
    break;

    case State::kSteeping:
      Steeping(time_ms);
    break;

    case State::kDunkTeabagDown:
      DunkTeabagDown(time_ms);
    break;

    case State::kDunkTeabagUp:
      DunkTeabagUp(time_ms);
    break;

    case State::kSteepComplete:
      next_state = SteepState::kSteepComplete;
    break;
  }

  UpdateDisplay(time_ms);

  return next_state;
}

void Steep::StartSteep(uint32_t time_ms) {
  time_between_dunks_ms_ =
      static_cast<uint32_t>(parameters_->steep_time_ms /
                            (parameters_->dunk_count + 1));
  dunk_interval_end_ms_ = time_ms + time_between_dunks_ms_;
  dunks_remaining_ = parameters_->dunk_count;
  state_ = State::kSteeping;
}

void Steep::Steeping(uint32_t time_ms) {
  if (time_ms > dunk_interval_end_ms_) {
    if (dunks_remaining_ == 0) {
      // The last dunk was the last and this interval represents the end of the
      // steeping period.
      state_ = State::kSteepComplete;
    } else {
      // Determine when the next dunk after this one will be and keep dunking.
      dunk_interval_end_ms_ = time_ms + time_between_dunks_ms_;
      dunk_start_ms_ = time_ms;
      state_ = State::kDunkTeabagDown;
    }
  }
}

void Steep::DunkTeabagDown(uint32_t time_ms) {
  winch_drive_->DriveForward();
  if (time_ms  > (dunk_start_ms_ + kBagLowerDurationMs)) {
    winch_drive_->Stop();
    dunk_start_ms_ = time_ms;
    state_ = State::kDunkTeabagUp;
  }
}

void Steep::DunkTeabagUp(uint32_t time_ms) {
  winch_drive_->DriveReverse();
  if (time_ms  > (dunk_start_ms_ + kBagLowerDurationMs)) {
    winch_drive_->Stop();
    dunks_remaining_--;
    state_ = State::kSteeping;
  }
}

void Steep::UpdateDisplay(uint32_t time_ms) {
  uint32_t steep_time_remaining =
      static_cast<uint32_t>(parameters_->steep_time_ms) -
      static_cast<uint32_t>(time_ms - parameters_->steep_start_time_ms);

  int steep_time_display_counts =
      MillisecondsToSteepCounts(steep_time_remaining);

  UpdateDisplayTime(
      &steep_time_display_counts, &parameters_->steep_time_indication);

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
