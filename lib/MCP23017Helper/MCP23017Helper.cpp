/**
 * MCP23017Helper.cpp
 *
 * Implementation of simplified MCP23017 wrapper
 *
 * Author: ON7KGK Michael
 */

#include "MCP23017Helper.h"

// ===================================================================
// Constructor
// ===================================================================
MCP23017Helper::MCP23017Helper() {
    _initialized = false;

    // Initialize button state arrays
    for (int i = 0; i < 16; i++) {
        _lastState[i] = HIGH;
        _currentState[i] = HIGH;
        _lastChangeTime[i] = 0;
    }
}

// ===================================================================
// Initialization
// ===================================================================
bool MCP23017Helper::begin(uint8_t i2cAddress) {
    if (!_mcp.begin_I2C(i2cAddress)) {
        _initialized = false;
        return false;
    }

    _initialized = true;
    return true;
}

// ===================================================================
// Configure pin mode
// ===================================================================
void MCP23017Helper::pinMode(uint8_t pin, uint8_t mode) {
    if (!_initialized || pin > 15) return;

    _mcp.pinMode(pin, mode);
}

// ===================================================================
// Digital read (direct, no debounce)
// ===================================================================
bool MCP23017Helper::digitalRead(uint8_t pin) {
    if (!_initialized || pin > 15) return HIGH;

    return _mcp.digitalRead(pin);
}

// ===================================================================
// Digital write
// ===================================================================
void MCP23017Helper::digitalWrite(uint8_t pin, bool value) {
    if (!_initialized || pin > 15) return;

    _mcp.digitalWrite(pin, value ? HIGH : LOW);
}

// ===================================================================
// Update button states (call in loop)
// ===================================================================
void MCP23017Helper::update() {
    if (!_initialized) return;

    for (uint8_t pin = 0; pin < 16; pin++) {
        bool rawState = _mcp.digitalRead(pin);
        uint32_t now = millis();

        // Check if state changed
        if (rawState != _lastState[pin]) {
            _lastChangeTime[pin] = now;
            _lastState[pin] = rawState;
        }

        // Update current state only if stable for debounce time
        if ((now - _lastChangeTime[pin]) > MCP_DEBOUNCE_MS) {
            _currentState[pin] = rawState;
        }
    }
}

// ===================================================================
// Button pressed (returns true on falling edge after debounce)
// ===================================================================
bool MCP23017Helper::buttonPressed(uint8_t pin) {
    if (!_initialized || pin > 15) return false;

    static bool lastDebounced[16] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,
                                      HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

    bool current = _currentState[pin];
    bool pressed = (lastDebounced[pin] == HIGH && current == LOW);

    lastDebounced[pin] = current;

    return pressed;
}

// ===================================================================
// Button released (returns true on rising edge after debounce)
// ===================================================================
bool MCP23017Helper::buttonReleased(uint8_t pin) {
    if (!_initialized || pin > 15) return false;

    static bool lastDebounced[16] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,
                                      HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

    bool current = _currentState[pin];
    bool released = (lastDebounced[pin] == LOW && current == HIGH);

    lastDebounced[pin] = current;

    return released;
}

// ===================================================================
// Button state (returns current debounced state)
// ===================================================================
bool MCP23017Helper::buttonState(uint8_t pin) {
    if (!_initialized || pin > 15) return HIGH;

    return _currentState[pin];
}
