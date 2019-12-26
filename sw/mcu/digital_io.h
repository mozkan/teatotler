#ifndef MCU_DIGITAL_IO_H
#define MCU_DIGITAL_IO_H

#include <cstdint>

namespace mcu {
namespace io {

enum class ResistorMode : uint32_t {
  kNone = 0,
  kPullDown,
  kPullUp,
};

enum class Value {
  kLow = 0,
  kHigh,
};

class IDigitalInput {
 public:
  virtual void SetResistorMode(ResistorMode mode) = 0;
  virtual Value Read() = 0;
};

class IDigitalOutput {
 public:
  virtual void Set() = 0;
  virtual void Clear() = 0;
};

}  // namespace io
}  // namespace mcu

#endif /* MCU_DIGITAL_IO_H */
