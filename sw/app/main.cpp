#include <cr_section_macros.h>

#include "mcu/gpio.h"
#include "mcu/mcu.h"
#include "mcu/pins.h"

int main(void) {
  mcu::Mcu mcu;

  mcu::Gpio gpio_btn_debug(mcu::McuPio::kPIO0_12);
  gpio_btn_debug.SetDirectionToInput();
  gpio_btn_debug.SetResistorMode(mcu::Gpio::ResistorMode::kPullUp);

  mcu::Gpio gpio_btn_start(mcu::McuPio::kPIO0_4);
  gpio_btn_start.SetDirectionToInput();
  gpio_btn_start.SetResistorMode(mcu::Gpio::ResistorMode::kPullUp);

  mcu::Gpio gpio_btn_reset(mcu::McuPio::kPIO0_28);
  gpio_btn_reset.SetDirectionToInput();
  gpio_btn_reset.SetResistorMode(mcu::Gpio::ResistorMode::kPullUp);

  mcu::Gpio gpio_btn_raise(mcu::McuPio::kPIO0_24);
  gpio_btn_raise.SetDirectionToInput();
  gpio_btn_raise.SetResistorMode(mcu::Gpio::ResistorMode::kPullUp);

  mcu::Gpio gpio_btn_lower(mcu::McuPio::kPIO0_25);
  gpio_btn_lower.SetDirectionToInput();
  gpio_btn_lower.SetResistorMode(mcu::Gpio::ResistorMode::kPullUp);

  volatile int i = 0;

  while(1) {
    while (gpio_btn_lower.Read() != mcu::Gpio::Value::kLow) {
      i++;
    }

    while (gpio_btn_lower.Read() != mcu::Gpio::Value::kHigh) {
      i++;
    }
  }

  return 0;
}
