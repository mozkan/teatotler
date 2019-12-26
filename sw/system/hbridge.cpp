#include "system/hbridge.h"

BasicHBridge::BasicHBridge(
  mcu::io::IDigitalOutput* dir_a, mcu::io::IDigitalOutput* dir_b,
  mcu::io::IDigitalOutput* pwm_a, mcu::io::IDigitalOutput* pwm_b)
    : dir_a_(dir_a), dir_b_(dir_b), pwm_a_(pwm_a), pwm_b_(pwm_b) {
  Stop();
}

void BasicHBridge::DriveForward() {
  Stop();
  dir_a_->Clear();
  pwm_b_->Set();
}

void BasicHBridge::DriveReverse() {
  Stop();
  dir_b_->Clear();
  pwm_a_->Set();
}

void BasicHBridge::Stop() {
  dir_a_->Set();
  dir_b_->Set();
  pwm_a_->Clear();
  pwm_b_->Clear();
}
