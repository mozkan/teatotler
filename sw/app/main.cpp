#include <cr_section_macros.h>

#include "mcu/interval_timer.h"
#include "mcu/mcu.h"
#include "mcu/pins.h"
#include "system/display.h"
#include "system/hbridge.h"

using namespace mcu;

int main(void) {
  Mcu mcu;

#if 0
  io::IDigitalInput* debug_btn = mcu.GetDigitalInput(McuPio::kPIO0_10);
  debug_btn->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalInput* start_btn = mcu.GetDigitalInput(McuPio::kPIO0_4);
  start_btn->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalInput* reset_btn = mcu.GetDigitalInput(McuPio::kPIO0_11);
  reset_btn->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalInput* raise_btn = mcu.GetDigitalInput(McuPio::kPIO0_6);
  raise_btn->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalInput* lower_btn = mcu.GetDigitalInput(McuPio::kPIO0_23);
  lower_btn->SetResistorMode(io::ResistorMode::kPullUp);
#endif

#if 0
  sys::BasicHBridge h_bridge(mcu.GetDigitalOutput(McuPio::kPIO0_9),
                             mcu.GetDigitalOutput(McuPio::kPIO0_8),
                             mcu.GetDigitalOutput(McuPio::kPIO0_19),
                             mcu.GetDigitalOutput(McuPio::kPIO0_20));
#endif

#if 1
  IIntervalTimer* timer = mcu.GetIntervalTimer(0, Mcu::SystemClockHz() / 1000);
  sys::LinearDisplay display(1,
                             {{ mcu.GetDigitalOutput(McuPio::kPIO0_25),
                                mcu.GetDigitalOutput(McuPio::kPIO0_1),
                                mcu.GetDigitalOutput(McuPio::kPIO0_26) }},
                             {{ mcu.GetDigitalOutput(McuPio::kPIO0_24),
                                mcu.GetDigitalOutput(McuPio::kPIO0_15),
                                mcu.GetDigitalOutput(McuPio::kPIO0_27) }});
  uint32_t time = 0;
  uint8_t swap_pixels = 0;

  while (1) {
    while (!timer->TimerExpired()) {
    }
    time++;

    if ((time % 1000) == 0) {
      if (swap_pixels & 0x01) {
        display.Update({{ true, false, true,
                          false, true, false,
                          true, false, true }});
      } else {
        display.Update({{ false, true, false,
                          true, false, true,
                          false, true, false }});
      }

      swap_pixels++;
    }

    display.Run(time);
  }
#endif

#if 0
  IIntervalTimer* timer = mcu.GetIntervalTimer(0, Mcu::SystemClockHz());

  while(1) {
    //h_bridge.DriveForward();
    //while (lower_btn->Read() != io::Value::kLow) { i++; }
    while (!timer->TimerExpired()) {
    }

    //h_bridge.Stop();
    //while (lower_btn->Read() != io::Value::kHigh) { i++; }
    while (!timer->TimerExpired()) {
    }

    //h_bridge.DriveReverse();
    //while (lower_btn->Read() != io::Value::kLow) { i++; }
    while (!timer->TimerExpired()) {
    }

    //h_bridge.Stop();
    //while (lower_btn->Read() != io::Value::kHigh) { i++; }
    while (!timer->TimerExpired()) {
    }

    //while (lower_btn->Read() != io::Value::kLow) { i++; }
    while (!timer->TimerExpired()) {
    }

    //while (lower_btn->Read() != io::Value::kHigh) { i++; }
    while (!timer->TimerExpired()) {
    }
  }
#endif
  return 0;
}
