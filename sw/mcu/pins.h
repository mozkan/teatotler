#ifndef MCU_PINS_H
#define MCU_PINS_H

#include <cstdint>

namespace mcu {

enum class McuPio : int {
  kPIO0_0 = 0,
  kPIO0_1,
  kPIO0_2,
  kPIO0_3,
  kPIO0_4,
  kPIO0_5,
  kPIO0_6,
  kPIO0_7,
  kPIO0_8,
  kPIO0_9,
  kPIO0_10,
  kPIO0_11,
  kPIO0_12,
  kPIO0_13,
  kPIO0_14,
  kPIO0_15,
  kPIO0_16,
  kPIO0_17,
  kPIO0_18,
  kPIO0_19,
  kPIO0_20,
  kPIO0_21,
  kPIO0_22,
  kPIO0_23,
  kPIO0_24,
  kPIO0_25,
  kPIO0_26,
  kPIO0_27,
  kPIO0_28
};

static constexpr int kMcuPioCount() {
  return static_cast<int>(McuPio::kPIO0_28);
}

}  // mcu

#endif /* MCU_PINS_H */
