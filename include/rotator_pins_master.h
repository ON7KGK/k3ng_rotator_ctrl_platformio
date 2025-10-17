/* -------------------------------------   ANTENNA Unit (Master) - Pin Definitions ------------------------------------------
 *
 * Hardware: Arduino Nano R4 Minima (Renesas RA4M1)
 * Architecture: RS485 Master/Remote
 *
 * Pin allocation based on RS485_PINS_ALLOCATION.md
 *
 * Most pins can be disabled by setting them to 0 (zero). If you're not using a pin or function, set it to 0.
 *
 */

#ifndef ROTATOR_PINS_MASTER_H
#define ROTATOR_PINS_MASTER_H

/* ========== RS485 COMMUNICATION PINS (MANDATORY) ========== */
// These pins are reserved for RS485 and CANNOT be used for other purposes

// RS485 uses Serial1 hardware UART (D0/D1)
// D0 (RX1) = RS485 RX - connected to RO on RS485 module
// D1 (TX1) = RS485 TX - connected to DI on RS485 module
#define rs485_txrx_serial Serial1           // Hardware UART for RS485
#define rs485_enable_pin 8                  // DE (Driver Enable) - goes high during TX
#define rs485_disable_pin 9                 // RE (Receiver Enable) - goes low during RX

/* ========== AZIMUTH MOTOR CONTROL PINS ========== */

#define rotate_cw 2                        // goes high to activate rotator R (CW) rotation - pin 1 on Yaesu connector
#define rotate_ccw 3                       // goes high to activate rotator L (CCW) rotation - pin 2 on Yaesu connector
#define rotate_cw_ccw 0                     // goes high for both CW and CCW rotation (not used)
#define rotate_cw_pwm 0                     // optional - PWM CW output (set to 0 to disable)
#define rotate_ccw_pwm 0                    // optional - PWM CCW output (set to 0 to disable)
#define rotate_cw_ccw_pwm 0                 // optional - PWM on CW and CCW output (set to 0 to disable)
#define rotate_cw_freq 0                    // optional - CW variable frequency output
#define rotate_ccw_freq 0                   // optional - CCW variable frequency output
#define azimuth_speed_voltage 0             // optional - PWM output for speed control voltage
#define brake_az 0                          // goes high to disengage azimuth brake (set to 0 to disable)
#define az_speed_pot 0                      // connect to wiper of potentiometer for speed control (set to 0 to disable)
#define az_stepper_motor_pulse 0            // for stepper motor (not used)
#define az_rotation_stall_detected 0        // stall detection pin (optional)

/* ========== ELEVATION MOTOR CONTROL PINS ========== */
#ifdef FEATURE_ELEVATION_CONTROL
  #define rotate_up 4                      // goes high to activate rotator elevation up
  #define rotate_down 5                    // goes high to activate rotator elevation down
  #define rotate_up_or_down 0               // goes high when elevation up or down is activated (not used)
  #define rotate_up_pwm 0                   // optional - PWM UP output (set to 0 to disable)
  #define rotate_down_pwm 0                 // optional - PWM DOWN output (set to 0 to disable)
  #define rotate_up_down_pwm 0              // optional - PWM on both UP and DOWN (set to 0 to disable)
  #define rotate_up_freq 0                  // optional - UP variable frequency output
  #define rotate_down_freq 0                // optional - DOWN variable frequency output
  #define elevation_speed_voltage 0         // optional - PWM output for speed control voltage
  #define brake_el 0                        // goes high to disengage elevation brake (set to 0 to disable)
  #define el_stepper_motor_pulse 0          // for stepper motor (not used)
  #define el_rotation_stall_detected 0      // stall detection pin (optional)
#endif //FEATURE_ELEVATION_CONTROL

/* ========== GPS PINS (Serial2 Hardware UART) ========== */
// GPS on Serial2 (A4=TX2, A5=RX2)
// A4 (TX2) = GPS TX
// A5 (RX2) = GPS RX
// Configuration in code: Serial2.begin(9600) or appropriate baud rate

/* ========== AZIMUTH POSITION SENSOR PINS ========== */

// HH-12 SSI pins - always define to avoid K3NG compilation errors
// Set to 0 when using potentiometers instead
#define az_hh12_clock_pin 0                 // SSI Clock (CLK) - D2 when using SSI
#define az_hh12_cs_pin 0                    // SSI Chip Select (CS) - D3 when using SSI
#define az_hh12_data_pin 0                  // SSI Data Out (DO) - D4 when using SSI

// Analog Potentiometer (when not using SSI)
// Uses analog pin A1 for azimuth potentiometer
// A0 is used for motor control, A4/A5 for GPS, and A6/A7 don't exist on Nano R4 Minima
#ifdef FEATURE_AZ_POSITION_POTENTIOMETER
  #define rotator_analog_az A1               // A1 as analog input - Azimuth potentiometer
#endif

/* ========== ELEVATION POSITION SENSOR PINS ========== */

// HH-12 SSI pins - always define to avoid K3NG compilation errors
// Set to 0 when using potentiometers instead
#define el_hh12_clock_pin 0                 // SSI Clock (CLK) - D5 when using SSI (conflict if using D5 for pot)
#define el_hh12_cs_pin 0                    // SSI Chip Select (CS) - D6 when using SSI
#define el_hh12_data_pin 0                  // SSI Data Out (DO) - D7 when using SSI

// Analog Potentiometer (when not using SSI)
// Uses digital pin D5 as analog input
#ifdef FEATURE_EL_POSITION_POTENTIOMETER
  #define rotator_analog_el 5               // D5 as analog input - Elevation potentiometer
#endif

/* ========== LIMIT SWITCHES (OPTIONAL) ========== */

#ifdef FEATURE_LIMIT_SENSE
  #define az_limit_sense_pin 0              // input - low stops azimuthal rotation (TBD)
  #define el_limit_sense_pin 0              // input - low stops elevation rotation (TBD)
#endif

// Individual limit switches (OPTIONAL - if needed)
// Note: A6/A7 can be used but require external pullup resistors
// #define az_limit_cw A6                   // Azimuth CW limit (needs external pullup)
// #define az_limit_ccw A7                  // Azimuth CCW limit (needs external pullup)
// #define el_limit_up 0                    // Elevation UP limit (TBD - need free pin)
// #define el_limit_down 0                  // Elevation DOWN limit (TBD - need free pin)

/* ========== ETHERNET PINS (OPTIONAL - SPI) ========== */
// Ethernet module W5100/W5500 uses hardware SPI
// WARNING: Using Ethernet disables SSI encoders on D2-D7!
#ifdef FEATURE_ETHERNET
  // SPI pins are hardware-defined on Nano R4:
  // D10 = CS (Chip Select) - configurable
  // D11 = MOSI (Master Out Slave In) - hardware
  // D12 = MISO (Master In Slave Out) - hardware
  // D13 = SCK (SPI Clock) - hardware
  #define ethernet_cs_pin 10                // Ethernet Chip Select
#endif

/* ========== LOCAL CONTROL BUTTONS (OPTIONAL) ========== */
// Set to 0 to disable local buttons (control via RS485 from Remote)
// Set to actual pin number if you want local buttons at antenna site

#define button_cw 0                         // normally open button to ground for manual CW rotation
#define button_ccw 0                        // normally open button to ground for manual CCW rotation
#define button_up 0                         // normally open button to ground for manual up elevation
#define button_down 0                       // normally open button to ground for manual down rotation
#define button_stop 0                       // normally open button to ground for STOP

/* ========== INDICATOR LEDS (OPTIONAL) ========== */

#define serial_led 0                        // LED blinks when command received on serial port (set to 0 to disable)
#define overlap_led 0                       // line goes active when azimuth rotator is in overlap (> 360 rotators)
#define rotation_indication_pin 0           // indicates rotation in progress
#define blink_led 0                         // general purpose blink LED

/* ========== PARK FEATURE (OPTIONAL) ========== */

#ifdef FEATURE_PARK
  #define button_park 0                     // park button
  #define park_in_progress_pin 0            // goes high when park is in progress
  #define parked_pin 0                      // goes high when in parked position
#endif

/* ========== TRACKING FEATURES (GPS/MOON/SUN) ========== */

#ifdef FEATURE_GPS
  #define gps_sync 0                        // GPS sync indicator pin (optional)
#endif

#ifdef FEATURE_MOON_TRACKING
  #define moon_tracking_active_pin 0        // indicates moon tracking is active
  #define moon_tracking_activate_line 0     // line to activate moon tracking
  #define moon_tracking_button 0            // button to activate moon tracking
#endif

#ifdef FEATURE_SUN_TRACKING
  #define sun_tracking_active_pin 0         // indicates sun tracking is active
  #define sun_tracking_activate_line 0      // line to activate sun tracking
  #define sun_tracking_button 0             // button to activate sun tracking
#endif

// Satellite tracking pins (always define even if feature disabled)
#define satellite_tracking_active_pin 0   // indicates satellite tracking is active
#define satellite_tracking_activate_line 0 // line to activate satellite tracking
#define satellite_tracking_button 0       // button to activate satellite tracking

/* ========== DISABLED FEATURES ON MASTER ========== */

// Display pins - NOT USED on Master (display is on Remote unit)
#define lcd_4_bit_rs_pin 0
#define lcd_4_bit_enable_pin 0
#define lcd_4_bit_d4_pin 0
#define lcd_4_bit_d5_pin 0
#define lcd_4_bit_d6_pin 0
#define lcd_4_bit_d7_pin 0

// Preset encoders - NOT USED on Master (on Remote unit)
#ifdef FEATURE_AZ_PRESET_ENCODER
  #define az_rotary_preset_pin1 0           // CW Encoder Pin
  #define az_rotary_preset_pin2 0           // CCW Encoder Pin
#endif

#ifdef FEATURE_EL_PRESET_ENCODER
  #define el_rotary_preset_pin1 0           // UP Encoder Pin
  #define el_rotary_preset_pin2 0           // DOWN Encoder Pin
#endif

#define az_preset_pot 0                     // preset potentiometer (not used)
#define preset_start_button 0               // preset start button (on Remote)

/* ========== TRACKING PINS (OPTIONAL) ========== */

#ifdef FEATURE_MOON_TRACKING
  #define moon_tracking_active_pin 0        // output - high when moon tracking is active
  #define moon_tracking_activate_line 0     // input - activate moon tracking
  #define moon_tracking_button 0            // input - button to activate moon tracking
#endif

#ifdef FEATURE_SUN_TRACKING
  #define sun_tracking_active_pin 0         // output - high when sun tracking is active
  #define sun_tracking_activate_line 0      // input - activate sun tracking
  #define sun_tracking_button 0             // input - button to activate sun tracking
#endif

#ifdef FEATURE_SATELLITE_TRACKING
  #define satellite_tracking_active_pin 0   // output - high when satellite tracking is active
  #define satellite_tracking_activate_line 0 // input - activate satellite tracking
  #define satellite_tracking_button 0       // input - button to activate satellite tracking
#endif

#ifdef FEATURE_GPS
  #define gps_sync 0                        // input - GPS sync pulse
#endif

/* ========== MISCELLANEOUS ========== */

#define heading_reading_inhibit_pin 0       // input - high suspends azimuth/elevation readings (RF interference protection)

/* ========== PIN ALLOCATION SUMMARY ========== */
/*
 * USED PINS (minimum configuration with SSI encoders):
 * D0  - RS485 RX (Serial1)
 * D1  - RS485 TX (Serial1)
 * D2  - AZ SSI CLK
 * D3  - AZ SSI CS
 * D4  - AZ SSI DO
 * D5  - EL SSI CLK
 * D6  - EL SSI CS
 * D7  - EL SSI DO
 * D8  - RS485 DE
 * D9  - RS485 RE
 * A0  - Motor AZ CW
 * A1  - Motor AZ CCW
 * A2  - Motor EL UP
 * A3  - Motor EL DOWN
 * A4  - GPS TX (Serial2)
 * A5  - GPS RX (Serial2)
 *
 * AVAILABLE PINS:
 * D10-D13 - Available if not using Ethernet (can be used for limit switches, local buttons, etc.)
 * A6-A7   - Available but no internal pullup (can be used for limit switches with external pullup)
 *
 * ETHERNET OPTION (disables D10-D13 for other uses):
 * D10 - Ethernet CS
 * D11 - Ethernet MOSI (SPI)
 * D12 - Ethernet MISO (SPI)
 * D13 - Ethernet SCK (SPI)
 */

#endif // ROTATOR_PINS_MASTER_H
