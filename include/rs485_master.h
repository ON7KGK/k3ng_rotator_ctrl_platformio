/*
 * rs485_master.h
 *
 * RS485 Master (ANTENNA Unit)
 * - Gère les capteurs de position (potentiomètres, encodeurs)
 * - Contrôle les moteurs AZ/EL
 * - Répond aux requêtes du Remote
 * - Envoie des mises à jour périodiques
 */

#ifndef RS485_MASTER_H
#define RS485_MASTER_H

#include <Arduino.h>
#include "rs485_protocol.h"
#include "rs485_config.h"

// ============================================================================
// ÉTATS DU MASTER
// ============================================================================

enum MasterState : uint8_t {
    MASTER_IDLE = 0,
    MASTER_WAITING_REQUEST,
    MASTER_PROCESSING_CMD,
    MASTER_SENDING_RESPONSE,
    MASTER_SENDING_UPDATE
};

// ============================================================================
// CLASSE RS485 MASTER
// ============================================================================

class RS485_Master {
private:
    MasterState state;
    RotatorState rotatorState;
    uint32_t lastUpdateTime;
    uint32_t updateInterval;
    uint32_t requestCount;
    uint32_t responseCount;

    // Traitement des commandes
    void processCommand(RS485_Frame* request);
    void sendResponse(uint8_t command, const uint8_t* data, uint8_t len);
    void sendACK();
    void sendNAK(uint8_t errorCode);

    // Mise à jour de l'état
    void updateRotatorState();
    void sendStatusUpdate();

public:
    RS485_Master();

    // Initialisation
    void begin();

    // Boucle principale
    void process();

    // Accesseurs
    RotatorState* getState() { return &rotatorState; }
    void setPosition(float az, float el);
    void setTarget(float az, float el);
    void setStatus(uint8_t flags);

    // Statistiques
    uint32_t getRequestCount() { return requestCount; }
    uint32_t getResponseCount() { return responseCount; }
};

// ============================================================================
// FONCTIONS GLOBALES
// ============================================================================

// Initialisation du master
void rs485_master_init();

// Traitement périodique
void rs485_master_process();

// Mise à jour de l'état rotateur
void rs485_master_update_position(float azimuth, float elevation);
void rs485_master_update_target(float azimuth, float elevation);
void rs485_master_update_status(uint8_t flags);

#endif // RS485_MASTER_H
