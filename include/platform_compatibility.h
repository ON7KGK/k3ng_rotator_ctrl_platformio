/*
  platform_compatibility.h

  Platform-specific compatibility layer for Arduino Giga R1 WiFi (STM32H747)
  and other ARM/STM32 platforms
*/

#ifndef PLATFORM_COMPATIBILITY_H
#define PLATFORM_COMPATIBILITY_H

#if defined(ARDUINO_GIGA) || defined(ARDUINO_ARCH_MBED) || defined(ARDUINO_ARCH_STM32)
  #include <stdlib.h>
  #include <stdio.h>

  // dtostrf compatibility for STM32/ARM platforms (AVR has it built-in)
  #ifndef dtostrf
    inline char* dtostrf(double val, signed char width, unsigned char prec, char* sout) {
      char fmt[20];
      sprintf(fmt, "%%%d.%df", width, prec);
      sprintf(sout, fmt, val);
      return sout;
    }
  #endif
#endif

#endif // PLATFORM_COMPATIBILITY_H
