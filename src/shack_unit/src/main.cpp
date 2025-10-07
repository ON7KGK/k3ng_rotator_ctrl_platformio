/**
 * SHACK UNIT - Main Firmware
 *
 * This is the master unit with user interface (LCD + buttons)
 * Sends commands to ANTENNA unit via RS485 and displays position
 *
 * Responsibilities:
 * - Read buttons (MCP23017)
 * - Send rotation commands to ANTENNA via RS485
 * - Receive position updates from ANTENNA
 * - Display position on LCD
 * - Keep-alive heartbeat with ANTENNA
 *
 * Author: ON7KGK Michael
 * Based on: K3NG Rotator Controller concepts
 */

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"
#include "RS485Protocol.h"
#include "MCP23017Helper.h"

// ===================================================================
// Global objects
// ===================================================================
RS485Protocol rs485;
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS);

#ifdef FEATURE_MCP23017_BUTTONS
MCP23017Helper mcp;
#endif

// ===================================================================
// Position tracking
// ===================================================================
float currentAzimuth = 0.0;
float currentElevation = 0.0;
bool positionValid = false;
uint32_t lastPositionUpdateTime = 0;

// ===================================================================
// Button state
// ===================================================================
bool cwButtonPressed = false;
bool ccwButtonPressed = false;

// ===================================================================
// Timing
// ===================================================================
uint32_t lastLcdUpdateTime = 0;
uint32_t lastHeartbeatTime = 0;
uint32_t lastPositionRequestTime = 0;

// ===================================================================
// Communication status
// ===================================================================
bool antennaConnected = false;
uint32_t lastAntennaResponseTime = 0;

// ===================================================================
// Function prototypes
// ===================================================================
void setupRS485();
void setupMCP23017();
void setupLCD();
void handleButtons();
void handleRS485Responses();
void updateDisplay();
void sendHeartbeat();
void requestPosition();
void showSplashScreen();
void displayConnectionStatus();

// ===================================================================
// Setup
// ===================================================================
void setup() {
    // Debug serial
    #ifdef DEBUG_ENABLED
    DEBUG_SERIAL.begin(DEBUG_BAUD);
    delay(1000);
    DEBUG_PRINTLN("=== SHACK UNIT Starting ===");
    #endif

    // Initialize I2C
    Wire.begin();

    // Initialize LCD
    setupLCD();
    showSplashScreen();

    #ifdef FEATURE_MCP23017_BUTTONS
    // Initialize MCP23017
    setupMCP23017();
    #else
    DEBUG_PRINTLN("MCP23017 disabled (no FEATURE_MCP23017_BUTTONS)");
    #endif

    // Initialize RS485
    setupRS485();

    // Built-in LED
    pinMode(LED_BUILTIN_PIN, OUTPUT);

    DEBUG_PRINTLN("SHACK UNIT Ready");
    DEBUG_PRINTLN("Send commands with buttons...");
}

// ===================================================================
// Main loop
// ===================================================================
void loop() {
    #ifdef FEATURE_MCP23017_BUTTONS
    // Update MCP button states
    mcp.update();

    // Handle button presses
    handleButtons();
    #endif

    // Handle RS485 responses from ANTENNA
    handleRS485Responses();

    // Send heartbeat periodically
    if (millis() - lastHeartbeatTime >= HEARTBEAT_INTERVAL_MS) {
        sendHeartbeat();
        lastHeartbeatTime = millis();
    }

    // Request position periodically
    if (millis() - lastPositionRequestTime >= POSITION_REQUEST_INTERVAL_MS) {
        requestPosition();
        lastPositionRequestTime = millis();
    }

    // Update LCD display
    if (millis() - lastLcdUpdateTime >= LCD_UPDATE_INTERVAL_MS) {
        updateDisplay();
        lastLcdUpdateTime = millis();
    }

    // Check antenna connection timeout
    if (millis() - lastAntennaResponseTime > COMM_TIMEOUT_MS * 10) {
        antennaConnected = false;
    }
}

// ===================================================================
// Setup RS485
// ===================================================================
void setupRS485() {
    RS485_SERIAL.begin(RS485_BAUD);
    rs485.begin(&RS485_SERIAL, RS485_DE_RE_PIN, RS485_BAUD);

    DEBUG_PRINT("RS485 initialized @ ");
    DEBUG_PRINT(RS485_BAUD);
    DEBUG_PRINTLN(" baud");
}

// ===================================================================
// Setup MCP23017
// ===================================================================
void setupMCP23017() {
    #ifdef FEATURE_MCP23017_BUTTONS
    if (!mcp.begin(MCP_I2C_ADDRESS)) {
        DEBUG_PRINTLN("! MCP23017 not found!");
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("MCP23017 ERROR!");
        lcd.setCursor(0, 2);
        lcd.print("Comment out");
        lcd.setCursor(0, 3);
        lcd.print("FEATURE_MCP23017");
        while(1);  // Halt
    }

    // Configure button pins as INPUT_PULLUP
    mcp.pinMode(MCP_BTN_CW, INPUT_PULLUP);
    mcp.pinMode(MCP_BTN_CCW, INPUT_PULLUP);
    // Add more buttons:
    // mcp.pinMode(MCP_BTN_UP, INPUT_PULLUP);
    // mcp.pinMode(MCP_BTN_DOWN, INPUT_PULLUP);

    DEBUG_PRINTLN("MCP23017 initialized");
    #endif
}

// ===================================================================
// Setup LCD
// ===================================================================
void setupLCD() {
    lcd.init();
    lcd.backlight();
    lcd.clear();

    DEBUG_PRINTLN("LCD initialized");
}

// ===================================================================
// Show splash screen
// ===================================================================
void showSplashScreen() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Nano R4 Rotator");
    lcd.setCursor(0, 1);
    lcd.print("   System v1.0");
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(0, 3);
    lcd.print("   by ON7KGK");

    delay(SPLASH_DURATION_MS);
    lcd.clear();
}

// ===================================================================
// Handle button presses
// ===================================================================
void handleButtons() {
    #ifdef FEATURE_MCP23017_BUTTONS
    // CW button
    if (mcp.buttonPressed(MCP_BTN_CW)) {
        DEBUG_PRINTLN("Button CW pressed");
        rs485.sendCommand(CMD_ROTATE_CW, 0, 0);
        cwButtonPressed = true;
    }
    if (mcp.buttonReleased(MCP_BTN_CW)) {
        DEBUG_PRINTLN("Button CW released");
        rs485.sendCommand(CMD_ROTATE_STOP_AZ, 0, 0);
        cwButtonPressed = false;
    }

    // CCW button
    if (mcp.buttonPressed(MCP_BTN_CCW)) {
        DEBUG_PRINTLN("Button CCW pressed");
        rs485.sendCommand(CMD_ROTATE_CCW, 0, 0);
        ccwButtonPressed = true;
    }
    if (mcp.buttonReleased(MCP_BTN_CCW)) {
        DEBUG_PRINTLN("Button CCW released");
        rs485.sendCommand(CMD_ROTATE_STOP_AZ, 0, 0);
        ccwButtonPressed = false;
    }

    // Add more buttons (UP, DOWN, STOP) here...
    #endif
}

// ===================================================================
// Handle RS485 responses from ANTENNA
// ===================================================================
void handleRS485Responses() {
    if (!rs485.available()) return;

    uint8_t cmd;
    uint16_t param1, param2;

    if (rs485.receivePacket(cmd, param1, param2)) {
        lastAntennaResponseTime = millis();
        antennaConnected = true;

        DEBUG_PRINT("RX: 0x");
        DEBUG_PRINT(cmd, HEX);
        DEBUG_PRINT(" P1:");
        DEBUG_PRINT(param1);
        DEBUG_PRINT(" P2:");
        DEBUG_PRINTLN(param2);

        switch (cmd) {
            case CMD_PONG:
                // Heartbeat response
                DEBUG_PRINTLN("<- PONG received");
                break;

            case CMD_POSITION_REPORT:
                // Position update (param1 = AZ*10, param2 = EL*10)
                currentAzimuth = param1 / 10.0;
                currentElevation = param2 / 10.0;
                positionValid = true;
                lastPositionUpdateTime = millis();

                DEBUG_PRINT("<- Position: AZ=");
                DEBUG_PRINT(currentAzimuth);
                DEBUG_PRINT("° EL=");
                DEBUG_PRINT(currentElevation);
                DEBUG_PRINTLN("°");
                break;

            default:
                DEBUG_PRINT("Unknown response: 0x");
                DEBUG_PRINTLN(cmd, HEX);
                break;
        }
    }
}

// ===================================================================
// Update LCD display
// ===================================================================
void updateDisplay() {
    lcd.clear();

    // Line 0: Title
    lcd.setCursor(0, 0);
    lcd.print("Nano R4 Rotator");

    // Line 1: Azimuth
    lcd.setCursor(0, 1);
    lcd.print("AZ: ");
    if (positionValid) {
        lcd.print(currentAzimuth, SHOW_DECIMAL_PLACES);
        lcd.print((char)223);  // Degree symbol
    } else {
        lcd.print("---");
    }

    // Line 2: Elevation
    lcd.setCursor(0, 2);
    lcd.print("EL: ");
    if (positionValid) {
        lcd.print(currentElevation, SHOW_DECIMAL_PLACES);
        lcd.print((char)223);  // Degree symbol
    } else {
        lcd.print("---");
    }

    // Line 3: Status
    lcd.setCursor(0, 3);
    if (antennaConnected) {
        if (cwButtonPressed) {
            lcd.print("Status: CW");
        } else if (ccwButtonPressed) {
            lcd.print("Status: CCW");
        } else {
            lcd.print("Status: OK");
        }
    } else {
        lcd.print("Status: NO ANTENNA!");
    }
}

// ===================================================================
// Send heartbeat (ping) to ANTENNA
// ===================================================================
void sendHeartbeat() {
    rs485.sendCommand(CMD_PING, 0, 0);
    DEBUG_PRINTLN("-> PING sent");
}

// ===================================================================
// Request position from ANTENNA
// ===================================================================
void requestPosition() {
    rs485.sendCommand(CMD_GET_POSITION, 0, 0);
    DEBUG_PRINTLN("-> Position request sent");
}
