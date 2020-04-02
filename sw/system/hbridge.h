#ifndef SYSTEM_HBRIDGE_H
#define SYSTEM_HBRIDGE_H

#include "mcu/digital_io.h"
#include "util/periodic_task.h"

namespace sys {

class BasicHBridge : public util::PeriodicTask {
 public:
  BasicHBridge(uint32_t run_interval,
               mcu::io::IDigitalOutput* dir_a, mcu::io::IDigitalOutput* dir_b,
               mcu::io::IDigitalOutput* pwm_a, mcu::io::IDigitalOutput* pwm_b);
  ~BasicHBridge() = default;

  void DriveForward();
  void DriveReverse();
  void Stop();

 private:
  void DoRunIteration(uint32_t time_ms) override;

  mcu::io::IDigitalOutput* dir_a_;
  mcu::io::IDigitalOutput* dir_b_;
  mcu::io::IDigitalOutput* pwm_a_;
  mcu::io::IDigitalOutput* pwm_b_;
};

}  // namespace sys

#endif /* SYSTEM_HBRIDGE_H */
