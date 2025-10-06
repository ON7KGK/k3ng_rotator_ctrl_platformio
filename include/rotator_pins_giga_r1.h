/* -------------------------------------   Pin Definitions for Arduino Giga R1 WiFi ------------------------------------------

  Optimized pin mapping for Arduino Giga R1 WiFi (STM32H747XI)

  Key Giga R1 WiFi features:
  - 76 GPIO pins total
  - PWM pins: D2-D13 (12 pins)
  - Analog inputs: A0-A11 (12 pins)
  - DAC outputs: A12, A13 (2 pins)
  - 4 UARTs: Serial (USB), Serial1 (D0/D1), Serial2 (D17/D16), Serial3 (D14/D15)
  - 3 I2C: Wire (D20/D21), Wire1 (D58/D57), Wire2 (custom)
  - 2 SPI: SPI (D11/D12/D13), SPI1 (custom)
  - All pins are interrupt-capable (unlike AVR boards)
  - Logic level: 3.3V (NOT 5V tolerant!)

  Most pins can be disabled by setting them to 0 (zero).

*/

/* ========== AZIMUTH CONTROL PINS ========== */

// Digital outputs for relay/motor control
#define rotate_cw 22              // HIGH = CW rotation - Yaesu pin 1
#define rotate_ccw 23             // HIGH = CCW rotation - Yaesu pin 2
#define rotate_cw_ccw  0          // goes high for both CW and CCW rotation (set to 0 if not used)

// PWM outputs for speed control (use PWM-capable pins D2-D13)
#define rotate_cw_pwm 5           // PWM D5 - CW speed control
#define rotate_ccw_pwm 6          // PWM D6 - CCW speed control
#define rotate_cw_ccw_pwm 0       // PWM for both directions (set to 0 if using separate CW/CCW PWM)

// Frequency outputs (any digital pin)
#define rotate_cw_freq 0          // CW variable frequency output (0 = disabled)
#define rotate_ccw_freq 0         // CCW variable frequency output (0 = disabled)

// Manual control buttons (normally open to ground)
#define button_cw 24              // Manual CW button
#define button_ccw 25             // Manual CCW button
#define button_stop 26            // Emergency stop button
#define preset_start_button 27    // Preset start button

// Azimuth position sensing
#define rotator_analog_az A0      // Analog azimuth voltage from rotator - Yaesu pin 4
#define az_speed_pot A1           // Speed control potentiometer (1K-10K)
#define az_preset_pot A2          // Preset position potentiometer (1K-10K)

// Azimuth control outputs
#define azimuth_speed_voltage 7   // PWM D7 - continuous speed voltage to rotator
#define brake_az 28               // HIGH = disengage azimuth brake

// Status indicators
#define overlap_led 29            // HIGH when azimuth > 360° (overlap mode)
#define rotation_indication_pin 30 // HIGH during any rotation
#define serial_led 31             // Blinks on serial command received

// Stepper motor (if used)
#define az_stepper_motor_pulse 0  // Pulse output for stepper driver (0 = disabled)
#define az_rotation_stall_detected 0 // Stall detection input (0 = disabled)

/* ========== ELEVATION CONTROL PINS ========== */

#ifdef FEATURE_ELEVATION_CONTROL
  // Digital outputs for relay/motor control
  #define rotate_up 32            // HIGH = elevation UP
  #define rotate_down 33          // HIGH = elevation DOWN
  #define rotate_up_or_down 0     // goes high when elevation up OR down (0 = disabled)

  // PWM outputs for elevation speed control
  #define rotate_up_pwm 9         // PWM D9 - UP speed control
  #define rotate_down_pwm 10      // PWM D10 - DOWN speed control
  #define rotate_up_down_pwm 0    // PWM for both directions (0 = disabled)

  // Frequency outputs
  #define rotate_up_freq 0        // UP variable frequency output (0 = disabled)
  #define rotate_down_freq 0      // DOWN variable frequency output (0 = disabled)

  // Elevation position sensing
  #define rotator_analog_el A3    // Analog elevation voltage from rotator

  // Manual control buttons
  #define button_up 34            // Manual UP button
  #define button_down 35          // Manual DOWN button

  // Elevation control outputs
  #define elevation_speed_voltage 11 // PWM D11 - continuous speed voltage
  #define brake_el 36             // HIGH = disengage elevation brake

  // Stepper motor (if used)
  #define el_stepper_motor_pulse 0 // Pulse output for stepper driver (0 = disabled)
  #define el_rotation_stall_detected 0 // Stall detection input (0 = disabled)
#endif //FEATURE_ELEVATION_CONTROL

/* ========== ROTARY ENCODER PINS ========== */

// Preset encoders (for manual position entry)
#ifdef FEATURE_AZ_PRESET_ENCODER
  #define az_rotary_preset_pin1 37 // Azimuth encoder CW
  #define az_rotary_preset_pin2 38 // Azimuth encoder CCW
#endif

#ifdef FEATURE_EL_PRESET_ENCODER
  #define el_rotary_preset_pin1 39 // Elevation encoder UP
  #define el_rotary_preset_pin2 40 // Elevation encoder DOWN
#endif

// Position encoders (for reading actual antenna position)
#if defined(FEATURE_AZ_POSITION_ROTARY_ENCODER) || defined(FEATURE_AZ_POSITION_ROTARY_ENCODER_USE_PJRC_LIBRARY)
  #define az_rotary_position_pin1 41 // Azimuth position encoder A
  #define az_rotary_position_pin2 42 // Azimuth position encoder B
#endif

#if defined(FEATURE_EL_POSITION_ROTARY_ENCODER) || defined(FEATURE_EL_POSITION_ROTARY_ENCODER_USE_PJRC_LIBRARY)
  #define el_rotary_position_pin1 43 // Elevation position encoder A
  #define el_rotary_position_pin2 44 // Elevation position encoder B
#endif

/* ========== PULSE INPUT PINS (for position feedback) ========== */

#ifdef FEATURE_AZ_POSITION_PULSE_INPUT
  #define az_position_pulse_pin 45 // Azimuth pulse input (all Giga pins support interrupts)
  #define AZ_POSITION_PULSE_PIN_INTERRUPT 45 // On Giga R1, interrupt = pin number
#endif

#ifdef FEATURE_EL_POSITION_PULSE_INPUT
  #define el_position_pulse_pin 46 // Elevation pulse input
  #define EL_POSITION_PULSE_PIN_INTERRUPT 46 // On Giga R1, interrupt = pin number
#endif

/* ========== INCREMENTAL ENCODER PINS ========== */

#ifdef FEATURE_AZ_POSITION_INCREMENTAL_ENCODER
  #define az_incremental_encoder_pin_phase_a 47
  #define az_incremental_encoder_pin_phase_b 48
  #define az_incremental_encoder_pin_phase_z 49
  #define AZ_POSITION_INCREMENTAL_ENCODER_A_PIN_INTERRUPT 47
  #define AZ_POSITION_INCREMENTAL_ENCODER_B_PIN_INTERRUPT 48
#endif

#ifdef FEATURE_EL_POSITION_INCREMENTAL_ENCODER
  #define el_incremental_encoder_pin_phase_a 50
  #define el_incremental_encoder_pin_phase_b 51
  #define el_incremental_encoder_pin_phase_z 52
  #define EL_POSITION_INCREMENTAL_ENCODER_A_PIN_INTERRUPT 50
  #define EL_POSITION_INCREMENTAL_ENCODER_B_PIN_INTERRUPT 51
#endif

/* ========== LCD DISPLAY PINS (4-bit mode) ========== */

// Classic 4-bit LCD (HD44780 compatible)
#define lcd_4_bit_rs_pin 53
#define lcd_4_bit_enable_pin 54
#define lcd_4_bit_d4_pin 55
#define lcd_4_bit_d5_pin 56
#define lcd_4_bit_d6_pin 57
#define lcd_4_bit_d7_pin 58

/* ========== JOYSTICK CONTROL ========== */

#ifdef FEATURE_JOYSTICK_CONTROL
  #define pin_joystick_x A4       // Joystick X-axis analog input
  #define pin_joystick_y A5       // Joystick Y-axis analog input
#endif

/* ========== ABSOLUTE ENCODER PINS (HH12/AS5045 SSI) ========== */

#if defined(FEATURE_AZ_POSITION_HH12_AS5045_SSI) || defined(FEATURE_AZ_POSITION_HH12_AS5045_SSI_RELATIVE)
  #define az_hh12_clock_pin 59
  #define az_hh12_cs_pin 60
  #define az_hh12_data_pin 61
#endif

#ifdef FEATURE_EL_POSITION_HH12_AS5045_SSI
  #define el_hh12_clock_pin 62
  #define el_hh12_cs_pin 63
  #define el_hh12_data_pin 64
#endif

/* ========== PARK FEATURE PINS ========== */

#ifdef FEATURE_PARK
  #define button_park 65          // Park button (momentary to ground)
  #define park_in_progress_pin 66 // HIGH when parking in progress
  #define parked_pin 67           // HIGH when in parked position
#endif

/* ========== LIMIT SENSE PINS ========== */

#ifdef FEATURE_LIMIT_SENSE
  #define az_limit_sense_pin 68   // Input - LOW stops azimuth rotation
  #define el_limit_sense_pin 69   // Input - LOW stops elevation rotation
#endif

/* ========== TRACKING FEATURE PINS ========== */

#ifdef FEATURE_MOON_TRACKING
  #define moon_tracking_active_pin 70    // Output - HIGH when moon tracking active
  #define moon_tracking_activate_line 0  // Input - ground to activate (not for button)
  #define moon_tracking_button 71        // Button - momentary to ground
#endif

#ifdef FEATURE_SUN_TRACKING
  #define sun_tracking_active_pin 72     // Output - HIGH when sun tracking active
  #define sun_tracking_activate_line 0   // Input - ground to activate (not for button)
  #define sun_tracking_button 73         // Button - momentary to ground
#endif

// Satellite tracking (added 2020.07.24.01)
#define satellite_tracking_active_pin 74   // Output - HIGH when satellite tracking active
#define satellite_tracking_activate_line 0 // Input - ground to activate (not for button)
#define satellite_tracking_button 75       // Button - momentary to ground

/* ========== GPS PINS ========== */

#ifdef FEATURE_GPS
  #define gps_sync 76             // GPS sync pulse output
  // GPS uses Serial2 (pins 17 RX / 16 TX) - configured in rotator_settings.h
#endif

/* ========== ANALOG OUTPUT PINS (DAC) ========== */

#ifdef FEATURE_ANALOG_OUTPUT_PINS
  #define pin_analog_az_out A12   // DAC output for azimuth (0-3.3V)
  #define pin_analog_el_out A13   // DAC output for elevation (0-3.3V)
  // Note: Giga R1 has true DAC on A12/A13, not just PWM!
#endif

/* ========== MISCELLANEOUS PINS ========== */

#define heading_reading_inhibit_pin 0 // Input - HIGH suspends position readings (RF interference)

#ifdef FEATURE_POWER_SWITCH
  #define power_switch 0          // Power switch control (0 = disabled)
#endif

#ifdef FEATURE_EL_POSITION_MEMSIC_2125
  #define pin_memsic_2125_x A6
  #define pin_memsic_2125_y A7
#endif

#ifdef FEATURE_SUN_PUSHBUTTON_AZ_EL_CALIBRATION
  #define pin_sun_pushbutton_calibration 0 // Normally HIGH, button pulls LOW
#endif

#ifdef FEATURE_MOON_PUSHBUTTON_AZ_EL_CALIBRATION
  #define pin_moon_pushbutton_calibration 0 // Normally HIGH, button pulls LOW
#endif

#if defined(FEATURE_AZ_POSITION_A2_ABSOLUTE_ENCODER) || defined(FEATURE_EL_POSITION_A2_ABSOLUTE_ENCODER)
  #define pin_sei_bus_busy 0
  #define pin_sei_bus_send_receive 0
#endif

#ifdef FEATURE_AUTOPARK
  #define pin_autopark_disable 0       // Pull LOW to disable autopark
  #define pin_autopark_timer_reset 0   // Pull LOW to reset autopark timer (tie to PTT)
#endif

#ifdef FEATURE_AUDIBLE_ALERT
  #define pin_audible_alert 8     // PWM D8 - can drive piezo buzzer with tone()
#endif

// Blink LED - use built-in LED
#define blink_led LED_BUILTIN

//#define pin_status_led 0   // Status LED - blinks during rotation (0 = use blink_led instead)
//#define reset_pin 0        // HIGH to reset unit (0 = disabled)

/* ========== I2C DISPLAY PINS ========== */

#ifdef FEATURE_YOURDUINO_I2C_LCD
  // Uses Wire (I2C) on pins 20 (SDA) / 21 (SCL) - no pin definitions needed
  #define En_pin  2
  #define Rw_pin  1
  #define Rs_pin  0
  #define D4_pin  4
  #define D5_pin  5
  #define D6_pin  6
  #define D7_pin  7
#endif

#ifdef FEATURE_YWROBOT_I2C_DISPLAY
  // Uses Wire (I2C) on pins 20 (SDA) / 21 (SCL)
  #define ywrobot_address 0x3F
  #define ywrobot_pin_en 2
  #define ywrobot_pin_rw 1
  #define ywrobot_pin_rs 0
  #define ywrobot_pin_d4 4
  #define ywrobot_pin_d5 5
  #define ywrobot_pin_d6 6
  #define ywrobot_pin_d7 7
  #define ywrobot_pin_bl 3
  #define ywrobot_blpol POSITIVE
#endif

/* ========== PIN USAGE SUMMARY ==========

CRITICAL PINS (DO NOT USE):
- D0/D1: Serial1 (Nextion display)
- D14/D15: Serial3
- D16/D17: Serial2 (GPS)
- D18/D19: Reserved (I2C Wire1 on some variants)
- D20/D21: I2C Wire (SDA/SCL)

PWM PINS (D2-D13):
- D5: rotate_cw_pwm
- D6: rotate_ccw_pwm
- D7: azimuth_speed_voltage
- D8: pin_audible_alert
- D9: rotate_up_pwm
- D10: rotate_down_pwm
- D11: elevation_speed_voltage
- D12: Available
- D13: Available (also SPI SCK)

ANALOG INPUTS (A0-A11):
- A0: rotator_analog_az (azimuth position)
- A1: az_speed_pot
- A2: az_preset_pot
- A3: rotator_analog_el (elevation position)
- A4: pin_joystick_x
- A5: pin_joystick_y
- A6: pin_memsic_2125_x
- A7: pin_memsic_2125_y
- A8-A11: Available

DAC OUTPUTS (A12-A13):
- A12: pin_analog_az_out (true DAC 0-3.3V)
- A13: pin_analog_el_out (true DAC 0-3.3V)

DIGITAL I/O (D22-D76):
- D22-D76: General purpose digital I/O assigned above

NOTES:
- All Giga R1 pins are 3.3V logic (NOT 5V tolerant!)
- All pins support external interrupts (unlike AVR)
- 76 GPIO total provides lots of expansion room
- PWM only works on pins 2-13 (12 pins total)
- True DAC on A12/A13 (not PWM-based like most Arduinos)

*/
