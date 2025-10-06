/* Arduino Nano R4 - SHACK Unit Features
 *
 * Master unit configuration for distributed rotator control system
 * Communicates with ANTENNA unit via RS485
 *
 * This unit handles:
 * - User interface (display, buttons)
 * - Ethernet connectivity for remote control
 * - RS485 master communication
 * - Position display (received from antenna unit)
 * - Rotation commands (sent to antenna unit)
 * - Clock, tracking calculations (Moon/Sun/Satellite)
 */

#ifndef rotator_features_nano_r4_shack_h
#define rotator_features_nano_r4_shack_h

/* ---------- Main Features ---------- */
#define FEATURE_ELEVATION_CONTROL      // AZ/EL rotator support
#define FEATURE_YAESU_EMULATION        // GS-232 protocol emulation
// #define FEATURE_EASYCOM_EMULATION   // Easycom protocol
// #define FEATURE_DCU_1_EMULATION     // DCU-1 protocol (azimuth only)

/* ---------- Tracking Features ---------- */
// Temporarily disabled due to TimeLib compilation errors
// Enable these once TimeLib dependencies are resolved
// #define FEATURE_MOON_TRACKING          // Moon tracking calculations
// #define FEATURE_SUN_TRACKING           // Sun tracking calculations
// #define FEATURE_SATELLITE_TRACKING     // Satellite tracking
// #define FEATURE_CLOCK                  // Real-time clock

/* ---------- GPS - Synchronized from Antenna Unit ---------- */
// GPS is physically connected to ANTENNA unit
// Coordinates and time are synchronized via RS485
// #define FEATURE_GPS                 // Disabled on SHACK, enabled on ANTENNA
// #define FEATURE_RTC_DS1307          // Optional RTC if not using GPS sync
// #define FEATURE_RTC_PCF8583         // Alternative RTC

// #define OPTION_SYNC_MASTER_CLOCK_TO_SLAVE       // Sync clock from ANTENNA GPS (requires FEATURE_CLOCK)
// #define OPTION_SYNC_MASTER_COORDINATES_TO_SLAVE // Sync coordinates from ANTENNA GPS

/* ---------- Network Features ---------- */
// Temporarily disabled - enable after basic compilation works
// #define FEATURE_ETHERNET               // Ethernet support (W5100/W5500)

/* ---------- Master/Slave Communication ---------- */
#define FEATURE_MASTER_WITH_SERIAL_SLAVE        // RS485 master mode
#define FEATURE_MASTER_SEND_AZ_ROTATION_COMMANDS_TO_REMOTE  // Send AZ commands to slave
#define FEATURE_MASTER_SEND_EL_ROTATION_COMMANDS_TO_REMOTE  // Send EL commands to slave

/* ---------- Position Sensing - Get from Remote Unit ---------- */
// All position sensors are on ANTENNA unit
#define FEATURE_AZ_POSITION_GET_FROM_REMOTE_UNIT  // Receive AZ position via RS485
#define FEATURE_EL_POSITION_GET_FROM_REMOTE_UNIT  // Receive EL position via RS485

/* ---------- Display Options ---------- */
// Choose ONE display type (uncomment only one):

// Option 1: Classic 4-bit LCD
// #define FEATURE_4_BIT_LCD_DISPLAY

// Option 2: I2C LCD (various types - choose one) - RECOMMENDED
#define FEATURE_ADAFRUIT_I2C_LCD        // Default I2C LCD (change to your model if different)
// #define FEATURE_YOURDUINO_I2C_LCD
// #define FEATURE_RFROBOT_I2C_DISPLAY
// #define FEATURE_YWROBOT_I2C_DISPLAY
// #define FEATURE_SAINSMART_I2C_LCD
// #define FEATURE_MIDAS_I2C_DISPLAY
// #define FEATURE_FABO_LCD_PCF8574_DISPLAY

// Option 3: Nextion Display (conflicts with RS485 on Serial1)
// #define FEATURE_NEXTION_DISPLAY      // Disabled - Serial1 used for RS485
// User must enable 4-bit LCD or I2C LCD instead (uncomment one above)

// Adafruit LCD buttons (if using FEATURE_ADAFRUIT_I2C_LCD)
// #define FEATURE_ADAFRUIT_BUTTONS     // Use LCD shield buttons for control

/* ---------- Display Options ---------- */
#define OPTION_DISPLAY_STATUS
#define OPTION_DISPLAY_HEADING
#define OPTION_DISPLAY_HEADING_AZ_ONLY
#define OPTION_DISPLAY_HEADING_EL_ONLY
// Clock display options disabled (require FEATURE_CLOCK)
// #define OPTION_DISPLAY_HHMM_CLOCK              // HH:MM clock display
// #define OPTION_DISPLAY_HHMMSS_CLOCK         // HH:MM:SS clock display
// #define OPTION_DISPLAY_ALT_HHMM_CLOCK_AND_MAIDENHEAD
// #define OPTION_DISPLAY_CONSTANT_HHMMSS_CLOCK_AND_MAIDENHEAD
// #define OPTION_DISPLAY_BIG_CLOCK
// #define OPTION_CLOCK_ALWAYS_HAVE_HOUR_LEADING_ZERO
// #define OPTION_DISPLAY_GPS_INDICATOR           // GPS status indicator (requires GPS sync)
// #define OPTION_DISPLAY_DIRECTION_STATUS     // N, NE, E, SE, S, SW, W, NW
// #define OPTION_DISPLAY_MOON_OR_SUN_OR_SAT_TRACKING_CONDITIONAL  // Requires tracking features
#define OPTION_DISPLAY_VERSION_ON_STARTUP
// #define OPTION_LCD_HEADING_FIELD_FIXED_DECIMAL_PLACE

/* ---------- Preset Rotary Encoders (Optional - requires MCP23017 I2C expander) ---------- */
// Uncomment if using rotary encoders for preset AZ/EL input
// #define FEATURE_AZ_PRESET_ENCODER
// #define FEATURE_EL_PRESET_ENCODER

#ifdef FEATURE_AZ_PRESET_ENCODER
  #define OPTION_ENCODER_HALF_STEP_MODE
  #define OPTION_ENCODER_ENABLE_PULLUPS
  // #define OPTION_PRESET_ENCODER_RELATIVE_CHANGE
  #define OPTION_PRESET_ENCODER_0_360_DEGREES
#endif

/* ---------- Control Options ---------- */
#define OPTION_GS_232B_EMULATION       // GS-232B (vs GS-232A)
#define FEATURE_ROTATION_INDICATOR_PIN // LED/pin indicates rotation active
// #define OPTION_SERIAL_HELP_TEXT     // Enable help command
// #define OPTION_C_COMMAND_SENDS_AZ_AND_EL  // For Ham Radio Deluxe
// #define OPTION_DELAY_C_CMD_OUTPUT         // For Ham Radio Deluxe

/* ---------- Speed Control ---------- */
#define OPTION_EL_SPEED_FOLLOWS_AZ_SPEED  // EL speed tracks AZ speed

/* ---------- Timed Buffer ---------- */
// #define FEATURE_TIMED_BUFFER        // Yaesu timed buffer commands

/* ---------- Park Feature ---------- */
// #define FEATURE_PARK                // Park position support
// #define FEATURE_AUTOPARK            // Auto-park after timeout

/* ---------- Manual Rotation Limits ---------- */
// #define OPTION_AZ_MANUAL_ROTATE_LIMITS  // Limit manual rotation
// #define OPTION_EL_MANUAL_ROTATE_LIMITS

/* ---------- Calibration Features ---------- */
// #define FEATURE_SUN_PUSHBUTTON_AZ_EL_CALIBRATION
// #define FEATURE_MOON_PUSHBUTTON_AZ_EL_CALIBRATION
// #define FEATURE_AZIMUTH_CORRECTION   // Calibration table correction
// #define FEATURE_ELEVATION_CORRECTION

/* ---------- Joystick Control (Optional) ---------- */
// #define FEATURE_JOYSTICK_CONTROL
// #define OPTION_JOYSTICK_REVERSE_X_AXIS
// #define OPTION_JOYSTICK_REVERSE_Y_AXIS

/* ---------- Ancillary Pin Control ---------- */
// #define FEATURE_ANCILLARY_PIN_CONTROL  // \F, \N, \P commands

/* ---------- Analog Output ---------- */
// #define FEATURE_ANALOG_OUTPUT_PINS  // PWM analog voltage outputs

/* ---------- Audible Alert ---------- */
// #define FEATURE_AUDIBLE_ALERT       // Buzzer/speaker alerts

/* ---------- Button Options ---------- */
// #define OPTION_BUTTON_RELEASE_NO_SLOWDOWN  // No slowdown on button release

/* ---------- Power Control ---------- */
// #define FEATURE_POWER_SWITCH

/* ---------- Language ---------- */
#define LANGUAGE_ENGLISH
// #define LANGUAGE_SPANISH
// #define LANGUAGE_CZECH
// #define LANGUAGE_ITALIAN
// #define LANGUAGE_PORTUGUESE_BRASIL
// #define LANGUAGE_GERMAN
// #define LANGUAGE_FRENCH
// #define LANGUAGE_DUTCH
// #define LANGUAGE_NORWEGIAN_BOKMAAL

/* ---------- Memory Optimization ---------- */
// #define OPTION_SAVE_MEMORY_EXCLUDE_EXTENDED_COMMANDS
// #define OPTION_SAVE_MEMORY_EXCLUDE_BACKSLASH_CMDS

/* ---------- Advanced Options ---------- */
// #define OPTION_EXTERNAL_ANALOG_REFERENCE
// #define OPTION_ALLOW_ROTATIONAL_AND_CONFIGURATION_CMDS_AT_BOOT_UP
// #define OPTION_MORE_SERIAL_CHECKS

/* ---------- Nextion Display Options ---------- */
#ifdef FEATURE_NEXTION_DISPLAY
  // #define OPTION_DEPRECATED_NEXTION_INIT_CODE_1  // Old firmware compatibility
  // #define OPTION_DEPRECATED_NEXTION_INIT_CODE_2
  // #define OPTION_SEND_NEXTION_RESET_AT_BOOTUP
#endif

/* ---------- Features NOT Used on SHACK Unit ---------- */
// These are handled by ANTENNA unit:
// - FEATURE_AZ_POSITION_POTENTIOMETER (on ANTENNA)
// - FEATURE_AZ_POSITION_ROTARY_ENCODER (on ANTENNA)
// - FEATURE_AZ_POSITION_HH12_AS5045_SSI (on ANTENNA)
// - FEATURE_EL_POSITION_* (on ANTENNA)
// - FEATURE_LIMIT_SENSE (on ANTENNA)
// - FEATURE_STEPPER_MOTOR (on ANTENNA)
// - FEATURE_GPS (physical GPS on ANTENNA, data synced to SHACK)

#endif /* rotator_features_nano_r4_shack_h */
