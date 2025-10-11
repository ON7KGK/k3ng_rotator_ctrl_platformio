/*
 * test_rs485_remote.cpp
 *
 * Test RS485 Remote (SHACK/MAISON Unit) - Phase 3
 *
 * Ce programme teste:
 * - Polling périodique du Master
 * - Réception et affichage des positions
 * - Envoi de commandes (Move To, Stop, Park)
 * - Interface utilisateur simulée
 * - Pas de collision (le Remote attend toujours la réponse)
 *
 * À charger sur le Nano R4 de l'unité MAISON/SHACK
 */

#include <Arduino.h>
#include "rs485_config.h"
#include "rs485_protocol.h"
#include "rs485_remote.h"

// Déclarations externes
extern void rs485_remote_init();
extern void rs485_remote_process();
extern RotatorState* rs485_remote_get_state();
extern bool rs485_remote_move_to(float azimuth, float elevation);
extern bool rs485_remote_stop();
extern bool rs485_remote_park();

// ============================================================================
// VARIABLES GLOBALES
// ============================================================================

uint32_t lastDisplayUpdate = 0;
uint32_t displayInterval = 2000;  // Afficher toutes les 2 secondes

uint32_t lastCommandTime = 0;
uint32_t commandInterval = 15000;  // Envoyer une commande toutes les 15 secondes
uint8_t commandIndex = 0;

// ============================================================================
// AFFICHAGE POSITION
// ============================================================================

void display_position() {
    RotatorState* state = rs485_remote_get_state();

    if (!state) {
        Serial.println("No state data available");
        return;
    }

    Serial.println("\n--- Position Update ---");
    Serial.print("Azimuth:   ");
    Serial.print(state->azimuth_current, 1);
    Serial.print("° -> ");
    Serial.print(state->azimuth_target, 1);
    Serial.println("°");

    Serial.print("Elevation: ");
    Serial.print(state->elevation_current, 1);
    Serial.print("° -> ");
    Serial.print(state->elevation_target, 1);
    Serial.println("°");

    Serial.print("Status:    0x");
    Serial.print(state->status_flags, HEX);
    Serial.print(" [");
    if (state->status_flags & STATUS_MOVING_AZ) Serial.print("AZ_MOVING ");
    if (state->status_flags & STATUS_MOVING_EL) Serial.print("EL_MOVING ");
    if (state->status_flags & STATUS_GPS_VALID) Serial.print("GPS_OK ");
    if (state->status_flags & STATUS_ERROR) Serial.print("ERROR ");
    Serial.println("]");

    Serial.print("Age:       ");
    Serial.print(millis() - state->timestamp);
    Serial.println(" ms");
}

// ============================================================================
// SIMULATION COMMANDES UTILISATEUR
// ============================================================================

void simulate_user_commands() {
    uint32_t currentTime = millis();

    // Envoyer une commande périodiquement pour tester
    if (currentTime - lastCommandTime >= commandInterval) {
        lastCommandTime = currentTime;

        Serial.println("\n>>> Sending test command...");

        switch (commandIndex) {
            case 0:
                Serial.println("Command: Move to AZ=270° EL=60°");
                if (rs485_remote_move_to(270.0, 60.0)) {
                    Serial.println("✓ Move command accepted");
                } else {
                    Serial.println("✗ Move command failed");
                }
                break;

            case 1:
                Serial.println("Command: Move to AZ=180° EL=45°");
                if (rs485_remote_move_to(180.0, 45.0)) {
                    Serial.println("✓ Move command accepted");
                } else {
                    Serial.println("✗ Move command failed");
                }
                break;

            case 2:
                Serial.println("Command: STOP");
                if (rs485_remote_stop()) {
                    Serial.println("✓ Stop command accepted");
                } else {
                    Serial.println("✗ Stop command failed");
                }
                break;

            case 3:
                Serial.println("Command: PARK (go to 0°/0°)");
                if (rs485_remote_park()) {
                    Serial.println("✓ Park command accepted");
                } else {
                    Serial.println("✗ Park command failed");
                }
                break;
        }

        commandIndex = (commandIndex + 1) % 4;
    }
}

// ============================================================================
// STATISTIQUES
// ============================================================================

void print_statistics() {
    static uint32_t lastStatTime = 0;

    if (millis() - lastStatTime > 10000) {  // Toutes les 10 secondes
        lastStatTime = millis();

        Serial.println();
        Serial.println("=== REMOTE Statistics ===");
        // TODO: Ajouter accesseurs statistiques
        Serial.println("Polling Master every 100ms...");
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
    Serial.println("  K3NG Rotator - RS485 REMOTE");
    Serial.println("  SHACK/MAISON Unit");
    Serial.println("  Arduino Nano R4 Minima");
    Serial.println("  Phase 3: Master/Slave");
    Serial.println("====================================\n");

    // Initialiser RS485 Remote
    rs485_remote_init();

    Serial.println("Remote started!");
    Serial.println("Polling Master for position updates...");
    Serial.println("Sending test commands every 15 seconds...\n");

    delay(1000);
}

// ============================================================================
// LOOP
// ============================================================================

void loop() {
    // Traiter les communications RS485 (polling + broadcast)
    rs485_remote_process();

    // Afficher la position périodiquement
    uint32_t currentTime = millis();
    if (currentTime - lastDisplayUpdate >= displayInterval) {
        lastDisplayUpdate = currentTime;
        display_position();
    }

    // Simuler des commandes utilisateur
    simulate_user_commands();

    // Afficher les statistiques
    print_statistics();

    // Petit délai
    delay(10);
}
