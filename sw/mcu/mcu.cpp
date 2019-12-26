#include "mcu/mcu.h"

#include "mcu/lpc822.h"
#include "util/assert.h"

namespace mcu {

namespace {

constexpr int PioIndex(McuPio pio) {
  return static_cast<int>(pio);
}

}  // namespace

Mcu::Mcu() {
  for (auto& gpio : gpios_) {
    gpio = nullptr;
  }

  for (auto& channel : mrt_channels_) {
    channel = nullptr;
  }

  EnablePeripheralClocks();
  ResetPeripherals();
}

Mcu::~Mcu() {
  for (auto& gpio : gpios_) {
    if (gpio != nullptr) {
      delete gpio;
    }
  }

  for (auto& channel : mrt_channels_) {
    if (channel != nullptr) {
      delete channel;
    }
  }
}

io::IDigitalInput* Mcu::GetDigitalInput(McuPio pio) {
  Gpio* gpio = CreateGpio(pio);
  gpio->SetDirectionToInput();

  return gpio;
}

io::IDigitalOutput* Mcu::GetDigitalOutput(McuPio pio) {
  Gpio* gpio = CreateGpio(pio);
  gpio->SetDirectionToOutput();
  gpio->SetResistorMode(io::ResistorMode::kNone);

  return gpio;
}

IIntervalTimer* Mcu::GetIntervalTimer(int channel, uint32_t period_ticks) {
  // Multiple channels not supported right now.
  util::Assert(channel == 0);

  mrt_channels_[0] = new Mrt(period_ticks);

  return mrt_channels_[0];
}

uint32_t Mcu::SystemClockHz() {
  return 12U * 1000U * 1000U;
}

void Mcu::EnablePeripheralClocks() {
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_GPIO(1U);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_SWM(1U);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_SCT(1U);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_MRT(1U);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_UART0(1U);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_IOCON(1U);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_ACMP(1U);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_ADC(1U);
}

void Mcu::ResetPeripherals() {
  SYSCON->PRESETCTRL &= ~SYSCON_PRESETCTRL_GPIO_RST_N(1U);
  SYSCON->PRESETCTRL |= SYSCON_PRESETCTRL_GPIO_RST_N(1U);

  SYSCON->PRESETCTRL &= ~SYSCON_PRESETCTRL_SCT_RST_N(1U);
  SYSCON->PRESETCTRL |= SYSCON_PRESETCTRL_SCT_RST_N(1U);

  SYSCON->PRESETCTRL &= ~SYSCON_PRESETCTRL_MRT_RST_N(1U);
  SYSCON->PRESETCTRL |= SYSCON_PRESETCTRL_MRT_RST_N(1U);

  SYSCON->PRESETCTRL &= ~SYSCON_PRESETCTRL_UARTFRG_RST_N(1U);
  SYSCON->PRESETCTRL |=
      SYSCON_PRESETCTRL_UARTFRG_RST_N(1U);
  SYSCON->PRESETCTRL &= ~SYSCON_PRESETCTRL_UART0_RST_N(1U);
  SYSCON->PRESETCTRL |= SYSCON_PRESETCTRL_UART0_RST_N(1U);

  SYSCON->PRESETCTRL &= ~SYSCON_PRESETCTRL_ACMP_RST_N(1U);
  SYSCON->PRESETCTRL |= SYSCON_PRESETCTRL_ACMP_RST_N(1U);

  SYSCON->PRESETCTRL &= ~SYSCON_PRESETCTRL_ADC_RST_N(1U);
  SYSCON->PRESETCTRL |= SYSCON_PRESETCTRL_ADC_RST_N(1U);
}

Gpio* Mcu::CreateGpio(McuPio pio) {
  util::Assert(gpios_[PioIndex(pio)] == nullptr);

  Gpio* gpio = new Gpio(pio);
  gpios_[PioIndex(pio)] = gpio;

  return gpio;
}

}  // namespace mcu
