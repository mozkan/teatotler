#include "system/panel_buttons.h"

namespace sys {

PanelButtons::PanelButtons(uint32_t run_interval,
                           mcu::io::IDigitalInput* start_button,
                           mcu::io::IDigitalInput* reset_button,
                           mcu::io::IDigitalInput* raise_button,
                           mcu::io::IDigitalInput* lower_button)
    : PeriodicTask(run_interval),
      buttons_{{ { start_button, PanelButtons::Button::kStart },
                 { reset_button, PanelButtons::Button::kReset },
                 { raise_button, PanelButtons::Button::kRaise },
                 { lower_button, PanelButtons::Button::kLower } }} {}

PanelButtons::Button PanelButtons::CheckPressedButton() {
  // Return the first pressed button detected.
  for (auto& button : buttons_) {
    // Pressed buttons read low because the board uses pullups on all buttons.
    if (button.debounced_input.GetValue() == mcu::io::Value::kLow) {
      return button.button_id;
    }
  }

  return PanelButtons::Button::kNone;
}

void PanelButtons::DoRunIteration(uint32_t time_ms) {
  for (auto& button : buttons_) {
    button.debounced_input.Sample(time_ms);
  }
}

}  // namespace sys
