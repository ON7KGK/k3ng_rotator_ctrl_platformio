/*
 * rs485_remote.cpp
 *
 * Implémentation RS485 Remote (SHACK/MAISON Unit)
 */

#include "rs485_remote.h"

// Déclarations externes (rs485_common.cpp)
extern void rs485_build_frame(RS485_Frame* frame, uint8_t addr, uint8_t cmd,
                              const uint8_t* data, uint8_t len);
extern bool rs485_send_frame(const RS485_Frame* frame);
extern bool rs485_receive_frame(RS485_Frame* frame, uint32_t timeout_ms);

// Instance globale
static RS485_Remote* remoteInstance = nullptr;

// ============================================================================
// CONSTRUCTEUR
// ============================================================================

RS485_Remote::RS485_Remote() {
    state = REMOTE_IDLE;
    lastPollTime = 0;
    pollInterval = RS485_UPDATE_RATE;  // 100ms par défaut
    requestCount = 0;
    responseCount = 0;
    timeoutCount = 0;
    retryCount = 0;

    // Initialiser l'état
    remoteRotatorState.azimuth_current = 0.0;
    remoteRotatorState.elevation_current = 0.0;
    remoteRotatorState.azimuth_target = 0.0;
    remoteRotatorState.elevation_target = 0.0;
    remoteRotatorState.status_flags = 0;
    remoteRotatorState.error_code = ERR_NONE;
    remoteRotatorState.timestamp = 0;
}

// ============================================================================
// INITIALISATION
// ============================================================================

void RS485_Remote::begin() {
    // Configuration pins DE/RE
    pinMode(RS485_DE_PIN, OUTPUT);
    pinMode(RS485_RE_PIN, OUTPUT);

    // Mode réception par défaut
    RS485_RX_MODE();

    // Configuration Serial1
    RS485_SERIAL.begin(RS485_BAUD_RATE, RS485_DATA_BITS);

    RS485_DEBUGLN("RS485 Remote initialized");
    state = REMOTE_IDLE;
}

// ============================================================================
// ENVOI REQUÊTE
// ============================================================================

bool RS485_Remote::sendRequest(uint8_t command, const uint8_t* data, uint8_t len) {
    RS485_Frame request;
    rs485_build_frame(&request, MASTER_ADDRESS, command, data, len);

    requestCount++;
    return rs485_send_frame(&request);
}

// ============================================================================
// ATTENTE RÉPONSE
// ============================================================================

bool RS485_Remote::waitResponse(RS485_Frame* response, uint32_t timeout_ms) {
    if (rs485_receive_frame(response, timeout_ms)) {
        responseCount++;
        return true;
    }

    timeoutCount++;
    RS485_DEBUGLN("Remote: timeout waiting response");
    return false;
}

// ============================================================================
// TRAITEMENT RÉPONSE
// ============================================================================

void RS485_Remote::processResponse(RS485_Frame* response) {
    switch (response->command) {
        case RSP_POSITION_DATA:
        case RSP_STATUS_DATA:
        case MSG_POSITION_UPDATE:
            // Copier l'état reçu
            if (response->length == sizeof(RotatorState)) {
                memcpy(&remoteRotatorState, response->data, sizeof(RotatorState));

                // Mettre à jour le timestamp local pour calculer l'âge correctement
                remoteRotatorState.timestamp = millis();

                RS485_DEBUG("Position updated: AZ=");
                RS485_DEBUG(remoteRotatorState.azimuth_current);
                RS485_DEBUG(" EL=");
                RS485_DEBUG(remoteRotatorState.elevation_current);
                RS485_DEBUG(" Status=0x");
                RS485_DEBUG_HEX(remoteRotatorState.status_flags);
                RS485_DEBUGLN("");
            }
            break;

        case RSP_ACK:
            RS485_DEBUGLN("Remote: ACK received");
            break;

        case RSP_NAK:
            RS485_DEBUG("Remote: NAK received, error code: ");
            if (response->length > 0) {
                RS485_DEBUGLN(response->data[0]);
            } else {
                RS485_DEBUGLN("unknown");
            }
            break;

        default:
            RS485_DEBUG("Remote: unknown response: 0x");
            RS485_DEBUG_HEX(response->command);
            RS485_DEBUGLN("");
            break;
    }
}

// ============================================================================
// POLLING POSITION
// ============================================================================

void RS485_Remote::pollPosition() {
    // Envoyer une requête de position
    if (sendRequest(CMD_GET_POSITION, nullptr, 0)) {
        state = REMOTE_WAITING_RESPONSE;

        // Attendre la réponse
        RS485_Frame response;
        if (waitResponse(&response, RS485_TIMEOUT_MS)) {
            processResponse(&response);
            state = REMOTE_IDLE;
            retryCount = 0;
        } else {
            // Timeout
            state = REMOTE_TIMEOUT;
            retryCount++;

            if (retryCount >= RS485_RETRY_COUNT) {
                RS485_DEBUGLN("Remote: max retries reached");
                retryCount = 0;
                state = REMOTE_IDLE;
            }
        }
    }
}

// ============================================================================
// BOUCLE PRINCIPALE
// ============================================================================

void RS485_Remote::process() {
    // Écouter les broadcasts du Master (mises à jour non sollicitées)
    RS485_Frame broadcast;
    if (rs485_receive_frame(&broadcast, 50)) {  // Timeout 50ms (trame 31 bytes ≈ 16ms)
        if (broadcast.address == BROADCAST_ADDRESS ||
            broadcast.address == REMOTE_ADDRESS) {
            processResponse(&broadcast);
        }
    }

    // POLLING DÉSACTIVÉ - On utilise uniquement les broadcasts du Master
    // Le Master envoie déjà les positions toutes les 100ms en broadcast
    // Le polling est redondant et crée des timeouts inutiles

    /*
    // Polling périodique (DÉSACTIVÉ)
    uint32_t currentTime = millis();
    if (currentTime - lastPollTime >= pollInterval) {
        lastPollTime = currentTime;
        pollPosition();
    }
    */
}

// ============================================================================
// COMMANDES PUBLIQUES
// ============================================================================

bool RS485_Remote::requestPosition() {
    if (sendRequest(CMD_GET_POSITION, nullptr, 0)) {
        RS485_Frame response;
        if (waitResponse(&response, RS485_TIMEOUT_MS)) {
            processResponse(&response);
            return true;
        }
    }
    return false;
}

bool RS485_Remote::requestStatus() {
    if (sendRequest(CMD_GET_STATUS, nullptr, 0)) {
        RS485_Frame response;
        if (waitResponse(&response, RS485_TIMEOUT_MS)) {
            processResponse(&response);
            return true;
        }
    }
    return false;
}

bool RS485_Remote::sendMoveTo(float azimuth, float elevation) {
    float targets[2] = {azimuth, elevation};

    if (sendRequest(CMD_MOVE_TO, (uint8_t*)targets, sizeof(targets))) {
        RS485_Frame response;
        if (waitResponse(&response, RS485_TIMEOUT_MS)) {
            processResponse(&response);
            return (response.command == RSP_ACK);
        }
    }
    return false;
}

bool RS485_Remote::sendStop() {
    if (sendRequest(CMD_STOP, nullptr, 0)) {
        RS485_Frame response;
        if (waitResponse(&response, RS485_TIMEOUT_MS)) {
            processResponse(&response);
            return (response.command == RSP_ACK);
        }
    }
    return false;
}

bool RS485_Remote::sendPark() {
    if (sendRequest(CMD_PARK, nullptr, 0)) {
        RS485_Frame response;
        if (waitResponse(&response, RS485_TIMEOUT_MS)) {
            processResponse(&response);
            return (response.command == RSP_ACK);
        }
    }
    return false;
}

// ============================================================================
// ACCESSEURS
// ============================================================================

bool RS485_Remote::isConnected() {
    // Connexion OK si on a reçu une réponse récemment
    uint32_t timeSinceLastResponse = millis() - remoteRotatorState.timestamp;
    return (timeSinceLastResponse < RS485_COMM_LOSS_TIMEOUT);
}

float RS485_Remote::getSuccessRate() {
    if (requestCount == 0) return 0.0;
    return (responseCount * 100.0) / requestCount;
}

// ============================================================================
// FONCTIONS GLOBALES
// ============================================================================

void rs485_remote_init() {
    if (!remoteInstance) {
        remoteInstance = new RS485_Remote();
    }
    remoteInstance->begin();
}

void rs485_remote_process() {
    if (remoteInstance) {
        remoteInstance->process();
    }
}

bool rs485_remote_request_position() {
    if (remoteInstance) {
        return remoteInstance->requestPosition();
    }
    return false;
}

bool rs485_remote_move_to(float azimuth, float elevation) {
    if (remoteInstance) {
        return remoteInstance->sendMoveTo(azimuth, elevation);
    }
    return false;
}

bool rs485_remote_stop() {
    if (remoteInstance) {
        return remoteInstance->sendStop();
    }
    return false;
}

bool rs485_remote_park() {
    if (remoteInstance) {
        return remoteInstance->sendPark();
    }
    return false;
}

RotatorState* rs485_remote_get_state() {
    if (remoteInstance) {
        return remoteInstance->getState();
    }
    return nullptr;
}
