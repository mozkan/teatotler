#include <cr_section_macros.h>

#include "mcu/gpio.h"
#include "mcu/pinmux.h"
#include "mcu/pins.h"

int main(void) {
  mcu::PinMux mux;

  mcu::Gpio gpio_btn_debug(mcu::McuPin::kPIO0_12, mux);
  gpio_btn_debug.SetDirectionToInput();
  gpio_btn_debug.EnablePullup();

  volatile static int i = 0 ;
  while(1) {
      i++ ;
  }

  return 0 ;
}
