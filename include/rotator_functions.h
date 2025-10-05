#ifndef ROTATOR_FUNCTIONS_H
#define ROTATOR_FUNCTIONS_H

void initialize_serial();

void initialize_peripherals();

void read_settings_from_eeprom();

void initialize_pins();

void read_azimuth(byte force_read);

#if defined(FEATURE_ELEVATION_CONTROL)
void read_elevation(byte force_read);
void el_check_operation_timeout();
void update_el_variable_outputs(byte speed_voltage);
#endif

void initialize_display();

void initialize_rotary_encoders();

void initialize_interrupts();

void run_this_once();

void service_process_debug(byte action,byte process_id);

void service_process_debug(byte action,byte process_id);

void check_serial();

void service_request_queue();

void service_rotation();

void az_check_operation_timeout();

void check_buttons();

void check_overlap();

void check_brake_release();

void check_az_speed_pot();

void check_az_speed_pot();

void check_az_preset_potentiometer();

void output_debug();

void check_for_dirty_configuration();

void service_blink_led();

void check_for_reset_flag();

void digitalWriteEnhanced(uint8_t pin, uint8_t writevalue);

int analogReadEnhanced(uint8_t pin);

void update_az_variable_outputs(byte speed_voltage);

int analogReadEnhanced(uint8_t pin);

int digitalReadEnhanced(uint8_t pin);

void submit_request(byte axis, byte request, float parm, byte called_by);

void initialize_eeprom_with_defaults();

void write_settings_to_eeprom();

void analogWriteEnhanced(uint8_t pin, int writevalue);

void change_tracking(byte action);

void stop_rotation();

void pinModeEnhanced(uint8_t pin, uint8_t mode);

void read_headings();

void brake_release(byte az_or_el, byte operation);

void process_yaesu_command(byte * yaesu_command_buffer, int yaesu_command_buffer_index, byte source_port, char * return_string);

byte process_backslash_command(byte input_buffer[], int input_buffer_index, byte source_port, byte include_response_code, char * return_string, byte input_source);

byte current_az_state();

byte current_el_state();

#if defined(FEATURE_NEXTION_DISPLAY)
void service_nextion_display();
void convert_polar_to_cartesian(byte coordinate_conversion,double azimuth_in,double elevation_in,double* x,double* y);
#endif

#if defined(FEATURE_GPS)
void service_gps();
#endif

#if defined(FEATURE_CLOCK)
char* timezone_modified_clock_string();
char* zulu_clock_string();
char* clock_status_string();
char* coordinates_to_maidenhead(float latitude, float longitude);
#endif

#if defined(FEATURE_MOON_TRACKING)
void service_moon_tracking();
void update_moon_position();
#endif

#if defined(FEATURE_SUN_TRACKING)
void service_sun_tracking();
void update_sun_position();
#endif

#if defined(FEATURE_ETHERNET)
void service_ethernet();
#endif

#if defined(FEATURE_AUDIBLE_ALERT)
byte audible_alert(byte alert_type);
#endif

#if defined(FEATURE_ROTATION_INDICATOR_PIN)
void service_rotation_indicator_pin();
#endif

#if defined(FEATURE_TIMED_BUFFER)
void check_timed_interval();
#endif

#if defined(FEATURE_AZ_ROTATION_STALL_DETECTION)
void az_check_rotation_stall();
#endif

#if defined(FEATURE_EL_ROTATION_STALL_DETECTION)
void el_check_rotation_stall();
#endif

#if defined(OPTION_AZ_MANUAL_ROTATE_LIMITS)
void check_az_manual_rotate_limit();
#endif

#if defined(OPTION_EL_MANUAL_ROTATE_LIMITS)
void check_el_manual_rotate_limit();
#endif

#if defined(FEATURE_JOYSTICK_CONTROL)
void check_joystick();
#endif

#if defined(FEATURE_AZ_PRESET_ENCODER) || defined(FEATURE_EL_PRESET_ENCODER)
void check_preset_encoders();
#endif

#if defined(FEATURE_PARK)
void service_park();
void initiate_park();
void deactivate_park();
#endif

#if defined(FEATURE_AUTOPARK)
void service_autopark();
#endif

#if defined(FEATURE_LIMIT_SENSE)
void check_limit_sense();
#endif

#if defined(FEATURE_AZIMUTH_CORRECTION)
float correct_azimuth(float azimuth_in);
#endif

#if defined(FEATURE_ELEVATION_CORRECTION)
float correct_elevation(float elevation_in);
#endif

#if (defined(OPTION_SERIAL_HELP_TEXT) || defined(FEATURE_TIMED_BUFFER)) && (defined(FEATURE_REMOTE_UNIT_SLAVE) || defined(FEATURE_YAESU_EMULATION) || defined(FEATURE_EASYCOM_EMULATION))
void print_to_port(char* string_to_print, byte port_to_print);
#endif

#if defined(FEATURE_SATELLITE_TRACKING)
void service_satellite_tracking(byte initialize_satellite_tracking, byte initialize);
byte service_calc_satellite_data(byte sat_array_element, byte number_of_days_to_scan, byte command, byte print_header, byte service_calc_service_or_initialize, byte print_done, byte simulate_az_el);
void initialize_tle_file_area_eeprom(byte initialize_type);
void load_satellite_tle_into_P13(const char* sat_name, const char* tle_line1, const char* tle_line2, byte load_hardcoded_tle, byte make_it_the_current_satellite);
void print_aos_los_satellite_status();
void print_current_satellite_status();
void warm_initialize_satellite_array();
void clear_satellite_array();
char* satellite_aos_los_string(byte satellite_array_position);
void send_vt100_code(char* code);
#endif

#if defined(FEATURE_CLOCK) && defined(OPTION_USE_OLD_TIME_CODE)
void cleartime(tm* time_struct);
char* tm_date_string(tm* time_struct);
char* tm_month_and_day_string(tm* time_struct);
char* tm_time_string_short(tm* time_struct);
char* tm_time_string_long(tm* time_struct);
long difftime(tm* time1, tm* time2, int* days, int* hours, int* minutes, int* seconds);
#endif

#if defined(FEATURE_CLOCK) && !defined(OPTION_USE_OLD_TIME_CODE)
void cleartime(tmElements_t* time_struct);
char* tm_date_string(tmElements_t* time_struct);
char* tm_month_and_day_string(tmElements_t* time_struct);
char* tm_time_string_short(tmElements_t* time_struct);
char* tm_time_string_long(tmElements_t* time_struct);
long difftime(tmElements_t* time1, tmElements_t* time2, int* days, int* hours, int* minutes, int* seconds);
#endif

#endif //ROTATOR_FUNCTIONS_H