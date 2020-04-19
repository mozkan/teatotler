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
