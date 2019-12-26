#ifndef MCU_MCU_H
#define MCU_MCU_H

#include <cstdint>

#include <array>

#include "mcu/digital_io.h"
#include "mcu/gpio.h"
#include "mcu/interval_timer.h"
#include "mcu/mrt.h"
#include "mcu/pins.h"

namespace mcu {

class Mcu {
 public:
  Mcu();
  ~Mcu();

  io::IDigitalInput* GetDigitalInput(McuPio pio);
  io::IDigitalOutput* GetDigitalOutput(McuPio pio);
  IIntervalTimer* GetIntervalTimer(int channel, uint32_t period_ticks);

  static uint32_t SystemClockHz();

 private:
  void EnablePeripheralClocks();
  void ResetPeripherals();
  Gpio* CreateGpio(McuPio pio);

  std::array<Gpio*, kMcuPioCount()> gpios_;
  std::array<Mrt*, 1> mrt_channels_;
};

} //  namespace mcu

#endif /* MCU_MCU_H */
