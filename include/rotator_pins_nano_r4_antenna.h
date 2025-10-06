/* Arduino Nano R4 - ANTENNA Unit (Slave)
 *
 * This unit is located at the antenna (up to 100m from shack) and provides:
 * - Motor control (relay-based or PWM stepper drivers like TB6600)
 * - Position sensors (HH12 SPI, rotary encoders, potentiometers, pulse inputs)
 * - GPS receiver (Serial2)
 * - Limit switches (optional)
 * - RS485 communication to SHACK unit (receives commands, sends position)
 *
 * RS485 Slave receives rotation commands and reports positions
 *
 * Nano R4 Pin Assignment:
 * - Serial (USB): Debug port (optional, may not be accessible at antenna)
 * - Serial1 (D0/D1): RS485 communication to SHACK unit
 * - Serial2 (A4/A5): GPS receiver
 * - SPI (D10-D13): Position sensors (HH12 AS5045)
 * - Digital pins: Motor controls, limit switches
 * - PWM pins: Stepper motor pulse/direction signals
 * - Analog pins: Potentiometer position sensors, speed control
 */

#ifndef rotator_pins_nano_r4_antenna_h
#define rotator_pins_nano_r4_antenna_h

/* ---------- Azimuth Motor Control ---------- */
// For relay-based control (standard rotators)
#define rotate_cw 2              // Relay control for CW rotation
#define rotate_ccw 3             // Relay control for CCW rotation
#define rotate_cw_ccw 0          // Combined relay (if single direction relay)

// For PWM speed control (variable speed rotators)
#define rotate_cw_pwm 0          // PWM output for CW speed (if supported by rotator)
#define rotate_ccw_pwm 0         // PWM output for CCW speed
#define rotate_cw_ccw_pwm 0      // Combined PWM output

// For stepper motors (TB6600, DM542, etc.)
#define rotate_cw_freq 0         // CW frequency output for stepper
#define rotate_ccw_freq 0        // CCW frequency output for stepper

// For stepper motor drivers (requires FEATURE_STEPPER_MOTOR)
// TB6600 connections: PUL+ (pulse), DIR+ (direction), ENA+ (enable)
#define az_stepper_motor_pulse 9     // PWM output for stepper pulse (PUL+ on TB6600)
#define az_stepper_motor_direction 8 // Direction pin (DIR+ on TB6600)
#define az_stepper_motor_enable 7    // Enable pin (ENA+ on TB6600) - LOW to enable

/* ---------- Azimuth Position Sensing ---------- */
#define rotator_analog_az A0     // Analog potentiometer input (for FEATURE_AZ_POSITION_POTENTIOMETER)

// For HH12 AS5045 SSI absolute encoder (SPI-based)
#ifdef FEATURE_AZ_POSITION_HH12_AS5045_SSI
  #define az_hh12_clock_pin 13    // SPI SCK
  #define az_hh12_cs_pin 10       // SPI CS (chip select)
  #define az_hh12_data_pin 12     // SPI MISO
#endif

/* ---------- Azimuth Additional Controls ---------- */
#define azimuth_speed_voltage 0  // PWM for analog speed control voltage
#define overlap_led 0            // Indicator for >360° overlap
#define brake_az 4               // Brake release pin (HIGH to release)
#define az_speed_pot A1          // Speed control potentiometer
#define az_preset_pot 0          // Not used on antenna unit
#define az_rotation_stall_detected 0  // Stall detection input

/* ---------- Azimuth Limit Switches (Optional) ---------- */
#ifdef FEATURE_LIMIT_SENSE
  #define az_limit_sense_pin A2  // Analog input for limit sensing
  // Or use digital pins:
  #define az_ccw_limit_pin 0     // CCW limit switch (to ground)
  #define az_cw_limit_pin 0      // CW limit switch (to ground)
#endif

/* ---------- Elevation Motor Control ---------- */
#ifdef FEATURE_ELEVATION_CONTROL
  // For relay-based control
  #define rotate_up 5            // Relay control for UP rotation
  #define rotate_down 6          // Relay control for DOWN rotation
  #define rotate_up_or_down 0

  // For PWM speed control
  #define rotate_up_pwm 0        // PWM output for UP speed
  #define rotate_down_pwm 0      // PWM output for DOWN speed
  #define rotate_up_down_pwm 0   // Combined PWM

  // For stepper motors
  #define rotate_up_freq 0       // UP frequency output
  #define rotate_down_freq 0     // DOWN frequency output

  // For stepper motor drivers (requires FEATURE_STEPPER_MOTOR)
  #define el_stepper_motor_pulse 11    // PWM output for stepper pulse
  #define el_stepper_motor_direction 12  // Direction pin
  #define el_stepper_motor_enable 13   // Enable pin

  /* ---------- Elevation Position Sensing ---------- */
  #define rotator_analog_el A3   // Analog potentiometer input

  // For HH12 AS5045 SSI absolute encoder
  #ifdef FEATURE_EL_POSITION_HH12_AS5045_SSI
    #define el_hh12_clock_pin 13  // SPI SCK (shared with AZ if using separate CS)
    #define el_hh12_cs_pin 11     // SPI CS (chip select) - different from AZ
    #define el_hh12_data_pin 12   // SPI MISO (shared)
  #endif

  /* ---------- Elevation Additional Controls ---------- */
  #define brake_el 0             // Brake release pin
  #define elevation_speed_voltage 0  // PWM for speed control
  #define el_rotation_stall_detected 0  // Stall detection

  /* ---------- Elevation Limit Switches (Optional) ---------- */
  #ifdef FEATURE_LIMIT_SENSE
    #define el_limit_sense_pin A6  // Analog limit sensing (Nano R4 has A6)
    // Or use digital pins:
    #define el_down_limit_pin 0    // DOWN limit switch
    #define el_up_limit_pin 0      // UP limit switch
  #endif

  // Manual control buttons (not typically used on antenna unit, but available)
  #define button_up 0
  #define button_down 0
#endif /* FEATURE_ELEVATION_CONTROL */

/* ---------- Manual Control Buttons (Optional on antenna unit) ---------- */
#define button_cw 0              // Not typically used on antenna unit
#define button_ccw 0
#define button_stop 0
#define preset_start_button 0
#define button_park 0

/* ---------- Status Indicators ---------- */
#define serial_led 0             // LED for serial activity
#define blink_led 0              // General status LED
#define rotation_indication_pin 0  // Rotation indicator
#define heading_reading_inhibit_pin 0  // Inhibit heading reading (optional)

/* ---------- Rotary Encoders for Position Sensing ---------- */
#if defined(FEATURE_AZ_POSITION_ROTARY_ENCODER) || defined(FEATURE_AZ_POSITION_ROTARY_ENCODER_USE_PJRC_LIBRARY)
  #define az_rotary_position_pin1 2  // Encoder A phase (interrupt capable)
  #define az_rotary_position_pin2 3  // Encoder B phase (interrupt capable)
#endif

#if defined(FEATURE_EL_POSITION_ROTARY_ENCODER) || defined(FEATURE_EL_POSITION_ROTARY_ENCODER_USE_PJRC_LIBRARY)
  #define el_rotary_position_pin1 4  // Encoder A phase
  #define el_rotary_position_pin2 5  // Encoder B phase
#endif

/* ---------- Pulse Input for Position (Alternative to encoders) ---------- */
#ifdef FEATURE_AZ_POSITION_PULSE_INPUT
  #define az_position_pulse_pin 2           // Must be interrupt capable
  #define AZ_POSITION_PULSE_PIN_INTERRUPT 0 // Nano R4: pin 2 = interrupt 0
#endif

#ifdef FEATURE_EL_POSITION_PULSE_INPUT
  #define el_position_pulse_pin 3           // Must be interrupt capable
  #define EL_POSITION_PULSE_PIN_INTERRUPT 1 // Nano R4: pin 3 = interrupt 1
#endif

/* ---------- Incremental Encoder (3-phase) ---------- */
#ifdef FEATURE_AZ_POSITION_INCREMENTAL_ENCODER
  #define az_incremental_encoder_pin_a 2
  #define az_incremental_encoder_pin_b 3
  #define az_incremental_encoder_pin_z 4  // Index/zero pulse
#endif

#ifdef FEATURE_EL_POSITION_INCREMENTAL_ENCODER
  #define el_incremental_encoder_pin_a 5
  #define el_incremental_encoder_pin_b 6
  #define el_incremental_encoder_pin_z 7
#endif

/* ---------- Preset Encoders (Not used on antenna unit) ---------- */
#ifdef FEATURE_AZ_PRESET_ENCODER
  #define az_rotary_preset_pin1 0
  #define az_rotary_preset_pin2 0
#endif

#ifdef FEATURE_EL_PRESET_ENCODER
  #define el_rotary_preset_pin1 0
  #define el_rotary_preset_pin2 0
#endif

/* ---------- LCD Display (Disabled on antenna unit) ---------- */
#define lcd_4_bit_rs_pin 0
#define lcd_4_bit_enable_pin 0
#define lcd_4_bit_d4_pin 0
#define lcd_4_bit_d5_pin 0
#define lcd_4_bit_d6_pin 0
#define lcd_4_bit_d7_pin 0
// No display on antenna unit - all UI is on SHACK unit

/* ---------- GPS Configuration ---------- */
#ifdef FEATURE_GPS
  // GPS uses Serial2 on Nano R4 (A4=TX, A5=RX in some configurations)
  // Or dedicated hardware serial pins if available
  // Configuration in rotator_settings.h:
  // #define GPS_PORT Serial2
  // #define GPS_PORT_BAUD_RATE 9600
#endif

/* ---------- RS485 Communication ---------- */
// RS485 uses Serial1 (D0/D1) with external MAX485/MAX3485 transceiver
// Additional control pins for RS485:
#define rs485_tx_enable_pin 14   // DE (Driver Enable) on MAX485 - HIGH to transmit
#define rs485_rx_enable_pin 14   // /RE (Receiver Enable) on MAX485 - LOW to receive
                                 // Often DE and /RE are tied together

/* ---------- Ethernet (Disabled on antenna unit) ---------- */
// No Ethernet on antenna unit - all network connectivity is on SHACK unit

/* ---------- I2C Devices (Optional) ---------- */
// A4: SDA - Used by GPS Serial2 on some configs, check pinout
// A5: SCL - Used by GPS Serial2 on some configs, check pinout
// Can be used for I2C compass/accelerometer if GPS uses different pins

/* ---------- Compass/Accelerometer (Optional for absolute positioning) ---------- */
#ifdef FEATURE_AZ_POSITION_HMC5883L
  // I2C compass - uses A4/A5
#endif

#ifdef FEATURE_EL_POSITION_ADXL345_USING_LOVE_ELECTRON_LIB
  // I2C accelerometer - uses A4/A5
#endif

/* ---------- Joystick (Not used on antenna unit) ---------- */
#ifdef FEATURE_JOYSTICK_CONTROL
  #define pin_joystick_x 0
  #define pin_joystick_y 0
#endif

/* ---------- Analog Output (Optional) ---------- */
#ifdef FEATURE_ANALOG_OUTPUT_PINS
  #define pin_analog_az_out 0
  #define pin_analog_el_out 0
#endif

/* ---------- Power Switch (Optional) ---------- */
#ifdef FEATURE_POWER_SWITCH
  #define power_switch 0
#endif

/* ---------- Ancillary Pins (Optional) ---------- */
#ifdef FEATURE_ANCILLARY_PIN_CONTROL
  #define ancillary_pin_1 0
  #define ancillary_pin_2 0
  #define ancillary_pin_3 0
  #define ancillary_pin_4 0
  #define ancillary_pin_5 0
  #define ancillary_pin_6 0
#endif

#endif /* rotator_pins_nano_r4_antenna_h */
