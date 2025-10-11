/*
 * rs485_protocol.h
 *
 * Protocole de communication RS485 pour K3NG Rotator Controller
 * Définitions des structures de trames et commandes
 *
 * Architecture: Master (ANTENNES) <-> Remote (MAISON)
 */

#ifndef RS485_PROTOCOL_H
#define RS485_PROTOCOL_H

#include <Arduino.h>
#include "rs485_config.h"

// ============================================================================
// ÉNUMÉRATIONS DES COMMANDES
// ============================================================================

enum RS485_Commands : uint8_t {
    // ========================================
    // REQUÊTES Remote -> Master (0x10-0x1F)
    // ========================================
    CMD_GET_POSITION    = 0x10,     // Demander position actuelle AZ/EL
    CMD_GET_STATUS      = 0x11,     // État complet du système
    CMD_GET_GPS_DATA    = 0x12,     // Données GPS
    CMD_GET_SENSORS     = 0x13,     // État de tous les capteurs

    // ========================================
    // COMMANDES Remote -> Master (0x20-0x2F)
    // ========================================
    CMD_MOVE_TO         = 0x20,     // Aller à position AZ/EL
    CMD_STOP            = 0x21,     // Arrêt d'urgence
    CMD_PARK            = 0x22,     // Position parking
    CMD_SET_PRESET      = 0x23,     // Définir un preset
    CMD_CALIBRATE       = 0x24,     // Démarrer calibration
    CMD_SET_PARK_POS    = 0x25,     // Définir position parking

    // ========================================
    // RÉPONSES Master -> Remote (0x30-0x3F)
    // ========================================
    RSP_POSITION_DATA   = 0x30,     // Données position actuelle
    RSP_STATUS_DATA     = 0x31,     // Données état système
    RSP_GPS_DATA        = 0x32,     // Données GPS
    RSP_SENSOR_DATA     = 0x33,     // Données capteurs
    RSP_ACK             = 0x3E,     // Acquittement OK
    RSP_NAK             = 0x3F,     // Erreur/Refus

    // ========================================
    // MESSAGES BROADCAST Master -> Remote (0x40-0x4F)
    // ========================================
    MSG_POSITION_UPDATE = 0x40,     // Mise à jour position (push)
    MSG_ALARM           = 0x41,     // Alarme (fin course, blocage)
    MSG_STATUS_CHANGE   = 0x42,     // Changement d'état système
    MSG_HEARTBEAT       = 0x43      // Heartbeat (keepalive)
};

// ============================================================================
// CODES D'ERREUR
// ============================================================================

enum RS485_ErrorCode : uint8_t {
    ERR_NONE            = 0x00,     // Pas d'erreur
    ERR_INVALID_CMD     = 0x01,     // Commande invalide
    ERR_INVALID_DATA    = 0x02,     // Données invalides
    ERR_OUT_OF_RANGE    = 0x03,     // Valeur hors limites
    ERR_BUSY            = 0x04,     // Système occupé
    ERR_TIMEOUT         = 0x05,     // Timeout
    ERR_CRC_FAILED      = 0x06,     // Erreur CRC
    ERR_BUFFER_FULL     = 0x07,     // Buffer plein
    ERR_COMM_LOST       = 0x08,     // Communication perdue
    ERR_HARDWARE        = 0x09      // Erreur matérielle
};

// ============================================================================
// FLAGS D'ÉTAT SYSTÈME
// ============================================================================

// Flags pour status_flags dans RotatorState
#define STATUS_MOVING_AZ        0x01    // Azimut en mouvement
#define STATUS_MOVING_EL        0x02    // Élévation en mouvement
#define STATUS_LIMIT_AZ_CCW     0x04    // Fin course AZ sens CCW
#define STATUS_LIMIT_AZ_CW      0x08    // Fin course AZ sens CW
#define STATUS_LIMIT_EL_DOWN    0x10    // Fin course EL bas
#define STATUS_LIMIT_EL_UP      0x20    // Fin course EL haut
#define STATUS_GPS_VALID        0x40    // GPS lock valide
#define STATUS_ERROR            0x80    // Erreur générale

// ============================================================================
// STRUCTURE DE TRAME RS485
// ============================================================================

/*
 * Format trame:
 * [START] [ADDR] [CMD] [LEN] [DATA...] [CRC_H] [CRC_L] [END]
 *
 * START: 0xAA
 * ADDR: Adresse destinataire (0x01=Master, 0x02=Remote, 0xFF=Broadcast)
 * CMD: Code commande (voir enum RS485_Commands)
 * LEN: Longueur données utiles (0-64)
 * DATA: Données utiles
 * CRC: CRC16-CCITT (polynomial 0x1021)
 * END: 0x55
 */

struct RS485_Frame {
    uint8_t  start_byte;                // 0xAA
    uint8_t  address;                   // Adresse destinataire
    uint8_t  command;                   // Code commande
    uint8_t  length;                    // Longueur données (0-64)
    uint8_t  data[RS485_MAX_DATA_SIZE]; // Données utiles
    uint16_t crc16;                     // CRC16-CCITT
    uint8_t  end_byte;                  // 0x55

    // Constructeur par défaut
    RS485_Frame() {
        start_byte = RS485_START_BYTE;
        end_byte = RS485_END_BYTE;
        address = 0;
        command = 0;
        length = 0;
        crc16 = 0;
        memset(data, 0, RS485_MAX_DATA_SIZE);
    }
};

// ============================================================================
// STRUCTURE ÉTAT ROTATEUR (pour échange données)
// ============================================================================

struct RotatorState {
    float    azimuth_current;       // Azimut actuel (degrés)
    float    elevation_current;     // Élévation actuelle (degrés)
    float    azimuth_target;        // Azimut cible (degrés)
    float    elevation_target;      // Élévation cible (degrés)
    uint8_t  status_flags;          // Flags d'état (voir STATUS_*)
    uint8_t  error_code;            // Code erreur actuel
    uint32_t timestamp;             // Timestamp (millis())

    // Constructeur
    RotatorState() {
        azimuth_current = 0.0;
        elevation_current = 0.0;
        azimuth_target = 0.0;
        elevation_target = 0.0;
        status_flags = 0;
        error_code = ERR_NONE;
        timestamp = 0;
    }
};

// ============================================================================
// STRUCTURE DONNÉES GPS (optionnel)
// ============================================================================

struct GPSData {
    float    latitude;              // Latitude (degrés)
    float    longitude;             // Longitude (degrés)
    float    altitude;              // Altitude (mètres)
    uint8_t  satellites;            // Nombre satellites
    uint8_t  fix_quality;           // Qualité fix GPS (0=invalid, 1=GPS, 2=DGPS)
    uint32_t timestamp;             // Timestamp GPS

    // Constructeur
    GPSData() {
        latitude = 0.0;
        longitude = 0.0;
        altitude = 0.0;
        satellites = 0;
        fix_quality = 0;
        timestamp = 0;
    }
};

// ============================================================================
// PROTOTYPES FONCTIONS DE BASE (implémentation future)
// ============================================================================

// Calcul CRC16-CCITT
uint16_t rs485_calculate_crc16(const uint8_t* data, uint16_t length);

// Vérification CRC
bool rs485_verify_crc16(const uint8_t* data, uint16_t length, uint16_t crc);

// Construction d'une trame
void rs485_build_frame(RS485_Frame* frame, uint8_t addr, uint8_t cmd,
                       const uint8_t* data, uint8_t len);

// Parsing d'une trame
bool rs485_parse_frame(RS485_Frame* frame, const uint8_t* buffer, uint16_t len);

// Envoi d'une trame
bool rs485_send_frame(const RS485_Frame* frame);

// Réception d'une trame
bool rs485_receive_frame(RS485_Frame* frame, uint32_t timeout_ms);

// ============================================================================
// MACROS UTILITAIRES
// ============================================================================

// Taille totale d'une trame (avec overhead)
#define RS485_FRAME_TOTAL_SIZE(data_len) ((data_len) + RS485_FRAME_OVERHEAD)

// Vérification validité adresse
#define RS485_IS_VALID_ADDRESS(addr) \
    ((addr) == MASTER_ADDRESS || (addr) == REMOTE_ADDRESS || (addr) == BROADCAST_ADDRESS)

// Vérification commande valide
#define RS485_IS_VALID_COMMAND(cmd) ((cmd) >= 0x10 && (cmd) <= 0x4F)

#endif // RS485_PROTOCOL_H
