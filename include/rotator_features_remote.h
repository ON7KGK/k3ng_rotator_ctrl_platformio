/* ---------------------- SHACK Unit (Remote) - Features Configuration ----------------------
 *
 * This file defines features for the SHACK Unit (Remote)
 * The Remote unit is located in the shack (100m from antenna)
 *
 * Responsibilities:
 * - User interface (display + buttons)
 * - Preset encoders for manual positioning
 * - Serial protocol emulation for PC control (Yaesu/Easycom)
 * - RS485 communication (Remote role)
 * - Relay commands to Master via RS485
 *
 * Hardware: Arduino Nano R4 Minima (Renesas RA4M1)
 * Architecture: RS485 Master/Remote
 *
 * See RS485_FEATURES_SPLIT.md for complete feature documentation
 */

#ifndef ROTATOR_FEATURES_REMOTE_H
#define ROTATOR_FEATURES_REMOTE_H

/* ========== MAIN FEATURES ========== */

#define FEATURE_ELEVATION_CONTROL          // AZ/EL rotator support

/* ========== RS485 COMMUNICATION ========== */

#define FEATURE_RS485_REMOTE               // RS485 Remote mode - sends commands, receives position broadcasts

/* ========== POSITION REPORTING (VIRTUAL) ========== */

// Remote receives position from Master via RS485, no physical sensors
// Define dummy sensor to satisfy K3NG dependencies
#define FEATURE_AZ_POSITION_ROTARY_ENCODER // Virtual encoder - position from RS485
#define FEATURE_EL_POSITION_ROTARY_ENCODER // Virtual encoder - position from RS485

/* ========== USER INTERFACE - DISPLAY ========== */

// Choose ONE display option:
// #define FEATURE_NEXTION_DISPLAY         // Nextion display on Serial2 (A4/A5)
                                           // Serial2 available because NO GPS on Remote
#define FEATURE_SAINSMART_I2C_LCD          // SainSmart I2C LCD 20x4 (address 0x3F) on A4/A5 (SDA/SCL)
// #define FEATURE_ADAFRUIT_I2C_LCD        // Alternative: I2C LCD on A4/A5 (SDA/SCL)
// #define FEATURE_4_BIT_LCD_DISPLAY       // Alternative: 4-bit LCD (uses 6 pins)

// Note: Display configured for I2C address 0x3F (or 0x27 - use I2C scanner to detect)

/* ========== USER INTERFACE - BUTTONS ========== */

// Manual control buttons (pins defined in rotator_pins_remote.h)
#define button_cw                          // Rotation CW button (D2)
#define button_ccw                         // Rotation CCW button (D3)
#define button_up                          // Elevation UP button (D4)
#define button_down                        // Elevation DOWN button (D5)
#define button_stop                        // STOP button (D6)

// Optional tracking buttons
// #define button_moon_track               // Moon tracking button (D7)
// #define button_sun_track                // Sun tracking button (A0)

/* ========== USER INTERFACE - PRESET ENCODERS ========== */

// Rotary encoders for preset positioning (OPTIONAL)
#define FEATURE_AZ_PRESET_ENCODER          // Azimuth preset encoder (D10-D11)
#define FEATURE_EL_PRESET_ENCODER          // Elevation preset encoder (D12-D13)
// #define preset_start_button             // Start preset button (A1)

/* ========== SERIAL PROTOCOL EMULATION ========== */

// PC control via USB Serial (for PSTRotator, etc.)
#define FEATURE_YAESU_EMULATION            // Yaesu GS-232 protocol on USB Serial
// #define FEATURE_EASYCOM_EMULATION       // Alternative: Easycom protocol
// #define FEATURE_DCU_1_EMULATION         // Alternative: DCU-1 protocol

// IMPORTANT: Commands received on USB Serial are NOT relayed to RS485
// PSTRotator communicates only via USB Serial, RS485 is separate

/* ========== LANGUAGE ========== */

#define LANGUAGE_ENGLISH                   // UI language

/* ========== DISABLED FEATURES (on Master instead) ========== */

// Physical motor control is on Master unit
// #define rotate_cw                       // On Master unit (A0)
// #define rotate_ccw                      // On Master unit (A1)
// #define rotate_up                       // On Master unit (A2)
// #define rotate_down                     // On Master unit (A3)

// Position sensors on Master unit
// #define FEATURE_AZ_POSITION_HH12_AS5045_SSI  // On Master unit
// #define FEATURE_EL_POSITION_HH12_AS5045_SSI  // On Master unit
// #define FEATURE_AZ_POSITION_POTENTIOMETER    // On Master unit
// #define FEATURE_EL_POSITION_POTENTIOMETER    // On Master unit

// GPS & Tracking on Master unit
// Physical GPS hardware is on Master, but we need these features defined
// for K3NG compilation dependencies (types like tmElements_t, cTime, etc.)
// The Remote displays data received via RS485, doesn't use these features directly
#define FEATURE_GPS                        // For TinyGPS types (no physical GPS)
#define OPTION_GPS_USE_TINY_GPS_LIBRARY    // Use TinyGPS library types
#define FEATURE_CLOCK                      // For Time library types (tmElements_t)
#define FEATURE_MOON_TRACKING              // For moon2 types (cTime, etc.)
#define FEATURE_SUN_TRACKING               // For sunpos types (cSunCoordinates)
// #define FEATURE_SATELLITE_TRACKING      // Disabled - not implemented yet

// Ethernet NOT PHYSICALLY supported on Remote (Master only)
// However, we define FEATURE_ETHERNET to satisfy K3NG code compilation
// The Ethernet library types are compiled but hardware is never initialized
#define FEATURE_ETHERNET                   // For compilation only - no physical Ethernet on Remote

// Hardware safety features on Master
// #define az_limit_cw                     // On Master unit
// #define az_limit_ccw                    // On Master unit
// #define el_limit_up                     // On Master unit
// #define el_limit_down                   // On Master unit

/* ========== NOTES ========== */

// Remote unit configuration:
// - Provides user interface in shack
// - Displays position data received via RS485 from Master
// - Sends rotation commands to Master via RS485
// - Accepts commands from PC via USB Serial (PSTRotator)
// - NO direct motor control
// - NO physical sensors (position comes from Master via RS485)

#endif // ROTATOR_FEATURES_REMOTE_H
