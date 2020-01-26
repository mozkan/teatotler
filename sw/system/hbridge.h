#ifndef SYSTEM_HBRIDGE_H
#define SYSTEM_HBRIDGE_H

#include "mcu/digital_io.h"

namespace sys {

// TODO: Inherit from IManagedDriver.
class BasicHBridge {
 public:
  BasicHBridge(mcu::io::IDigitalOutput* dir_a, mcu::io::IDigitalOutput* dir_b,
               mcu::io::IDigitalOutput* pwm_a, mcu::io::IDigitalOutput* pwm_b);
  ~BasicHBridge() = default;

  //void Run(uint32_t time_ms) override;

  void DriveForward();
  void DriveReverse();
  void Stop();

 private:
  mcu::io::IDigitalOutput* dir_a_;
  mcu::io::IDigitalOutput* dir_b_;
  mcu::io::IDigitalOutput* pwm_a_;
  mcu::io::IDigitalOutput* pwm_b_;
};

}  // namespace sys

#endif /* SYSTEM_HBRIDGE_H */
