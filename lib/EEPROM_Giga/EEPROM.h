/*
  EEPROM.h - EEPROM emulation for Arduino Giga R1 WiFi (STM32H747)

  This is a minimal EEPROM emulation wrapper for the Giga R1 WiFi
  Uses Mbed OS KVStore API for persistent storage in flash memory

  Compatible with standard Arduino EEPROM API
*/

#ifndef EEPROM_h
#define EEPROM_h

#include <Arduino.h>

#if defined(ARDUINO_GIGA) || defined(ARDUINO_ARCH_MBED)

// For now, use a simple RAM-based stub for compilation
// TODO: Implement proper FlashIAP/KVStore backend for persistent storage

#define EEPROM_SIZE 4096

class EEPROMClass {
private:
    uint8_t _data[EEPROM_SIZE];
    bool _initialized = false;

public:
    EEPROMClass() {
        memset(_data, 0xFF, EEPROM_SIZE);
    }

    uint8_t read(int address) {
        if (address < 0 || address >= EEPROM_SIZE) return 0;
        return _data[address];
    }

    void write(int address, uint8_t value) {
        if (address < 0 || address >= EEPROM_SIZE) return;
        _data[address] = value;
    }

    void update(int address, uint8_t value) {
        if (read(address) != value) {
            write(address, value);
        }
    }

    uint16_t length() {
        return EEPROM_SIZE;
    }

    // Template functions for get/put
    template<typename T> T &get(int address, T &t) {
        if (address < 0 || address + sizeof(T) > EEPROM_SIZE) return t;
        uint8_t* ptr = (uint8_t*)&t;
        for (size_t i = 0; i < sizeof(T); i++) {
            *ptr++ = read(address++);
        }
        return t;
    }

    template<typename T> const T &put(int address, const T &t) {
        if (address < 0 || address + sizeof(T) > EEPROM_SIZE) return t;
        const uint8_t* ptr = (const uint8_t*)&t;
        for (size_t i = 0; i < sizeof(T); i++) {
            update(address++, *ptr++);
        }
        return t;
    }
};

extern EEPROMClass EEPROM;

#endif // ARDUINO_GIGA || ARDUINO_ARCH_MBED

#endif // EEPROM_h
