#include "app/steeper.h"

namespace application {

using namespace steeper_internal;

namespace {

constexpr uint32_t kSteeperRunIntervalMs = 10;

constexpr int kMillisecondsPerSteepTimeIncrement = 30;
constexpr int kMaxSteepTimeCounts = sys::LinearDisplay::kPixelCount;

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
          teatotler->GetRotarySwitch()) {}

void Steeper::DoRunIteration(uint32_t time_ms) {
  switch (state_) {
    case SteepState::kSetSteepTime:
      state_ = set_steep_time_state_.Run(time_ms);
    break;

    case SteepState::kSetDunkCount:
      //state_ = set_dunk_count_state_.Run();
    break;

    case SteepState::kSteep:
      //state_ = steep_state_.Run(time_ms);
    break;

    case SteepState ::kSteepComplete:
      //state_ = steep_complete_state_.Run(time_ms);
    break;
  }
}

}  // namespace application
