#include <cr_section_macros.h>

#include <array>

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
  std::array<bool, 9> pixels;

  sys::BasicHBridge* hbridge = teatotler.GetHBridge();
  hbridge->DriveForward();

  uint32_t time = 0;
  int i = 0;
  while (1) {
    bool expired;
    teatotler.WaitUntilNextTick(&expired);
    (void)expired; // Temporary.

    time++;
#if 0
    if ((time % 1000) == 0) {
      pixels[i] = true;
      display->Update(pixels);

      if ((i % 2) == 0) {
        hbridge->DriveForward();
      } else {
        hbridge->DriveReverse();
      }

      i++;
      if (i == 9) {
        for (auto& pixel : pixels) {
          pixel = false;
        }
        i = 0;
      }
    }
#endif
    teatotler.RunDrivers(time);
  }

  return 0;
}
