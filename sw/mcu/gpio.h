#ifndef MCU_GPIO_H
#define MCU_GPIO_H

#include "mcu/digital_io.h"
#include "mcu/pins.h"

namespace mcu {

class Gpio : public io::IDigitalInput, public io::IDigitalOutput {
 public:
  Gpio(McuPio pio);
  virtual ~Gpio() = default;
  void SetDirectionToOutput();
  void SetDirectionToInput();
  void SetResistorMode(io::ResistorMode mode) override;
  io::Value Read() override;
  void Set() override;
  void Clear() override;

 private:
   int port_pin_;
   int pio_offset_;
};

}  // mcu

#endif /* MCU_GPIO_H */
