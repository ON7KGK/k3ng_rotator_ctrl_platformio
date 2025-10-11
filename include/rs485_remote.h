/*
 * rs485_remote.h
 *
 * RS485 Remote (SHACK/MAISON Unit)
 * - Interface utilisateur (display, boutons, encodeurs)
 * - Envoie des commandes au Master
 * - Reçoit et affiche les positions
 * - Polling périodique
 */

#ifndef RS485_REMOTE_H
#define RS485_REMOTE_H

#include <Arduino.h>
#include "rs485_protocol.h"
#include "rs485_config.h"

// ============================================================================
// ÉTATS DU REMOTE
// ============================================================================

enum RemoteState : uint8_t {
    REMOTE_IDLE = 0,
    REMOTE_SENDING_REQUEST,
    REMOTE_WAITING_RESPONSE,
    REMOTE_PROCESSING_RESPONSE,
    REMOTE_TIMEOUT
};

// ============================================================================
// CLASSE RS485 REMOTE
// ============================================================================

class RS485_Remote {
private:
    RemoteState state;
    RotatorState remoteRotatorState;  // État reçu du Master
    uint32_t lastPollTime;
    uint32_t pollInterval;
    uint32_t requestCount;
    uint32_t responseCount;
    uint32_t timeoutCount;
    uint8_t retryCount;

    // Gestion des requêtes
    bool sendRequest(uint8_t command, const uint8_t* data, uint8_t len);
    bool waitResponse(RS485_Frame* response, uint32_t timeout_ms);
    void processResponse(RS485_Frame* response);

    // Polling
    void pollPosition();

public:
    RS485_Remote();

    // Initialisation
    void begin();

    // Boucle principale
    void process();

    // Commandes vers le Master
    bool requestPosition();
    bool requestStatus();
    bool sendMoveTo(float azimuth, float elevation);
    bool sendStop();
    bool sendPark();

    // Accesseurs
    RotatorState* getState() { return &remoteRotatorState; }
    bool isConnected();

    // Statistiques
    uint32_t getRequestCount() { return requestCount; }
    uint32_t getResponseCount() { return responseCount; }
    uint32_t getTimeoutCount() { return timeoutCount; }
    float getSuccessRate();
};

// ============================================================================
// FONCTIONS GLOBALES
// ============================================================================

// Initialisation du remote
void rs485_remote_init();

// Traitement périodique
void rs485_remote_process();

// Commandes
bool rs485_remote_request_position();
bool rs485_remote_move_to(float azimuth, float elevation);
bool rs485_remote_stop();
bool rs485_remote_park();

// Accesseurs état
RotatorState* rs485_remote_get_state();

#endif // RS485_REMOTE_H
