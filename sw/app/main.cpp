#include <cr_section_macros.h>

#include <array>

#include "mcu/digital_io.h"
#include "mcu/interval_timer.h"
#include "mcu/mcu.h"
#include "mcu/pins.h"
#include "system/debounced_input.h"
#include "system/display.h"
#include "system/hbridge.h"
#include "system/teatotler.h"

using namespace mcu;

int main(void) {
  sys::Teatotler teatotler;

#if 1
  io::IDigitalInput* start_btn_input = teatotler.GetInput(McuPio::kPIO0_4);
  //start_btn_input->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalInput* reset_btn_input = teatotler.GetInput(McuPio::kPIO0_11);
  //reset_btn_input->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalInput* raise_btn_input = teatotler.GetInput(McuPio::kPIO0_6);
  //raise_btn_input->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalInput* lower_btn_input = teatotler.GetInput(McuPio::kPIO0_23);
  //lower_btn_input->SetResistorMode(io::ResistorMode::kPullUp);
#endif

  sys::LinearDisplay* display = teatotler.GetDisplay();
  std::array<bool, 9> pixels;

  //sys::BasicHBridge* hbridge = teatotler.GetHBridge();
  sys::DebouncedInput start_button(start_btn_input);
  sys::DebouncedInput reset_button(reset_btn_input);
  sys::DebouncedInput raise_button(raise_btn_input);
  sys::DebouncedInput lower_button(lower_btn_input);

  uint32_t time = 0;
  //int i = 0;
  while (1) {
    bool expired;
    teatotler.WaitUntilNextTick(&expired);
    (void)expired; // Temporary.

    time++;

    start_button.Sample(time);
    reset_button.Sample(time);
    raise_button.Sample(time);
    lower_button.Sample(time);

    if (start_button.GetValue() == mcu::io::Value::kLow) {
      pixels[0] = true;
    } else {
      pixels[0] = false;
    }

    if (reset_button.GetValue() == mcu::io::Value::kLow) {
      pixels[1] = true;
    } else {
      pixels[1] = false;
    }

    if (raise_button.GetValue() == mcu::io::Value::kLow) {
      pixels[2] = true;
    } else {
      pixels[2] = false;
    }

    if (lower_button.GetValue() == mcu::io::Value::kLow) {
      pixels[3] = true;
    } else {
      pixels[3] = false;
    }

    display->Update(pixels);

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
