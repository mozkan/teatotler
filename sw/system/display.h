#ifndef SYSTEM_DISPLAY_H
#define SYSTEM_DISPLAY_H

#include <cstdint>

#include <array>

#include "mcu/digital_io.h"
#include "util/periodic_task.h"

namespace sys {

class LinearDisplay : public util::PeriodicTask {
 public:
  static constexpr int kRows = 3;
  static constexpr int kColumns = 3;
  static constexpr int kPixelCount = kRows * kColumns;

  // rows should be ordered such that they drive adjacent pixels.
  // columns should be ordered such that they drive adjacent groupings of rows.
  LinearDisplay(uint32_t run_interval,
                std::array<mcu::io::IDigitalOutput*, kRows> rows,
                std::array<mcu::io::IDigitalOutput*, kColumns> columns);
  virtual ~LinearDisplay() = default;

  void Update(const std::array<bool, kPixelCount>& image);

 private:
  void DoRunIteration(uint32_t time_ms) override;
  void IncrementColumn();
  void EnergizeNextColumn();
  void ClearRows();
  void DriveRows();

  std::array<mcu::io::IDigitalOutput*, kRows> rows_;
  std::array<mcu::io::IDigitalOutput*, kColumns> columns_;
  std::array<bool, kPixelCount> image_buffer_;

  int active_column_;
  uint32_t last_update_ms_;
};

}  // namespace sys

#endif /* SYSTEM_DISPLAY_H */
