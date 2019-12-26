#ifndef MCU_INTERVAL_TIMER_H
#define MCU_INTERVAL_TIMER_H

namespace mcu {

class IIntervalTimer {
 public:
  virtual bool TimerExpired() = 0;
};

}  // namespace mcu

#endif /* MCU_INTERVAL_TIMER_H */
