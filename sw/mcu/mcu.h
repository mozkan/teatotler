#ifndef MCU_MCU_H
#define MCU_MCU_H

#include <array>

#include "mcu/digital_io.h"
#include "mcu/gpio.h"
#include "mcu/pins.h"

namespace mcu {

class Mcu {
 public:
  Mcu();
  ~Mcu();

 io::IDigitalInput* GetDigitalInput(McuPio pio);
 io::IDigitalOutput* GetDigitalOutput(McuPio pio);
 //IIntervalTimer* GetIntervalTimer(int channel);

 private:
  void EnablePeripheralClocks();
  void ResetPeripherals();
  Gpio* CreateGpio(McuPio pio);

  std::array<Gpio*, kMcuPioCount()> gpios_;
};

} //  namespace mcu

#endif /* MCU_MCU_H */
