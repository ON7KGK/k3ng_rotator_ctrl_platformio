/**
 * ANTENNA UNIT - Main Firmware
 *
 * This is the slave unit that receives commands from SHACK via RS485
 * and controls the physical rotator motors and reads position encoders.
 *
 * Responsibilities:
 * - Listen for RS485 commands from SHACK
 * - Control azimuth/elevation motors (relays)
 * - Read HH12 absolute encoders via SPI
 * - Send position updates to SHACK
 * - Motor safety timeout
 *
 * Author: ON7KGK Michael
 * Based on: K3NG Rotator Controller concepts
 */

#include <Arduino.h>
#include <SPI.h>
#include "config.h"
#include "RS485Protocol.h"
#include "HH12Reader.h"

// ===================================================================
// Global objects
// ===================================================================
RS485Protocol rs485;
HH12Reader azimuthEncoder;
HH12Reader elevationEncoder;

// ===================================================================
// Motor state
// ===================================================================
enum MotorState {
    MOTOR_STOPPED,
    MOTOR_CW,
    MOTOR_CCW,
    MOTOR_UP,
    MOTOR_DOWN
};

MotorState azimuthMotorState = MOTOR_STOPPED;
MotorState elevationMotorState = MOTOR_STOPPED;

uint32_t lastMotorCommandTime = 0;   // For safety timeout

// ===================================================================
// Position tracking
// ===================================================================
float currentAzimuth = 0.0;
float currentElevation = 0.0;
uint32_t lastPositionReadTime = 0;
uint32_t lastPositionReportTime = 0;

// ===================================================================
// Heartbeat
// ===================================================================
uint32_t lastHeartbeatTime = 0;
bool heartbeatLedState = false;

// ===================================================================
// Function prototypes
// ===================================================================
void setupPins();
void setupRS485();
void setupEncoders();
void readEncoders();
void handleRS485Commands();
void controlMotors();
void sendPositionReport();
void stopAllMotors();
void updateHeartbeat();

// ===================================================================
// Setup
// ===================================================================
void setup() {
    // Debug serial
    #ifdef DEBUG_ENABLED
    DEBUG_SERIAL.begin(DEBUG_BAUD);
    delay(1000);  // Wait for serial to initialize
    DEBUG_PRINTLN("=== ANTENNA UNIT Starting ===");
    #endif

    // Initialize pins
    setupPins();

    // Initialize SPI for encoders
    SPI.begin();
    setupEncoders();

    // Initialize RS485
    setupRS485();

    // All motors off at startup
    stopAllMotors();

    DEBUG_PRINTLN("ANTENNA UNIT Ready");
    DEBUG_PRINTLN("Waiting for SHACK commands...");
}

// ===================================================================
// Main loop
// ===================================================================
void loop() {
    // Handle incoming RS485 commands
    handleRS485Commands();

    // Read encoder positions periodically
    if (millis() - lastPositionReadTime >= ENCODER_READ_INTERVAL_MS) {
        readEncoders();
        lastPositionReadTime = millis();
    }

    // Send position report periodically
    if (millis() - lastPositionReportTime >= POSITION_REPORT_INTERVAL_MS) {
        sendPositionReport();
        lastPositionReportTime = millis();
    }

    // Control motors (includes safety timeout)
    controlMotors();

    // Update heartbeat LED
    updateHeartbeat();
}

// ===================================================================
// Setup pins
// ===================================================================
void setupPins() {
    // Motor relay pins
    pinMode(MOTOR_AZ_CW_PIN, OUTPUT);
    pinMode(MOTOR_AZ_CCW_PIN, OUTPUT);
    pinMode(MOTOR_EL_UP_PIN, OUTPUT);
    pinMode(MOTOR_EL_DOWN_PIN, OUTPUT);

    // Set all relays to inactive
    digitalWrite(MOTOR_AZ_CW_PIN, RELAY_ACTIVE_HIGH ? LOW : HIGH);
    digitalWrite(MOTOR_AZ_CCW_PIN, RELAY_ACTIVE_HIGH ? LOW : HIGH);
    digitalWrite(MOTOR_EL_UP_PIN, RELAY_ACTIVE_HIGH ? LOW : HIGH);
    digitalWrite(MOTOR_EL_DOWN_PIN, RELAY_ACTIVE_HIGH ? LOW : HIGH);

    // Status LED
    pinMode(LED_BUILTIN_PIN, OUTPUT);
    digitalWrite(LED_BUILTIN_PIN, LOW);

    DEBUG_PRINTLN("Pins configured");
}

// ===================================================================
// Setup RS485
// ===================================================================
void setupRS485() {
    RS485_SERIAL.begin(RS485_BAUD);
    rs485.begin(&RS485_SERIAL, RS485_DE_RE_PIN, RS485_BAUD);

    DEBUG_PRINT("RS485 initialized on Serial1 @ ");
    DEBUG_PRINT(RS485_BAUD);
    DEBUG_PRINTLN(" baud");
}

// ===================================================================
// Setup encoders
// ===================================================================
void setupEncoders() {
    azimuthEncoder.begin(HH12_AZ_CS_PIN, AZ_REVERSE_DIRECTION);
    elevationEncoder.begin(HH12_EL_CS_PIN, EL_REVERSE_DIRECTION);

    // Set offsets if configured
    if (AZ_OFFSET_DEGREES != 0.0) {
        azimuthEncoder.setOffsetDegrees(AZ_OFFSET_DEGREES);
    }
    if (EL_OFFSET_DEGREES != 0.0) {
        elevationEncoder.setOffsetDegrees(EL_OFFSET_DEGREES);
    }

    // Initial read
    currentAzimuth = azimuthEncoder.readDegrees();
    currentElevation = elevationEncoder.readDegrees();

    DEBUG_PRINT("Encoders initialized - AZ: ");
    DEBUG_PRINT(currentAzimuth);
    DEBUG_PRINT("°  EL: ");
    DEBUG_PRINT(currentElevation);
    DEBUG_PRINTLN("°");
}

// ===================================================================
// Read encoders
// ===================================================================
void readEncoders() {
    currentAzimuth = azimuthEncoder.readDegrees();
    currentElevation = elevationEncoder.readDegrees();

    // Limit elevation to 0-180° range
    if (currentElevation > 180.0) {
        currentElevation = 180.0;
    }
    if (currentElevation < 0.0) {
        currentElevation = 0.0;
    }
}

// ===================================================================
// Handle RS485 commands from SHACK
// ===================================================================
void handleRS485Commands() {
    if (!rs485.available()) return;

    uint8_t cmd;
    uint16_t param1, param2;

    if (rs485.receivePacket(cmd, param1, param2)) {
        DEBUG_PRINT("RX CMD: 0x");
        DEBUG_PRINT(cmd, HEX);
        DEBUG_PRINT(" P1:");
        DEBUG_PRINT(param1);
        DEBUG_PRINT(" P2:");
        DEBUG_PRINTLN(param2);

        // Update last command time for safety timeout
        lastMotorCommandTime = millis();

        // Process command
        switch (cmd) {
            case CMD_PING:
                // Respond to ping
                rs485.sendCommand(CMD_PONG, 0, 0);
                DEBUG_PRINTLN("-> PONG");
                break;

            case CMD_ROTATE_CW:
                azimuthMotorState = MOTOR_CW;
                DEBUG_PRINTLN("-> Rotate CW");
                break;

            case CMD_ROTATE_CCW:
                azimuthMotorState = MOTOR_CCW;
                DEBUG_PRINTLN("-> Rotate CCW");
                break;

            case CMD_ROTATE_UP:
                elevationMotorState = MOTOR_UP;
                DEBUG_PRINTLN("-> Rotate UP");
                break;

            case CMD_ROTATE_DOWN:
                elevationMotorState = MOTOR_DOWN;
                DEBUG_PRINTLN("-> Rotate DOWN");
                break;

            case CMD_ROTATE_STOP_AZ:
                azimuthMotorState = MOTOR_STOPPED;
                DEBUG_PRINTLN("-> Stop AZ");
                break;

            case CMD_ROTATE_STOP_EL:
                elevationMotorState = MOTOR_STOPPED;
                DEBUG_PRINTLN("-> Stop EL");
                break;

            case CMD_ROTATE_STOP_ALL:
                azimuthMotorState = MOTOR_STOPPED;
                elevationMotorState = MOTOR_STOPPED;
                DEBUG_PRINTLN("-> Stop ALL");
                break;

            case CMD_GET_POSITION:
                // Send position immediately
                sendPositionReport();
                break;

            default:
                DEBUG_PRINT("Unknown command: 0x");
                DEBUG_PRINTLN(cmd, HEX);
                break;
        }
    }
}

// ===================================================================
// Control motors based on current state
// ===================================================================
void controlMotors() {
    // Safety timeout - stop motors if no command received recently
    if ((azimuthMotorState != MOTOR_STOPPED || elevationMotorState != MOTOR_STOPPED) &&
        (millis() - lastMotorCommandTime > MOTOR_TIMEOUT_MS)) {
        DEBUG_PRINTLN("! MOTOR TIMEOUT - Stopping all");
        stopAllMotors();
        return;
    }

    // Control azimuth motor
    bool azCwActive = (azimuthMotorState == MOTOR_CW);
    bool azCcwActive = (azimuthMotorState == MOTOR_CCW);

    digitalWrite(MOTOR_AZ_CW_PIN, (RELAY_ACTIVE_HIGH && azCwActive) ? HIGH : LOW);
    digitalWrite(MOTOR_AZ_CCW_PIN, (RELAY_ACTIVE_HIGH && azCcwActive) ? HIGH : LOW);

    // Control elevation motor
    bool elUpActive = (elevationMotorState == MOTOR_UP);
    bool elDownActive = (elevationMotorState == MOTOR_DOWN);

    digitalWrite(MOTOR_EL_UP_PIN, (RELAY_ACTIVE_HIGH && elUpActive) ? HIGH : LOW);
    digitalWrite(MOTOR_EL_DOWN_PIN, (RELAY_ACTIVE_HIGH && elDownActive) ? HIGH : LOW);
}

// ===================================================================
// Send position report to SHACK
// ===================================================================
void sendPositionReport() {
    // Convert degrees to param format (degrees * 10)
    uint16_t azParam = (uint16_t)(currentAzimuth * 10.0);
    uint16_t elParam = (uint16_t)(currentElevation * 10.0);

    rs485.sendCommand(CMD_POSITION_REPORT, azParam, elParam);

    DEBUG_PRINT("TX Position: AZ=");
    DEBUG_PRINT(currentAzimuth);
    DEBUG_PRINT("° EL=");
    DEBUG_PRINT(currentElevation);
    DEBUG_PRINTLN("°");
}

// ===================================================================
// Stop all motors
// ===================================================================
void stopAllMotors() {
    azimuthMotorState = MOTOR_STOPPED;
    elevationMotorState = MOTOR_STOPPED;

    digitalWrite(MOTOR_AZ_CW_PIN, RELAY_ACTIVE_HIGH ? LOW : HIGH);
    digitalWrite(MOTOR_AZ_CCW_PIN, RELAY_ACTIVE_HIGH ? LOW : HIGH);
    digitalWrite(MOTOR_EL_UP_PIN, RELAY_ACTIVE_HIGH ? LOW : HIGH);
    digitalWrite(MOTOR_EL_DOWN_PIN, RELAY_ACTIVE_HIGH ? LOW : HIGH);
}

// ===================================================================
// Update heartbeat LED
// ===================================================================
void updateHeartbeat() {
    if (millis() - lastHeartbeatTime >= HEARTBEAT_INTERVAL_MS) {
        heartbeatLedState = !heartbeatLedState;
        digitalWrite(LED_BUILTIN_PIN, heartbeatLedState);
        lastHeartbeatTime = millis();
    }
}
