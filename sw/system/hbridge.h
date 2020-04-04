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
  virtual ~BasicHBridge() = default;

  void DriveForward();
  void DriveReverse();
  void Stop();

 private:
  enum class RunState {
    kForward,
    kReverse,
    kDeadBand,
    kStopped,
  };

  void DoRunIteration(uint32_t time_ms) override;
  void SetForwardOutputs();
  void SetReverseOutputs();
  void SetStopOutputs();

  RunState current_run_state_;
  RunState next_run_state_;

  uint32_t deadband_start_ms_;

  mcu::io::IDigitalOutput* dir_a_;
  mcu::io::IDigitalOutput* dir_b_;
  mcu::io::IDigitalOutput* pwm_a_;
  mcu::io::IDigitalOutput* pwm_b_;
};

}  // namespace sys

#endif /* SYSTEM_HBRIDGE_H */
