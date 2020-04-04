#ifndef APP_STEEPER_H
#define APP_STEEPER_H

#include <cstdint>

#include <array>

#include "system/hbridge.h"
#include "system/display.h"
#include "system/panel_buttons.h"
#include "system/rotary_switch.h"
#include "system/teatotler.h"
#include "util/periodic_task.h"

namespace application {

class Steeper : public util::PeriodicTask {
 public:
  Steeper(sys::Teatotler* teatotler);
  virtual ~Steeper() = default;

 private:
  void DoRunIteration(uint32_t time_ms) override;

  sys::BasicHBridge* winch_drive_;
  sys::LinearDisplay* time_indicator_;
  sys::PanelButtons* buttons_;
  sys::RotarySwitch* knob_;

  //std::array<bool, 9> time_points_ = {false};
  std::array<bool, 9> pixels = {false};
  int rotation = 0;
};

}  // namespace application

#endif /* APP_STEEPER_H */
