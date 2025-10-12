/* -------------------------------------   SHACK Unit (Remote) - Pin Definitions ------------------------------------------
 *
 * Hardware: Arduino Nano R4 Minima (Renesas RA4M1)
 * Architecture: RS485 Master/Remote
 *
 * Pin allocation based on RS485_PINS_ALLOCATION.md
 *
 * Most pins can be disabled by setting them to 0 (zero). If you're not using a pin or function, set it to 0.
 *
 */

#ifndef ROTATOR_PINS_REMOTE_H
#define ROTATOR_PINS_REMOTE_H

/* ========== RS485 COMMUNICATION PINS (MANDATORY) ========== */
// These pins are reserved for RS485 and CANNOT be used for other purposes

// RS485 uses Serial1 hardware UART (D0/D1)
// D0 (RX1) = RS485 RX - connected to RO on RS485 module
// D1 (TX1) = RS485 TX - connected to DI on RS485 module
#define rs485_txrx_serial Serial1           // Hardware UART for RS485
#define rs485_enable_pin 8                  // DE (Driver Enable) - goes high during TX
#define rs485_disable_pin 9                 // RE (Receiver Enable) - goes low during RX

/* ========== DISPLAY PINS ========== */

// Option A: Nextion Display on Serial2 (RECOMMENDED)
// Serial2 uses A4 (TX2) and A5 (RX2)
// A4 (TX2) = Nextion RX (blue wire)
// A5 (RX2) = Nextion TX (yellow wire)
// Configuration: Serial2.begin(9600) or baud rate set in Nextion editor
#ifdef FEATURE_NEXTION_DISPLAY
  // Nextion uses Serial2 hardware UART - no pin definitions needed here
  // Configure in code: Serial2.begin(9600);
#endif

// Option B: LCD I2C (ALTERNATIVE - if not using Nextion)
// Uses A4 (SDA) and A5 (SCL) - same pins as Nextion Serial2
#ifdef FEATURE_ADAFRUIT_I2C_LCD
  // I2C uses hardware pins A4 (SDA) and A5 (SCL)
  // No pin definitions needed - handled by Wire library
#endif

// Option C: Classic 4-bit LCD (NOT RECOMMENDED - uses too many pins)
#ifdef FEATURE_4_BIT_LCD_DISPLAY
  #define lcd_4_bit_rs_pin 10               // Register Select
  #define lcd_4_bit_enable_pin 11           // Enable
  #define lcd_4_bit_d4_pin 12               // Data 4
  #define lcd_4_bit_d5_pin 13               // Data 5
  #define lcd_4_bit_d6_pin 0                // Data 6 (need pin assignment)
  #define lcd_4_bit_d7_pin 0                // Data 7 (need pin assignment)
#else
  #define lcd_4_bit_rs_pin 0
  #define lcd_4_bit_enable_pin 0
  #define lcd_4_bit_d4_pin 0
  #define lcd_4_bit_d5_pin 0
  #define lcd_4_bit_d6_pin 0
  #define lcd_4_bit_d7_pin 0
#endif

/* ========== MANUAL CONTROL BUTTONS ========== */

#define button_cw 2                         // normally open button to ground for manual CW rotation
#define button_ccw 3                        // normally open button to ground for manual CCW rotation
#define button_up 4                         // normally open button to ground for manual up elevation
#define button_down 5                       // normally open button to ground for manual down rotation
#define button_stop 6                       // normally open button to ground for STOP (emergency stop)

// Optional tracking buttons
// #define button_moon_track 7              // Moon tracking button (optional)
// #define button_sun_track A0              // Sun tracking button (optional)

/* ========== PRESET ROTARY ENCODERS (OPTIONAL) ========== */

// Azimuth Preset Encoder
#ifdef FEATURE_AZ_PRESET_ENCODER
  #define az_rotary_preset_pin1 10          // CW Encoder Pin A
  #define az_rotary_preset_pin2 11          // CCW Encoder Pin B
#else
  #define az_rotary_preset_pin1 0
  #define az_rotary_preset_pin2 0
#endif

// Elevation Preset Encoder
#ifdef FEATURE_EL_PRESET_ENCODER
  #define el_rotary_preset_pin1 12          // UP Encoder Pin A
  #define el_rotary_preset_pin2 13          // DOWN Encoder Pin B
#else
  #define el_rotary_preset_pin1 0
  #define el_rotary_preset_pin2 0
#endif

// Preset Start Button
#ifdef FEATURE_AZ_PRESET_ENCODER || FEATURE_EL_PRESET_ENCODER
  #define preset_start_button A1            // Preset start button (normally open to ground)
#else
  #define preset_start_button 0
#endif

/* ========== PARK FEATURE (OPTIONAL) ========== */

#ifdef FEATURE_PARK
  #define button_park 0                     // park button (optional)
  #define park_in_progress_pin 0            // indicator - park in progress
  #define parked_pin 0                      // indicator - parked position reached
#endif

/* ========== INDICATOR LEDS (OPTIONAL) ========== */

#define serial_led 0                        // LED blinks when command received on serial port
#define rotation_indication_pin 0           // indicates rotation in progress
#define blink_led 0                         // general purpose blink LED

/* ========== DISABLED FEATURES ON REMOTE ========== */

// Motor control pins - NOT USED on Remote (motors are on Master unit)
#define rotate_cw 0
#define rotate_ccw 0
#define rotate_cw_ccw 0
#define rotate_cw_pwm 0
#define rotate_ccw_pwm 0
#define rotate_cw_ccw_pwm 0
#define rotate_cw_freq 0
#define rotate_ccw_freq 0
#define azimuth_speed_voltage 0
#define brake_az 0
#define az_speed_pot 0
#define az_stepper_motor_pulse 0
#define az_rotation_stall_detected 0

#ifdef FEATURE_ELEVATION_CONTROL
  #define rotate_up 0
  #define rotate_down 0
  #define rotate_up_or_down 0
  #define rotate_up_pwm 0
  #define rotate_down_pwm 0
  #define rotate_up_down_pwm 0
  #define rotate_up_freq 0
  #define rotate_down_freq 0
  #define elevation_speed_voltage 0
  #define brake_el 0
  #define el_stepper_motor_pulse 0
  #define el_rotation_stall_detected 0
#endif

// Position sensor pins - NOT USED on Remote (sensors are on Master unit)
// Position data received via RS485 from Master
#define rotator_analog_az 0
#define rotator_analog_el 0

#if defined(FEATURE_AZ_POSITION_HH12_AS5045_SSI) || defined(FEATURE_AZ_POSITION_HH12_AS5045_SSI_RELATIVE)
  #define az_hh12_clock_pin 0
  #define az_hh12_cs_pin 0
  #define az_hh12_data_pin 0
#endif

#ifdef FEATURE_EL_POSITION_HH12_AS5045_SSI
  #define el_hh12_clock_pin 0
  #define el_hh12_cs_pin 0
  #define el_hh12_data_pin 0
#endif

// Limit switches - NOT USED on Remote (on Master unit)
#ifdef FEATURE_LIMIT_SENSE
  #define az_limit_sense_pin 0
  #define el_limit_sense_pin 0
#endif

// Ethernet - NOT SUPPORTED on Remote (Master only)
#ifdef FEATURE_ETHERNET
  #define ethernet_cs_pin 0
  // Remote unit does not support Ethernet
  #error "FEATURE_ETHERNET is not supported on Remote unit. Only Master supports Ethernet."
#endif

/* ========== MISCELLANEOUS ========== */

#define overlap_led 0                       // overlap indicator (not used on Remote)
#define az_preset_pot 0                     // preset potentiometer (not used - using rotary encoder)
#define heading_reading_inhibit_pin 0       // not used on Remote

/* ========== PIN ALLOCATION SUMMARY ========== */
/*
 * USED PINS (recommended configuration):
 * D0  - RS485 RX (Serial1)
 * D1  - RS485 TX (Serial1)
 * D2  - Button CW
 * D3  - Button CCW
 * D4  - Button UP
 * D5  - Button DOWN
 * D6  - Button STOP
 * D7  - Button Moon Track (optional)
 * D8  - RS485 DE
 * D9  - RS485 RE
 * D10 - AZ Preset Encoder Pin 1 (if enabled)
 * D11 - AZ Preset Encoder Pin 2 (if enabled)
 * D12 - EL Preset Encoder Pin 1 (if enabled)
 * D13 - EL Preset Encoder Pin 2 (if enabled)
 * A0  - Button Sun Track (optional)
 * A1  - Preset Start Button (if encoders enabled)
 * A4  - Display TX (Nextion Serial2) or I2C SDA (LCD)
 * A5  - Display RX (Nextion Serial2) or I2C SCL (LCD)
 *
 * AVAILABLE PINS:
 * A2  - Free (if not using sun track button)
 * A3  - Free
 * A6  - Available but no internal pullup
 * A7  - Available but no internal pullup
 *
 * Total used: 18/20 pins (with all features enabled)
 * Total free: 2 pins (A2, A3) + A6/A7 (no pullup)
 */

#endif // ROTATOR_PINS_REMOTE_H
