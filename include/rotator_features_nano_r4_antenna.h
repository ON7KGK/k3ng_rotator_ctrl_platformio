/* Arduino Nano R4 - ANTENNA Unit Features
 *
 * Slave unit configuration for distributed rotator control system
 * Communicates with SHACK unit via RS485
 *
 * This unit handles:
 * - Motor control (relay-based or stepper with PWM)
 * - Position sensing (encoders, potentiometers, absolute encoders)
 * - GPS receiver (coordinates sent to SHACK via RS485)
 * - Limit switches
 * - RS485 slave communication
 */

#ifndef rotator_features_nano_r4_antenna_h
#define rotator_features_nano_r4_antenna_h

/* ---------- Main Features ---------- */
// Temporarily disabled - test AZ-only first
// #define FEATURE_ELEVATION_CONTROL      // AZ/EL rotator support

/* ---------- Remote Unit Configuration ---------- */
#define FEATURE_REMOTE_UNIT_SLAVE      // This is a slave unit controlled by SHACK master

/* ---------- Motor Control Type ---------- */
// Choose ONE motor control method:

// Option 1: Standard Relay Control (default for Yaesu-style rotators)
// No additional defines needed - uses rotate_cw/rotate_ccw pins

// Option 2: Stepper Motor Control (TB6600, DM542, etc.)
// Uncomment this for stepper motor support:
// #define FEATURE_STEPPER_MOTOR

#ifdef FEATURE_STEPPER_MOTOR
  // Stepper motor speed options (choose ONE):
  // #define OPTION_STEPPER_MOTOR_MAX_2_KHZ
  // #define OPTION_STEPPER_MOTOR_MAX_5_KHZ
  // #define OPTION_STEPPER_MOTOR_MAX_10_KHZ
  #define OPTION_STEPPER_MOTOR_MAX_20_KHZ  // Recommended for TB6600

  // #define OPTION_STEPPER_DO_NOT_USE_DIGITALWRITEFAST_LIBRARY

  // Timer selection (default is TimerFive):
  // #define OPTION_STEPPER_MOTOR_USE_TIMER_ONE_INSTEAD_OF_FIVE
#endif

/* ---------- Position Sensors - AZIMUTH ---------- */
// Choose ONE position sensor type for azimuth:

// Option 1: Analog Potentiometer (voltage from rotator or homebrew pot) - SIMPLE START
#define FEATURE_AZ_POSITION_POTENTIOMETER   // Easiest to compile and test

// Option 2: Rotary Encoder (incremental)
// #define FEATURE_AZ_POSITION_ROTARY_ENCODER
// #define FEATURE_AZ_POSITION_ROTARY_ENCODER_USE_PJRC_LIBRARY

// Option 3: Pulse Input
// #define FEATURE_AZ_POSITION_PULSE_INPUT

// Option 4: HH12 AS5045 SSI Absolute Encoder (SPI-based) - Enable after basic compile works
// #define FEATURE_AZ_POSITION_HH12_AS5045_SSI
// #define FEATURE_AZ_POSITION_HH12_AS5045_SSI_RELATIVE  // For >360° rotation
// #define OPTION_REVERSE_AZ_HH12_AS5045

// Option 5: Digital Compass (I2C)
// #define FEATURE_AZ_POSITION_HMC5883L
// #define FEATURE_AZ_POSITION_HMC5883L_USING_JARZEBSKI_LIBRARY
// #define FEATURE_AZ_POSITION_DFROBOT_QMC5883
// #define FEATURE_AZ_POSITION_MECHASOLUTION_QMC5883
// #define FEATURE_AZ_POSITION_ADAFRUIT_LSM303
// #define FEATURE_AZ_POSITION_POLOLU_LSM303

// Option 6: Incremental Encoder (3-phase with index)
// #define FEATURE_AZ_POSITION_INCREMENTAL_ENCODER

// Option 7: A2 Absolute Encoder
// #define FEATURE_AZ_POSITION_A2_ABSOLUTE_ENCODER

/* ---------- Position Sensors - ELEVATION ---------- */
// Choose ONE position sensor type for elevation:

// Option 1: Analog Potentiometer - SIMPLE START
#define FEATURE_EL_POSITION_POTENTIOMETER   // Easiest to compile and test

// Option 2: Rotary Encoder
// #define FEATURE_EL_POSITION_ROTARY_ENCODER
// #define FEATURE_EL_POSITION_ROTARY_ENCODER_USE_PJRC_LIBRARY

// Option 3: Pulse Input
// #define FEATURE_EL_POSITION_PULSE_INPUT

// Option 4: HH12 AS5045 SSI Absolute Encoder - Enable after basic compile works
// #define FEATURE_EL_POSITION_HH12_AS5045_SSI
// #define OPTION_REVERSE_EL_HH12_AS5045

// Option 5: ADXL345 Accelerometer (I2C)
// #define FEATURE_EL_POSITION_ADXL345_USING_LOVE_ELECTRON_LIB
// #define FEATURE_EL_POSITION_ADXL345_USING_ADAFRUIT_LIB
// #define FEATURE_EL_POSITION_ADAFRUIT_LSM303
// #define FEATURE_EL_POSITION_POLOLU_LSM303

// Option 6: MEMSIC 2125 Accelerometer
// #define FEATURE_EL_POSITION_MEMSIC_2125

// Option 7: Incremental Encoder
// #define FEATURE_EL_POSITION_INCREMENTAL_ENCODER

// Option 8: A2 Absolute Encoder
// #define FEATURE_EL_POSITION_A2_ABSOLUTE_ENCODER

/* ---------- Position Sensor Options ---------- */
#ifdef FEATURE_AZ_POSITION_ROTARY_ENCODER
  #define OPTION_AZ_POSITION_ROTARY_ENCODER_HARD_LIMIT
  #define OPTION_ENCODER_HALF_STEP_MODE
  #define OPTION_ENCODER_ENABLE_PULLUPS
#endif

#ifdef FEATURE_EL_POSITION_ROTARY_ENCODER
  #define OPTION_EL_POSITION_ROTARY_ENCODER_HARD_LIMIT
#endif

#ifdef FEATURE_AZ_POSITION_PULSE_INPUT
  #define OPTION_AZ_POSITION_PULSE_HARD_LIMIT
  #define OPTION_POSITION_PULSE_INPUT_PULLUPS
  // #define OPTION_PULSE_IGNORE_AMBIGUOUS_PULSES
#endif

#ifdef FEATURE_EL_POSITION_PULSE_INPUT
  #define OPTION_EL_POSITION_PULSE_HARD_LIMIT
  // #define OPTION_EL_PULSE_DEBOUNCE
#endif

#ifdef FEATURE_AZ_POSITION_INCREMENTAL_ENCODER
  #define OPTION_INCREMENTAL_ENCODER_PULLUPS
  // #define OPTION_SCANCON_2RMHF3600_INC_ENCODER
#endif

/* ---------- Limit Switches (Optional) ---------- */
// #define FEATURE_LIMIT_SENSE           // Enable limit switch support

/* ---------- GPS Support ---------- */
// Temporarily disabled - enable after basic compilation works
// #define FEATURE_GPS                      // GPS receiver on this unit
// #define OPTION_GPS_USE_TINY_GPS_LIBRARY  // Use TinyGPS library for NMEA parsing
// #define OPTION_DONT_READ_GPS_PORT_AS_OFTEN
// #define OPTION_GPS_DO_PORT_FLUSHES
// #define OPTION_GPS_EXCLUDE_MISSING_LF_CR_HANDLING

/* ---------- Clock and Tracking ---------- */
// Clock and tracking calculations are done on SHACK unit
// GPS data (time, coordinates) is sent to SHACK via RS485
// #define FEATURE_CLOCK                 // Disabled - SHACK handles this
// #define FEATURE_RTC_DS1307            // Optional local RTC
// #define FEATURE_RTC_PCF8583

// Sync options (for slave unit):
// GPS data from this unit will be sent to SHACK master

/* ---------- Rotation Stall Detection (Optional) ---------- */
// #define FEATURE_AZ_ROTATION_STALL_DETECTION
// #define FEATURE_EL_ROTATION_STALL_DETECTION
// #define OPTION_ROTATION_STALL_DETECTION_SERIAL_MESSAGE

/* ---------- Control Options ---------- */
// #define OPTION_EL_SPEED_FOLLOWS_AZ_SPEED
// #define OPTION_BUTTON_RELEASE_NO_SLOWDOWN

/* ---------- Manual Rotation Limits ---------- */
// #define OPTION_AZ_MANUAL_ROTATE_LIMITS
// #define OPTION_EL_MANUAL_ROTATE_LIMITS

/* ---------- Park Feature ---------- */
// #define FEATURE_PARK
// #define FEATURE_AUTOPARK

/* ---------- Ancillary Pin Control ---------- */
// #define FEATURE_ANCILLARY_PIN_CONTROL

/* ---------- Analog Output ---------- */
// #define FEATURE_ANALOG_OUTPUT_PINS

/* ---------- Power Control ---------- */
// #define FEATURE_POWER_SWITCH

/* ---------- Rotation Indicator ---------- */
// #define FEATURE_ROTATION_INDICATOR_PIN

/* ---------- ADC Resolution ---------- */
// #define FEATURE_ADC_RESOLUTION12  // 12-bit ADC (if supported by position sensors)

/* ---------- Advanced Options ---------- */
// #define OPTION_EXTERNAL_ANALOG_REFERENCE
// #define OPTION_DISABLE_HMC5883L_ERROR_CHECKING
// #define OPTION_MORE_SERIAL_CHECKS
// #define OPTION_ALLOW_ROTATIONAL_AND_CONFIGURATION_CMDS_AT_BOOT_UP

/* ---------- Language (minimal - no display on antenna unit) ---------- */
#define LANGUAGE_ENGLISH

/* ---------- Features NOT Used on ANTENNA Unit ---------- */
// These are handled by SHACK unit:
// - FEATURE_YAESU_EMULATION (SHACK only)
// - FEATURE_EASYCOM_EMULATION (SHACK only)
// - FEATURE_ETHERNET (SHACK only)
// - FEATURE_NEXTION_DISPLAY (SHACK only)
// - FEATURE_4_BIT_LCD_DISPLAY (SHACK only)
// - FEATURE_*_I2C_LCD (SHACK only)
// - FEATURE_MOON_TRACKING (calculations on SHACK)
// - FEATURE_SUN_TRACKING (calculations on SHACK)
// - FEATURE_SATELLITE_TRACKING (calculations on SHACK)
// - FEATURE_AZ_PRESET_ENCODER (SHACK only)
// - FEATURE_EL_PRESET_ENCODER (SHACK only)
// - FEATURE_JOYSTICK_CONTROL (SHACK only)

/* ---------- Communication Protocol ---------- */
// This unit uses the built-in K3NG remote slave protocol
// RS485 communication via Serial1 with MAX485/MAX3485 transceiver
// Protocol automatically handles:
// - Position reporting to master
// - Rotation command reception from master
// - GPS data transmission to master
// - Status updates

#endif /* rotator_features_nano_r4_antenna_h */
