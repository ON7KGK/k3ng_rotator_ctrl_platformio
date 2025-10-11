/*
 * test_rs485_protocol.cpp
 *
 * Test avancé du protocole RS485 avec trames structurées et CRC16
 *
 * Ce programme teste:
 * - Construction de trames RS485_Frame
 * - Calcul et vérification CRC16
 * - Envoi/réception de trames complètes
 * - Validation de l'intégrité des données
 *
 * Pour tester:
 * 1. Charger ce programme sur les 2 Nano R4
 * 2. Connecter les modules RS485 (voir schéma)
 * 3. Ouvrir Serial Monitor (115200 bauds) sur chaque carte
 * 4. Observer l'échange de trames structurées
 */

#include <Arduino.h>
#include "rs485_config.h"
#include "rs485_protocol.h"

// Déclaration des fonctions externes (implémentées dans rs485_common.cpp)
extern uint16_t rs485_calculate_crc16(const uint8_t* data, uint16_t length);
extern bool rs485_verify_crc16(const uint8_t* data, uint16_t length, uint16_t crc);
extern void rs485_build_frame(RS485_Frame* frame, uint8_t addr, uint8_t cmd,
                              const uint8_t* data, uint8_t len);
extern bool rs485_parse_frame(RS485_Frame* frame, const uint8_t* buffer, uint16_t len);
extern bool rs485_send_frame(const RS485_Frame* frame);
extern bool rs485_receive_frame(RS485_Frame* frame, uint32_t timeout_ms);

// ============================================================================
// VARIABLES GLOBALES
// ============================================================================

uint32_t lastSendTime = 0;
uint32_t sendInterval = 1000;      // Envoyer toutes les 1000ms
uint32_t messageCounter = 0;
uint32_t rxSuccessCount = 0;
uint32_t rxErrorCount = 0;
uint32_t txCount = 0;

// État rotateur simulé pour les tests
RotatorState rotatorState;

// ============================================================================
// INITIALISATION RS485
// ============================================================================

void rs485_init() {
    // Configuration des pins DE/RE
    pinMode(RS485_DE_PIN, OUTPUT);
    pinMode(RS485_RE_PIN, OUTPUT);

    // Mode réception par défaut
    RS485_RX_MODE();

    // Configuration Serial1 pour RS485
    RS485_SERIAL.begin(RS485_BAUD_RATE, RS485_DATA_BITS);

    // Attendre que le port série soit prêt
    while (!RS485_SERIAL) {
        delay(10);
    }

    Serial.println("RS485 Protocol initialized");
    Serial.print("Baud rate: ");
    Serial.println(RS485_BAUD_RATE);
    Serial.print("DE pin: ");
    Serial.println(RS485_DE_PIN);
    Serial.print("RE pin: ");
    Serial.println(RS485_RE_PIN);
    Serial.println();
}

// ============================================================================
// TEST CRC16
// ============================================================================

void test_crc16() {
    Serial.println("=== CRC16 Test ===");

    // Test avec données connues
    uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint16_t crc = rs485_calculate_crc16(test_data, 5);

    Serial.print("Test data: ");
    for (int i = 0; i < 5; i++) {
        Serial.print("0x");
        Serial.print(test_data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    Serial.print("Calculated CRC16: 0x");
    Serial.println(crc, HEX);

    // Vérifier le CRC
    bool valid = rs485_verify_crc16(test_data, 5, crc);
    Serial.print("CRC verification: ");
    Serial.println(valid ? "OK" : "FAILED");
    Serial.println();
}

// ============================================================================
// ENVOI TRAME DE TEST
// ============================================================================

void send_test_frame() {
    RS485_Frame frame;

    // Simuler des données de position
    rotatorState.azimuth_current = 180.5 + (messageCounter % 360);
    rotatorState.elevation_current = 45.2 + (messageCounter % 90);
    rotatorState.azimuth_target = 270.0;
    rotatorState.elevation_target = 60.0;
    rotatorState.status_flags = STATUS_MOVING_AZ | STATUS_GPS_VALID;
    rotatorState.error_code = ERR_NONE;
    rotatorState.timestamp = millis();

    // Construire la trame
    rs485_build_frame(&frame, REMOTE_ADDRESS, CMD_GET_POSITION,
                     (uint8_t*)&rotatorState, sizeof(RotatorState));

    // Envoyer
    if (rs485_send_frame(&frame)) {
        txCount++;
        Serial.print("[TX #");
        Serial.print(txCount);
        Serial.print("] CMD: 0x");
        Serial.print(frame.command, HEX);
        Serial.print(" | AZ: ");
        Serial.print(rotatorState.azimuth_current, 1);
        Serial.print("° | EL: ");
        Serial.print(rotatorState.elevation_current, 1);
        Serial.print("° | CRC: 0x");
        Serial.println(frame.crc16, HEX);
    } else {
        Serial.println("[TX] Send failed!");
    }
}

// ============================================================================
// RÉCEPTION TRAME
// ============================================================================

void receive_test_frame() {
    RS485_Frame frame;

    // Essayer de recevoir une trame (timeout court)
    if (rs485_receive_frame(&frame, 50)) {
        rxSuccessCount++;

        Serial.print("[RX #");
        Serial.print(rxSuccessCount);
        Serial.print("] CMD: 0x");
        Serial.print(frame.command, HEX);
        Serial.print(" | Addr: 0x");
        Serial.print(frame.address, HEX);
        Serial.print(" | Len: ");
        Serial.print(frame.length);
        Serial.print(" | CRC: 0x");
        Serial.print(frame.crc16, HEX);

        // Si c'est une trame de position, décoder
        if (frame.command == CMD_GET_POSITION && frame.length == sizeof(RotatorState)) {
            RotatorState* state = (RotatorState*)frame.data;

            Serial.print(" | AZ: ");
            Serial.print(state->azimuth_current, 1);
            Serial.print("° | EL: ");
            Serial.print(state->elevation_current, 1);
            Serial.print("° | Status: 0x");
            Serial.print(state->status_flags, HEX);
        }

        Serial.println();
    }
}

// ============================================================================
// AFFICHAGE STATISTIQUES
// ============================================================================

void print_statistics() {
    static uint32_t lastStatTime = 0;

    if (millis() - lastStatTime > 10000) {  // Toutes les 10 secondes
        lastStatTime = millis();

        Serial.println();
        Serial.println("=== Statistics ===");
        Serial.print("TX count: ");
        Serial.println(txCount);
        Serial.print("RX success: ");
        Serial.println(rxSuccessCount);
        Serial.print("RX errors: ");
        Serial.println(rxErrorCount);

        if (txCount > 0) {
            float successRate = (rxSuccessCount * 100.0) / txCount;
            Serial.print("Success rate: ");
            Serial.print(successRate, 1);
            Serial.println("%");
        }

        Serial.println();
    }
}

// ============================================================================
// SETUP
// ============================================================================

void setup() {
    // Initialiser Serial pour debug USB
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }

    Serial.println("\n\n====================================");
    Serial.println("  K3NG Rotator - RS485 Protocol Test");
    Serial.println("  Arduino Nano R4 Minima");
    Serial.println("  Phase 2: Frames + CRC16");
    Serial.println("====================================\n");

    // Test CRC16
    test_crc16();

    // Initialiser RS485
    rs485_init();

    Serial.println("Test started!");
    Serial.println("Sending position frames every second...\n");

    delay(1000);
}

// ============================================================================
// LOOP
// ============================================================================

void loop() {
    // Vérifier s'il y a des trames à recevoir
    receive_test_frame();

    // Envoyer une trame périodiquement
    uint32_t currentTime = millis();
    if (currentTime - lastSendTime >= sendInterval) {
        lastSendTime = currentTime;
        messageCounter++;

        send_test_frame();
    }

    // Afficher les statistiques
    print_statistics();

    // Petit délai
    delay(10);
}
