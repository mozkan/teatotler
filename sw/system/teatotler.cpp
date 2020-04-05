#include "system/teatotler.h"

#include <functional>

#include "mcu/digital_io.h"
#include "mcu/pins.h"
#include "util/assert.h"

namespace sys {

namespace {

constexpr uint32_t kDisplayUpdatePeriodMs = 1;
constexpr uint32_t kHBridgeUpdatePeriodMs = 10;
constexpr uint32_t kPanelButtonsUpdatePeriodMs = 5;
constexpr uint32_t kRotarySwitchUpdatePeriodMs = 5;

}  // namespace

Teatotler::Teatotler(mcu::Mcu* mcu)
    : h_bridge_(kHBridgeUpdatePeriodMs,
                mcu->GetDigitalOutput(mcu::McuPio::kPIO0_9),
                mcu->GetDigitalOutput(mcu::McuPio::kPIO0_8),
                mcu->GetDigitalOutput(mcu::McuPio::kPIO0_19),
                mcu->GetDigitalOutput(mcu::McuPio::kPIO0_20)),
      display_(kDisplayUpdatePeriodMs,
               {{ mcu->GetDigitalOutput(mcu::McuPio::kPIO0_25),
                  mcu->GetDigitalOutput(mcu::McuPio::kPIO0_1),
                  mcu->GetDigitalOutput(mcu::McuPio::kPIO0_26) }},
               {{ mcu->GetDigitalOutput(mcu::McuPio::kPIO0_24),
                  mcu->GetDigitalOutput(mcu::McuPio::kPIO0_15),
                  mcu->GetDigitalOutput(mcu::McuPio::kPIO0_27) }}),
      panel_buttons_(kPanelButtonsUpdatePeriodMs,
                     mcu->GetDigitalInput(mcu::McuPio::kPIO0_4),
                     mcu->GetDigitalInput(mcu::McuPio::kPIO0_11),
                     mcu->GetDigitalInput(mcu::McuPio::kPIO0_6),
                     mcu->GetDigitalInput(mcu::McuPio::kPIO0_23)),
      rotary_switch_(kRotarySwitchUpdatePeriodMs,
                     mcu->GetDigitalInput(mcu::McuPio::kPIO0_0),
                     mcu->GetDigitalInput(mcu::McuPio::kPIO0_14)),
      tick_timer_(mcu->GetIntervalTimer(0, mcu::Mcu::SystemClockHz() / 1000)),
      driver_tasks_{{ &h_bridge_, &display_, &panel_buttons_,
                      &rotary_switch_ }} {
  tick_timer_.SetDealineMissedHandler([this](){
    MissedTickHandler();
  });
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

TickTimer* Teatotler::GetTickTimer() {
  return &tick_timer_;
}

void Teatotler::RunDrivers(uint32_t time_ms) {
  for (auto& task : driver_tasks_) {
    task->Run(time_ms);
  }
}

void Teatotler::MissedTickHandler() {
  // Unset the missed tick handler just in case we hit it again. We want to stop
  // execution so it doesn't matter at this point.
  tick_timer_.SetDealineMissedHandler([](){});

  // Set one LED on the display in a conspicuous location and stop executing new
  // code.
  display_.Update({{ false, false, false, false, true,
                     false, false, false, false }});

  constexpr uint32_t kErrorDurationMs = 5000;
  uint32_t start_time = tick_timer_.WaitUntilNextTick();

  // Display the error for kErrorDurationMs milliseconds.
  uint32_t time = 0;
  do {
    time = tick_timer_.WaitUntilNextTick();
    display_.Run(time);
  } while (time < (start_time + kErrorDurationMs));

  // TODO: Replace this with low power sleep so that this error condition
  // doesn't drain the battery.
  util::StopExecutionHere();
}

}  // namespace sys
