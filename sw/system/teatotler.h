#ifndef SYSTEM_TEATOTLER_H
#define SYSTEM_TEATOTLER_H

#include <cstdint>

#include <array>

#include "mcu/digital_io.h"
#include "mcu/mcu.h"
#include "system/hbridge.h"
#include "system/display.h"
#include "system/panel_buttons.h"
#include "system/rotary_switch.h"
#include "util/periodic_task.h"

namespace sys {

class Teatotler {
 public:
  Teatotler();
  ~Teatotler() = default;

  void WaitUntilNextTick(bool* deadline_missed);
  void RunDrivers(uint32_t time_ms);

  BasicHBridge* GetHBridge();
  LinearDisplay* GetDisplay();
  PanelButtons* GetPanelButtons();
  RotarySwitch* GetRotarySwitch();

 private:
  mcu::Mcu mcu_;
  mcu::IIntervalTimer* timer_;

  BasicHBridge h_bridge_;
  LinearDisplay display_;
  PanelButtons panel_buttons_;
  RotarySwitch rotary_switch_;

  std::array<util::PeriodicTask*, 4> driver_tasks_;
};

}  // namespace sys

#endif /* SYSTEM_TEATOTLER_H */
