#include "system/teatotler.h"

#include "mcu/pins.h"

namespace sys {

namespace {

constexpr int kDisplayUpdatePeriodMs = 1;
constexpr int kHBridgeUpdatePeriodMs = 10;

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
      driver_tasks_{{ &h_bridge_, &display_ }} {}

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

void Teatotler::RunDrivers(uint32_t time_ms) {
  for (auto& task : driver_tasks_) {
    task->Run(time_ms);
  }
}

}  // namespace sys
