#include "mcu/mrt.h"

#include "mcu/lpc822.h"

// Only use channel 0 in this implementation.
Mrt::Mrt(uint32_t period) : channel_(0) {
  // Loads period into the INTVAL:IVALUE counter. Forces the load to take effect
  // immediately.
  MRT0->CHANNEL[channel_].INTVAL = period & 0x7FFFFFFF;
  MRT0->CHANNEL[channel_].INTVAL |= MRT_CHANNEL_INTVAL_LOAD(1);

  MRT0->CHANNEL[channel_].CTRL = 0;
  // Enable timer interrupt.
  MRT0->CHANNEL[channel_].CTRL |= MRT_CHANNEL_CTRL_INTEN(1);
  // Select repeat interrupt mode.
  MRT0->CHANNEL[channel_].CTRL |= MRT_CHANNEL_CTRL_MODE(0);
}

bool Mrt::TimerExpired() {
  bool timeout_occurred = false;

  if (MRT0->CHANNEL[channel_].STAT & MRT_CHANNEL_STAT_INTFLAG_MASK) {
    MRT0->CHANNEL[channel_].STAT |= MRT_CHANNEL_STAT_INTFLAG_MASK;
    timeout_occurred = true;
  }

  return timeout_occurred;
}
