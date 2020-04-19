#include "app/steeper_internal.h"

#include <array>

namespace application {
namespace steeper_internal {

namespace {

// Returns the saturated count value between the limits of 0 and max_counts.
void SaturateCounts(int* counts, int max_counts) {
  if (*counts > max_counts) {
    *counts = max_counts;
  } else if (*counts < 0) {
    *counts = 0;
  }
}

// Worker for the update display functions.
//
// Writes the display values to the given buffer based on the number of counts
// and the skip value.
// skip sets the number of pixels to skip when calculating which ones to set. A
// skip value of zero sets all the pixels contiguously, one for each count. A
// skip value of one sets every other pixel for a given count, etc.
void DoUpdateDisplay(
    int knob_rotation, std::array<bool, kMaxSteepTimeCounts>* buffer,
    int skip) {
  // Clear all the pixels first.
  for (int pixel = 0; pixel < sys::LinearDisplay::kPixelCount; pixel++) {
    (*buffer)[pixel] = false;
  }

  int counts = knob_rotation;

  // Set the "on" pixels based on the rotation and skip counts.
  for (int pixel = 0; pixel < counts; pixel++) {
    (*buffer)[pixel * (skip + 1)] = true;
  }
}

}  // namespace

void UpdateDisplayTime(
    int* knob_rotation, std::array<bool, kMaxSteepTimeCounts>* buffer) {
  // Saturate the value if there are more knob rotations than valid time counts
  // displayable on the indicator.
  SaturateCounts(knob_rotation, kMaxSteepTimeCounts);

  DoUpdateDisplay(*knob_rotation, buffer, 0);
}

void UpdateDisplayWholeCounts(
  int* knob_rotation, std::array<bool, kMaxSteepTimeCounts>* buffer) {
  // Saturate the value if there are more knob rotations than valid whole numbe
  // counts displayable on the indicator.
  SaturateCounts(knob_rotation, kMaxWholeNumberCounts);

  DoUpdateDisplay(*knob_rotation, buffer, 1);
}

uint32_t SteepCountsToMilliseconds(int counts) {
  return ((counts - 1) * 1000 * kSecondsPerSteepTimeIncrement) +
         kDisplayTimeStartingOffsetMs;
}

int MillisecondsToSteepCounts(uint32_t steep_time_ms) {
  return steep_time_ms / 1000 / kSecondsPerSteepTimeIncrement;
}

}  // namespace steeper_internal
}  // namespace application
