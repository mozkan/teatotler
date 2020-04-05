#include "app/steeper_internal.h"

#include <array>

namespace application {
namespace steeper_internal {

void UpdateDisplayParameters(
    int* knob_rotation, std::array<bool, kMaxSteepTimeCounts>* buffer) {
  // Saturate the value if there are more knob rotations than valid counts
  // displayable on the indicator.
  if (*knob_rotation > kMaxSteepTimeCounts) {
    *knob_rotation = kMaxSteepTimeCounts;
  } else if (*knob_rotation < 0) {
    *knob_rotation = 0;
  }

  int steep_time_counts = *knob_rotation;

  // Set the indicator's "on" pixels.
  int steep_time_increment;
  for (steep_time_increment = 0; steep_time_increment < steep_time_counts;
       steep_time_increment++) {
    (*buffer)[steep_time_increment] = true;
  }

  // Set the remaining "off" pixels.
  for (steep_time_increment = steep_time_counts;
       steep_time_increment < kMaxSteepTimeCounts;
       steep_time_increment++) {
    (*buffer)[steep_time_increment] = false;
  }
}

uint32_t SteepCountsToMilliseconds(int counts) {
  return counts * 1000 * kMillisecondsPerSteepTimeIncrement;
}

int MillisecondsToSteepCounts(uint32_t steep_time_ms) {
  return steep_time_ms / 1000 / kMillisecondsPerSteepTimeIncrement;
}

}  // namespace steeper_internal
}  // namespace application
