#include <cstdint>

#include "app/steeper_internal.h"
#include "system/teatotler.h"

namespace application {
namespace steeper_internal {

namespace {

constexpr int kBlinkPeriodMs = 500;

constexpr std::array<bool, kMaxSteepTimeCounts> kBlankDisplay {{
  false, false, false, false, false, false, false, false, false
}};

}  // namespace

SetDunkCount::SetDunkCount(
    SteepParameters* parameters, sys::LinearDisplay* time_indicator,
    sys::PanelButtons* buttons, sys::RotarySwitch* knob)
    : parameters_(parameters),  time_indicator_(time_indicator),
      buttons_(buttons), knob_(knob), toggle_blink_(0)  {}

SteepState SetDunkCount::Run(uint32_t time_ms) {
  // Leave this state by default. Only stay if the RESET button is held.
  SteepState next_state = SteepState::kSetSteepTime;

  if (buttons_->CheckPressedButton() == sys::PanelButtons::Button::kReset) {
    next_state = SteepState::kSetDunkCount;
  }

  UpdateDisplay(time_ms);

  return next_state;
}

void SetDunkCount::UpdateDisplay(uint32_t time_ms) {
  parameters_->dunk_count += knob_->GetRotation();

  UpdateDisplayParameters(&parameters_->dunk_count,
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
