/*
 * test_rs485.cpp
 *
 * Test de base communication RS485 pour Arduino Nano R4 Minima
 *
 * Ce programme teste:
 * - Configuration Serial1 pour RS485
 * - Contrôle pins DE/RE (8 et 9)
 * - Émission d'un message "TEST" toutes les secondes
 * - Réception et affichage des données reçues
 *
 * Pour tester:
 * 1. Charger ce programme sur les 2 Nano R4
 * 2. Connecter les modules RS485:
 *    - Nano1 TX1 -> RS485-1 DI
 *    - Nano1 RX1 -> RS485-1 RO
 *    - Nano1 D8  -> RS485-1 DE
 *    - Nano1 D9  -> RS485-1 RE
 *    - RS485-1 A/B <-> RS485-2 A/B (paire torsadée)
 *    - Résistances 120Ω aux extrémités A-B
 * 3. Ouvrir Serial Monitor (115200 bauds) sur chaque carte
 * 4. Observer l'échange de messages
 */

#include <Arduino.h>
#include "rs485_config.h"
#include "rs485_protocol.h"

// ============================================================================
// VARIABLES GLOBALES
// ============================================================================

uint32_t lastSendTime = 0;
uint32_t sendInterval = 1000;  // Envoyer toutes les 1000ms
uint32_t messageCounter = 0;

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

    RS485_DEBUGLN("RS485 initialized");
    RS485_DEBUG("Baud rate: ");
    RS485_DEBUGLN(RS485_BAUD_RATE);
    RS485_DEBUG("DE pin: ");
    RS485_DEBUGLN(RS485_DE_PIN);
    RS485_DEBUG("RE pin: ");
    RS485_DEBUGLN(RS485_RE_PIN);
}

// ============================================================================
// ENVOI MESSAGE RS485
// ============================================================================

void rs485_send_string(const char* message) {
    // Passer en mode transmission
    RS485_TX_MODE();

    // Petit délai pour stabilisation
    delayMicroseconds(RS485_CHAR_DELAY_US);

    // Envoyer le message
    RS485_SERIAL.print(message);

    // Attendre que la transmission soit complète
    RS485_SERIAL.flush();

    // Délai supplémentaire pour s'assurer que tout est envoyé
    delayMicroseconds(RS485_CHAR_DELAY_US);

    // Repasser en mode réception
    RS485_RX_MODE();

    RS485_DEBUG("Sent: ");
    RS485_DEBUGLN(message);
}

// ============================================================================
// RÉCEPTION MESSAGE RS485
// ============================================================================

void rs485_receive() {
    if (RS485_SERIAL.available()) {
        String received = "";

        // Lire tous les caractères disponibles
        while (RS485_SERIAL.available()) {
            char c = RS485_SERIAL.read();
            received += c;

            // Petit délai pour permettre aux chars suivants d'arriver
            delayMicroseconds(100);
        }

        if (received.length() > 0) {
            RS485_DEBUG("Received: ");
            RS485_DEBUGLN(received);
        }
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
    Serial.println("  K3NG Rotator - RS485 Test");
    Serial.println("  Arduino Nano R4 Minima");
    Serial.println("====================================\n");

    // Initialiser RS485
    rs485_init();

    Serial.println("\nTest started!");
    Serial.println("Sending 'TEST' message every second...\n");

    delay(1000);
}

// ============================================================================
// LOOP
// ============================================================================

void loop() {
    // Vérifier s'il y a des données à recevoir
    rs485_receive();

    // Envoyer un message périodiquement
    uint32_t currentTime = millis();
    if (currentTime - lastSendTime >= sendInterval) {
        lastSendTime = currentTime;
        messageCounter++;

        // Créer le message avec compteur
        char message[32];
        snprintf(message, sizeof(message), "TEST #%lu\n", messageCounter);

        // Envoyer via RS485
        rs485_send_string(message);
    }

    // Petit délai pour ne pas surcharger le CPU
    delay(10);
}
