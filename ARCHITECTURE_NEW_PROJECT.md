# Architecture Nouveau Projet - Nano R4 Rotator System

## 🎯 Vision

Système distribué **propre**, **modulaire** et **évolutif** pour contrôle de rotateur d'antenne avec deux Arduino Nano R4 communiquant via RS485.

---

## 📁 Structure de projet proposée

```
nano_r4_rotator/
│
├── platformio.ini              # Configuration PlatformIO (2 environnements)
├── README.md                   # Documentation utilisateur
├── CHANGELOG.md                # Historique des versions
│
├── lib/                        # Bibliothèques réutilisables
│   ├── RS485Protocol/          # Communication RS485
│   │   ├── RS485Protocol.h
│   │   └── RS485Protocol.cpp
│   │
│   ├── MCP23017Helper/         # Extension I/O via MCP23017
│   │   ├── MCP23017Helper.h
│   │   └── MCP23017Helper.cpp
│   │
│   ├── MotorController/        # Contrôle moteurs (relais + steppers)
│   │   ├── MotorController.h
│   │   └── MotorController.cpp
│   │
│   ├── PositionSensor/         # Lecture capteurs position
│   │   ├── PositionSensor.h
│   │   └── PositionSensor.cpp
│   │
│   ├── YaesuProtocol/          # Parser commandes Yaesu GS-232
│   │   ├── YaesuProtocol.h
│   │   └── YaesuProtocol.cpp
│   │
│   └── ConfigManager/          # Gestion configuration EEPROM
│       ├── ConfigManager.h
│       └── ConfigManager.cpp
│
├── shack_unit/                 # Firmware SHACK (Master)
│   ├── src/
│   │   ├── main.cpp            # Point d'entrée
│   │   ├── DisplayManager.cpp  # Gestion display LCD
│   │   ├── UserInput.cpp       # Boutons, Serial, Ethernet
│   │   └── TrackingEngine.cpp  # Calculs tracking (optionnel)
│   │
│   └── include/
│       ├── config.h            # Configuration utilisateur
│       ├── pins.h              # Définition pins SHACK
│       └── version.h           # Numéro version
│
└── antenna_unit/               # Firmware ANTENNA (Slave)
    ├── src/
    │   ├── main.cpp            # Point d'entrée
    │   ├── MotorDriver.cpp     # Driver moteurs
    │   ├── SensorReader.cpp    # Lecture capteurs
    │   └── GPSParser.cpp       # Parser GPS NMEA (optionnel)
    │
    └── include/
        ├── config.h            # Configuration utilisateur
        ├── pins.h              # Définition pins ANTENNA
        └── version.h           # Numéro version
```

---

## 🧩 Architecture modulaire

### Principe: Une classe = Une responsabilité

#### 1️⃣ RS485Protocol (Bibliothèque partagée)

**Responsabilité**: Communication fiable entre SHACK et ANTENNA

**API publique**:
```cpp
class RS485Protocol {
public:
    // Initialisation
    void begin(Stream& serial, uint8_t dePin, uint8_t rePin, uint32_t baud = 9600);

    // Envoi
    bool sendCommand(uint8_t cmd, uint16_t param1 = 0, uint16_t param2 = 0);
    bool sendResponse(uint8_t cmd, uint16_t param1 = 0, uint16_t param2 = 0);

    // Réception
    bool available();
    bool receivePacket(uint8_t& cmd, uint16_t& param1, uint16_t& param2);

    // Status
    uint32_t getErrorCount();
    uint32_t getPacketCount();
};
```

**Format paquet** (8 octets):
```
[START][CMD][PARAM1_H][PARAM1_L][PARAM2_H][PARAM2_L][CRC][END]
```

**Commandes définies**:
```cpp
// Master → Slave
#define CMD_PING           0x01
#define CMD_ROTATE_CW      0x10
#define CMD_ROTATE_CCW     0x11
#define CMD_ROTATE_UP      0x12
#define CMD_ROTATE_DOWN    0x13
#define CMD_STOP_AZ        0x14
#define CMD_STOP_EL        0x15
#define CMD_STOP_ALL       0x16
#define CMD_GET_POSITION   0x20
#define CMD_SET_SPEED      0x30

// Slave → Master
#define CMD_PONG           0x81
#define CMD_POSITION       0x90  // Param1=AZ, Param2=EL
#define CMD_STATUS         0x91
#define CMD_ERROR          0xFF
```

---

#### 2️⃣ MCP23017Helper (Bibliothèque partagée)

**Responsabilité**: Gestion simple du MCP23017 (16 pins I/O supplémentaires)

**API publique**:
```cpp
class MCP23017Helper {
public:
    // Initialisation
    bool begin(uint8_t i2cAddress = 0x20);

    // Configuration
    void pinMode(uint8_t pin, uint8_t mode);  // pin 0-15, mode INPUT/OUTPUT
    void pullUp(uint8_t pin, bool enable);

    // Lecture/Écriture
    bool digitalRead(uint8_t pin);
    void digitalWrite(uint8_t pin, bool value);

    // Lecture/Écriture multiple (efficace)
    uint16_t readAll();           // Lit les 16 pins d'un coup
    void writeAll(uint16_t value); // Écrit les 16 pins d'un coup

    // Interrupts (avancé)
    void enableInterrupt(uint8_t pin);
    bool interruptTriggered();
};
```

**Utilisation SHACK**:
- Pins 0-7 (Port A): Boutons (INPUT + PULLUP)
- Pins 8-15 (Port B): LEDs status (OUTPUT)

**Utilisation ANTENNA**:
- Pins 0-3: Fins de course (INPUT + PULLUP)
- Pins 4-7: LEDs status moteur (OUTPUT)
- Pins 8-11: Relais auxiliaires (OUTPUT)
- Pins 12-15: Réservé futur

---

#### 3️⃣ MotorController (Bibliothèque ANTENNA)

**Responsabilité**: Contrôle abstrait des moteurs (relais OU steppers)

**API publique**:
```cpp
class MotorController {
public:
    enum MotorType { RELAY, STEPPER_PWM };

    // Initialisation
    void begin(MotorType type);
    void configurePins(uint8_t cwPin, uint8_t ccwPin, uint8_t upPin, uint8_t downPin);

    // Contrôle
    void rotateCW(uint8_t speed = 255);   // speed 0-255
    void rotateCCW(uint8_t speed = 255);
    void rotateUP(uint8_t speed = 255);
    void rotateDOWN(uint8_t speed = 255);
    void stopAZ();
    void stopEL();
    void stopAll();

    // Status
    bool isRotating();
    uint8_t getSpeed();
};
```

**Implémentation**:
- Mode RELAY: digitalWrite HIGH/LOW sur pins
- Mode STEPPER: génère PWM sur pins via analogWrite()

---

#### 4️⃣ PositionSensor (Bibliothèque ANTENNA)

**Responsabilité**: Lecture position azimut/élévation (abstraction capteur)

**API publique**:
```cpp
class PositionSensor {
public:
    enum SensorType { POTENTIOMETER, ENCODER_INCREMENTAL, ENCODER_ABSOLUTE_SPI };

    // Initialisation
    void begin(SensorType type);
    void configureAZ(uint8_t pin1, uint8_t pin2 = 0);
    void configureEL(uint8_t pin1, uint8_t pin2 = 0);
    void setCalibration(float azMin, float azMax, float elMin, float elMax);

    // Lecture
    float readAzimuth();      // Retourne degrés (0-360)
    float readElevation();    // Retourne degrés (0-180)

    // Calibration
    void calibrateAzimuth(float actualDegrees);
    void calibrateElevation(float actualDegrees);
};
```

**Implémentation**:
- POTENTIOMETER: analogRead() → conversion linéaire
- ENCODER_INCREMENTAL: comptage pulses + interrupts
- ENCODER_ABSOLUTE_SPI: lecture HH12 AS5045

---

#### 5️⃣ YaesuProtocol (Bibliothèque SHACK)

**Responsabilité**: Parser commandes Yaesu GS-232

**API publique**:
```cpp
class YaesuProtocol {
public:
    struct Command {
        char type;           // 'C', 'M', 'W', etc.
        uint16_t azimuth;
        uint16_t elevation;
        bool valid;
    };

    // Initialisation
    void begin(Stream& serial);

    // Parsing
    bool available();
    Command parseCommand();

    // Réponses
    void sendPosition(uint16_t az, uint16_t el);
    void sendAck();
    void sendError();
};
```

**Commandes supportées**:
```
C    - Get position
M360 - Rotate to azimuth 360°
W180 045 - Rotate to AZ=180° EL=45°
S    - Stop
```

---

## 🔄 Flow de données

### Scénario 1: Rotation manuelle (bouton CW)

```
[SHACK]                          [ANTENNA]
   │                                  │
   │ 1. Bouton CW appuyé             │
   │    (MCP23017 pin 0)              │
   │                                  │
   │ 2. RS485: CMD_ROTATE_CW ──────> │
   │                                  │
   │                                  │ 3. Active relais CW
   │                                  │    (ou PWM stepper)
   │                                  │
   │ <────── CMD_STATUS              │ 4. Envoie status
   │                                  │
   │                                  │ 5. Lit position
   │ <────── CMD_POSITION            │    (potentiomètre)
   │    (AZ=123°, EL=45°)             │
   │                                  │
   │ 6. Affiche sur LCD               │
```

### Scénario 2: Commande Yaesu depuis PC

```
[PC]          [SHACK]                    [ANTENNA]
 │               │                            │
 │ M360 ──────> │ 1. Parse Yaesu             │
 │               │                            │
 │               │ 2. RS485: CMD_ROTATE ───> │
 │               │    (target=360°)           │
 │               │                            │
 │               │                            │ 3. Rotation vers 360°
 │               │                            │
 │               │ <────── CMD_POSITION      │ 4. Feedback position
 │               │                            │
 │               │ 5. Calcul si on arrive    │
 │               │                            │
 │               │ 6. RS485: CMD_STOP ─────> │
 │               │                            │
 │ <──── +0360  │ 7. Envoie position PC      │
```

---

## ⚙️ Configuration utilisateur

### SHACK config.h
```cpp
// Hardware
#define USE_MCP23017_SHACK    true
#define USE_LCD_I2C           true
#define USE_ETHERNET          false  // Désactivé Phase 1

// RS485
#define RS485_SERIAL          Serial1
#define RS485_DE_PIN          14
#define RS485_RE_PIN          14
#define RS485_BAUD            9600

// MCP23017 SHACK
#define MCP_ADDRESS_SHACK     0x20
#define MCP_BTN_CW            0    // Pin 0 = Bouton CW
#define MCP_BTN_CCW           1
#define MCP_BTN_UP            2
#define MCP_BTN_DOWN          3
#define MCP_BTN_STOP          4
#define MCP_LED_ROTATING      8    // Pin 8 = LED rotation
#define MCP_LED_ERROR         9

// Display
#define LCD_I2C_ADDRESS       0x27
#define LCD_COLS              20
#define LCD_ROWS              4
```

### ANTENNA config.h
```cpp
// Hardware
#define USE_MCP23017_ANTENNA  true
#define MOTOR_TYPE            RELAY  // ou STEPPER_PWM
#define SENSOR_TYPE           POTENTIOMETER

// RS485
#define RS485_SERIAL          Serial1
#define RS485_DE_PIN          14
#define RS485_RE_PIN          14
#define RS485_BAUD            9600

// Moteur pins
#define MOTOR_CW_PIN          2
#define MOTOR_CCW_PIN         3
#define MOTOR_UP_PIN          4
#define MOTOR_DOWN_PIN        5

// Capteur pins
#define SENSOR_AZ_PIN         A0
#define SENSOR_EL_PIN         A1

// Calibration
#define AZ_MIN_VOLTAGE        0.0
#define AZ_MAX_VOLTAGE        5.0
#define AZ_MIN_DEGREES        0.0
#define AZ_MAX_DEGREES        360.0

// MCP23017 ANTENNA
#define MCP_ADDRESS_ANTENNA   0x21
#define MCP_LIMIT_CW          0    // Fin de course CW
#define MCP_LIMIT_CCW         1
#define MCP_LIMIT_UP          2
#define MCP_LIMIT_DOWN        3
```

---

## 📦 Phases de développement

### Phase 1: Communication de base ✅ (Demain avec ton matériel)
- RS485 ping/pong
- MCP23017 lecture boutons
- Moteur relais ON/OFF
- Position potentiomètre
- Display LCD basique

**Résultat**: Tu peux tourner le rotateur manuellement !

### Phase 2: Contrôle intelligent
- Rotation vers cible (PID simple)
- Limites soft
- Timeout sécurité

### Phase 3: Interface PC
- Yaesu GS-232
- USB Serial

### Phase 4: Features avancées
- GPS
- Tracking
- Ethernet
- Steppers PWM

---

## 🛠️ Outils de développement

### Debug SHACK
```cpp
// Active mode debug dans config.h
#define DEBUG_SERIAL          true
#define DEBUG_BAUD            115200

// Dans le code
#ifdef DEBUG_SERIAL
  Serial.println("Button CW pressed");
  Serial.print("Position AZ: "); Serial.println(az);
#endif
```

### Debug ANTENNA
```cpp
// LED intégrée pour debug sans Serial
void blinkError(int count) {
    for(int i=0; i<count; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(200);
        digitalWrite(LED_BUILTIN, LOW);
        delay(200);
    }
}
```

---

## 📊 Estimation mémoire Nano R4

**RAM disponible**: 32 KB
**Flash disponible**: 256 KB

**Estimation Phase 1**:
- Code: ~30 KB Flash
- Variables globales: ~2 KB RAM
- Stack: ~2 KB RAM
- Buffers RS485: ~1 KB RAM

**Marge confortable**: 90% libre !

---

## ✅ Avantages de cette architecture

1. **Modulaire**: Chaque bibliothèque testable séparément
2. **Évolutif**: Ajouter features sans tout casser
3. **Lisible**: Code simple, commenté
4. **Flexible**: Changement config sans recompiler
5. **Robuste**: Gestion erreurs à chaque niveau
6. **Performant**: Optimisé ARM dès le départ

---

**Prochaine étape**: Créer le code réel après validation de tes choix ! 🚀
