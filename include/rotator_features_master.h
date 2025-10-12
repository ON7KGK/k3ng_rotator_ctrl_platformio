/* ---------------------- ANTENNA Unit (Master) - Features Configuration ----------------------
 *
 * This file defines features for the ANTENNA Unit (Master)
 * The Master unit is located at the antenna (100m from shack)
 *
 * Responsibilities:
 * - Physical control of motors (AZ/EL)
 * - Reading position sensors (SSI encoders or potentiometers)
 * - GPS reception and tracking calculations
 * - Moon/Sun/Satellite tracking
 * - RS485 communication (Master role)
 * - Optional Ethernet interface
 *
 * Hardware: Arduino Nano R4 Minima (Renesas RA4M1)
 * Architecture: RS485 Master/Remote
 *
 * See RS485_FEATURES_SPLIT.md for complete feature documentation
 */

#ifndef ROTATOR_FEATURES_MASTER_H
#define ROTATOR_FEATURES_MASTER_H

/* ========== MAIN FEATURES ========== */

#define FEATURE_ELEVATION_CONTROL          // AZ/EL rotator support

/* ========== RS485 COMMUNICATION ========== */

#define FEATURE_RS485_MASTER               // RS485 Master mode - listens for commands, sends position broadcasts

/* ========== POSITION SENSORS ========== */

// Choose ONE option for Azimuth position sensing:
#define FEATURE_AZ_POSITION_HH12_AS5045_SSI          // SSI absolute encoder (HH-12)
// #define FEATURE_AZ_POSITION_POTENTIOMETER         // Alternative: analog potentiometer

// Choose ONE option for Elevation position sensing:
#define FEATURE_EL_POSITION_HH12_AS5045_SSI          // SSI absolute encoder (HH-12)
// #define FEATURE_EL_POSITION_POTENTIOMETER         // Alternative: analog potentiometer

/* ========== GPS & TRACKING ========== */

#define FEATURE_GPS                        // GPS on Serial2 (A4/A5)
#define FEATURE_CLOCK                      // Real-time clock for tracking
#define FEATURE_MOON_TRACKING              // Moon tracking calculations
#define FEATURE_SUN_TRACKING               // Sun tracking calculations
#define FEATURE_SATELLITE_TRACKING         // Satellite tracking support

/* ========== NETWORKING (OPTIONAL) ========== */

#define FEATURE_ETHERNET                   // Ethernet interface (W5100/W5500) - OPTIONAL
                                           // Uses SPI pins D10-D13 (conflicts with encoders on these pins)

/* ========== LANGUAGE ========== */

#define LANGUAGE_ENGLISH                   // UI language

/* ========== DISABLED FEATURES (on Remote instead) ========== */

// User interface features are on the Remote unit
// #define FEATURE_NEXTION_DISPLAY         // Display on Remote unit
// #define FEATURE_4_BIT_LCD_DISPLAY       // Display on Remote unit
// #define FEATURE_ADAFRUIT_I2C_LCD        // Display on Remote unit

// Manual control via Remote unit
// Buttons CAN be present on Master for local control if needed (100m from shack)
// Uncomment these if you want local buttons on Master:
// #define button_cw                       // Local CW button (pin TBD)
// #define button_ccw                      // Local CCW button (pin TBD)
// #define button_up                       // Local UP button (pin TBD)
// #define button_down                     // Local DOWN button (pin TBD)
// #define button_stop                     // Local STOP button (pin TBD)

// Preset encoders on Remote unit only
// #define FEATURE_AZ_PRESET_ENCODER       // On Remote unit
// #define FEATURE_EL_PRESET_ENCODER       // On Remote unit

// Serial protocols handled by Remote unit (USB connection to PC)
// #define FEATURE_YAESU_EMULATION         // On Remote unit (USB Serial)
// #define FEATURE_EASYCOM_EMULATION       // On Remote unit (USB Serial)
// #define FEATURE_DCU_1_EMULATION         // On Remote unit (USB Serial)

/* ========== NOTES ========== */

// Master unit configuration:
// - Runs autonomously at antenna
// - Receives commands via RS485 from Remote unit
// - Broadcasts position every 100ms via RS485
// - Performs all tracking calculations locally
// - Controls motors based on target positions
// - Optional local buttons for manual control at antenna site

#endif // ROTATOR_FEATURES_MASTER_H
