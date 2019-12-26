#include <cr_section_macros.h>

#include "mcu/mcu.h"
#include "mcu/mrt.h"
#include "mcu/pins.h"
#include "system/hbridge.h"

using namespace mcu;

int main(void) {
  Mcu mcu;

  io::IDigitalInput* debug_btn = mcu.GetDigitalInput(McuPio::kPIO0_10);
  debug_btn->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalInput* start_btn = mcu.GetDigitalInput(McuPio::kPIO0_4);
  start_btn->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalInput* reset_btn = mcu.GetDigitalInput(McuPio::kPIO0_11);
  reset_btn->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalInput* raise_btn = mcu.GetDigitalInput(McuPio::kPIO0_6);
  raise_btn->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalInput* lower_btn = mcu.GetDigitalInput(McuPio::kPIO0_23);
  lower_btn->SetResistorMode(io::ResistorMode::kPullUp);

  io::IDigitalOutput* led_row1 = mcu.GetDigitalOutput(McuPio::kPIO0_25);
  led_row1->Set();

  io::IDigitalOutput* led_row2 = mcu.GetDigitalOutput(McuPio::kPIO0_1);
  led_row2->Set();

  io::IDigitalOutput* led_row3 = mcu.GetDigitalOutput(McuPio::kPIO0_26);
  led_row3->Set();

  io::IDigitalOutput* led_col1 = mcu.GetDigitalOutput(McuPio::kPIO0_27);
  led_col1->Set();

  io::IDigitalOutput* led_col2 = mcu.GetDigitalOutput(McuPio::kPIO0_15);
  led_col2->Set();

  io::IDigitalOutput* led_col3 = mcu.GetDigitalOutput(McuPio::kPIO0_24);
  led_col3->Set();

  BasicHBridge h_bridge(mcu.GetDigitalOutput(McuPio::kPIO0_9),
                        mcu.GetDigitalOutput(McuPio::kPIO0_8),
                        mcu.GetDigitalOutput(McuPio::kPIO0_19),
                        mcu.GetDigitalOutput(McuPio::kPIO0_20));

  Mrt mrt(12000000);

  //volatile int i = 0;

  while(1) {
    h_bridge.DriveForward();
    led_col1->Clear();
    //while (lower_btn.Read() != io::Value::kLow) { i++; }
    while (!mrt.TimerExpired()) {
    }

    h_bridge.Stop();
    led_col1->Set();
    //while (lower_btn.Read() != io::Value::kHigh) { i++; }
    while (!mrt.TimerExpired()) {
    }

    h_bridge.DriveReverse();
    led_col2->Clear();
    //while (lower_btn.Read() != io::Value::kLow) { i++; }
    while (!mrt.TimerExpired()) {
    }

    h_bridge.Stop();
    led_col2->Set();
    //while (lower_btn.Read() != io::Value::kHigh) { i++; }
    while (!mrt.TimerExpired()) {
    }

    led_col3->Clear();
    //while (lower_btn.Read() != io::Value::kLow) { i++; }
    while (!mrt.TimerExpired()) {
    }

    led_col3->Set();
    //while (lower_btn.Read() != io::Value::kHigh) { i++; }
    while (!mrt.TimerExpired()) {
    }
  }

  return 0;
}
