#include "mbed.h"
void updateTime();
void shiftOutMSBFirst(uint8_t value);
void wrttoshiftregister(uint8_t segments, uint8_t digit);
void dispnumb(int number, bool showDecimal = false, int decimalPos = -1);

