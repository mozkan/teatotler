#ifndef SYSTEM_TEATOTLER_H
#define SYSTEM_TEATOTLER_H

#include <cstdint>

#include <array>

#include "mcu/digital_io.h"
#include "mcu/mcu.h"
#include "system/hbridge.h"
#include "system/display.h"
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
  // PanelButtons* GetPanelButtons();
  // RotaryKnob* GetRotaryKnob();
  //DEBUG
  mcu::io::IDigitalInput* GetInput(mcu::McuPio input) {
    return mcu_.GetDigitalInput(input);
  }
  //END DEBUG

 private:
  mcu::Mcu mcu_;
  mcu::IIntervalTimer* timer_;

  BasicHBridge h_bridge_;
  LinearDisplay display_;
  //PanelButtons panel_buttons_;
  // RotaryKnob

  std::array<util::PeriodicTask*, 2> driver_tasks_;
};

}  // namespace sys

#endif /* SYSTEM_TEATOTLER_H */
