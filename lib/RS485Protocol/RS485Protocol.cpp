/**
 * RS485Protocol.cpp
 *
 * Implementation of RS485 communication protocol
 * Inspired by K3NG remote unit protocol
 *
 * Author: ON7KGK Michael
 */

#include "RS485Protocol.h"

// ===================================================================
// Constructor
// ===================================================================
RS485Protocol::RS485Protocol() {
    _serial = nullptr;
    _deRePin = 0;
    _rxIndex = 0;
    _lastByteTime = 0;
    _packetsReceived = 0;
    _packetsSent = 0;
    _errorCount = 0;
}

// ===================================================================
// Initialization
// ===================================================================
void RS485Protocol::begin(Stream* serial, uint8_t deRePin, uint32_t baudRate) {
    _serial = serial;
    _deRePin = deRePin;

    // Configure DE/RE pin (Driver Enable / Receiver Enable)
    pinMode(_deRePin, OUTPUT);
    setReceiveMode();  // Default to receive mode

    // Note: Serial port should be initialized before calling this
    // Example: Serial1.begin(baudRate);

    resetCounters();
}

// ===================================================================
// Send command (Master or Slave)
// ===================================================================
bool RS485Protocol::sendCommand(uint8_t cmd, uint16_t param1, uint16_t param2) {
    if (!_serial) return false;

    // Build packet
    uint8_t packet[PACKET_SIZE];
    packet[0] = PACKET_START_BYTE;
    packet[1] = cmd;
    packet[2] = (param1 >> 8) & 0xFF;   // Param1 high byte
    packet[3] = param1 & 0xFF;          // Param1 low byte
    packet[4] = (param2 >> 8) & 0xFF;   // Param2 high byte
    packet[5] = param2 & 0xFF;          // Param2 low byte
    packet[6] = calculateCRC(packet, 6); // CRC of first 6 bytes
    packet[7] = PACKET_END_BYTE;

    // Switch to transmit mode
    setTransmitMode();
    delayMicroseconds(10);  // Small delay for MAX485 switching

    // Send packet
    _serial->write(packet, PACKET_SIZE);
    _serial->flush();  // Wait for transmission complete

    // Switch back to receive mode
    delayMicroseconds(10);
    setReceiveMode();

    _packetsSent++;
    return true;
}

// ===================================================================
// Check if packet available (non-blocking)
// ===================================================================
bool RS485Protocol::available() {
    if (!_serial) return false;

    // Check for timeout (reset buffer if too long between bytes)
    if (_rxIndex > 0 && (millis() - _lastByteTime > PACKET_TIMEOUT_MS)) {
        _rxIndex = 0;  // Reset buffer
        _errorCount++;
    }

    // Read available bytes
    while (_serial->available()) {
        uint8_t byte = _serial->read();
        _lastByteTime = millis();

        // Looking for start byte
        if (_rxIndex == 0) {
            if (byte == PACKET_START_BYTE) {
                _rxBuffer[_rxIndex++] = byte;
            }
            continue;
        }

        // Collecting packet bytes
        _rxBuffer[_rxIndex++] = byte;

        // Check if packet complete
        if (_rxIndex >= PACKET_SIZE) {
            if (validatePacket()) {
                return true;  // Complete valid packet ready
            } else {
                _rxIndex = 0;  // Invalid, reset
                _errorCount++;
            }
        }
    }

    return false;
}

// ===================================================================
// Receive packet (call only if available() returns true)
// ===================================================================
bool RS485Protocol::receivePacket(uint8_t& cmd, uint16_t& param1, uint16_t& param2) {
    if (_rxIndex != PACKET_SIZE) return false;

    // Extract data from buffer
    cmd = _rxBuffer[1];
    param1 = (_rxBuffer[2] << 8) | _rxBuffer[3];
    param2 = (_rxBuffer[4] << 8) | _rxBuffer[5];

    // Reset buffer for next packet
    _rxIndex = 0;
    _packetsReceived++;

    return true;
}

// ===================================================================
// Flush receive buffer
// ===================================================================
void RS485Protocol::flushReceiveBuffer() {
    if (_serial) {
        while (_serial->available()) {
            _serial->read();
        }
    }
    _rxIndex = 0;
}

// ===================================================================
// Reset statistics
// ===================================================================
void RS485Protocol::resetCounters() {
    _packetsReceived = 0;
    _packetsSent = 0;
    _errorCount = 0;
}

// ===================================================================
// Set transmit mode (DE=HIGH, /RE=HIGH)
// ===================================================================
void RS485Protocol::setTransmitMode() {
    digitalWrite(_deRePin, HIGH);
}

// ===================================================================
// Set receive mode (DE=LOW, /RE=LOW)
// ===================================================================
void RS485Protocol::setReceiveMode() {
    digitalWrite(_deRePin, LOW);
}

// ===================================================================
// Calculate CRC (simple XOR checksum)
// ===================================================================
uint8_t RS485Protocol::calculateCRC(uint8_t* data, uint8_t length) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i];
    }
    return crc;
}

// ===================================================================
// Validate received packet
// ===================================================================
bool RS485Protocol::validatePacket() {
    // Check start and end bytes
    if (_rxBuffer[0] != PACKET_START_BYTE) return false;
    if (_rxBuffer[7] != PACKET_END_BYTE) return false;

    // Check CRC
    uint8_t expectedCRC = calculateCRC(_rxBuffer, 6);
    if (_rxBuffer[6] != expectedCRC) return false;

    return true;
}
