/**
 * RS485Protocol.h
 *
 * Communication protocol for SHACK (Master) <-> ANTENNA (Slave)
 * Based on K3NG remote unit protocol but simplified and modernized
 *
 * Author: ON7KGK Michael
 * License: Same as K3NG Rotator Controller
 */

#ifndef RS485_PROTOCOL_H
#define RS485_PROTOCOL_H

#include <Arduino.h>

// ===================================================================
// Command definitions (inspired by K3NG remote protocol)
// ===================================================================

// Master -> Slave commands
#define CMD_PING                0x00  // Heartbeat
#define CMD_ROTATE_CW           0x10  // Rotate azimuth clockwise
#define CMD_ROTATE_CCW          0x11  // Rotate azimuth counter-clockwise
#define CMD_ROTATE_UP           0x12  // Rotate elevation up
#define CMD_ROTATE_DOWN         0x13  // Rotate elevation down
#define CMD_ROTATE_STOP_AZ      0x14  // Stop azimuth
#define CMD_ROTATE_STOP_EL      0x15  // Stop elevation
#define CMD_ROTATE_STOP_ALL     0x16  // Stop all motors
#define CMD_ROTATE_TO_AZ        0x20  // Rotate to azimuth (param1 = degrees * 10)
#define CMD_ROTATE_TO_EL        0x21  // Rotate to elevation (param1 = degrees * 10)
#define CMD_SET_SPEED_AZ        0x30  // Set azimuth speed (0-255)
#define CMD_SET_SPEED_EL        0x31  // Set elevation speed (0-255)
#define CMD_GET_STATUS          0x40  // Request status update
#define CMD_GET_POSITION        0x41  // Request position update

// Slave -> Master responses
#define CMD_PONG                0x80  // Heartbeat response
#define CMD_POSITION_REPORT     0x90  // Position: param1=AZ*10, param2=EL*10
#define CMD_STATUS_REPORT       0x91  // Status flags
#define CMD_ACK                 0xA0  // Command acknowledged
#define CMD_ERROR               0xFF  // Error occurred

// Status flags (for CMD_STATUS_REPORT param1)
#define STATUS_ROTATING_CW      0x0001
#define STATUS_ROTATING_CCW     0x0002
#define STATUS_ROTATING_UP      0x0004
#define STATUS_ROTATING_DOWN    0x0008
#define STATUS_AT_CW_LIMIT      0x0010
#define STATUS_AT_CCW_LIMIT     0x0020
#define STATUS_AT_UP_LIMIT      0x0040
#define STATUS_AT_DOWN_LIMIT    0x0080
#define STATUS_ENCODER_ERROR    0x0100
#define STATUS_MOTOR_ERROR      0x0200

// ===================================================================
// Packet structure
// ===================================================================
// [START] [CMD] [PARAM1_H] [PARAM1_L] [PARAM2_H] [PARAM2_L] [CRC] [END]
//    1      1        1          1          1          1        1     1  = 8 bytes
// ===================================================================

#define PACKET_START_BYTE       0xAA
#define PACKET_END_BYTE         0x55
#define PACKET_SIZE             8
#define PACKET_TIMEOUT_MS       100   // Timeout for receiving complete packet

class RS485Protocol {
public:
    // Constructor
    RS485Protocol();

    // Initialization
    void begin(Stream* serial, uint8_t deRePin, uint32_t baudRate = 9600);

    // Sending (Master or Slave)
    bool sendCommand(uint8_t cmd, uint16_t param1 = 0, uint16_t param2 = 0);

    // Receiving (non-blocking)
    bool available();
    bool receivePacket(uint8_t& cmd, uint16_t& param1, uint16_t& param2);

    // Utility
    void flushReceiveBuffer();
    uint32_t getPacketsReceived() { return _packetsReceived; }
    uint32_t getPacketsSent() { return _packetsSent; }
    uint32_t getErrorCount() { return _errorCount; }
    void resetCounters();

private:
    // Hardware
    Stream* _serial;
    uint8_t _deRePin;           // DE/RE pin (tied together on MAX485)

    // Receive buffer
    uint8_t _rxBuffer[PACKET_SIZE];
    uint8_t _rxIndex;
    uint32_t _lastByteTime;

    // Statistics
    uint32_t _packetsReceived;
    uint32_t _packetsSent;
    uint32_t _errorCount;

    // Helper functions
    void setTransmitMode();
    void setReceiveMode();
    uint8_t calculateCRC(uint8_t* data, uint8_t length);
    bool validatePacket();
};

#endif // RS485_PROTOCOL_H
