#ifndef MCU_MCU_H
#define MCU_MCU_H

#include <array>
#include <bitset>

#include "mcu/pins.h"

namespace mcu {

class Mcu {
 public:
  Mcu();
  ~Mcu() = default;

 //IDigitalInput* GetDigitalInput(McuPio pio);
 //IDigitalOutput* GetDigitalInput(McuPio pio);
 //IIntervalTimer* GetIntervalTimer(int channel);

 private:
  void EnablePeripheralClocks();
  void ResetPeripherals();

  std::bitset<kMcuPioCount()> pios_;
};

} //  namespace mcu

#endif /* MCU_MCU_H */
