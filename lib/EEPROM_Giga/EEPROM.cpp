/*
  EEPROM.cpp - EEPROM emulation for Arduino Giga R1 WiFi (STM32H747)
*/

#include "EEPROM.h"

#if defined(ARDUINO_GIGA) || defined(ARDUINO_ARCH_MBED)

EEPROMClass EEPROM;

#endif // ARDUINO_GIGA || ARDUINO_ARCH_MBED
