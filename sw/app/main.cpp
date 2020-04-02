#include <cr_section_macros.h>

#include "mcu/interval_timer.h"
#include "mcu/mcu.h"
#include "mcu/pins.h"
#include "system/display.h"
#include "system/hbridge.h"
#include "system/teatotler.h"

using namespace mcu;

int main(void) {
  sys::Teatotler teatotler;

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

  sys::LinearDisplay* display = teatotler.GetDisplay();

  uint32_t time = 0;
  uint8_t swap_pixels = 0;

  while (1) {
    bool expired;
    teatotler.WaitUntilNextTick(&expired);
    (void)expired; // Temporary.

    time++;

    if ((time % 1000) == 0) {
      if (swap_pixels & 0x01) {
        display->Update({{ true, false, true,
                           false, true, false,
                           true, false, true }});
      } else {
        display->Update({{ false, true, false,
                           true, false, true,
                           false, true, false }});
      }

      swap_pixels++;
    }

    teatotler.RunDrivers(time);
  }

  return 0;
}
