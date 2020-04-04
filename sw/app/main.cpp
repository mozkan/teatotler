#include <cr_section_macros.h>

#include <array>

#include "mcu/digital_io.h"
#include "mcu/interval_timer.h"
#include "mcu/mcu.h"
#include "mcu/pins.h"
#include "system/display.h"
#include "system/hbridge.h"
#include "system/panel_buttons.h"
#include "system/teatotler.h"

using namespace mcu;

int main(void) {
  sys::Teatotler teatotler;

#if 0
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
  //sys::BasicHBridge* hbridge = teatotler.GetHBridge();
  sys::PanelButtons* panel = teatotler.GetPanelButtons();

  std::array<bool, 9> pixels = {false};
  uint32_t time = 0;
  //int i = 0;

  while (1) {
    bool expired;
    teatotler.WaitUntilNextTick(&expired);
    (void)expired; // Temporary.

    time++;

    if (panel->CheckPressedButton() ==
        sys::PanelButtons::Button::kStart) {
      pixels[0] = true;
    } else if (panel->CheckPressedButton() ==
               sys::PanelButtons::Button::kReset) {
      pixels[1] = true;
    } else if (panel->CheckPressedButton() ==
               sys::PanelButtons::Button::kRaise) {
      pixels[2] = true;
    } else if (panel->CheckPressedButton() ==
               sys::PanelButtons::Button::kLower) {
      pixels[3] = true;
    } else {
      pixels[0] = false;
      pixels[1] = false;
      pixels[2] = false;
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
