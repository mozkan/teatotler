#include <cstdint>

#include "app/steeper_internal.h"
#include "system/teatotler.h"

namespace application {
namespace steeper_internal {

namespace {

constexpr int kBlinkPeriodMs = 500;
constexpr int kTotalBlinkTimeMs = 30000;

constexpr std::array<bool, kMaxSteepTimeCounts> kFullDisplay {{
  true, true, true, true, true, true, true, true, true
}};

}  // namespace

SteepComplete::SteepComplete(
    sys::LinearDisplay* time_indicator, sys::BasicHBridge* winch_drive)
    : state_(State::kStart), time_indicator_(time_indicator),
      winch_drive_(winch_drive), raise_start_ms_(0), blink_start_ms_(0),
      toggle_blink_(0) {}

SteepState SteepComplete::Run(uint32_t time_ms) {
  SteepState next_state = SteepState::kSteepComplete;

  switch (state_) {
    case State::kStart:
      Start(time_ms);
    break;

    case State::kRaiseTeabag:
      RaiseTeabag(time_ms);
    break;

    case State::kBlink:
      Blink(time_ms);
    break;

    case State::kEnd:
      End();
      next_state = SteepState::kSteepComplete;
    break;
  }

  return next_state;
}

void SteepComplete::Start(uint32_t time_ms) {
  raise_start_ms_ = time_ms;
  state_ = State::kRaiseTeabag;
}

void SteepComplete::RaiseTeabag(uint32_t time_ms) {
  winch_drive_->DriveForward();

  if (time_ms > (raise_start_ms_ + kBagLowerDurationMs)) {
    winch_drive_->Stop();
    blink_start_ms_ = time_ms;
    state_ = State::kBlink;
  }
}

void SteepComplete::Blink(uint32_t time_ms) {
  if ((time_ms % kBlinkPeriodMs) == 0) {
    toggle_blink_++;
  }

  if (toggle_blink_ & 0x01) {
    time_indicator_->Update(kFullDisplay);
  } else {
    time_indicator_->Update(kBlankDisplay);
  }

  if (time_ms > (blink_start_ms_ + kTotalBlinkTimeMs)) {
    state_ = State::kEnd;
  }
}

void SteepComplete::End() {
  time_indicator_->Update(kBlankDisplay);

  // Save steep parameters to memory preserved during lowest power state.
  // ...

  // Enter lowest power state here.
  // ...
}

}  // namespace steeper_internal
}  // namespace application
