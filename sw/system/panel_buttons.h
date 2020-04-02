#ifndef SYSTEM_PANEL_BUTTONS_H
#define SYSTEM_PANEL_BUTTONS_H

#include <cstdint>

#include <array>

#include "mcu/digital_io.h"
#include "system/debounced_input.h"
#include "util/periodic_task.h"

namespace sys {

// Class to aggregate all front panel buttons. Provides a simple interface to
// detect one button press at a time.
class PanelButtons : public util::PeriodicTask {
 public:
  enum class Button {
    kStart = 0,
    kReset,
    kRaise,
    kLower,
    kNone,
  };

  PanelButtons(uint32_t run_interval,
               mcu::io::IDigitalInput* start_button,
               mcu::io::IDigitalInput* reset_button,
               mcu::io::IDigitalInput* raise_button,
               mcu::io::IDigitalInput* lower_button);
  ~PanelButtons() = default;

  // Returns a Button representing one of the panel buttons in the pressed state
  // or kNone if no buttons are pressed.
  Button CheckPressedButton();

 private:
  struct InputAndButton {
    DebouncedInput debounced_input;
    Button button_id;
  };

  void DoRunIteration(uint32_t time_ms) override;

  std::array<InputAndButton, 4> buttons_;
};

}  // namespace sys

#endif /* SYSTEM_PANEL_BUTTONS_H */
