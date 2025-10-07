/**
 * config.h - SHACK Unit Configuration
 *
 * This file contains all user-configurable settings for the SHACK unit
 *
 * SHACK Unit responsibilities:
 * - User interface (LCD, buttons)
 * - Send commands to ANTENNA via RS485
 * - Receive position from ANTENNA
 * - Display current position
 *
 * Author: ON7KGK Michael
 */

#ifndef SHACK_CONFIG_H
#define SHACK_CONFIG_H

// ===================================================================
// FEATURE ACTIVATION (comment/uncomment to enable/disable)
// ===================================================================
// Like K3NG style: comment out features you don't have/want

#define FEATURE_MCP23017_BUTTONS      // MCP23017 I/O expander for buttons
// #define FEATURE_DIRECT_GPIO_BUTTONS   // Use Nano R4 GPIO pins for buttons (not implemented yet)
// #define FEATURE_ETHERNET              // Ethernet interface (not implemented yet)
// #define FEATURE_NEXTION_DISPLAY       // Nextion display on Serial2 (not implemented yet)

// ===================================================================
// HARDWARE CONFIGURATION
// ===================================================================

// RS485 Communication
#define RS485_SERIAL          Serial1   // Hardware serial for RS485
#define RS485_BAUD            9600      // Baud rate
#define RS485_DE_RE_PIN       14        // DE and /RE pins tied together (pin A0 = D14)

// LCD Display - I2C 20x4
#define LCD_I2C_ADDRESS       0x3F      // Your LCD address
#define LCD_COLS              20        // 20 columns
#define LCD_ROWS              4         // 4 rows

// MCP23017 I/O Expander - Buttons
#define MCP_I2C_ADDRESS       0x20      // MCP23017 address
#define MCP_BTN_CW            0         // Pin 0 = CW button
#define MCP_BTN_CCW           1         // Pin 1 = CCW button
// Add more buttons as needed:
// #define MCP_BTN_UP         2
// #define MCP_BTN_DOWN       3
// #define MCP_BTN_STOP       4

// Status LED (built-in LED)
#define LED_BUILTIN_PIN       13

// ===================================================================
// DISPLAY SETTINGS
// ===================================================================

// LCD update interval (milliseconds)
#define LCD_UPDATE_INTERVAL_MS   500    // Update display every 500ms

// Display format
#define SHOW_DECIMAL_PLACES      1      // Show 1 decimal place (123.4°)

// ===================================================================
// BUTTON SETTINGS
// ===================================================================

// Button hold time for continuous rotation (milliseconds)
#define BUTTON_HOLD_TIME_MS      500    // Hold 500ms to start continuous rotation

// ===================================================================
// COMMUNICATION SETTINGS
// ===================================================================

// Position request interval (how often to ask ANTENNA for position)
#define POSITION_REQUEST_INTERVAL_MS  1000  // Request every 1 second

// Heartbeat/ping interval (keep-alive with ANTENNA)
#define HEARTBEAT_INTERVAL_MS         2000  // Ping every 2 seconds

// Communication timeout (how long to wait for response)
#define COMM_TIMEOUT_MS               500   // 500ms timeout

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
// ADVANCED SETTINGS
// ===================================================================

// LCD backlight timeout (0 = always on)
#define LCD_BACKLIGHT_TIMEOUT_MS     0      // Always on

// Startup splash screen duration
#define SPLASH_DURATION_MS           2000   // 2 seconds

#endif // SHACK_CONFIG_H
