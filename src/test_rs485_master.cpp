/*
 * test_rs485_master.cpp
 *
 * Test RS485 Master (ANTENNA Unit) - Phase 3
 *
 * Ce programme teste:
 * - Réception des requêtes du Remote
 * - Envoi des réponses avec l'état du rotateur
 * - Broadcast périodique des positions
 * - Pas de collision (le Master ne transmet que en réponse ou broadcast)
 *
 * À charger sur le Nano R4 de l'unité ANTENNE
 */

#include <Arduino.h>
#include "rs485_config.h"
#include "rs485_protocol.h"
#include "rs485_master.h"

// Déclarations externes
extern void rs485_master_init();
extern void rs485_master_process();

// ============================================================================
// VARIABLES GLOBALES
// ============================================================================

extern RS485_Master* masterInstance;  // Déclaré dans rs485_master.cpp

// ============================================================================
// SIMULATION CAPTEURS
// ============================================================================

void simulate_sensors() {
    // Simuler la lecture des potentiomètres/encodeurs
    // Dans la vraie application, ici on lira les pins analogiques

    static float sim_az = 180.0;
    static float sim_el = 45.0;
    static float target_az = 270.0;
    static float target_el = 60.0;

    // Simuler un mouvement progressif vers la cible
    if (abs(sim_az - target_az) > 1.0) {
        sim_az += (target_az > sim_az) ? 0.5 : -0.5;
    }

    if (abs(sim_el - target_el) > 0.5) {
        sim_el += (target_el > sim_el) ? 0.2 : -0.2;
    }

    // Mettre à jour l'état
    rs485_master_update_position(sim_az, sim_el);
    rs485_master_update_target(target_az, target_el);

    // Flags de status
    uint8_t status = 0;
    if (abs(sim_az - target_az) > 1.0) status |= STATUS_MOVING_AZ;
    if (abs(sim_el - target_el) > 0.5) status |= STATUS_MOVING_EL;
    status |= STATUS_GPS_VALID;  // Simuler GPS OK

    rs485_master_update_status(status);
}

// ============================================================================
// STATISTIQUES
// ============================================================================

void print_statistics() {
    static uint32_t lastStatTime = 0;

    if (millis() - lastStatTime > 10000) {  // Toutes les 10 secondes
        lastStatTime = millis();

        Serial.println();
        Serial.println("=== MASTER Statistics ===");
        // TODO: Ajouter accesseurs dans la classe Master
        Serial.println("Waiting for requests from Remote...");
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
    Serial.println("  K3NG Rotator - RS485 MASTER");
    Serial.println("  ANTENNA Unit");
    Serial.println("  Arduino Nano R4 Minima");
    Serial.println("  Phase 3: Master/Slave");
    Serial.println("====================================\n");

    // Initialiser RS485 Master
    rs485_master_init();

    Serial.println("Master started!");
    Serial.println("Listening for commands from Remote...");
    Serial.println("Broadcasting position every 100ms...\n");

    delay(1000);
}

// ============================================================================
// LOOP
// ============================================================================

void loop() {
    // Simuler la lecture des capteurs
    simulate_sensors();

    // Traiter les requêtes RS485 et envoyer les broadcasts
    rs485_master_process();

    // Afficher les statistiques
    print_statistics();

    // Petit délai
    delay(10);
}
