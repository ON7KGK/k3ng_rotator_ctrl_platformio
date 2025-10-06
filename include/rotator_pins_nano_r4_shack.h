/* Arduino Nano R4 - SHACK Unit (Master)
 *
 * This unit is located in the shack and provides:
 * - User interface (Display, Buttons, LEDs)
 * - Ethernet connectivity (W5100/W5500)
 * - RS485 communication to ANTENNA unit
 * - Optional rotary encoders for AZ/EL preset (via MCP23017 I2C expander)
 *
 * RS485 Master sends rotation commands and receives position data
 *
 * Nano R4 Pin Assignment:
 * - Serial (USB): Control/Debug port
 * - Serial1 (D0/D1): RS485 communication to ANTENNA unit
 * - I2C (A4/A5): Display and optional MCP23017 for encoder inputs
 * - SPI (D10-D13): Ethernet W5100/W5500
 * - Digital pins: Buttons, LEDs
 * - Analog pins: Optional speed pots
 */

#ifndef rotator_pins_nano_r4_shack_h
#define rotator_pins_nano_r4_shack_h

/* ---------- Azimuth Pins (Disabled - controlled via RS485 slave) ---------- */
#define rotate_cw 0              // Disabled - sent to remote unit via RS485
#define rotate_ccw 0             // Disabled - sent to remote unit via RS485
#define rotate_cw_ccw 0
#define rotate_cw_pwm 0
#define rotate_ccw_pwm 0
#define rotate_cw_ccw_pwm 0
#define rotate_cw_freq 0
#define rotate_ccw_freq 0
#define rotator_analog_az 0      // Position received from remote unit via RS485
#define azimuth_speed_voltage 0
#define overlap_led 0
#define brake_az 0
#define az_stepper_motor_pulse 0
#define az_rotation_stall_detected 0

/* ---------- Manual Control Buttons ---------- */
#define button_cw 6              // Manual CW rotation button (to ground)
#define button_ccw 7             // Manual CCW rotation button (to ground)
#define button_stop 5            // Stop button (to ground)

/* ---------- Status LEDs ---------- */
#define serial_led 4             // LED blinks on serial command
#define blink_led 3              // General status LED
#define rotation_indication_pin 2 // Active when rotating

/* ---------- Speed Control ---------- */
#define az_speed_pot A0          // Optional: 1K-10K pot for azimuth speed control
#define az_preset_pot 0          // Not used in this configuration
#define preset_start_button 0    // Not used

/* ---------- Elevation Pins (Disabled - controlled via RS485 slave) ---------- */
#ifdef FEATURE_ELEVATION_CONTROL
  #define rotate_up 0            // Disabled - sent to remote unit via RS485
  #define rotate_down 0          // Disabled - sent to remote unit via RS485
  #define rotate_up_or_down 0
  #define rotate_up_pwm 0
  #define rotate_down_pwm 0
  #define rotate_up_down_pwm 0
  #define rotate_up_freq 0
  #define rotate_down_freq 0
  #define rotator_analog_el 0    // Position received from remote unit via RS485
  #define button_up 8            // Manual UP elevation button
  #define button_down 9          // Manual DOWN elevation button
  #define brake_el 0
  #define elevation_speed_voltage 0
  #define el_stepper_motor_pulse 0
  #define el_rotation_stall_detected 0
#endif

/* ---------- Rotary Encoder Preset (Optional - via MCP23017 I2C) ---------- */
// These pins are on MCP23017 I2C expander if FEATURE_AZ_PRESET_ENCODER is enabled
#ifdef FEATURE_AZ_PRESET_ENCODER
  #define az_rotary_preset_pin1 0  // Set to MCP23017 pin if using I2C expander
  #define az_rotary_preset_pin2 0  // Set to MCP23017 pin if using I2C expander
#endif

#ifdef FEATURE_EL_PRESET_ENCODER
  #define el_rotary_preset_pin1 0  // Set to MCP23017 pin if using I2C expander
  #define el_rotary_preset_pin2 0  // Set to MCP23017 pin if using I2C expander
#endif

/* Position encoders disabled - all position sensing is on remote ANTENNA unit */
#if defined(FEATURE_AZ_POSITION_ROTARY_ENCODER) || defined(FEATURE_AZ_POSITION_ROTARY_ENCODER_USE_PJRC_LIBRARY)
  #define az_rotary_position_pin1 0
  #define az_rotary_position_pin2 0
#endif

#if defined(FEATURE_EL_POSITION_ROTARY_ENCODER) || defined(FEATURE_EL_POSITION_ROTARY_ENCODER_USE_PJRC_LIBRARY)
  #define el_rotary_position_pin1 0
  #define el_rotary_position_pin2 0
#endif

#ifdef FEATURE_AZ_POSITION_PULSE_INPUT
  #define az_position_pulse_pin 0
  #define AZ_POSITION_PULSE_PIN_INTERRUPT 0
#endif

#ifdef FEATURE_EL_POSITION_PULSE_INPUT
  #define el_position_pulse_pin 0
  #define EL_POSITION_PULSE_PIN_INTERRUPT 0
#endif

#ifdef FEATURE_PARK
  #define button_park 0
#endif

/* ---------- LCD Display Pins ---------- */
// Classic 4-bit LCD (if using FEATURE_4_BIT_LCD_DISPLAY)
#define lcd_4_bit_rs_pin 0        // Disabled if using I2C display
#define lcd_4_bit_enable_pin 0
#define lcd_4_bit_d4_pin 0
#define lcd_4_bit_d5_pin 0
#define lcd_4_bit_d6_pin 0
#define lcd_4_bit_d7_pin 0

// I2C LCD uses A4/A5 (standard I2C pins)
// Nextion Display uses Serial2 (defined below)

/* ---------- Serial Ports Configuration ---------- */
// Serial (USB): Main control port for PC communication
// Serial1 (D0/D1): RS485 to ANTENNA unit
// Serial2: Reserved for Nextion display if needed (can be reassigned)

#ifdef FEATURE_NEXTION_DISPLAY
  // Nextion could use Serial1 if RS485 moved to Serial2
  // Or use Serial2 if available (may need software serial on Nano R4)
  // Default: We'll use Serial1 for RS485, so Nextion would need software serial
  // or reconfigure to use an alternate serial port
#endif

/* ---------- GPS (Disabled on SHACK unit) ---------- */
#ifdef FEATURE_GPS
  // GPS is on ANTENNA unit, not SHACK
  // Coordinates will be synchronized via RS485
#endif

/* ---------- Ethernet Pins (SPI) ---------- */
#ifdef FEATURE_ETHERNET
  // Ethernet uses standard SPI pins on Nano R4:
  // MISO: D12
  // MOSI: D11
  // SCK:  D13
  // CS:   D10 (configurable)
  #define ethernet_cs_pin 10
#endif

/* ---------- I2C Devices ---------- */
// A4: SDA (I2C Data)
// A5: SCL (I2C Clock)
// Used for: I2C LCD displays, MCP23017 I/O expander (optional)

/* ---------- Joystick (Optional) ---------- */
#ifdef FEATURE_JOYSTICK_CONTROL
  #define pin_joystick_x A1
  #define pin_joystick_y A2
#endif

/* ---------- Analog Output (Optional) ---------- */
#ifdef FEATURE_ANALOG_OUTPUT_PINS
  #define pin_analog_az_out 0
  #define pin_analog_el_out 0
#endif

/* ---------- Audible Alert (Optional) ---------- */
#ifdef FEATURE_AUDIBLE_ALERT
  #define pin_audible_alert 0  // PWM capable pin for speaker/buzzer
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

#endif /* rotator_pins_nano_r4_shack_h */
