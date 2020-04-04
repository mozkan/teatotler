#include "app/steeper.h"

#include "system/teatotler.h"

namespace application {

namespace {

constexpr uint32_t kSteeperRunIntervalMs = 10;

}  // namespace

Steeper::Steeper(sys::Teatotler* teatotler)
    : PeriodicTask(kSteeperRunIntervalMs),
      winch_drive_(teatotler->GetHBridge()),
      time_indicator_(teatotler->GetDisplay()),
      buttons_(teatotler->GetPanelButtons()),
      knob_(teatotler->GetRotarySwitch()) {}

void Steeper::DoRunIteration(uint32_t /*time_ms*/) {
  rotation += knob_->GetRotation();
  int pixel_limit = rotation;
  if (pixel_limit > 9) { pixel_limit = 9; }
  if (pixel_limit < 0) { pixel_limit = 0; }
  int pixel ;
  for (pixel = 0; pixel < pixel_limit; pixel++) { pixels[pixel] = true; }
  for (pixel = pixel_limit; pixel < 9; pixel++) { pixels[pixel] = false; }

  time_indicator_->Update(pixels);
}

}  // namespace application
