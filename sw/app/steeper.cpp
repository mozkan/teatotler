#include "app/steeper.h"
#include "system/hbridge.h"
#include "system/display.h"
#include "system/panel_buttons.h"
#include "system/rotary_switch.h"

using namespace application::steeper_internal;

namespace application {

namespace {

constexpr uint32_t kSteeperRunIntervalMs = 10;

}  // namespace

Steeper::Steeper(sys::Teatotler* teatotler)
    : PeriodicTask(kSteeperRunIntervalMs),
      steep_parameters_(),
      state_(SteepState::kSetSteepTime),
      set_steep_time_state_(
          &steep_parameters_,
          teatotler->GetHBridge(),
          teatotler->GetDisplay(),
          teatotler->GetPanelButtons(),
          teatotler->GetRotarySwitch()),
      set_dunk_count_state_(
          &steep_parameters_,
          teatotler->GetDisplay(),
          teatotler->GetPanelButtons(),
          teatotler->GetRotarySwitch()),
      steep_state_(
          &steep_parameters_,
          teatotler->GetHBridge(),
          teatotler->GetDisplay(),
          teatotler->GetPanelButtons()) {}

void Steeper::DoRunIteration(uint32_t time_ms) {
  switch (state_) {
    case SteepState::kSetSteepTime:
      state_ = set_steep_time_state_.Run(time_ms);
    break;

    case SteepState::kSetDunkCount:
      state_ = set_dunk_count_state_.Run(time_ms);
    break;

    case SteepState::kSteep:
      state_ = steep_state_.Run(time_ms);
    break;

    case SteepState ::kSteepComplete:
      // TODO: Write steep settings to non-volatile memory and sleep via a
      // fourth state class.
    break;
  }
}

}  // namespace application
