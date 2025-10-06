# K3NG Rotator Controller - Arduino Nano R4 Duo System (RS485)

## Architecture distribuée avec communication RS485

Cette branche `NanoR4_Duo_Test1` implémente une architecture distribuée pour le contrôleur de rotateur K3NG utilisant **deux Arduino Nano R4** communicant via **RS485**.

---

## Vue d'ensemble

### Pourquoi deux unités ?

Les installations radioamateurs ont souvent besoin de séparer l'interface utilisateur (dans le shack) et le contrôle moteur/capteurs (aux antennes) qui peuvent être distants de 100m ou plus. La communication RS485 permet cette distance tout en restant fiable.

### Architecture du système

```
┌─────────────────────────────────┐         RS485          ┌──────────────────────────────────┐
│   NANO R4 SHACK (Master)        │◄─────(100m max)──────►│  NANO R4 ANTENNA (Slave)         │
│                                 │                        │                                  │
│  • Display (4bit/I2C/Nextion*)  │                        │  • Contrôle moteurs (relais/PWM) │
│  • Boutons (CW/CCW/UP/DOWN/STOP)│                        │  • Moteurs pas à pas (TB6600)   │
│  • LEDs status                  │                        │  • Capteurs position (SPI/I2C)  │
│  • Encodeur rotatif (opt MCP)   │                        │  • GPS (Serial2)                │
│  • Ethernet W5100/W5500         │                        │  • Fins de course (optionnel)   │
│  • Tracking Moon/Sun/Satellite  │                        │  • Frein azimut/élévation       │
│  • RS485 Master (Serial1)       │                        │  • RS485 Slave (Serial1)        │
└─────────────────────────────────┘                        └──────────────────────────────────┘
```

*Note: Nextion nécessite Serial1, donc incompatible avec RS485 sur Serial1. Utiliser I2C LCD ou implémenter SoftwareSerial.

---

## Configuration matérielle

### NANO R4 SHACK (Unité maison)

**Ports série:**
- **Serial (USB)**: Contrôle PC (Yaesu/Easycom emulation)
- **Serial1 (D0/D1)**: RS485 vers unité ANTENNA (9600 baud)

**Pins I2C (A4/A5):**
- Display I2C (LCD 20x4 recommandé)
- MCP23017 (optionnel, pour encodeurs rotatifs)

**Pins SPI (D10-D13):**
- Ethernet W5100/W5500 (CS sur D10)

**Pins digitales:**
- D2: `rotation_indication_pin` - LED indiquant rotation active
- D3: `blink_led` - LED status général
- D4: `serial_led` - LED activité série
- D5: `button_stop` - Bouton stop
- D6: `button_cw` - Bouton rotation CW manuelle
- D7: `button_ccw` - Bouton rotation CCW manuelle
- D8: `button_up` - Bouton élévation UP
- D9: `button_down` - Bouton élévation DOWN

**Pins analogiques:**
- A0: `az_speed_pot` - Potentiomètre vitesse azimut (optionnel)

**Fichiers de configuration:**
- [rotator_hardware.h](include/rotator_hardware.h): Décommenter `#define HARDWARE_NANO_R4_SHACK`
- [rotator_features_nano_r4_shack.h](include/rotator_features_nano_r4_shack.h)
- [rotator_pins_nano_r4_shack.h](include/rotator_pins_nano_r4_shack.h)
- [rotator_settings_nano_r4_shack.h](include/rotator_settings_nano_r4_shack.h)

---

### NANO R4 ANTENNA (Unité antennes)

**Ports série:**
- **Serial (USB)**: Debug/monitoring (optionnel, peut être inaccessible)
- **Serial1 (D0/D1)**: RS485 esclave depuis SHACK (9600 baud)
- **Serial2 (A4/A5)**: GPS NMEA (9600 baud)

**Pins RS485:**
- D14 (A0): `rs485_tx_enable_pin` / `rs485_rx_enable_pin` - DE/RE du MAX485 (souvent liés ensemble)

**Contrôle moteurs - RELAIS:**
- D2: `rotate_cw` - Relais rotation CW
- D3: `rotate_ccw` - Relais rotation CCW
- D4: `brake_az` - Déblocage frein azimut
- D5: `rotate_up` - Relais rotation UP
- D6: `rotate_down` - Relais rotation DOWN

**Contrôle moteurs - STEPPERS (TB6600, DM542, etc.):**
- D7: `az_stepper_motor_enable` - Enable moteur azimut
- D8: `az_stepper_motor_direction` - Direction moteur azimut
- D9: `az_stepper_motor_pulse` - Pulse PWM moteur azimut (20kHz max)
- D11: `el_stepper_motor_pulse` - Pulse PWM moteur élévation
- D12: `el_stepper_motor_direction` - Direction moteur élévation
- D13: `el_stepper_motor_enable` - Enable moteur élévation

**Capteurs position SPI (HH12 AS5045):**
- D10: `az_hh12_cs_pin` - Chip Select azimut
- D11: `el_hh12_cs_pin` - Chip Select élévation
- D12: `MISO` - Données SPI
- D13: `SCK` - Clock SPI

**Alternative - Encodeurs rotatifs:**
- D2/D3: Encodeur azimut (pins interrupt)
- D4/D5: Encodeur élévation

**Alternative - Potentiomètres:**
- A0: `rotator_analog_az` - Position azimut
- A3: `rotator_analog_el` - Position élévation

**Fins de course (optionnel):**
- A2: `az_limit_sense_pin` - Fin de course azimut
- A6: `el_limit_sense_pin` - Fin de course élévation (Nano R4 only)

**Fichiers de configuration:**
- [rotator_hardware.h](include/rotator_hardware.h): Décommenter `#define HARDWARE_NANO_R4_ANTENNA`
- [rotator_features_nano_r4_antenna.h](include/rotator_features_nano_r4_antenna.h)
- [rotator_pins_nano_r4_antenna.h](include/rotator_pins_nano_r4_antenna.h)
- [rotator_settings_nano_r4_antenna.h](include/rotator_settings_nano_r4_antenna.h)

---

## Protocole de communication RS485

### Matériel RS485 requis

**Module MAX485 ou MAX3485:**
- VCC: 5V
- GND: GND
- DI (Data Input): TX du Nano R4 (D1)
- RO (Receiver Output): RX du Nano R4 (D0)
- DE (Driver Enable): Pin A0 (ou lié avec /RE)
- /RE (Receiver Enable): Pin A0 (ou LOW si toujours en écoute)
- A, B: Connectés aux terminaux A, B de l'autre MAX485 (câble torsadé)

**Câblage entre les deux unités:**
```
SHACK MAX485                    ANTENNA MAX485
    A ◄──────────────────────────► A
    B ◄──────────────────────────► B
   GND◄──────────────────────────►GND (référence commune recommandée)
```

**Résistances de terminaison:**
- 120Ω entre A et B à chaque extrémité (SHACK et ANTENNA)

### Protocole logiciel

Le système utilise le protocole maître/esclave intégré de K3NG:
- **FEATURE_MASTER_WITH_SERIAL_SLAVE** (SHACK)
- **FEATURE_REMOTE_UNIT_SLAVE** (ANTENNA)

**Commandes SHACK → ANTENNA:**
- Commandes de rotation (CW, CCW, UP, DOWN, STOP)
- Requêtes de position
- Demandes de status

**Réponses ANTENNA → SHACK:**
- Positions azimut/élévation actuelles
- Données GPS (coordonnées, heure)
- Status des fins de course
- Alarmes et erreurs

---

## Compilation avec PlatformIO

### Environnements disponibles

Deux environnements sont configurés dans [platformio.ini](platformio.ini):

#### 1. nano_r4_shack
```bash
pio run -e nano_r4_shack
pio run -e nano_r4_shack --target upload
```

#### 2. nano_r4_antenna
```bash
pio run -e nano_r4_antenna
pio run -e nano_r4_antenna --target upload
```

### Bibliothèques requises

**SHACK:**
- arduino-libraries/LiquidCrystal (4-bit LCD)
- arduino-libraries/Ethernet (W5100/W5500)
- bblanchon/ArduinoJson (Nextion, si utilisé)
- Adafruit_LiquidCrystal (I2C LCD, selon modèle)

**ANTENNA:**
- mikalhart/TinyGPSPlus (GPS NMEA parsing)
- paulstoffregen/TimerOne (si moteurs steppers activés)

---

## Configuration des features

### Unité SHACK - Features principales

Dans [rotator_features_nano_r4_shack.h](include/rotator_features_nano_r4_shack.h):

```cpp
// Communication
#define FEATURE_MASTER_WITH_SERIAL_SLAVE        // Mode maître RS485
#define FEATURE_MASTER_SEND_AZ_ROTATION_COMMANDS_TO_REMOTE
#define FEATURE_MASTER_SEND_EL_ROTATION_COMMANDS_TO_REMOTE

// Position depuis esclave
#define FEATURE_AZ_POSITION_GET_FROM_REMOTE_UNIT
#define FEATURE_EL_POSITION_GET_FROM_REMOTE_UNIT

// Synchronisation GPS depuis esclave
#define OPTION_SYNC_MASTER_CLOCK_TO_SLAVE
#define OPTION_SYNC_MASTER_COORDINATES_TO_SLAVE

// Interface utilisateur
#define FEATURE_ADAFRUIT_I2C_LCD               // ou FEATURE_4_BIT_LCD_DISPLAY
#define FEATURE_ELEVATION_CONTROL
#define FEATURE_YAESU_EMULATION

// Tracking
#define FEATURE_MOON_TRACKING
#define FEATURE_SUN_TRACKING
#define FEATURE_SATELLITE_TRACKING
#define FEATURE_CLOCK

// Réseau
#define FEATURE_ETHERNET
```

### Unité ANTENNA - Features principales

Dans [rotator_features_nano_r4_antenna.h](include/rotator_features_nano_r4_antenna.h):

```cpp
// Mode esclave
#define FEATURE_REMOTE_UNIT_SLAVE

// Contrôle moteur - choisir UN type:
// Option 1: Relais standard (par défaut)
// Option 2: Moteurs pas à pas
// #define FEATURE_STEPPER_MOTOR
// #define OPTION_STEPPER_MOTOR_MAX_20_KHZ

// Capteurs position AZIMUT - choisir UN type:
#define FEATURE_AZ_POSITION_HH12_AS5045_SSI    // Encodeur absolu SPI
// #define FEATURE_AZ_POSITION_POTENTIOMETER    // Potentiomètre
// #define FEATURE_AZ_POSITION_ROTARY_ENCODER   // Encodeur incrémental
// #define FEATURE_AZ_POSITION_PULSE_INPUT      // Entrée impulsions
// #define FEATURE_AZ_POSITION_HMC5883L         // Boussole I2C

// Capteurs position ELEVATION - choisir UN type:
#define FEATURE_EL_POSITION_HH12_AS5045_SSI    // Encodeur absolu SPI
// #define FEATURE_EL_POSITION_POTENTIOMETER    // Potentiomètre
// #define FEATURE_EL_POSITION_ROTARY_ENCODER   // Encodeur incrémental
// #define FEATURE_EL_POSITION_ADXL345_USING_ADAFRUIT_LIB  // Accéléromètre

// GPS
#define FEATURE_GPS
#define OPTION_GPS_USE_TINY_GPS_LIBRARY
```

---

## Options de contrôle moteur

### Option 1: Contrôle par relais (standard)

Configuration par défaut pour rotateurs Yaesu G-450/G-800/G-1000/G-2800, etc.

**Pins utilisées:**
- `rotate_cw` / `rotate_ccw` pour azimut
- `rotate_up` / `rotate_down` pour élévation
- `brake_az` / `brake_el` pour freins (optionnel)

**Avantages:**
- Simple, fiable
- Compatible avec la plupart des rotateurs commerciaux
- Pas de programmation complexe

### Option 2: Moteurs pas à pas (TB6600, DM542, etc.)

Pour rotateurs DIY avec moteurs steppers NEMA17/23/34.

**Activation:**
```cpp
#define FEATURE_STEPPER_MOTOR
#define OPTION_STEPPER_MOTOR_MAX_20_KHZ  // Fréquence max PWM
```

**Pins TB6600:**
- `az_stepper_motor_pulse`: Impulsions PWM (PUL+ sur TB6600)
- `az_stepper_motor_direction`: Direction (DIR+ sur TB6600)
- `az_stepper_motor_enable`: Activation (ENA+ sur TB6600, LOW = actif)

**Configuration moteur:**
Régler microstepping sur TB6600 (DIP switches):
- 400 pas/tour: 1/2 step
- 800 pas/tour: 1/4 step
- 1600 pas/tour: 1/8 step (recommandé)
- 3200 pas/tour: 1/16 step

**Bibliothèque requise:**
```ini
lib_deps = paulstoffregen/TimerOne@^1.1.1
```

---

## Options de capteurs de position

### HH12 AS5045 SSI (Recommandé)

Encodeur absolu SPI 12-bit, résolution 0.088° (4096 positions).

**Avantages:**
- Absolu (pas besoin de calibration au boot)
- Haute résolution
- Interface SPI rapide
- Pas de drift

**Connexions SPI:**
```
HH12         Nano R4
CLK    ──→   D13 (SCK)
CS     ──→   D10 (azimut) ou D11 (élévation)
DO     ──→   D12 (MISO)
VCC    ──→   3.3V ou 5V (vérifier datasheet)
GND    ──→   GND
```

### Potentiomètre (Simple)

**Avantages:**
- Très simple
- Économique
- Fiable

**Inconvénients:**
- Limité à 360° (ou 180° pour élévation)
- Résolution moyenne
- Usure mécanique

**Connexion:**
```
Potentiomètre 10kΩ linéaire
Pin 1 (GND)  ──→  GND
Pin 2 (wiper)──→  A0 (azimut) ou A3 (élévation)
Pin 3 (VCC)  ──→  5V
```

### Encodeur rotatif incrémental

**Avantages:**
- Bonne résolution
- Rotation illimitée possible
- Économique

**Inconvénients:**
- Nécessite calibration au boot
- Peut perdre position en cas de coupure
- Nécessite pins interrupt

**Connexion:**
```
Encodeur     Nano R4
Phase A ──→  D2 (interrupt 0)
Phase B ──→  D3 (interrupt 1)
VCC     ──→  5V
GND     ──→  GND
```

---

## Configuration GPS

L'unité ANTENNA reçoit le GPS et synchronise les données vers le SHACK via RS485.

**Module GPS recommandé:**
- Ublox NEO-6M/7M/8M
- MTK3339 (Adafruit Ultimate GPS)
- Tout module NMEA 9600 baud

**Connexion Serial2 (Nano R4):**
```
GPS Module    Nano R4
TX      ──→   A5 (RX Serial2)
RX      ──→   A4 (TX Serial2) - optionnel si GPS one-way
VCC     ──→   3.3V (vérifier module!)
GND     ──→   GND
```

**Configuration dans rotator_settings_nano_r4_antenna.h:**
```cpp
#define GPS_PORT Serial2
#define GPS_PORT_BAUD_RATE 9600
```

**Données synchronisées vers SHACK:**
- Latitude / Longitude
- Altitude
- Heure UTC
- Nombre de satellites
- Fix status

---

## Ethernet (SHACK uniquement)

L'unité SHACK peut être contrôlée via réseau Ethernet.

**Modules compatibles:**
- W5100 (Arduino Ethernet Shield)
- W5500 (plus récent, recommandé)

**Connexion SPI:**
```
W5x00 Shield   Nano R4
MOSI    ──→    D11
MISO    ──→    D12
SCK     ──→    D13
CS      ──→    D10
```

**Configuration dans rotator_settings_nano_r4_shack.h:**
```cpp
#define ETHERNET_MAC_ADDRESS 0xDE,0xAD,0xBE,0xEF,0xFE,0xED
#define ETHERNET_IP_ADDRESS 192,168,1,177
#define ETHERNET_IP_GATEWAY 192,168,1,1
#define ETHERNET_IP_SUBNET_MASK 255,255,255,0
#define ETHERNET_TCP_PORT_0 23  // Telnet
```

**Accès réseau:**
- Telnet: `telnet 192.168.1.177 23`
- Compatible N1MM, Ham Radio Deluxe, SatPC32, etc.

---

## Dépannage

### La compilation échoue

**Problème: Erreurs TimeLib / tmElements_t**

Si vous obtenez des erreurs liées à `tmElements_t`, `setTime`, `cleartime`:

1. Désactivez temporairement les features Clock/Tracking:
```cpp
// #define FEATURE_CLOCK
// #define FEATURE_MOON_TRACKING
// #define FEATURE_SUN_TRACKING
// #define FEATURE_SATELLITE_TRACKING
```

2. Ou installez TimeLib manuellement:
```bash
pio lib install "Time"
```

**Problème: "You must specify one AZ position sensor feature"**

Vérifiez dans `rotator_features_nano_r4_antenna.h` qu'UNE option de capteur azimut est activée:
```cpp
#define FEATURE_AZ_POSITION_HH12_AS5045_SSI  // ← Une seule active!
```

### Pas de communication RS485

**Vérifications:**
1. Câblage A-B correct et inversé entre modules
2. Résistances de terminaison 120Ω présentes
3. Baud rate identique (9600) sur SHACK et ANTENNA
4. Pin DE/RE du MAX485 correctement configurée
5. Masse commune entre les deux unités

**Test manuel:**
Téléverser un sketch de test série sur Serial1 pour vérifier la liaison physique.

### GPS ne fonctionne pas

1. Vérifier que le module reçoit 3.3V (pas 5V!)
2. Tester le GPS avec Serial Monitor USB
3. Placer l'antenne GPS en extérieur (vue dégagée du ciel)
4. Attendre 30-60 secondes pour le premier fix
5. Vérifier TinyGPSPlus est installé: `pio lib install "TinyGPSPlus"`

### Moteurs ne tournent pas

**Relais:**
1. Vérifier voltage d'activation relais (5V ou 12V?)
2. Tester avec LED sur pin avant relais
3. Vérifier alim moteur séparée

**Steppers TB6600:**
1. Vérifier ENA+ est LOW (pull-down ou pin OUTPUT LOW)
2. DIP switches microstepping configurés
3. Alimentation 12-36V sur TB6600
4. Courant réglé (potentiomètre sur TB6600)

---

## Schéma de principe

### SHACK Unit
```
                       Arduino Nano R4 (SHACK)
                     ┌───────────────────────┐
USB (PC control) ────┤ Serial (USB)          │
                     │                       │
                     │    Serial1 (D0/D1)    ├──┐
                     │    ┌──────────────┐   │  │  RS485
                     │    │   MAX485     │   │  │  100m max
                D14 ─┼───►│ DE/RE        │   │  │
                     │    │              │   │  │
                     │    │ DI       A ──┼───┼──┼─────► Vers ANTENNA
                     │    │          B ──┼───┼──┘
                     │    │ RO           │   │
                     │    └──────────────┘   │
                     │                       │
                     │    I2C (A4/A5)        ├────► LCD 20x4 I2C
                     │                       │
                     │    SPI (D10-D13)      ├────► W5500 Ethernet
                     │                       │
                D2-D9 ─┼───────────────────────► Boutons & LEDs
                     │                       │
                     └───────────────────────┘
```

### ANTENNA Unit
```
                       Arduino Nano R4 (ANTENNA)
                     ┌───────────────────────┐
                     │    Serial1 (D0/D1)    │
                     │    ┌──────────────┐   │
RS485 depuis ────────┼────┤   MAX485     │   │
SHACK                │    │ DE/RE        │   │
                     │    │              │   │
  A ─────────────────┼────┤ A        DI  │   │
  B ─────────────────┼────┤ B        RO  │   │
                     │    └──────────────┘   │
                     │                       │
                     │    Serial2 (A4/A5)    ├────► GPS Module
                     │                       │
                     │    SPI (D10-D13)      ├────► HH12 AS5045 encoders
                     │                       │
               D2-D3 ─┼─────────────────────────► Relais CW/CCW
               D5-D6 ─┼─────────────────────────► Relais UP/DOWN
                     │                       │
         ou D7-D9,   ─┼─────────────────────────► TB6600 Stepper AZ
            D11-D13  ─┼─────────────────────────► TB6600 Stepper EL
                     │                       │
                     └───────────────────────┘
```

---

## Auteurs & Crédits

**Portage Nano R4 Duo RS485:**
- ON7KGK Michael (2025)

**K3NG Rotator Controller original:**
- K3NG Anthony Good
- Contributions: K1EL, W6EL, VK4GHZ, et la communauté K3NG

**PlatformIO port:**
- N0SFH Neil

**Bibliothèques utilisées:**
- TinyGPSPlus (Mikal Hart)
- Ethernet (Arduino)
- TimeLib (Paul Stoffregen)
- Moon2 (K1EL)
- SunPos

---

## Licence

Même licence que le projet K3NG Rotator Controller original.

---

## Support & Communauté

- **K3NG Wiki:** https://github.com/k3ng/k3ng_rotator_controller/wiki
- **K3NG Group:** https://groups.io/g/radioartisan
- **Issues:** https://github.com/k3ng/k3ng_rotator_controller/issues

---

## TODO / Améliorations futures

- [ ] Finaliser compilation sans erreurs TimeLib
- [ ] Tester communication RS485 réelle
- [ ] Implémenter SoftwareSerial pour Nextion + RS485
- [ ] Ajouter support MCP23017 pour encodeurs rotatifs
- [ ] Créer schémas Fritzing/KiCad
- [ ] Tests terrain avec câble RS485 100m
- [ ] Documentation vidéo YouTube

---

**73 de ON7KGK**
