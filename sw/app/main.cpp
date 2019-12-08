#include <cr_section_macros.h>

#include "mcu/gpio.h"
#include "mcu/mcu.h"
#include "mcu/pins.h"

int main(void) {
  mcu::Mcu mcu;

  mcu::Gpio gpio_btn_debug(mcu::McuPio::kPIO0_12);
  gpio_btn_debug.SetDirectionToInput();
  gpio_btn_debug.SetResistorMode(mcu::Gpio::ResistorMode::kPullUp);

  volatile int i = 0;

  while(1) {
    while (gpio_btn_debug.Read() != mcu::Gpio::Value::kLow) {
      i++;
    }

    while (gpio_btn_debug.Read() != mcu::Gpio::Value::kHigh) {
      i++;
    }
  }

  return 0 ;
}
