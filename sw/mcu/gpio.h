#ifndef MCU_GPIO_H
#define MCU_GPIO_H

#include "mcu/pinmux.h"
#include "mcu/pins.h"

namespace mcu {

class Gpio {
 public:
  Gpio(McuPin pin, PinMux& mux);
  ~Gpio() = default;
  void SetDirectionToOutput();
  void SetDirectionToInput();
  void EnablePullup();
  void DisablePullup();

 private:
};

}  // mcu

#endif /* MCU_GPIO_H */
