#include "app/steeper.h"
#include "mcu/mcu.h"
#include "system/teatotler.h"

int main(void) {
  mcu::Mcu mcu;
  sys::Teatotler teatotler(&mcu);
  application::Steeper steeper(&teatotler);

  sys::TickTimer* tick_timer = teatotler.GetTickTimer();
  uint32_t time = 0;

  while (1) {
    time = tick_timer->WaitUntilNextTick();

    teatotler.RunDrivers(time);
    steeper.Run(time);
  }

  return 0;
}

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

    display->Update(pixels);
#endif
