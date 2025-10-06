/**
 * config.h - ANTENNA Unit Configuration
 *
 * This file contains all user-configurable settings for the ANTENNA unit
 *
 * ANTENNA Unit responsibilities:
 * - Receive commands from SHACK via RS485
 * - Control motors (relays)
 * - Read position encoders (HH12)
 * - Send position back to SHACK
 *
 * Author: ON7KGK Michael
 */

#ifndef ANTENNA_CONFIG_H
#define ANTENNA_CONFIG_H

// ===================================================================
// HARDWARE CONFIGURATION
// ===================================================================

// RS485 Communication
#define RS485_SERIAL          Serial1   // Hardware serial for RS485
#define RS485_BAUD            9600      // Baud rate
#define RS485_DE_RE_PIN       14        // DE and /RE pins tied together (pin A0 = D14)

// Position Encoders - HH12 AS5045 SSI (SPI)
#define HH12_AZ_CS_PIN        10        // Chip Select for Azimuth encoder
#define HH12_EL_CS_PIN        9         // Chip Select for Elevation encoder
// SPI pins: MISO=12, MOSI=11, SCK=13 (hardware SPI)

// Motor Control - Relays
#define MOTOR_AZ_CW_PIN       2         // Azimuth CW relay
#define MOTOR_AZ_CCW_PIN      3         // Azimuth CCW relay
#define MOTOR_EL_UP_PIN       4         // Elevation UP relay
#define MOTOR_EL_DOWN_PIN     5         // Elevation DOWN relay

// Status LED (built-in LED for debugging)
#define LED_BUILTIN_PIN       13        // Nano R4 built-in LED

// ===================================================================
// ENCODER CALIBRATION
// ===================================================================

// Azimuth encoder settings
#define AZ_REVERSE_DIRECTION  false     // Reverse azimuth reading if needed
#define AZ_OFFSET_DEGREES     0.0       // Offset in degrees (0-360)

// Elevation encoder settings
#define EL_REVERSE_DIRECTION  true      // Reverse elevation reading (typically true)
#define EL_OFFSET_DEGREES     0.0       // Offset in degrees (0-180)

// ===================================================================
// MOTOR CONTROL SETTINGS
// ===================================================================

// Safety timeout - motors stop after this time if no new command
#define MOTOR_TIMEOUT_MS      5000      // 5 seconds

// Relay active level
#define RELAY_ACTIVE_HIGH     true      // true = relay ON with HIGH, false = relay ON with LOW

// ===================================================================
// POSITION REPORTING
// ===================================================================

// How often to send position updates to SHACK
#define POSITION_REPORT_INTERVAL_MS  500   // Send position every 500ms

// ===================================================================
// DEBUG SETTINGS
// ===================================================================

// Enable debug output on USB Serial (disable for production)
#define DEBUG_ENABLED         true
#define DEBUG_SERIAL          Serial    // USB Serial for debug
#define DEBUG_BAUD            115200

#ifdef DEBUG_ENABLED
  #define DEBUG_PRINT(...)      DEBUG_SERIAL.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...)    DEBUG_SERIAL.println(__VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINTLN(...)
#endif

// ===================================================================
// ADVANCED SETTINGS (rarely need to change)
// ===================================================================

// Encoder read interval (how often to read encoders)
#define ENCODER_READ_INTERVAL_MS  100   // Read every 100ms

// Heartbeat LED blink interval
#define HEARTBEAT_INTERVAL_MS     1000  // Blink every second

#endif // ANTENNA_CONFIG_H
