/**
 * HH12Reader.h
 *
 * Library for reading HH12 / AS5045 SSI absolute encoders
 * Based on K3NG implementation but simplified
 *
 * Resolution: 12-bit (4096 positions) = 0.088° per step
 *
 * Author: ON7KGK Michael
 * Based on: K3NG Rotator Controller HH12 code
 */

#ifndef HH12_READER_H
#define HH12_READER_H

#include <Arduino.h>
#include <SPI.h>

class HH12Reader {
public:
    // Constructor
    HH12Reader();

    // Initialization
    void begin(uint8_t csPin, bool reverseDirection = false);

    // Reading
    uint16_t readRaw();           // Read raw 12-bit value (0-4095)
    float readDegrees();          // Read position in degrees (0-359.9)

    // Calibration
    void setOffset(uint16_t offsetRaw);     // Set zero offset (raw counts)
    void setOffsetDegrees(float offsetDeg); // Set zero offset (degrees)
    void calibrateZero();                   // Set current position as zero

    // Configuration
    void setReverseDirection(bool reverse);

    // Status
    bool isConnected();           // Check if encoder responds
    uint16_t getOffset() { return _offset; }

private:
    uint8_t _csPin;
    uint16_t _offset;
    bool _reverseDirection;
    uint16_t _lastRawValue;

    // Helper functions
    uint16_t readRawSPI();
    bool validateReading(uint16_t value);
};

#endif // HH12_READER_H
