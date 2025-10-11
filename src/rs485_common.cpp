/*
 * rs485_common.cpp
 *
 * Implémentation des fonctions communes du protocole RS485
 * - Calcul CRC16-CCITT
 * - Construction/parsing de trames
 * - Envoi/réception via Serial1
 */

#include <Arduino.h>
#include "rs485_protocol.h"
#include "rs485_config.h"

// ============================================================================
// CALCUL CRC16-CCITT (Polynomial 0x1021)
// ============================================================================

uint16_t rs485_calculate_crc16(const uint8_t* data, uint16_t length) {
    uint16_t crc = 0xFFFF;  // Valeur initiale

    for (uint16_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;

        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;  // Polynomial CRC16-CCITT
            } else {
                crc = crc << 1;
            }
        }
    }

    return crc;
}

// ============================================================================
// VÉRIFICATION CRC16
// ============================================================================

bool rs485_verify_crc16(const uint8_t* data, uint16_t length, uint16_t crc) {
    uint16_t calculated_crc = rs485_calculate_crc16(data, length);
    return (calculated_crc == crc);
}

// ============================================================================
// CONSTRUCTION D'UNE TRAME
// ============================================================================

void rs485_build_frame(RS485_Frame* frame, uint8_t addr, uint8_t cmd,
                       const uint8_t* data, uint8_t len) {
    // Vérifications
    if (!frame || len > RS485_MAX_DATA_SIZE) {
        return;
    }

    // Remplir les champs de base
    frame->start_byte = RS485_START_BYTE;
    frame->address = addr;
    frame->command = cmd;
    frame->length = len;
    frame->end_byte = RS485_END_BYTE;

    // Copier les données
    if (data && len > 0) {
        memcpy(frame->data, data, len);
    }

    // Calculer le CRC sur: address + command + length + data
    uint8_t crc_buffer[RS485_MAX_DATA_SIZE + 3];
    crc_buffer[0] = frame->address;
    crc_buffer[1] = frame->command;
    crc_buffer[2] = frame->length;
    memcpy(&crc_buffer[3], frame->data, len);

    frame->crc16 = rs485_calculate_crc16(crc_buffer, len + 3);

    RS485_DEBUG("Frame built: addr=0x");
    RS485_DEBUG_HEX(addr);
    RS485_DEBUG(" cmd=0x");
    RS485_DEBUG_HEX(cmd);
    RS485_DEBUG(" len=");
    RS485_DEBUG(len);
    RS485_DEBUG(" crc=0x");
    RS485_DEBUG_HEX(frame->crc16 >> 8);
    RS485_DEBUG_HEX(frame->crc16 & 0xFF);
    RS485_DEBUGLN("");
}

// ============================================================================
// PARSING D'UNE TRAME
// ============================================================================

bool rs485_parse_frame(RS485_Frame* frame, const uint8_t* buffer, uint16_t len) {
    if (!frame || !buffer || len < RS485_FRAME_OVERHEAD) {
        RS485_DEBUGLN("Parse error: invalid parameters");
        return false;
    }

    // Vérifier start byte
    if (buffer[0] != RS485_START_BYTE) {
        RS485_DEBUGLN("Parse error: invalid start byte");
        return false;
    }

    // Extraire les champs
    frame->start_byte = buffer[0];
    frame->address = buffer[1];
    frame->command = buffer[2];
    frame->length = buffer[3];

    // Vérifier la longueur des données
    if (frame->length > RS485_MAX_DATA_SIZE) {
        RS485_DEBUGLN("Parse error: data length too large");
        return false;
    }

    // Vérifier la longueur totale du buffer
    uint16_t expected_len = RS485_FRAME_TOTAL_SIZE(frame->length);
    if (len < expected_len) {
        RS485_DEBUGLN("Parse error: buffer too short");
        return false;
    }

    // Copier les données
    memcpy(frame->data, &buffer[4], frame->length);

    // Extraire le CRC (big endian)
    uint16_t crc_offset = 4 + frame->length;
    frame->crc16 = ((uint16_t)buffer[crc_offset] << 8) | buffer[crc_offset + 1];

    // Extraire end byte
    frame->end_byte = buffer[crc_offset + 2];

    // Vérifier end byte
    if (frame->end_byte != RS485_END_BYTE) {
        RS485_DEBUGLN("Parse error: invalid end byte");
        return false;
    }

    // Vérifier le CRC
    uint8_t crc_buffer[RS485_MAX_DATA_SIZE + 3];
    crc_buffer[0] = frame->address;
    crc_buffer[1] = frame->command;
    crc_buffer[2] = frame->length;
    memcpy(&crc_buffer[3], frame->data, frame->length);

    if (!rs485_verify_crc16(crc_buffer, frame->length + 3, frame->crc16)) {
        RS485_DEBUGLN("Parse error: CRC mismatch");
        return false;
    }

    RS485_DEBUG("Frame parsed OK: addr=0x");
    RS485_DEBUG_HEX(frame->address);
    RS485_DEBUG(" cmd=0x");
    RS485_DEBUG_HEX(frame->command);
    RS485_DEBUG(" len=");
    RS485_DEBUGLN(frame->length);

    return true;
}

// ============================================================================
// ENVOI D'UNE TRAME
// ============================================================================

bool rs485_send_frame(const RS485_Frame* frame) {
    if (!frame) {
        return false;
    }

    // Calculer la taille totale de la trame
    uint16_t frame_size = RS485_FRAME_TOTAL_SIZE(frame->length);

    // Préparer le buffer d'envoi
    uint8_t tx_buffer[RS485_MAX_FRAME_SIZE];

    tx_buffer[0] = frame->start_byte;
    tx_buffer[1] = frame->address;
    tx_buffer[2] = frame->command;
    tx_buffer[3] = frame->length;
    memcpy(&tx_buffer[4], frame->data, frame->length);

    uint16_t crc_offset = 4 + frame->length;
    tx_buffer[crc_offset] = (frame->crc16 >> 8) & 0xFF;  // CRC high byte
    tx_buffer[crc_offset + 1] = frame->crc16 & 0xFF;      // CRC low byte
    tx_buffer[crc_offset + 2] = frame->end_byte;

    // Passer en mode transmission
    RS485_TX_MODE();
    delayMicroseconds(RS485_CHAR_DELAY_US);

    // Envoyer la trame
    size_t written = RS485_SERIAL.write(tx_buffer, frame_size);

    // Attendre que tout soit envoyé
    RS485_SERIAL.flush();
    delayMicroseconds(RS485_CHAR_DELAY_US);

    // Repasser en mode réception
    RS485_RX_MODE();

    RS485_DEBUG("Frame sent: ");
    RS485_DEBUG(written);
    RS485_DEBUG("/");
    RS485_DEBUG(frame_size);
    RS485_DEBUGLN(" bytes");

    return (written == frame_size);
}

// ============================================================================
// RÉCEPTION D'UNE TRAME
// ============================================================================

bool rs485_receive_frame(RS485_Frame* frame, uint32_t timeout_ms) {
    if (!frame) {
        return false;
    }

    uint8_t rx_buffer[RS485_MAX_FRAME_SIZE];
    uint16_t rx_index = 0;
    uint32_t start_time = millis();
    bool found_start = false;
    uint8_t expected_length = 0;

    // Mode réception (devrait déjà être actif)
    RS485_RX_MODE();

    while (millis() - start_time < timeout_ms) {
        if (RS485_SERIAL.available()) {
            uint8_t byte = RS485_SERIAL.read();

            // Chercher le start byte
            if (!found_start) {
                if (byte == RS485_START_BYTE) {
                    found_start = true;
                    rx_buffer[rx_index++] = byte;
                }
                continue;
            }

            // Ajouter le byte au buffer
            rx_buffer[rx_index++] = byte;

            // Protéger contre le dépassement de buffer
            if (rx_index >= RS485_MAX_FRAME_SIZE) {
                RS485_DEBUGLN("RX error: buffer overflow");
                return false;
            }

            // Une fois qu'on a address + command + length (indices 1,2,3)
            if (rx_index == 4) {
                expected_length = rx_buffer[3];
                if (expected_length > RS485_MAX_DATA_SIZE) {
                    RS485_DEBUGLN("RX error: invalid data length");
                    return false;
                }
            }

            // Vérifier si on a reçu la trame complète
            if (rx_index >= 4) {
                uint16_t expected_total = RS485_FRAME_TOTAL_SIZE(expected_length);
                if (rx_index >= expected_total) {
                    // Trame complète reçue, parser
                    return rs485_parse_frame(frame, rx_buffer, rx_index);
                }
            }
        }

        // Petit délai pour ne pas surcharger le CPU
        delayMicroseconds(100);
    }

    // Timeout
    if (found_start) {
        RS485_DEBUGLN("RX timeout (partial frame)");
    }

    return false;
}
