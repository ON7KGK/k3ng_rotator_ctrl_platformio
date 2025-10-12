/* ---------------------- K3NG RS485 Integration Wrapper ----------------------
 *
 * This wrapper provides a clean integration layer between the K3NG rotator
 * controller and the RS485 Master/Remote communication system.
 *
 * Purpose:
 * - Minimal modification to original K3NG code (only 2 function calls)
 * - Clean separation between K3NG code and RS485 code
 * - Easy to enable/disable RS485 functionality
 *
 * Architecture: RS485 Master/Remote
 * Hardware: Arduino Nano R4 Minima (Renesas RA4M1)
 *
 * Usage in k3ng_rotator_controller.cpp:
 *   void setup() {
 *     // ... existing K3NG setup code ...
 *     rs485_wrapper_setup();  // Add this line at end of setup()
 *   }
 *
 *   void loop() {
 *     // ... existing K3NG loop code ...
 *     rs485_wrapper_loop();   // Add this line at end of loop()
 *   }
 */

#ifndef K3NG_RS485_INTEGRATION_H
#define K3NG_RS485_INTEGRATION_H

#include <Arduino.h>

// Initialize RS485 communication (call from setup())
void rs485_wrapper_setup();

// Service RS485 communication (call from loop())
void rs485_wrapper_loop();

#endif // K3NG_RS485_INTEGRATION_H
