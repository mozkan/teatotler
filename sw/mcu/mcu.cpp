#include "mcu/lpc822.h"
#include "mcu/mcu.h"

namespace mcu {

namespace {

constexpr uint32_t kClockEnable = 1;
constexpr uint32_t kClockDisable = 0;

}  // namespace

Mcu::Mcu() {
  EnablePeripheralClocks();
}

void Mcu::EnablePeripheralClocks() {
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_GPIO(kClockEnable);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_SWM(kClockEnable);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_SCT(kClockEnable);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_MRT(kClockEnable);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_UART0(kClockEnable);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_IOCON(kClockEnable);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_ACMP(kClockEnable);
  SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_ADC(kClockEnable);
}

}  // namespace mcu
