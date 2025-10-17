/*
 * rs485_master.cpp
 *
 * Implémentation RS485 Master (ANTENNA Unit)
 */

#include "rs485_master.h"

// Déclarations externes (rs485_common.cpp)
extern void rs485_build_frame(RS485_Frame* frame, uint8_t addr, uint8_t cmd,
                              const uint8_t* data, uint8_t len);
extern bool rs485_send_frame(const RS485_Frame* frame);
extern bool rs485_receive_frame(RS485_Frame* frame, uint32_t timeout_ms);

// Instance globale
static RS485_Master* masterInstance = nullptr;

// ============================================================================
// CONSTRUCTEUR
// ============================================================================

RS485_Master::RS485_Master() {
    state = MASTER_IDLE;
    lastUpdateTime = 0;
    updateInterval = RS485_UPDATE_RATE;  // 100ms par défaut
    requestCount = 0;
    responseCount = 0;

    // Initialiser l'état du rotateur
    rotatorState.azimuth_current = 0.0;
    rotatorState.elevation_current = 0.0;
    rotatorState.azimuth_target = 0.0;
    rotatorState.elevation_target = 0.0;
    rotatorState.status_flags = 0;
    rotatorState.error_code = ERR_NONE;
    rotatorState.timestamp = 0;
}

// ============================================================================
// INITIALISATION
// ============================================================================

void RS485_Master::begin() {
    // Configuration pins DE/RE
    pinMode(RS485_DE_PIN, OUTPUT);
    pinMode(RS485_RE_PIN, OUTPUT);

    // Mode réception par défaut
    RS485_RX_MODE();

    // Configuration Serial1
    RS485_SERIAL.begin(RS485_BAUD_RATE, RS485_DATA_BITS);

    RS485_DEBUGLN("RS485 Master initialized");
    state = MASTER_WAITING_REQUEST;
}

// ============================================================================
// MISE À JOUR ÉTAT ROTATEUR
// ============================================================================

void RS485_Master::updateRotatorState() {
    // Simuler des variations de position (sera remplacé par lecture capteurs)
    // Pour le test, on fait varier légèrement la position
    static float az_sim = 180.0;
    static float el_sim = 45.0;
    static int8_t az_dir = 1;
    static int8_t el_dir = 1;

    az_sim += az_dir * 0.5;
    el_sim += el_dir * 0.2;

    if (az_sim > 360.0) { az_sim = 0.0; }
    if (az_sim < 0.0) { az_sim = 360.0; }
    if (el_sim > 90.0) { el_dir = -1; }
    if (el_sim < 0.0) { el_dir = 1; }

    rotatorState.azimuth_current = az_sim;
    rotatorState.elevation_current = el_sim;
    rotatorState.timestamp = millis();
}

// ============================================================================
// TRAITEMENT DES COMMANDES
// ============================================================================

void RS485_Master::processCommand(RS485_Frame* request) {
    RS485_DEBUG("Master processing command: 0x");
    RS485_DEBUG_HEX(request->command);
    RS485_DEBUGLN("");

    switch (request->command) {
        case CMD_GET_POSITION:
            // Envoyer la position actuelle
            sendResponse(RSP_POSITION_DATA, (uint8_t*)&rotatorState, sizeof(RotatorState));
            break;

        case CMD_GET_STATUS:
            // Envoyer l'état complet
            sendResponse(RSP_STATUS_DATA, (uint8_t*)&rotatorState, sizeof(RotatorState));
            break;

        case CMD_MOVE_TO:
            // Extraire la position cible
            if (request->length >= 8) {
                float* targets = (float*)request->data;
                rotatorState.azimuth_target = targets[0];
                rotatorState.elevation_target = targets[1];

                RS485_DEBUG("Move to AZ: ");
                RS485_DEBUG(rotatorState.azimuth_target);
                RS485_DEBUG(" EL: ");
                RS485_DEBUGLN(rotatorState.elevation_target);

                sendACK();
            } else {
                sendNAK(ERR_INVALID_DATA);
            }
            break;

        case CMD_STOP:
            // Arrêt d'urgence
            rotatorState.status_flags &= ~(STATUS_MOVING_AZ | STATUS_MOVING_EL);
            sendACK();
            RS485_DEBUGLN("Emergency stop!");
            break;

        case CMD_PARK:
            // Aller à la position parking
            rotatorState.azimuth_target = 0.0;
            rotatorState.elevation_target = 0.0;
            sendACK();
            RS485_DEBUGLN("Parking...");
            break;

        default:
            sendNAK(ERR_INVALID_CMD);
            break;
    }

    responseCount++;
}

// ============================================================================
// ENVOI RÉPONSE
// ============================================================================

void RS485_Master::sendResponse(uint8_t command, const uint8_t* data, uint8_t len) {
    RS485_Frame response;
    rs485_build_frame(&response, REMOTE_ADDRESS, command, data, len);
    rs485_send_frame(&response);
}

void RS485_Master::sendACK() {
    uint8_t ackData = ERR_NONE;
    sendResponse(RSP_ACK, &ackData, 1);
}

void RS485_Master::sendNAK(uint8_t errorCode) {
    sendResponse(RSP_NAK, &errorCode, 1);
}

// ============================================================================
// ENVOI MISE À JOUR PÉRIODIQUE
// ============================================================================

void RS485_Master::sendStatusUpdate() {
    RS485_Frame update;
    rs485_build_frame(&update, BROADCAST_ADDRESS, MSG_POSITION_UPDATE,
                     (uint8_t*)&rotatorState, sizeof(RotatorState));
    rs485_send_frame(&update);

    RS485_DEBUG("Status update sent: AZ=");
    RS485_DEBUG(rotatorState.azimuth_current);
    RS485_DEBUG(" EL=");
    RS485_DEBUGLN(rotatorState.elevation_current);
}

// ============================================================================
// BOUCLE PRINCIPALE
// ============================================================================

void RS485_Master::process() {
    // Mettre à jour l'état du rotateur
    updateRotatorState();

    // Vérifier s'il y a une requête du Remote
    RS485_Frame request;
    if (rs485_receive_frame(&request, 10)) {  // Timeout court (10ms)
        requestCount++;
        state = MASTER_PROCESSING_CMD;
        processCommand(&request);
        state = MASTER_WAITING_REQUEST;
    }

    // Envoyer des mises à jour périodiques (broadcast)
    uint32_t currentTime = millis();
    if (currentTime - lastUpdateTime >= updateInterval) {
        lastUpdateTime = currentTime;
        sendStatusUpdate();
    }
}

// ============================================================================
// SETTERS
// ============================================================================

void RS485_Master::setPosition(float az, float el) {
    rotatorState.azimuth_current = az;
    rotatorState.elevation_current = el;
}

void RS485_Master::setTarget(float az, float el) {
    rotatorState.azimuth_target = az;
    rotatorState.elevation_target = el;
}

void RS485_Master::setStatus(uint8_t flags) {
    rotatorState.status_flags = flags;
}

// ============================================================================
// FONCTIONS GLOBALES
// ============================================================================

void rs485_master_init() {
    // Serial already initialized in main setup()
    // Just initialize the Master instance
    if (!masterInstance) {
        masterInstance = new RS485_Master();
    }
    masterInstance->begin();
}

void rs485_master_process() {
    if (masterInstance) {
        masterInstance->process();
    }
}

void rs485_master_update_position(float azimuth, float elevation) {
    if (masterInstance) {
        masterInstance->setPosition(azimuth, elevation);
    }
}

void rs485_master_update_target(float azimuth, float elevation) {
    if (masterInstance) {
        masterInstance->setTarget(azimuth, elevation);
    }
}

void rs485_master_update_status(uint8_t flags) {
    if (masterInstance) {
        masterInstance->setStatus(flags);
    }
}
