#include <array>

#include "mcu/gpio.h"
#include "mcu/lpc822.h"

namespace mcu {

namespace {

enum class PioMode : uint32_t {
  kInactive = 0,
  kPullDown,
  kPullUp,
};

// Table that converts mcu::McuPin values to register offsets from the IOCON
// base register.
constexpr std::array<int, 29> kPioOffset {{
  17, // kPIO0_0
  11, // kPIO0_1
  6,  // kPIO0_2
  5,  // kPIO0_3
  4,  // kPIO0_4
  3,  // kPIO0_5
  16, // kPIO0_6
  15, // kPIO0_7
  14, // kPIO0_8
  13, // kPIO0_9
  8,  // kPIO0_10
  7,  // kPIO0_11
  2,  // kPIO0_12
  1,  // kPIO0_13
  18, // kPIO0_14
  10, // kPIO0_15
  9,  // kPIO0_16
  0,  // kPIO0_17
  30, // kPIO0_18
  29, // kPIO0_19
  28, // kPIO0_20
  27, // kPIO0_21
  26, // kPIO0_22
  25, // kPIO0_23
  24, // kPIO0_24
  23, // kPIO0_25
  22, // kPIO0_26
  21, // kPIO0_27
  20, // kPIO0_28
}};

constexpr uint8_t kBRegisterLogicHigh = 0x01;
constexpr uint8_t kBRegisterLogicLow = 0x00;

}  // namespace

Gpio::Gpio(McuPio pio)
    : port_pin_(static_cast<int>(pio)),
      pio_offset_(kPioOffset[static_cast<int>(pio)]) {}

void Gpio::SetDirectionToOutput() {
  GPIO->DIR[0] |= 0x1U << port_pin_;
}

void Gpio::SetDirectionToInput() {
  GPIO->DIR[0] &= ~(0x1U << port_pin_);
}

void Gpio::SetResistorMode(ResistorMode mode) {
  IOCON->PIO[pio_offset_] &= ~IOCON_PIO_MODE_MASK;

  if (mode == ResistorMode::kPullUp) {
    IOCON->PIO[pio_offset_] |=
        IOCON_PIO_MODE(static_cast<uint32_t>(ResistorMode::kPullUp));
  } else if (mode == ResistorMode::kPullDown) {
    IOCON->PIO[pio_offset_] |=
        IOCON_PIO_MODE(static_cast<uint32_t>(ResistorMode::kPullDown));
  } else {
    IOCON->PIO[pio_offset_] |=
        IOCON_PIO_MODE(static_cast<uint32_t>(ResistorMode::kNone));
  }
}

Gpio::Value Gpio::Read() {
  if (GPIO->B[0][port_pin_] == kBRegisterLogicHigh) {
    return Value::kHigh;
  } else {
    return Value::kLow;
  }
}

void Gpio::Set() {
  GPIO->SET[0] = 0x1U << port_pin_;
}

void Gpio::Clear() {
  GPIO->CLR[0] = 0x1U << port_pin_;
}

}  // mcu
