#include <cr_section_macros.h>

#include <array>

#include "mcu/digital_io.h"
#include "mcu/interval_timer.h"
#include "mcu/mcu.h"
#include "mcu/pins.h"
#include "system/debounced_input.h"
#include "system/display.h"
#include "system/hbridge.h"
#include "system/panel_buttons.h"
#include "system/rotary_switch.h"
#include "system/teatotler.h"

using namespace mcu;

int main(void) {
  sys::Teatotler teatotler;

  sys::LinearDisplay* display = teatotler.GetDisplay();
  //sys::BasicHBridge* hbridge = teatotler.GetHBridge();
  //sys::PanelButtons* panel = teatotler.GetPanelButtons();
  sys::RotarySwitch* rotary_switch = teatotler.GetRotarySwitch();

  std::array<bool, 9> pixels = {false};
  uint32_t time = 0;
  //int i = 0;
  int rotation = 0;

  while (1) {
    bool expired;
    teatotler.WaitUntilNextTick(&expired);
    (void)expired; // Temporary.

    time++;

    rotation += rotary_switch->GetRotation();
    int pixel_limit = rotation;
    if (pixel_limit > 9) { pixel_limit = 9; }
    if (pixel_limit < 0) { pixel_limit = 0; }
    int pixel ;
    for (pixel = 0; pixel < pixel_limit; pixel++) { pixels[pixel] = true; }
    for (pixel = pixel_limit; pixel < 9; pixel++) { pixels[pixel] = false; }

#if 0
    if (rotary_channel_a.GetValue() == mcu::io::Value::kLow) {
      pixels[0] = true;
    } else {
      pixels[0] = false;
    }

    if (rotary_channel_b.GetValue() == mcu::io::Value::kLow) {
      pixels[1] = true;
    } else {
      pixels[1] = false;
    }
#endif

#if 0
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
#endif

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

    display->Update(pixels);
    teatotler.RunDrivers(time);
  }

  return 0;
}
