#include "mbed.h"
#include "func.h"

// Shift register pins (common anode)
DigitalOut latchPin(D4);
DigitalOut clockPin(D7);
DigitalOut dataPin(D8);

// Buttons (switch1 for reset, switch2 for voltage display)
DigitalIn switch1(A1), switch2(A3);

// ADC for potentiometer (A0)
AnalogIn pot(A0);

// Common anode segment patterns (0-9 with decimal point)
const uint8_t digitPattern[10] = {
    static_cast<uint8_t>(~0x3F), // 0
    static_cast<uint8_t>(~0x06), // 1
    static_cast<uint8_t>(~0x5B), // 2
    static_cast<uint8_t>(~0x4F), // 3
    static_cast<uint8_t>(~0x66), // 4
    static_cast<uint8_t>(~0x6D), // 5
    static_cast<uint8_t>(~0x7D), // 6
    static_cast<uint8_t>(~0x07), // 7
    static_cast<uint8_t>(~0x7F), // 8
    static_cast<uint8_t>(~0x6F)  // 9
};

// Digit positions (left to right)
const uint8_t digitPos[4] = {0x01, 0x02, 0x04, 0x08};

// Timer and voltage variables
volatile int seconds = 0, minutes = 0;
volatile float minVoltage = 3.3f, maxVoltage = 0.0f;
Ticker timerTicker;


int main() {
    // Initialize hardware
    switch1.mode(PullUp);
    switch2.mode(PullUp);
    timerTicker.attach(&updateTime, 1.0f); // 1-second updates

    while (1) {
        // Reset timer on switch1 press
        if (!switch1) {
            seconds = 0;
            minutes = 0;
            ThisThread::sleep_for(200ms); // Debounce
        }

        // Read potentiometer voltage
        float voltage = pot.read() * 3.3f; // Convert to volts (0-3.3V)

        // Update min/max voltages
        if (voltage < minVoltage) minVoltage = voltage;
        if (voltage > maxVoltage) maxVoltage = voltage;

        // Display voltage if switch2 pressed, else display time
        if (!switch2) {
            int voltageScaled = static_cast<int>(voltage * 100); // e.g., 2.45V → 245
            dispnumb(voltageScaled, true, 1); // Show as X.XX
        } else {
            int timeDisplay = minutes * 100 + seconds; // MMSS format
            dispnumb(timeDisplay);
        }
    }
}