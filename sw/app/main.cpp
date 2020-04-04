#include <cr_section_macros.h>

#include <array>

#include "app/steeper.h"
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

int main(void) {
  sys::Teatotler teatotler;
  application::Steeper steeper(&teatotler);

  uint32_t time = 0;

  while (1) {
    bool expired;
    teatotler.WaitUntilNextTick(&expired);
    (void)expired; // Temporary.

    time++;

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

    //display->Update(pixels);

    teatotler.RunDrivers(time);
    steeper.Run(time);
  }

  return 0;
}
