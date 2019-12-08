#ifndef MCU_GPIO_H
#define MCU_GPIO_H

#include <cstdint>

#include "mcu/pins.h"

namespace mcu {

class Gpio {
 public:
  enum class ResistorMode : uint32_t {
    kNone = 0,
    kPullDown,
    kPullUp,
  };

  enum class Value {
    kLow = 0,
    kHigh,
  };

  Gpio(McuPio pio);
  ~Gpio() = default;
  void SetDirectionToOutput();
  void SetDirectionToInput();
  void SetResistorMode(ResistorMode mode);
  Value Read();
  void Set();
  void Clear();

 private:
   int port_pin_;
   int pio_offset_;
};

}  // mcu

#endif /* MCU_GPIO_H */
