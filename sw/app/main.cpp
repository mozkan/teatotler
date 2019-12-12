#include <cr_section_macros.h>

#include "mcu/gpio.h"
#include "mcu/mcu.h"
#include "mcu/mrt.h"
#include "mcu/pins.h"

class BasicHBridge {
 public:
  BasicHBridge()
      : gpio_h_bridge_dir_a_(mcu::McuPio::kPIO0_0),
        gpio_h_bridge_dir_b_(mcu::McuPio::kPIO0_14),
        gpio_h_bridge_pwm_a_(mcu::McuPio::kPIO0_16),
        gpio_h_bridge_pwm_b_(mcu::McuPio::kPIO0_15) {
    Stop();

    gpio_h_bridge_dir_a_.SetDirectionToOutput();
    gpio_h_bridge_dir_a_.SetResistorMode(mcu::Gpio::ResistorMode::kNone);
    gpio_h_bridge_dir_b_.SetDirectionToOutput();
    gpio_h_bridge_dir_b_.SetResistorMode(mcu::Gpio::ResistorMode::kNone);
    gpio_h_bridge_pwm_a_.SetDirectionToOutput();
    gpio_h_bridge_pwm_a_.SetResistorMode(mcu::Gpio::ResistorMode::kNone);
    gpio_h_bridge_pwm_b_.SetDirectionToOutput();
    gpio_h_bridge_pwm_b_.SetResistorMode(mcu::Gpio::ResistorMode::kNone);
  }

  ~BasicHBridge() = default;

  void DriveForward() {
    Stop();
    gpio_h_bridge_dir_a_.Clear();
    gpio_h_bridge_pwm_b_.Set();
  }

  void DriveReverse() {
    Stop();
    gpio_h_bridge_dir_b_.Clear();
    gpio_h_bridge_pwm_a_.Set();
  }

  void Stop() {
    gpio_h_bridge_dir_a_.Set();
    gpio_h_bridge_dir_b_.Set();
    gpio_h_bridge_pwm_a_.Clear();
    gpio_h_bridge_pwm_b_.Clear();
  }

 private:
  mcu::Gpio gpio_h_bridge_dir_a_;
  mcu::Gpio gpio_h_bridge_dir_b_;
  mcu::Gpio gpio_h_bridge_pwm_a_;
  mcu::Gpio gpio_h_bridge_pwm_b_;
};

int main(void) {
  mcu::Mcu mcu;

  mcu::Gpio gpio_btn_debug(mcu::McuPio::kPIO0_12);
  gpio_btn_debug.SetDirectionToInput();
  gpio_btn_debug.SetResistorMode(mcu::Gpio::ResistorMode::kPullUp);

  mcu::Gpio gpio_btn_start(mcu::McuPio::kPIO0_4);
  gpio_btn_start.SetDirectionToInput();
  gpio_btn_start.SetResistorMode(mcu::Gpio::ResistorMode::kPullUp);

  mcu::Gpio gpio_btn_reset(mcu::McuPio::kPIO0_28);
  gpio_btn_reset.SetDirectionToInput();
  gpio_btn_reset.SetResistorMode(mcu::Gpio::ResistorMode::kPullUp);

  mcu::Gpio gpio_btn_raise(mcu::McuPio::kPIO0_24);
  gpio_btn_raise.SetDirectionToInput();
  gpio_btn_raise.SetResistorMode(mcu::Gpio::ResistorMode::kPullUp);

  mcu::Gpio gpio_btn_lower(mcu::McuPio::kPIO0_25);
  gpio_btn_lower.SetDirectionToInput();
  gpio_btn_lower.SetResistorMode(mcu::Gpio::ResistorMode::kPullUp);

  mcu::Gpio gpio_led_row1(mcu::McuPio::kPIO0_19);
  gpio_led_row1.Set();
  gpio_led_row1.SetDirectionToOutput();

  mcu::Gpio gpio_led_row2(mcu::McuPio::kPIO0_22);
  gpio_led_row2.Set();
  gpio_led_row2.SetDirectionToOutput();

  mcu::Gpio gpio_led_row3(mcu::McuPio::kPIO0_18);
  gpio_led_row3.Set();
  gpio_led_row3.SetDirectionToOutput();

  mcu::Gpio gpio_led_col1(mcu::McuPio::kPIO0_17);
  gpio_led_col1.Set();
  gpio_led_col1.SetDirectionToOutput();

  mcu::Gpio gpio_led_col2(mcu::McuPio::kPIO0_21);
  gpio_led_col2.Set();
  gpio_led_col2.SetDirectionToOutput();

  mcu::Gpio gpio_led_col3(mcu::McuPio::kPIO0_20);
  gpio_led_col3.Set();
  gpio_led_col3.SetDirectionToOutput();

  //BasicHBridge h_bridge;
  Mrt mrt(12000000);

  //volatile int i = 0;

  while(1) {
    //h_bridge.DriveForward();
    gpio_led_col1.Clear();
    //while (gpio_btn_lower.Read() != mcu::Gpio::Value::kLow) { i++; }
    while (!mrt.TimeoutOccurred()) {
    }

    //h_bridge.Stop();
    gpio_led_col1.Set();
    //while (gpio_btn_lower.Read() != mcu::Gpio::Value::kHigh) { i++; }
    while (!mrt.TimeoutOccurred()) {
    }

    //h_bridge.DriveReverse();
    gpio_led_col2.Clear();
    //while (gpio_btn_lower.Read() != mcu::Gpio::Value::kLow) { i++; }
    while (!mrt.TimeoutOccurred()) {
    }

    //h_bridge.Stop();
    gpio_led_col2.Set();
    //while (gpio_btn_lower.Read() != mcu::Gpio::Value::kHigh) { i++; }
    while (!mrt.TimeoutOccurred()) {
    }

    gpio_led_col3.Clear();
    //while (gpio_btn_lower.Read() != mcu::Gpio::Value::kLow) { i++; }
    while (!mrt.TimeoutOccurred()) {
    }

    gpio_led_col3.Set();
    //while (gpio_btn_lower.Read() != mcu::Gpio::Value::kHigh) { i++; }
    while (!mrt.TimeoutOccurred()) {
    }
  }

  return 0;
}
