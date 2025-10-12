# K3NG Rotator - RS485 Configuration Quick Reference

**Projet:** Arduino Nano R4 Minima - RS485 Master/Remote Architecture
**Date:** 12 octobre 2025
**Version:** Phase 4 Complete - Ready for Hardware Testing

---

## 📍 ANTENNA Unit (Master) - Configuration

### Hardware
- **Board:** Arduino Nano R4 Minima (Renesas RA4M1)
- **Location:** À l'antenne (100m du shack)
- **Role:** Master RS485 - Contrôle moteurs + capteurs

### Features Actives
```cpp
✅ FEATURE_ELEVATION_CONTROL      // Contrôle Azimut + Élévation
✅ FEATURE_RS485_MASTER           // RS485 Master (broadcast position)
✅ FEATURE_AZ_POSITION_POTENTIOMETER  // Potentiomètre AZ (temporaire)
✅ FEATURE_EL_POSITION_POTENTIOMETER  // Potentiomètre EL (temporaire)
✅ FEATURE_GPS                    // GPS sur Serial2 (A4/A5)
✅ FEATURE_CLOCK                  // Horloge temps réel
✅ FEATURE_MOON_TRACKING          // Tracking Lune
✅ FEATURE_SUN_TRACKING           // Tracking Soleil
✅ FEATURE_ETHERNET               // Ethernet W5100/W5500
```

### Pins Utilisées

| Pin(s) | Fonction | Description |
|--------|----------|-------------|
| **D0 (RX1)** | RS485 RX | Serial1 - Réception RS485 |
| **D1 (TX1)** | RS485 TX | Serial1 - Transmission RS485 |
| **D8** | RS485 DE | Driver Enable (HIGH pendant TX) |
| **D9** | RS485 RE | Receiver Enable (LOW pendant RX) |
| **A0** | Motor CW | Rotation Azimut CW (Clockwise) |
| **A1** | Motor CCW | Rotation Azimut CCW (Counter-CW) |
| **A2** | Motor UP | Rotation Élévation UP |
| **A3** | Motor DOWN | Rotation Élévation DOWN |
| **D2** | Potentiometer AZ | Lecture position Azimut (analogRead sur pin digitale) |
| **D5** | Potentiometer EL | Lecture position Élévation (analogRead sur pin digitale) |
| **A4 (TX2)** | GPS TX | Serial2 - GPS module |
| **A5 (RX2)** | GPS RX | Serial2 - GPS module |
| **D10** | Ethernet CS | Chip Select Ethernet W5100/W5500 |
| **D11** | Ethernet MOSI | SPI - Master Out Slave In |
| **D12** | Ethernet MISO | SPI - Master In Slave Out |
| **D13** | Ethernet SCK | SPI - Clock |
| **USB Serial** | Debug | Monitor série (115200 bauds) |

### Pins Libres / Futures
| Pin(s) | Disponible pour |
|--------|-----------------|
| D3-D4, D6-D7 | Encodeurs SSI (quand migration depuis potentiomètres) |

### Build & Upload
```bash
# Compiler
pio run -e antenna_unit

# Upload (remplacer PORT par le port USB)
./upload_antenna.sh /dev/cu.usbmodem1101
```

### Utilisation Mémoire
- **Flash:** 59.3% (155 KB / 256 KB)
- **RAM:** 31.8% (10.4 KB / 32 KB)

---

## 🏠 SHACK Unit (Remote) - Configuration

### Hardware
- **Board:** Arduino Nano R4 Minima (Renesas RA4M1)
- **Location:** Dans le shack
- **Role:** Remote RS485 - Interface utilisateur

### Features Actives
```cpp
✅ FEATURE_ELEVATION_CONTROL          // Affichage AZ + EL
✅ FEATURE_RS485_REMOTE               // RS485 Remote (écoute broadcasts)
✅ FEATURE_AZ_POSITION_ROTARY_ENCODER // Virtual (position via RS485)
✅ FEATURE_EL_POSITION_ROTARY_ENCODER // Virtual (position via RS485)
✅ FEATURE_NEXTION_DISPLAY            // Écran Nextion sur Serial2
✅ button_cw, button_ccw              // Boutons manuels
✅ button_up, button_down, button_stop
✅ FEATURE_AZ_PRESET_ENCODER          // Encodeur preset Azimut
✅ FEATURE_EL_PRESET_ENCODER          // Encodeur preset Élévation
✅ FEATURE_YAESU_EMULATION            // USB Serial pour PSTRotator

⚠️ GPS/CLOCK/TRACKING/ETHERNET        // Définis pour types K3NG uniquement
                                      // Pas de hardware physique sur Remote
```

### Pins Utilisées

| Pin(s) | Fonction | Description |
|--------|----------|-------------|
| **D0 (RX1)** | RS485 RX | Serial1 - Réception RS485 |
| **D1 (TX1)** | RS485 TX | Serial1 - Transmission RS485 |
| **D8** | RS485 DE | Driver Enable (HIGH pendant TX) |
| **D9** | RS485 RE | Receiver Enable (LOW pendant RX) |
| **A4 (TX2)** | Nextion TX | Serial2 - Écran Nextion (RX Nextion) |
| **A5 (RX2)** | Nextion RX | Serial2 - Écran Nextion (TX Nextion) |
| **D2** | Button CW | Bouton rotation CW (normalement ouvert) |
| **D3** | Button CCW | Bouton rotation CCW (normalement ouvert) |
| **D4** | Button UP | Bouton élévation UP (normalement ouvert) |
| **D5** | Button DOWN | Bouton élévation DOWN (normalement ouvert) |
| **D6** | Button STOP | Bouton STOP (normalement ouvert) |
| **D10-D11** | Preset Encoder AZ | Encodeur rotatif preset Azimut (A/B) |
| **D12-D13** | Preset Encoder EL | Encodeur rotatif preset Élévation (A/B) |
| **A1** | Preset START | Bouton démarrage preset |
| **USB Serial** | PSTRotator | Yaesu/Easycom (PC control) |

### Pins Optionnelles (Non Utilisées)
| Pin(s) | Fonction Optionnelle |
|--------|----------------------|
| D7 | Bouton tracking Lune |
| A0 | Bouton tracking Soleil |

### Build & Upload
```bash
# Compiler
pio run -e shack_unit

# Upload (remplacer PORT par le port USB)
./upload_shack.sh /dev/cu.usbmodem1102
```

### Utilisation Mémoire
- **Flash:** 61.2% (160 KB / 256 KB)
- **RAM:** 32.3% (10.6 KB / 32 KB)

---

## 🔌 Connexions RS485

### Câblage RS485 Module

```
ANTENNA Unit (Master)          Module RS485 #1
  TX1 (D1)  ───────────────►   DI (Driver Input)
  RX1 (D0)  ◄───────────────   RO (Receiver Output)
  D8        ───────────────►   DE (Driver Enable)
  D9        ───────────────►   RE (Receiver Enable)
  5V        ───────────────►   VCC
  GND       ───────────────►   GND


Module RS485 #1                Module RS485 #2 (100m)
  A         ◄═══════════════►  A  (Twisted pair)
  B         ◄═══════════════►  B  (Twisted pair)

  Résistance 120Ω entre A-B aux 2 extrémités


SHACK Unit (Remote)            Module RS485 #2
  TX1 (D1)  ───────────────►   DI (Driver Input)
  RX1 (D0)  ◄───────────────   RO (Receiver Output)
  D8        ───────────────►   DE (Driver Enable)
  D9        ───────────────►   RE (Receiver Enable)
  5V        ───────────────►   VCC
  GND       ───────────────►   GND
```

### Paramètres RS485
- **Baud Rate:** 19200 (optimal pour 100m)
- **Mode:** Hardware Serial1 (TX1/RX1)
- **Protocol:** Master/Remote avec CRC16-CCITT
- **Update Rate:** 100ms (10 Hz)
- **Latence:** < 5ms mesurée

---

## 📝 Protocole RS485

### Master (ANTENNA) Broadcasts
- **Fréquence:** Toutes les 100ms
- **Contenu:** Position AZ/EL, Target AZ/EL, Status
- **Adresse:** 0xFF (broadcast)
- **Trame:** 31 bytes avec CRC16

### Remote (SHACK) Commands
- **Type:** Move To, Stop, Park
- **Mode:** Request/Response
- **Timeout:** 50ms

---

## 🔧 Configuration PlatformIO

### Fichiers de Configuration

| Unité | Features | Pins |
|-------|----------|------|
| **ANTENNA** | [rotator_features_master.h](include/rotator_features_master.h) | [rotator_pins_master.h](include/rotator_pins_master.h) |
| **SHACK** | [rotator_features_remote.h](include/rotator_features_remote.h) | [rotator_pins_remote.h](include/rotator_pins_remote.h) |

### Build Flags

**ANTENNA Unit:**
```ini
build_flags =
  -DUSE_ROTATOR_FEATURES_MASTER
  -DUSE_ROTATOR_PINS_MASTER
```

**SHACK Unit:**
```ini
build_flags =
  -DUSE_ROTATOR_FEATURES_REMOTE
  -DUSE_ROTATOR_PINS_REMOTE
```

---

## 📚 Documentation Complète

| Document | Description |
|----------|-------------|
| [RS485_PINS_ALLOCATION.md](RS485_PINS_ALLOCATION.md) | Allocation détaillée des pins (Master vs Remote) |
| [RS485_FEATURES_SPLIT.md](RS485_FEATURES_SPLIT.md) | Répartition des fonctionnalités |
| [PHASE4_SETUP.md](PHASE4_SETUP.md) | Guide de configuration Phase 4 |
| [PROGRESS.md](PROGRESS.md) | Historique du développement |
| [README.md](README.md) | Vue d'ensemble du projet |

---

## ⚠️ Notes Importantes

### ANTENNA Unit
- ⚠️ **Potentiomètres temporaires** sur D2 (AZ) et D5 (EL)
  - Pins A6/A7 n'existent pas sur Nano R4 Minima
  - Pins A0-A3 utilisées pour relais moteurs (ne pas confondre!)
  - D2/D5 en mode analogRead() pour lecture potentiomètres
- 🔄 **Migration future:** Encodeurs SSI HH-12 sur D3-D4 (AZ) et D6-D7 (EL)
- ⚠️ **Ethernet actif:** Pins D10-D13 (SPI) - Compatible avec potentiomètres D2/D5
- ✅ **GPS obligatoire** sur Serial2 (A4/A5) pour tracking

### SHACK Unit
- ⚠️ **Pas de GPS physique** - Serial2 utilisé pour Nextion
- ⚠️ **Position virtuelle** - Données reçues via RS485 uniquement
- ✅ **Nextion OU LCD I2C** - Même pins (A4/A5)
- ⚠️ **Types GPS/Tracking définis** - Pour compilation K3NG uniquement

### RS485
- ✅ **Câble blindé** recommandé pour 100m
- ✅ **Résistances 120Ω** obligatoires aux 2 extrémités
- ⚠️ **Pas de masse commune** - Isolation RS485
- ✅ **Twisted pair** A et B

---

## 🚀 Quick Start

### 1. Compiler les 2 firmwares
```bash
# Dans le répertoire du projet
pio run -e antenna_unit
pio run -e shack_unit
```

### 2. Upload sur les cartes
```bash
# ANTENNA Unit (vérifier le port USB)
./upload_antenna.sh /dev/cu.usbmodem1101

# SHACK Unit (vérifier le port USB)
./upload_shack.sh /dev/cu.usbmodem1102
```

### 3. Vérifier la communication
```bash
# Monitor série ANTENNA (Master)
pio device monitor -e antenna_unit -b 115200

# Monitor série SHACK (Remote)
pio device monitor -e shack_unit -b 115200
```

### 4. Tests attendus
- ✅ Master broadcast position toutes les 100ms
- ✅ Remote affiche "Age < 5ms"
- ✅ CRC validé sur toutes les trames
- ✅ Commandes Move/Stop/Park fonctionnelles
- ✅ Aucune collision RS485

---

## 📊 Status Actuel

| Phase | Status | Description |
|-------|--------|-------------|
| Phase 1 | ✅ Complete | Communication RS485 de base |
| Phase 2 | ✅ Complete | Protocole avec CRC16 |
| Phase 3 | ✅ Complete | Architecture Master/Remote |
| Phase 3.5 | ✅ Complete | Documentation architecture |
| Phase 4 | ✅ Complete | Intégration K3NG + Compilation OK |
| Phase 5 | ⏳ À venir | Tests hardware sur 100m |

---

**Version:** 1.0
**Dernière mise à jour:** 12 octobre 2025
**Développeur:** ON7KGK Michael
**Assistant:** Claude Code (Anthropic)

---

**🤖 Generated with [Claude Code](https://claude.com/claude-code)**
