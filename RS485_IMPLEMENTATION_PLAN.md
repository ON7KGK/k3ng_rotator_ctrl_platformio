# Plan d'Implémentation RS485 pour K3NG Rotator

## Architecture Globale

1. Topologie Maître-Esclave
[NANO R4 - ANTENNES (Maître)]          [NANO R4 - MAISON (Remote)]
├── Capteurs/Actionneurs                ├── Interface utilisateur
│   ├── Potentiomètres AZ/EL           │   ├── Écran Nextion/LCD I2C
│   ├── Encodeurs SSI HH-12            │   ├── Boutons preset
│   ├── GPS (Serial2)                  │   └── Encodeurs rotatifs
│   ├── Fins de course                 │
│   └── Contrôle moteurs               │
│                                       │
└── Module RS485 (Serial1) <---100m---> └── Module RS485 (Serial1)
2. Organisation du Code
Je créerais une structure modulaire avec ces nouveaux fichiers :
include/
├── rs485_protocol.h       // Définitions protocole
├── rs485_master.h         // Fonctions maître
├── rs485_remote.h         // Fonctions remote
├── rs485_messages.h       // Types de messages
└── rs485_config.h         // Configuration RS485

src/
├── rs485_master.cpp       // Implémentation maître
├── rs485_remote.cpp       // Implémentation remote
└── rs485_common.cpp       // Fonctions communes
3. Protocole de Communication
Structure des Trames
cpp// Dans rs485_protocol.h
struct RS485_Frame {
    uint8_t  start_byte;     // 0xAA
    uint8_t  address;        // Adresse destinataire (0xFF = broadcast)
    uint8_t  command;        // Type de commande
    uint8_t  length;         // Longueur des données
    uint8_t  data[64];       // Données utiles
    uint16_t crc16;          // Checksum CRC16
    uint8_t  end_byte;       // 0x55
};

// Types de commandes
enum RS485_Commands {
    // Requêtes du Remote vers Master
    CMD_GET_POSITION     = 0x10,  // Demander position actuelle
    CMD_GET_STATUS       = 0x11,  // État complet système
    CMD_GET_GPS_DATA     = 0x12,  // Données GPS
    CMD_GET_SENSORS      = 0x13,  // État capteurs
    
    // Commandes du Remote vers Master  
    CMD_MOVE_TO          = 0x20,  // Aller à position
    CMD_STOP             = 0x21,  // Arrêt d'urgence
    CMD_PARK             = 0x22,  // Position parking
    CMD_SET_PRESET       = 0x23,  // Définir preset
    
    // Réponses du Master
    RSP_POSITION_DATA    = 0x30,  // Position actuelle
    RSP_STATUS_DATA      = 0x31,  // État système
    RSP_GPS_DATA         = 0x32,  // Données GPS
    RSP_SENSOR_DATA      = 0x33,  // État capteurs
    RSP_ACK              = 0x3E,  // Acquittement
    RSP_NAK              = 0x3F,  // Erreur
    
    // Messages broadcast du Master
    MSG_POSITION_UPDATE  = 0x40,  // Mise à jour position
    MSG_ALARM           = 0x41,  // Alarme (fin course, blocage)
    MSG_STATUS_CHANGE   = 0x42   // Changement d'état
};
4. Configuration RS485
cpp// Dans rs485_config.h
#define RS485_BAUD_RATE     19200  // Vitesse optimale pour 100m
#define RS485_DE_PIN        8       // Pin Driver Enable
#define RS485_RE_PIN        9       // Pin Receiver Enable (peut être commun)
#define RS485_SERIAL_PORT   Serial1 // Port série utilisé

#define MASTER_ADDRESS      0x01
#define REMOTE_ADDRESS      0x02
#define BROADCAST_ADDRESS   0xFF

#define RS485_TIMEOUT_MS    100     // Timeout réponse
#define RS485_RETRY_COUNT   3       // Nombre de tentatives
#define RS485_UPDATE_RATE   100     // Mise à jour toutes les 100ms
5. Gestion Intelligente de la Communication
Mode Polling + Push
cpp// Stratégie hybride pour optimiser la bande passante

class RS485_Manager {
private:
    // Polling périodique (toutes les 100ms)
    void periodicPolling() {
        static uint32_t lastPoll = 0;
        if (millis() - lastPoll > RS485_UPDATE_RATE) {
            requestPositionUpdate();
            lastPoll = millis();
        }
    }
    
    // Push immédiat sur événements critiques
    void onCriticalEvent(uint8_t eventType) {
        // Fin de course, blocage, alarme...
        sendImmediateNotification(eventType);
    }
    
    // Buffer circulaire pour messages non critiques
    CircularBuffer<RS485_Frame, 32> txBuffer;
    CircularBuffer<RS485_Frame, 32> rxBuffer;
};
6. Gestion des Collisions et Fiabilité
cpp// Implémentation d'un mécanisme de token virtuel
class RS485_TokenManager {
private:
    enum TokenState {
        TOKEN_MASTER,     // Master peut transmettre
        TOKEN_REMOTE,     // Remote peut transmettre
        TOKEN_TRANSITION  // Changement de token
    };
    
    TokenState currentToken = TOKEN_MASTER;
    uint32_t tokenTimeout = 50;  // 50ms par token
    
public:
    bool canTransmit() {
        return (isMaster && currentToken == TOKEN_MASTER) ||
               (!isMaster && currentToken == TOKEN_REMOTE);
    }
    
    void manageToken() {
        static uint32_t lastSwitch = 0;
        if (millis() - lastSwitch > tokenTimeout) {
            switchToken();
            lastSwitch = millis();
        }
    }
};
7. Intégration avec le Code Existant
cpp// Dans rotator_features.h
#define FEATURE_RS485_MASTER        // ou FEATURE_RS485_REMOTE
#define FEATURE_RS485_COMMUNICATION

// Modification de la boucle principale
void loop() {
    #ifdef FEATURE_RS485_MASTER
        rs485_master_process();
        // Code existant pour contrôle moteurs
        check_operation();
        read_azimuth();
        read_elevation();
    #endif
    
    #ifdef FEATURE_RS485_REMOTE
        rs485_remote_process();
        // Gestion interface utilisateur
        update_nextion_display();
        check_preset_encoders();
    #endif
}
8. Gestion des Données Partagées
cpp// Structure de données synchronisée
struct RotatorState {
    float azimuth_current;
    float elevation_current;
    float azimuth_target;
    float elevation_target;
    uint8_t status_flags;
    GPSData gps_data;
    uint32_t timestamp;
    
    // Méthode de sérialisation pour RS485
    void serialize(uint8_t* buffer) {
        memcpy(buffer, this, sizeof(RotatorState));
    }
    
    void deserialize(uint8_t* buffer) {
        memcpy(this, buffer, sizeof(RotatorState));
    }
};
9. Gestion des Erreurs et Reconnexion
cppclass RS485_ErrorHandler {
private:
    uint16_t errorCount = 0;
    uint16_t crcErrors = 0;
    uint32_t lastGoodPacket = 0;
    
public:
    void handleCommunicationLoss() {
        if (millis() - lastGoodPacket > 5000) {
            // Perte de communication > 5 secondes
            enterSafeMode();
            attemptReconnection();
        }
    }
    
    void enterSafeMode() {
        // Arrêt des moteurs si Master
        // Affichage erreur si Remote
    }
};
10. Optimisations Spécifiques
Compression des Données
cpp// Envoi optimisé des positions (2 bytes par float au lieu de 4)
uint16_t compressAngle(float angle) {
    return (uint16_t)(angle * 100);  // Précision 0.01°
}

float decompressAngle(uint16_t compressed) {
    return compressed / 100.0f;
}
Mode Burst pour Tracking
cpp// En mode tracking satellite, augmenter le débit
void enterTrackingMode() {
    rs485_update_rate = 20;  // 20ms au lieu de 100ms
    enable_predictive_buffering = true;
}

## Phases d'Implémentation

Phase 1 : Créer les fichiers de base RS485 et tester la communication simple
Phase 2 : Implémenter le protocole de trames avec CRC
Phase 3 : Ajouter la gestion maître/esclave avec polling
Phase 4 : Intégrer avec les fonctions existantes du rotateur
Phase 5 : Optimiser la latence et la fiabilité
Phase 6 : Ajouter la gestion d'erreurs et les modes dégradés



### PHASE 1 - Communication de Base ✅/⏳/❌
- [ ] Créer include/rs485_config.h avec configuration pins et paramètres
- [ ] Créer include/rs485_protocol.h avec définitions de base
- [ ] Implémenter test simple émission/réception Serial1
- [ ] Configurer pins DE/RE (pins 8 et 9)
- [ ] Test loopback local (TX vers RX)
- [ ] Valider communication bidirectionnelle entre 2 Nano R4
- [ ] Créer src/test_rs485_basic.cpp pour tests

### PHASE 2 - Protocole de Trames ✅/⏳/❌
- [ ] Implémenter structure RS485_Frame complète
- [ ] Ajouter calcul et vérification CRC16
- [ ] Créer fonctions buildFrame() et parseFrame()
- [ ] Implémenter enum RS485_Commands (tous les types)
- [ ] Créer fonctions serialize/deserialize pour les données
- [ ] Gérer détection start_byte (0xAA) et end_byte (0x55)
- [ ] Tests unitaires validation trames corrompues
- [ ] Test envoi/réception trames complètes
- [ ] Créer src/test_rs485_protocol.cpp

### PHASE 3 - Architecture Maître/Esclave ✅/⏳/❌
- [ ] Créer include/rs485_master.h et rs485_master.cpp
- [ ] Créer include/rs485_remote.h et rs485_remote.cpp
- [ ] Implémenter système de polling périodique (100ms)
- [ ] Créer machine d'états pour Master
- [ ] Créer machine d'états pour Remote
- [ ] Implémenter gestion du token virtuel
- [ ] Ajouter buffers circulaires TX/RX (32 messages)
- [ ] Gérer les timeouts de communication (100ms)
- [ ] Implémenter système de retry (3 tentatives)
- [ ] Créer RS485_Manager classe principale
- [ ] Tests avec 2 Nano R4 en configuration réelle
- [ ] Mesurer latence aller-retour

### PHASE 4 - Intégration Code K3NG ✅/⏳/❌
- [ ] Ajouter #define FEATURE_RS485_MASTER dans rotator_features.h
- [ ] Ajouter #define FEATURE_RS485_REMOTE dans rotator_features.h
- [ ] Créer structure RotatorState pour données partagées
- [ ] Modifier loop() pour intégrer rs485_process()
- [ ] Côté Master : capturer données capteurs
  - [ ] Intégrer lecture azimuth/elevation
  - [ ] Intégrer données GPS (Serial2)
  - [ ] Intégrer état fins de course
  - [ ] Intégrer état moteurs
- [ ] Côté Remote : interface utilisateur
  - [ ] Rediriger affichage Nextion
  - [ ] Gérer encodeurs preset locaux
  - [ ] Gérer boutons commande
  - [ ] Implémenter commandes STOP/PARK
- [ ] Gérer conflits Serial1 (Nextion vs RS485)
- [ ] Résoudre problèmes mémoire RAM
- [ ] Tests système complet avec moteurs

### PHASE 5 - Optimisations Performance ✅/⏳/❌
- [ ] Implémenter compression données angles (uint16_t)
- [ ] Optimiser structure RotatorState (alignement mémoire)
- [ ] Créer mode Burst pour tracking satellite (20ms)
- [ ] Implémenter cache côté Remote (éviter requêtes inutiles)
- [ ] Ajouter mode "quiet" hors tracking
- [ ] Optimiser utilisation bande passante
  - [ ] Envoyer deltas au lieu de positions absolues
  - [ ] Grouper messages similaires
- [ ] Implémenter priorité messages (urgent vs normal)
- [ ] Profiler utilisation CPU
- [ ] Mesurer latence maximale en tracking
- [ ] Tests performances sur câble 100m réel

### PHASE 6 - Fiabilité et Gestion Erreurs ✅/⏳/❌
- [ ] Implémenter RS485_ErrorHandler classe
- [ ] Ajouter compteurs erreurs (CRC, timeout, frame)
- [ ] Créer mode dégradé (safe mode)
  - [ ] Arrêt moteurs si perte communication
  - [ ] Affichage erreur sur Remote
  - [ ] LED status indication
- [ ] Implémenter auto-reconnexion après perte
- [ ] Ajouter détection déconnexion câble
- [ ] Gérer corruption données (double validation)
- [ ] Implémenter watchdog communication
- [ ] Ajouter logs erreurs en EEPROM
- [ ] Créer commande diagnostic RS485
- [ ] Tests robustesse :
  - [ ] Déconnexion/reconnexion câble
  - [ ] Interférences EMI simulées
  - [ ] Reset Master pendant communication
  - [ ] Reset Remote pendant mouvement
- [ ] Documentation protocole complet
- [ ] Guide dépannage utilisateur

## État Actuel
Phase en cours : 1
Dernière modification : [date]
Développeur : [votre nom]
Version protocole : 1.0

## Notes Techniques
- Baud rate : 19200 (optimal pour 100m)
- Timeout standard : 100ms
- Buffer size : 32 messages
- CRC : CRC16-CCITT
- Update rate normal : 100ms
- Update rate tracking : 20ms

## Hardware Requis
- 2x Arduino Nano R4 Minima
- 2x Module RS485 (MAX485/SN75176B)
- Câble : Paire torsadée blindée Cat5e/Cat6
- 2x Résistances 120Ω (terminaison)
- Alimentation stable (éviter chutes tension sur 100m)

## Tests de Validation Finaux
- [ ] Test 24h en tracking satellite
- [ ] Test résistance intempéries (boîtier étanche)
- [ ] Test avec charge réelle (antennes lourdes)
- [ ] Test interférences TX radio amateur
- [ ] Validation avec utilisateur final



## État Actuel
Phase en cours : 1
Dernière modification : [11 octobre 2025]
Développeur : [ON7KGK]
Version protocole : 1.0

## Notes Techniques
- Baud rate : 19200 (optimal pour 100m)
- Timeout standard : 100ms
- Buffer size : 32 messages
- CRC : CRC16-CCITT
- Update rate normal : 100ms
- Update rate tracking : 20ms

## Hardware Requis
- 2x Arduino Nano R4 Minima
- 2x Module RS485 (MAX485/SN75176B)
- Câble : Paire torsadée blindée Cat5e/Cat6
- 2x Résistances 120Ω (terminaison)
- Alimentation stable (éviter chutes tension sur 100m)

## Tests de Validation Finaux
- [ ] Test 24h en tracking satellite
- [ ] Test résistance intempéries (boîtier étanche)
- [ ] Test avec charge réelle (antennes lourdes)
- [ ] Test interférences TX radio amateur
- [ ] Validation avec utilisateur final