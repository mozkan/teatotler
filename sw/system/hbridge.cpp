#include "system/hbridge.h"

namespace sys {

namespace {

constexpr uint32_t kDeadBandIntervalMs = 100;

}

BasicHBridge::BasicHBridge(
  uint32_t run_interval,
  mcu::io::IDigitalOutput* dir_a, mcu::io::IDigitalOutput* dir_b,
  mcu::io::IDigitalOutput* pwm_a, mcu::io::IDigitalOutput* pwm_b)
    : PeriodicTask(run_interval), current_run_state_(RunState::kStopped),
      next_run_state_(RunState::kStopped), deadband_start_ms_(0),
      dir_a_(dir_a), dir_b_(dir_b), pwm_a_(pwm_a), pwm_b_(pwm_b) {
  SetStopOutputs();
}

void BasicHBridge::DriveForward() {
  next_run_state_ = RunState::kForward;
}

void BasicHBridge::DriveReverse() {
  next_run_state_ = RunState::kReverse;
}

void BasicHBridge::Stop() {
  current_run_state_ = RunState::kStopped;
  next_run_state_ = RunState::kStopped;
}

void BasicHBridge::DoRunIteration(uint32_t time_ms) {
  switch (current_run_state_) {
    case RunState::kForward:
      SetForwardOutputs();
      if (next_run_state_ == RunState::kReverse) {
        current_run_state_ = RunState::kDeadBand;
        deadband_start_ms_ = time_ms;
      }
    break;

    case RunState::kDeadBand:
      SetStopOutputs();
      if ((time_ms - deadband_start_ms_) >= kDeadBandIntervalMs) {
        current_run_state_ = next_run_state_;
      }
    break;

    case RunState::kReverse:
      SetReverseOutputs();
      if (next_run_state_ == RunState::kForward) {
        current_run_state_ = RunState::kDeadBand;
        deadband_start_ms_ = time_ms;
      }
    break;

    case RunState::kStopped:
      SetStopOutputs();
      if (next_run_state_ == RunState::kForward) {
        current_run_state_ = RunState::kForward;
      }
      if (next_run_state_ == RunState::kReverse) {
        current_run_state_ = RunState::kReverse;
      }
    break;
  }
}

void BasicHBridge::SetForwardOutputs() {
  dir_a_->Clear();
  pwm_b_->Set();
}

void BasicHBridge::SetReverseOutputs() {
  dir_b_->Clear();
  pwm_a_->Set();
}

void BasicHBridge::SetStopOutputs() {
  dir_a_->Set();
  dir_b_->Set();
  pwm_a_->Clear();
  pwm_b_->Clear();
}

}  // namespace sys
