/*
 * rs485_config.h
 *
 * Configuration RS485 pour K3NG Rotator Controller
 * Arduino Nano R4 Minima
 *
 * Communication RS485 entre unité ANTENNES (Master) et unité MAISON (Remote)
 * Distance max: 100m sur paire torsadée blindée
 */

#ifndef RS485_CONFIG_H
#define RS485_CONFIG_H

#include <Arduino.h>

// ============================================================================
// CONFIGURATION MATÉRIELLE
// ============================================================================

// Port série utilisé pour RS485
#define RS485_SERIAL        Serial1

// Pins de contrôle RS485 (Driver Enable / Receiver Enable)
#define RS485_DE_PIN        8       // Driver Enable (HIGH = transmission)
#define RS485_RE_PIN        9       // Receiver Enable (LOW = réception)

// Paramètres série
#define RS485_BAUD_RATE     9600    // 9600 bauds - fiable sur 100m (25% utilisation pour broadcast 100ms)
#define RS485_DATA_BITS     SERIAL_8N1

// ============================================================================
// ADRESSAGE
// ============================================================================

#define MASTER_ADDRESS      0x01    // Unité ANTENNES
#define REMOTE_ADDRESS      0x02    // Unité MAISON
#define BROADCAST_ADDRESS   0xFF    // Broadcast à tous

// ============================================================================
// TEMPORISATION
// ============================================================================

#define RS485_TIMEOUT_MS        100     // Timeout attente réponse
#define RS485_RETRY_COUNT       3       // Nombre de tentatives
#define RS485_UPDATE_RATE       100     // Mise à jour normale (ms)
#define RS485_TRACKING_RATE     20      // Mise à jour tracking rapide (ms)
#define RS485_CHAR_DELAY_US     50      // Délai entre chars à 19200 bauds

// ============================================================================
// BUFFER ET TAILLE DE TRAME
// ============================================================================

#define RS485_MAX_DATA_SIZE     64      // Taille max données utiles
#define RS485_BUFFER_SIZE       32      // Buffer circulaire (messages)
#define RS485_FRAME_OVERHEAD    7       // start(1) + addr(1) + cmd(1) + len(1) + crc(2) + end(1)
#define RS485_MAX_FRAME_SIZE    (RS485_MAX_DATA_SIZE + RS485_FRAME_OVERHEAD)

// ============================================================================
// MARQUEURS DE TRAME
// ============================================================================

#define RS485_START_BYTE        0xAA    // Début de trame
#define RS485_END_BYTE          0x55    // Fin de trame

// ============================================================================
// GESTION D'ERREURS
// ============================================================================

#define RS485_MAX_CRC_ERRORS        10      // Erreurs CRC avant safe mode
#define RS485_COMM_LOSS_TIMEOUT     5000    // Perte comm (ms) avant safe mode
#define RS485_RECONNECT_DELAY       1000    // Délai entre tentatives reconnexion

// ============================================================================
// MODES DE FONCTIONNEMENT
// ============================================================================

// Décommenter selon le rôle de la carte
// #define RS485_MODE_MASTER       // Unité ANTENNES
// #define RS485_MODE_REMOTE       // Unité MAISON

// ============================================================================
// DEBUG
// ============================================================================

// Activer pour debug sur Serial (USB)
#define RS485_DEBUG_ENABLED     // Décommenter pour activer debug

#ifdef RS485_DEBUG_ENABLED
    #define RS485_DEBUG(x)      Serial.print(x)
    #define RS485_DEBUGLN(x)    Serial.println(x)
    #define RS485_DEBUG_HEX(x)  Serial.print(x, HEX)
#else
    #define RS485_DEBUG(x)
    #define RS485_DEBUGLN(x)
    #define RS485_DEBUG_HEX(x)
#endif

// ============================================================================
// MACROS UTILITAIRES
// ============================================================================

// Activation transmission RS485
#define RS485_TX_MODE() { digitalWrite(RS485_DE_PIN, HIGH); digitalWrite(RS485_RE_PIN, HIGH); }

// Activation réception RS485
#define RS485_RX_MODE() { digitalWrite(RS485_DE_PIN, LOW); digitalWrite(RS485_RE_PIN, LOW); }

// ============================================================================
// VALIDATION CONFIGURATION
// ============================================================================

#if RS485_MAX_DATA_SIZE > 255
    #error "RS485_MAX_DATA_SIZE doit être <= 255"
#endif

#if RS485_BAUD_RATE > 115200
    #warning "RS485_BAUD_RATE > 115200 peut être instable sur 100m"
#endif

#endif // RS485_CONFIG_H
