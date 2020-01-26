#include "system/display.h"

namespace sys {

constexpr int LinearDisplay::kRows;
constexpr int LinearDisplay::kColumns;

LinearDisplay::LinearDisplay(
    uint32_t run_interval,
    std::array<mcu::io::IDigitalOutput*, kRows> rows,
    std::array<mcu::io::IDigitalOutput*, kColumns> columns)
    : ManagedDriver(run_interval),
      rows_(rows), columns_(columns), active_column_(0), last_update_ms_(0) {
  ClearRows();

  for (auto& column : columns_) {
    column->Set();
  }

  for (auto& pixel : image_buffer_) {
    pixel = false;
  }
}

void LinearDisplay::Update(const std::array<bool, kPixelCount>& image) {
  image_buffer_ = image;
}

void LinearDisplay::DoRunIteration(uint32_t /*time_ms*/) {
  ClearRows();
  EnergizeNextColumn();
  DriveRows();
}

void LinearDisplay::IncrementColumn() {
  active_column_++;
  if (active_column_ >= kColumns) {
    active_column_ = 0;
  }
}

void LinearDisplay::EnergizeNextColumn() {
  columns_[active_column_]->Set();
  IncrementColumn();
  columns_[active_column_]->Clear();
}

void LinearDisplay::ClearRows() {
  for (auto& row : rows_) {
    row->Clear();
  }
}

void LinearDisplay::DriveRows() {
  int row_offset = active_column_ * kRows;
  for (int row = 0; row < kRows; row++) {
    if (image_buffer_[row_offset + row]) {
      rows_[row]->Set();
    } else {
      rows_[row]->Clear();
    }
  }
}

}  // namespace sys
