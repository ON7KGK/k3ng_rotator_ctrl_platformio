# K3NG Rotator Controller - Arduino Giga R1 WiFi Edition

## Description

Version du [K3NG Rotator Controller](https://github.com/k3ng/k3ng_rotator_controller) pour **PlatformIO** portée sur **Arduino Giga R1 WiFi** (STM32H747XI dual-core).

Basé sur le portage UNO R4 Minima, adapté pour la plateforme STM32H7.

## Avantages du Giga R1 WiFi

### Spécifications supérieures
- **MCU** : STM32H747XI Dual-Core (Cortex-M7 @ 480MHz + M4 @ 240MHz)
- **Mémoire** : 2MB Flash, 1MB SRAM + 8MB SDRAM (vs 256KB/32KB sur UNO R4)
- **Serial ports** : 4 UARTs matériels (vs 3 sur UNO R4)
- **GPIO** : 76 I/O (vs 20 sur UNO R4)
- **Format** : Arduino Mega compatible
- **Vitesse** : 480MHz (vs 48MHz sur UNO R4) - **10x plus rapide**

### Points clés
✅ **8x plus de Flash** : 2MB vs 256KB
✅ **32x plus de RAM** : 1MB vs 32KB
✅ **10x plus rapide** : 480MHz vs 48MHz
✅ **Plus de ports série** : 4 UARTs vs 3
✅ **Plus de GPIO** : 76 vs 20
✅ **SDRAM externe** : 8MB pour stockage TLE satellites

## Fonctionnalités principales

### Fonctionnalités de base
- **Contrôle Azimut + Élévation** (AZ/EL)
- **Émulation Yaesu GS-232B**
- **Écran Nextion** (Serial1)
- **GPS NMEA** sur Serial2 matériel
- **Moon Tracking** (suivi lunaire)
- **Sun Tracking** (suivi solaire)
- **Satellite Tracking** (suivi satellite avec TLE, AOS/LOS)
- **Ethernet** (support shield W5100/W5500)
- **Alerte sonore**
- **Indicateur de rotation**
- **Encodeurs rotatifs de preset** (AZ + EL)
- **Horloge/Clock** (synchronisation GPS)

### WiFi
Le module WiFi du Giga R1 n'est pas utilisé dans ce projet. Toutes les fonctionnalités réseau utilisent Ethernet.

### Configuration ports série
- **Serial** (USB-C) : Communication de contrôle @ 9600 bauds
- **Serial1** : Écran Nextion @ 115200 bauds
- **Serial2** (pins 17/16) : Module GPS @ 9600 bauds (UART matériel natif)
- **Serial3** : Disponible pour extensions futures

## Installation et compilation

### Prérequis
- [PlatformIO IDE](https://platformio.org/) (extension VSCode recommandée)
- Arduino Giga R1 WiFi

### Compilation
```bash
pio run -e giga_r1_m7
```

### Upload
```bash
pio run -e giga_r1_m7 -t upload
```

### Fichiers de configuration
- **`include/rotator_features.h`** : Activation/désactivation des fonctionnalités
- **`include/rotator_settings.h`** : Paramètres du rotateur
- **`include/rotator_pins.h`** : Configuration des pins I/O
- **`platformio.ini`** : Configuration PlatformIO

## Modifications pour Giga R1 WiFi

### 1. Platform STM32
Utilise `platform = ststm32` avec le board `giga_r1_m7` (core Cortex-M7)

### 2. Serial2 natif
Le Giga R1 a Serial2 matériel intégré (pins 17 RX / 16 TX), contrairement au UNO R4 qui nécessite une déclaration UART custom.

### 3. Émulation EEPROM
Le STM32H747 n'a pas d'EEPROM matériel. Une émulation RAM temporaire est fournie dans `lib/EEPROM_Giga/`.

**⚠️ IMPORTANT** : Les données EEPROM ne sont PAS persistantes actuellement. Pour un stockage permanent, il faudra implémenter FlashIAP ou KVStore (Mbed OS).

### 4. Compatibilité dtostrf
Une implémentation de `dtostrf()` est fournie dans `include/platform_compatibility.h` pour compatibilité AVR.

### 5. Stack Pointer (SP)
Le Stack Pointer AVR n'est pas disponible sur ARM. `SP` est défini à 0 pour la compatibilité de compilation.

## Utilisation mémoire

```
RAM:   11.6% (60.9KB / 511KB) - Beaucoup de marge!
Flash: 27.6% (217KB / 768KB) - Beaucoup de marge!
```

Le Giga R1 offre **énormément d'espace** pour des extensions futures :
- Stockage de multiples TLE satellites en RAM
- Logs étendus
- Calculs astronomiques complexes
- Interfaces utilisateur avancées

## Câblage GPS

Le GPS utilise le port série matériel Serial2 :
- **Pin 17 (RX2)** ← TXD du module GPS
- **Pin 16 (TX2)** → RXD du module GPS (optionnel)
- **VCC** → 3.3V ou 5V selon module
- **GND** → GND

## Problèmes connus

### EEPROM non-persistant
L'émulation EEPROM actuelle est en RAM. Les paramètres ne survivent pas au redémarrage.

**Solutions futures** :
- Implémenter FlashIAP pour stockage flash interne
- Utiliser Mbed OS KVStore API
- Stocker en QSPI Flash externe (8MB disponibles)

### Warnings de compilation
Quelques warnings bénins sont normaux :
- Variables non utilisées (fonctionnalités désactivées)
- Deprecated binary constants (B11111000 → 0b11111000)
- Fonctions sans return (bugs upstream)

Ces warnings n'affectent pas le fonctionnement.

## Différences vs UNO R4 Minima

| Caractéristique | UNO R4 Minima | Giga R1 WiFi | Gain |
|----------------|---------------|--------------|------|
| MCU            | Renesas RA4M1 (Cortex-M4 48MHz) | STM32H747XI (Cortex-M7 480MHz) | 10x |
| Flash          | 256KB | 2MB | 8x |
| SRAM           | 32KB | 1MB | 32x |
| UARTs          | 3 | 4 | +1 |
| GPIO           | 20 | 76 | +56 |
| Format         | UNO | Mega | Mega |
| SDRAM externe  | - | 8MB | +8MB |

## Documentation

### Documentation officielle K3NG
- [Wiki K3NG Rotator Controller](https://github.com/k3ng/k3ng_rotator_controller/wiki)
- [Dépôt original](https://github.com/k3ng/k3ng_rotator_controller)

### Documentation Giga R1 WiFi
- [Arduino Giga R1 WiFi](https://docs.arduino.cc/hardware/giga-r1-wifi/)
- [PlatformIO Giga R1](https://docs.platformio.org/en/latest/boards/ststm32/giga_r1_m7.html)
- [STM32H747XI Datasheet](https://www.st.com/en/microcontrollers-microprocessors/stm32h747xi.html)

## Contributions

Ce projet est basé sur le travail original de K3NG. Portage Giga R1 WiFi :
- Configuration PlatformIO STM32
- Adaptations Serial2 natif
- Émulation EEPROM ARM
- Compatibilité dtostrf/SP
- Fix PCF8583 narrowing conversion

## Licence

Ce projet hérite de la licence du projet original K3NG Rotator Controller.

## Remerciements

- **Anthony Good (K3NG)** - Auteur original du K3NG Rotator Controller
- **Neil (N0SFH)** - Portage initial vers VSCode PlatformIO
- **Michael (ON7KGK)** - Portage UNO R4 Minima et Giga R1 WiFi
- **Claude (Anthropic)** - Assistance au portage ARM et résolution des problèmes
- La communauté PlatformIO
- Arduino et STMicroelectronics

---

**Version** : 1.0 Giga R1 WiFi Edition
**Branche** : GigaR1_Test
**Dernière mise à jour** : Octobre 2025
**Testé avec** : PlatformIO 6.x, Arduino Framework Mbed 4.3.1
