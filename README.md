# K3NG Rotator Controller - Arduino UNO R4 Minima Edition

## Description

Version améliorée et portée du [K3NG Rotator Controller](https://github.com/k3ng/k3ng_rotator_controller) pour **PlatformIO** avec support complet pour **Arduino UNO R4 Minima** (Renesas RA4M1).

Ce projet a été adapté depuis l'architecture AVR (Arduino Mega 2560) vers l'architecture ARM Cortex-M4 du UNO R4 Minima, avec optimisation et ajout de nombreuses fonctionnalités.

## Fonctionnalités principales

### Fonctionnalités de base
- **Contrôle Azimut + Élévation** (AZ/EL)
- **Émulation Yaesu GS-232B**
- **Écran Nextion** (Serial1)
- **GPS NMEA** sur Serial2 matériel (pins A4/A5)
- **Moon Tracking** (suivi lunaire)
- **Sun Tracking** (suivi solaire)
- **Satellite Tracking** (suivi satellite avec TLE, AOS/LOS)
- **Ethernet** (support shield W5100/W5500)
- **Alerte sonore**
- **Indicateur de rotation**
- **Encodeurs rotatifs de preset** (AZ + EL)
- **Horloge/Clock** (synchronisation GPS)

### Fonctionnalités avancées testées
- Détection de fin de course (LIMIT_SENSE)
- Buffer temporisé Yaesu (TIMED_BUFFER)
- Texte d'aide série (SERIAL_HELP_TEXT)
- Mode Park & AutoPark
- Limites de rotation manuelle (AZ/EL)
- Correction azimut/élévation par table
- Détection de blocage rotation (STALL_DETECTION)
- Contrôle de pins auxiliaires (ANCILLARY_PIN_CONTROL)
- Support joystick analogique
- Compatibilité Ham Radio Deluxe

## Spécifications techniques

### Matériel supporté
- **Carte principale** : Arduino UNO R4 Minima (Renesas RA4M1)
- **Processeur** : ARM Cortex-M4 @ 48 MHz
- **Mémoire** : 32 KB RAM, 256 KB Flash, 8 KB EEPROM

### Utilisation mémoire (configuration complète)
- **RAM** : 32.1% utilisé (10516 / 32768 bytes) - **68% disponible**
- **Flash** : 61.6% utilisé (161488 / 262144 bytes) - **38% disponible**

### Configuration ports série
- **Serial** (USB) : Communication de contrôle @ 9600 bauds
- **Serial1** (D0/D1) : Écran Nextion @ 115200 bauds
- **Serial2** (A4/A5) : Module GPS @ 9600 bauds (UART matériel)

## Installation et compilation

### Prérequis
- [PlatformIO IDE](https://platformio.org/) (extension VSCode recommandée)
- Arduino UNO R4 Minima

### Compilation
```bash
# Compiler pour UNO R4 Minima
pio run -e uno_r4_minima

# Compiler et téléverser
pio run -e uno_r4_minima --target upload
```

### Compilation pour Arduino Mega 2560 (legacy)
```bash
pio run -e rotator_controller
```

## Configuration

### Fichiers de configuration principaux
- **`include/rotator_features.h`** : Activation/désactivation des fonctionnalités
- **`include/rotator_settings.h`** : Paramètres du rotateur (vitesses, limites, calibration)
- **`include/rotator_pins.h`** : Configuration des pins I/O
- **`platformio.ini`** : Configuration PlatformIO et bibliothèques

### Activation d'une fonctionnalité
Dans `include/rotator_features.h`, décommentez la ligne correspondante :
```cpp
#define FEATURE_ELEVATION_CONTROL      // Active le contrôle d'élévation
#define FEATURE_GPS                     // Active le support GPS
#define FEATURE_SATELLITE_TRACKING      // Active le suivi satellite
```

## Câblage GPS

Le GPS utilise le port série matériel Serial2 sur les pins :
- **A5 (RX)** ← TXD du module GPS
- **A4 (TX)** → RXD du module GPS (optionnel)
- **VCC** → 3.3V ou 5V selon module
- **GND** → GND

## Améliorations spécifiques UNO R4 Minima

### Corrections de compatibilité ARM
1. **Serial2 matériel** sur A4/A5 (pas besoin de SoftwareSerial)
2. **Exclusion Stack Pointer (SP)** - non disponible sur architecture Renesas
3. **Pin A6** - conditionnée (n'existe pas sur UNO R4)
4. **Bibliothèques incompatibles exclues** : TimerOne, TimerFive, RTClib (AVR uniquement)
5. **60+ prototypes de fonctions** ajoutés dans `rotator_functions.h`

### Optimisations
- Compilation optimisée pour ARM Cortex-M4
- Support complet de l'écosystème Arduino Renesas
- Gestion mémoire adaptée (32 KB RAM vs 8 KB sur Mega)

## Documentation

### Documentation officielle K3NG
- [Wiki K3NG Rotator Controller](https://github.com/k3ng/k3ng_rotator_controller/wiki)
- [Dépôt original](https://github.com/k3ng/k3ng_rotator_controller)

### Documentation UNO R4 Minima
- [Arduino UNO R4 Minima](https://docs.arduino.cc/hardware/uno-r4-minima/)
- [Spécifications techniques](https://docs.platformio.org/en/latest/boards/renesas-ra/uno_r4_minima.html)

## Développement

### Structure du projet
```
k3ng_rotator_ctrl_platformio-main/
├── include/              # Fichiers d'en-tête (.h)
│   ├── rotator_features.h
│   ├── rotator_settings.h
│   ├── rotator_pins.h
│   ├── rotator_functions.h
│   └── ...
├── lib/                  # Bibliothèques locales
├── src/                  # Code source (.cpp)
│   └── k3ng_rotator_controller.cpp
├── platformio.ini        # Configuration PlatformIO
└── README.md
```

### Ajout de bibliothèques
Éditez `platformio.ini` section `[env:uno_r4_minima]` :
```ini
lib_deps =
    arduino-libraries/LiquidCrystal@^1.0.7
    arduino-libraries/Ethernet@^2.0.2
    # Ajoutez vos bibliothèques ici
```

## Problèmes connus et solutions

### Bibliothèques AVR incompatibles
Les bibliothèques suivantes ne fonctionnent **pas** sur UNO R4 :
- `TimerOne` / `TimerFive` (spécifique AVR)
- `SoftwareSerial` (utiliser Serial1 ou Serial2 à la place)
- Certaines versions de `RTClib` (conflits Wire1)

**Solution** : Elles sont automatiquement exclues via `lib_ignore` dans `platformio.ini`

### Erreur "function not declared"
Si vous activez une fonctionnalité et obtenez une erreur de compilation :
1. Vérifiez que le prototype existe dans `include/rotator_functions.h`
2. Vérifiez les dépendances dans `include/rotator_dependencies.h`

## Exemples de commandes série

```
# Rotation azimut absolue
M 180

# Rotation élévation
W 45

# Status
C

# Arrêt
S

# Parking
P

# Aide (si OPTION_SERIAL_HELP_TEXT activé)
?
```

## Contributions

Ce projet est basé sur le travail original de K3NG. Les modifications pour PlatformIO et UNO R4 Minima incluent :
- Configuration PlatformIO complète
- Portage architecture ARM Renesas RA4M1
- Ajout de prototypes de fonctions pour compilation C++
- Support Serial2 matériel pour GPS
- Tests et validation de toutes les fonctionnalités principales

## Licence

Ce projet hérite de la licence du projet original K3NG Rotator Controller.

Voir le fichier [LICENSE](LICENSE) pour plus de détails.

## Remerciements

- **Anthony Good (K3NG)** - Auteur original du K3NG Rotator Controller
- **Neil (N0SFH)** - Portage initial vers VSCode PlatformIO
- **Claude (Anthropic)** - Assistance au portage ARM et résolution des problèmes de compilation
- La communauté PlatformIO
- Arduino et Renesas pour le UNO R4 Minima

---

**Version** : 1.0 UNO R4 Minima Edition
**Dernière mise à jour** : Octobre 2025
**Testé avec** : PlatformIO 6.x, Arduino Framework 1.4.1 (Renesas)
