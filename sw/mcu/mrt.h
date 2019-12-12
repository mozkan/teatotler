#ifndef MCU_MRT_H
#define MCU_MRT_H

#include <cstdint>

//#include <functional>

// Multi-rate timer peripheral driver. One channel.
class Mrt {
public:
 // Creates an MRT driver that starts counting from period down to zero at the
 // system clock frequency. Reloads the timer when zero is reached. Starts
 // immediately after construction.
 //
 // Valid range for period is 0 to 0x7FFFFFFF.
 Mrt(uint32_t period);

 // Returns true when the counter reaches zero. Resets on read of a successful
 // timeout (subsequent calls will return false until the counter reaches zero
 // again).
 bool TimeoutOccurred();

private:
 int channel_;
};

#endif /* MCU_MRT_H */
