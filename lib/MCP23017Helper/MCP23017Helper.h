/**
 * MCP23017Helper.h
 *
 * Simplified wrapper for MCP23017 I/O expander
 * Uses Adafruit MCP23017 library underneath
 *
 * Provides easy button reading with debouncing
 *
 * Author: ON7KGK Michael
 */

#ifndef MCP23017_HELPER_H
#define MCP23017_HELPER_H

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

#define MCP_DEBOUNCE_MS  50  // Debounce time for buttons

class MCP23017Helper {
public:
    // Constructor
    MCP23017Helper();

    // Initialization
    bool begin(uint8_t i2cAddress = 0x20);

    // Pin configuration
    void pinMode(uint8_t pin, uint8_t mode);  // pin 0-15, mode INPUT/OUTPUT/INPUT_PULLUP

    // Digital I/O
    bool digitalRead(uint8_t pin);
    void digitalWrite(uint8_t pin, bool value);

    // Button reading with debouncing
    bool buttonPressed(uint8_t pin);          // Returns true when button first pressed
    bool buttonReleased(uint8_t pin);         // Returns true when button released
    bool buttonState(uint8_t pin);            // Returns current debounced state

    // Update (call in loop)
    void update();                            // Update button states

private:
    Adafruit_MCP23X17 _mcp;
    bool _initialized;

    // Button state tracking (for 16 pins)
    bool _lastState[16];
    bool _currentState[16];
    uint32_t _lastChangeTime[16];

    // Helper
    bool readPinDebounced(uint8_t pin);
};

#endif // MCP23017_HELPER_H
