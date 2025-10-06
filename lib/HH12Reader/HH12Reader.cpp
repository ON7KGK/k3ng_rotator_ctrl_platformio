/**
 * HH12Reader.cpp
 *
 * Implementation for HH12 / AS5045 SSI encoder reading
 * Based on K3NG implementation
 *
 * Author: ON7KGK Michael
 */

#include "HH12Reader.h"

// ===================================================================
// Constructor
// ===================================================================
HH12Reader::HH12Reader() {
    _csPin = 0;
    _offset = 0;
    _reverseDirection = false;
    _lastRawValue = 0;
}

// ===================================================================
// Initialization
// ===================================================================
void HH12Reader::begin(uint8_t csPin, bool reverseDirection) {
    _csPin = csPin;
    _reverseDirection = reverseDirection;

    // Configure CS pin
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);  // CS inactive (HIGH)

    // SPI is initialized globally in main setup()
    // We just use it here
}

// ===================================================================
// Read raw 12-bit value (0-4095)
// ===================================================================
uint16_t HH12Reader::readRaw() {
    uint16_t raw = readRawSPI();

    // Apply offset
    int32_t adjusted = (int32_t)raw - (int32_t)_offset;

    // Handle wrap-around
    if (adjusted < 0) {
        adjusted += 4096;
    }
    if (adjusted >= 4096) {
        adjusted -= 4096;
    }

    // Apply direction reversal if needed
    if (_reverseDirection) {
        adjusted = 4096 - adjusted;
        if (adjusted >= 4096) adjusted = 0;
    }

    _lastRawValue = (uint16_t)adjusted;
    return _lastRawValue;
}

// ===================================================================
// Read position in degrees (0-359.9)
// ===================================================================
float HH12Reader::readDegrees() {
    uint16_t raw = readRaw();

    // Convert 12-bit (0-4095) to degrees (0-360)
    // 4096 counts = 360 degrees
    // 1 count = 0.087890625 degrees
    float degrees = (raw * 360.0f) / 4096.0f;

    return degrees;
}

// ===================================================================
// Set offset (raw counts)
// ===================================================================
void HH12Reader::setOffset(uint16_t offsetRaw) {
    _offset = offsetRaw % 4096;
}

// ===================================================================
// Set offset (degrees)
// ===================================================================
void HH12Reader::setOffsetDegrees(float offsetDeg) {
    // Convert degrees to raw counts
    uint16_t offsetRaw = (uint16_t)((offsetDeg * 4096.0f) / 360.0f);
    setOffset(offsetRaw);
}

// ===================================================================
// Calibrate zero (set current position as zero)
// ===================================================================
void HH12Reader::calibrateZero() {
    uint16_t currentRaw = readRawSPI();
    setOffset(currentRaw);
}

// ===================================================================
// Set reverse direction
// ===================================================================
void HH12Reader::setReverseDirection(bool reverse) {
    _reverseDirection = reverse;
}

// ===================================================================
// Check if encoder is connected and responding
// ===================================================================
bool HH12Reader::isConnected() {
    uint16_t reading1 = readRawSPI();
    delay(10);
    uint16_t reading2 = readRawSPI();

    // If readings are identical and stuck at 0 or 4095, encoder might be disconnected
    if (reading1 == reading2 && (reading1 == 0 || reading1 == 4095)) {
        return false;
    }

    // If readings are valid (within range), encoder is connected
    return validateReading(reading1) && validateReading(reading2);
}

// ===================================================================
// Read raw SPI value from HH12 encoder
// Based on K3NG implementation
// ===================================================================
uint16_t HH12Reader::readRawSPI() {
    uint16_t data = 0;

    // Activate CS (LOW)
    digitalWrite(_csPin, LOW);
    delayMicroseconds(1);  // Small delay for encoder to respond

    // Read 16 bits (12 data bits + status bits)
    // HH12 sends MSB first
    for (int i = 0; i < 16; i++) {
        digitalWrite(SCK, LOW);
        delayMicroseconds(1);

        digitalWrite(SCK, HIGH);
        delayMicroseconds(1);

        // Read bit on MISO
        if (i < 12) {  // Only keep first 12 bits
            data <<= 1;
            if (digitalRead(MISO)) {
                data |= 1;
            }
        }
    }

    // Deactivate CS (HIGH)
    digitalWrite(_csPin, HIGH);
    delayMicroseconds(1);

    // Mask to 12 bits
    data &= 0x0FFF;

    return data;
}

// ===================================================================
// Validate reading (check if within valid range)
// ===================================================================
bool HH12Reader::validateReading(uint16_t value) {
    // Valid range is 0-4095 (12-bit)
    return (value <= 4095);
}
