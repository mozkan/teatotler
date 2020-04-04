#include "system/teatotler.h"

#include "mcu/pins.h"

namespace sys {

namespace {

constexpr uint32_t kDisplayUpdatePeriodMs = 1;
constexpr uint32_t kHBridgeUpdatePeriodMs = 10;
constexpr uint32_t kPanelButtonsUpdatePeriodMs = 5;
constexpr uint32_t kRotarySwitchUpdatePeriodMs = 5;

}  // namespace

Teatotler::Teatotler()
    : mcu_(),
      timer_(mcu_.GetIntervalTimer(0, mcu::Mcu::SystemClockHz() / 1000)),
      h_bridge_(kHBridgeUpdatePeriodMs,
                mcu_.GetDigitalOutput(mcu::McuPio::kPIO0_9),
                mcu_.GetDigitalOutput(mcu::McuPio::kPIO0_8),
                mcu_.GetDigitalOutput(mcu::McuPio::kPIO0_19),
                mcu_.GetDigitalOutput(mcu::McuPio::kPIO0_20)),
      display_(kDisplayUpdatePeriodMs,
               {{ mcu_.GetDigitalOutput(mcu::McuPio::kPIO0_25),
                  mcu_.GetDigitalOutput(mcu::McuPio::kPIO0_1),
                  mcu_.GetDigitalOutput(mcu::McuPio::kPIO0_26) }},
               {{ mcu_.GetDigitalOutput(mcu::McuPio::kPIO0_24),
                  mcu_.GetDigitalOutput(mcu::McuPio::kPIO0_15),
                  mcu_.GetDigitalOutput(mcu::McuPio::kPIO0_27) }}),
      panel_buttons_(kPanelButtonsUpdatePeriodMs,
                     mcu_.GetDigitalInput(mcu::McuPio::kPIO0_4),
                     mcu_.GetDigitalInput(mcu::McuPio::kPIO0_11),
                     mcu_.GetDigitalInput(mcu::McuPio::kPIO0_6),
                     mcu_.GetDigitalInput(mcu::McuPio::kPIO0_23)),
      rotary_switch_(kRotarySwitchUpdatePeriodMs,
                     mcu_.GetDigitalInput(mcu::McuPio::kPIO0_0),
                     mcu_.GetDigitalInput(mcu::McuPio::kPIO0_14)),
      driver_tasks_{{ &h_bridge_, &display_, &panel_buttons_,
                      &rotary_switch_ }} {}

void Teatotler::WaitUntilNextTick(bool* deadline_missed) {
  if (timer_->TimerExpired()) {
    // If the timer has already expired we have missed our deadline.
    *deadline_missed = true;
  } else {
    // At least one call to TimerExpired() was false, meaning that the timer is
    // still running. We have met our deadline.
    *deadline_missed = false;

    // Wait until the timer expires.
    while (!timer_->TimerExpired()) {
    }
  }
}

BasicHBridge* Teatotler::GetHBridge() {
  return &h_bridge_;
}

LinearDisplay* Teatotler::GetDisplay() {
  return &display_;
}

PanelButtons* Teatotler::GetPanelButtons() {
  return &panel_buttons_;
}

RotarySwitch* Teatotler::GetRotarySwitch() {
  return &rotary_switch_;
}

void Teatotler::RunDrivers(uint32_t time_ms) {
  for (auto& task : driver_tasks_) {
    task->Run(time_ms);
  }
}

}  // namespace sys
