# Guide de câblage Arduino Giga R1 WiFi - K3NG Rotator Controller

## 📌 Configuration des pins optimisée pour Giga R1 WiFi

Ce document décrit l'affectation des pins dans [include/rotator_pins_giga_r1.h](include/rotator_pins_giga_r1.h) pour tirer le meilleur parti des capacités du Giga R1 WiFi.

---

## ⚠️ IMPORTANT : Niveau de tension

**Le Giga R1 WiFi fonctionne en logique 3.3V et N'EST PAS tolérant au 5V !**

- Toutes les entrées doivent être ≤ 3.3V
- Toutes les sorties fournissent 3.3V (pas 5V)
- Utilisez des **level shifters** si vous connectez des périphériques 5V
- Courant max par pin : 8mA (utilisez des drivers/transistors pour charges élevées)

---

## 🔌 Pins réservées (NE PAS UTILISER)

| Pins | Fonction | Utilisation K3NG |
|------|----------|------------------|
| **D0/D1** | Serial1 | **Écran Nextion** (115200 baud) |
| **D16 (TX2) / D17 (RX2)** | Serial2 | **Module GPS** (9600 baud) |
| **D14/D15** | Serial3 | Réservé pour extensions futures |
| **D20 (SDA) / D21 (SCL)** | I2C Wire | Écrans LCD I2C, capteurs I2C |
| **USB-C** | Serial (USB) | **Console de contrôle** (9600 baud) |

---

## 🎛️ Contrôle AZIMUT

### Sorties relais/moteur (3.3V logic - utiliser drivers!)
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D22** | `rotate_cw` | HIGH = Rotation CW (sens horaire) |
| **D23** | `rotate_ccw` | HIGH = Rotation CCW (sens anti-horaire) |
| **D28** | `brake_az` | HIGH = Désengager frein azimut |

### Sorties PWM pour contrôle de vitesse (3.3V, 500Hz-1kHz)
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D5** | `rotate_cw_pwm` | PWM vitesse rotation CW (0-100%) |
| **D6** | `rotate_ccw_pwm` | PWM vitesse rotation CCW (0-100%) |
| **D7** | `azimuth_speed_voltage` | PWM vitesse continue (actif toujours) |

### Entrées analogiques de position (0-3.3V)
| Pin | Fonction | Plage | Description |
|-----|----------|-------|-------------|
| **A0** | `rotator_analog_az` | 0-3.3V | Tension azimut du rotateur (connecteur Yaesu pin 4) |
| **A1** | `az_speed_pot` | 0-3.3V | Potentiomètre vitesse (1K-10K) |
| **A2** | `az_preset_pot` | 0-3.3V | Potentiomètre preset position (1K-10K) |

### Boutons de commande manuelle (NO vers GND)
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D24** | `button_cw` | Bouton rotation CW manuelle |
| **D25** | `button_ccw` | Bouton rotation CCW manuelle |
| **D26** | `button_stop` | Bouton arrêt d'urgence |
| **D27** | `preset_start_button` | Bouton démarrage preset |

### Indicateurs LED
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D29** | `overlap_led` | LED allumée si azimut > 360° |
| **D30** | `rotation_indication_pin` | LED allumée pendant rotation |
| **D31** | `serial_led` | LED clignote à chaque commande série |

---

## 📐 Contrôle ÉLÉVATION

### Sorties relais/moteur (3.3V logic - utiliser drivers!)
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D32** | `rotate_up` | HIGH = Élévation UP (montée) |
| **D33** | `rotate_down` | HIGH = Élévation DOWN (descente) |
| **D36** | `brake_el` | HIGH = Désengager frein élévation |

### Sorties PWM pour contrôle de vitesse
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D9** | `rotate_up_pwm` | PWM vitesse montée (0-100%) |
| **D10** | `rotate_down_pwm` | PWM vitesse descente (0-100%) |
| **D11** | `elevation_speed_voltage` | PWM vitesse continue |

### Entrées analogiques
| Pin | Fonction | Plage | Description |
|-----|----------|-------|-------------|
| **A3** | `rotator_analog_el` | 0-3.3V | Tension élévation du rotateur |

### Boutons manuels (NO vers GND)
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D34** | `button_up` | Bouton montée manuelle |
| **D35** | `button_down` | Bouton descente manuelle |

---

## 🔊 Alertes sonores

| Pin | Fonction | Description |
|-----|----------|-------------|
| **D8** | `pin_audible_alert` | Sortie PWM pour buzzer piezo (utiliser `tone()`) |
| **LED_BUILTIN** | `blink_led` | LED intégrée (clignote selon état) |

---

## 🖥️ Écran LCD 4-bit (HD44780)

| Pin | Fonction | Description |
|-----|----------|-------------|
| **D53** | `lcd_4_bit_rs_pin` | Register Select |
| **D54** | `lcd_4_bit_enable_pin` | Enable |
| **D55** | `lcd_4_bit_d4_pin` | Data 4 |
| **D56** | `lcd_4_bit_d5_pin` | Data 5 |
| **D57** | `lcd_4_bit_d6_pin` | Data 6 |
| **D58** | `lcd_4_bit_d7_pin` | Data 7 |

**Écrans I2C** : Utilisent automatiquement pins **D20 (SDA) / D21 (SCL)**

---

## 🕹️ Joystick analogique (optionnel)

| Pin | Fonction | Plage | Description |
|-----|----------|-------|-------------|
| **A4** | `pin_joystick_x` | 0-3.3V | Axe X (azimut) |
| **A5** | `pin_joystick_y` | 0-3.3V | Axe Y (élévation) |

---

## 🔄 Encodeurs rotatifs

### Encodeurs de preset (entrée manuelle)
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D37** | `az_rotary_preset_pin1` | Azimut encoder CW |
| **D38** | `az_rotary_preset_pin2` | Azimut encoder CCW |
| **D39** | `el_rotary_preset_pin1` | Élévation encoder UP |
| **D40** | `el_rotary_preset_pin2` | Élévation encoder DOWN |

### Encodeurs de position (feedback antenne)
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D41** | `az_rotary_position_pin1` | Azimut position encoder A |
| **D42** | `az_rotary_position_pin2` | Azimut position encoder B |
| **D43** | `el_rotary_position_pin1` | Élévation position encoder A |
| **D44** | `el_rotary_position_pin2` | Élévation position encoder B |

**Note** : Tous les pins du Giga R1 supportent les interrupts (contrairement aux AVR)

---

## 🌙 Tracking astronomique

### Moon Tracking
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D70** | `moon_tracking_active_pin` | HIGH quand tracking lune actif |
| **D71** | `moon_tracking_button` | Bouton activation (NO vers GND) |

### Sun Tracking
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D72** | `sun_tracking_active_pin` | HIGH quand tracking soleil actif |
| **D73** | `sun_tracking_button` | Bouton activation (NO vers GND) |

### Satellite Tracking
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D74** | `satellite_tracking_active_pin` | HIGH quand tracking satellite actif |
| **D75** | `satellite_tracking_button` | Bouton activation (NO vers GND) |

---

## 📡 GPS

| Pin | Fonction | Description |
|-----|----------|-------------|
| **D17 (RX2)** | GPS TXD | Réception données GPS NMEA @ 9600 baud |
| **D16 (TX2)** | GPS RXD | Émission vers GPS (optionnel) |
| **D76** | `gps_sync` | Pulse de synchronisation GPS (output) |

**Configuration Serial2** : Définie automatiquement (pins 17/16 natifs)

---

## 📤 Sorties analogiques (DAC)

⭐ **Le Giga R1 a de vrais DAC (pas juste PWM) !**

| Pin | Fonction | Plage | Description |
|-----|----------|-------------|-------------|
| **A12** | `pin_analog_az_out` | 0-3.3V | Sortie DAC azimut (12-bit, 0-4095) |
| **A13** | `pin_analog_el_out` | 0-3.3V | Sortie DAC élévation (12-bit, 0-4095) |

**Résolution** : 12-bit (4096 niveaux) vs 8-bit PWM sur AVR

---

## 🅿️ Park & Limits

### Park (stationnement)
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D65** | `button_park` | Bouton Park (NO vers GND) |
| **D66** | `park_in_progress_pin` | HIGH pendant parking |
| **D67** | `parked_pin` | HIGH quand position park atteinte |

### Limit Sense (fin de course)
| Pin | Fonction | Description |
|-----|----------|-------------|
| **D68** | `az_limit_sense_pin` | LOW = arrêt azimut (normalement HIGH) |
| **D69** | `el_limit_sense_pin` | LOW = arrêt élévation (normalement HIGH) |

---

## 📊 Récapitulatif PWM (pins 2-13 uniquement)

| Pin | Affectation | Fonction |
|-----|-------------|----------|
| D2 | - | Disponible |
| D3 | - | Disponible |
| D4 | - | Disponible |
| **D5** | ✅ **Utilisé** | `rotate_cw_pwm` |
| **D6** | ✅ **Utilisé** | `rotate_ccw_pwm` |
| **D7** | ✅ **Utilisé** | `azimuth_speed_voltage` |
| **D8** | ✅ **Utilisé** | `pin_audible_alert` |
| **D9** | ✅ **Utilisé** | `rotate_up_pwm` |
| **D10** | ✅ **Utilisé** | `rotate_down_pwm` |
| **D11** | ✅ **Utilisé** | `elevation_speed_voltage` |
| D12 | - | Disponible (aussi SPI MISO) |
| D13 | - | Disponible (aussi SPI SCK + LED) |

**Total PWM utilisés : 6/12**

---

## 📊 Récapitulatif Analog (A0-A13)

| Pin | Affectation | Fonction |
|-----|-------------|----------|
| **A0** | ✅ ADC | `rotator_analog_az` |
| **A1** | ✅ ADC | `az_speed_pot` |
| **A2** | ✅ ADC | `az_preset_pot` |
| **A3** | ✅ ADC | `rotator_analog_el` |
| **A4** | ✅ ADC | `pin_joystick_x` |
| **A5** | ✅ ADC | `pin_joystick_y` |
| **A6** | ✅ ADC | `pin_memsic_2125_x` (si capteur inclinomètre) |
| **A7** | ✅ ADC | `pin_memsic_2125_y` |
| A8-A11 | - | **Disponibles** (4 pins libres) |
| **A12** | ✅ **DAC** | `pin_analog_az_out` (sortie) |
| **A13** | ✅ **DAC** | `pin_analog_el_out` (sortie) |

**Total ADC utilisés : 8/12**
**Total DAC utilisés : 2/2**

---

## 🔧 Utilisation des pins

### Pins utilisées : **~60** sur 76 disponibles
### Pins libres : **~16** pour extensions futures

**Pins libres recommandées pour extensions :**
- D2, D3, D4 (PWM disponibles)
- D12, D13 (PWM + SPI)
- D18-D21 (attention I2C sur 20/21)
- A8-A11 (entrées analogiques)
- D76+ (pins hautes non affectées)

---

## 💡 Conseils de câblage

### 1. Protection des entrées 3.3V
```
Périphérique 5V ──┬─── Résistance 2.2kΩ ──┬─── Pin Giga (3.3V)
                  └─── Résistance 3.3kΩ ───┴─── GND
```
**Diviseur de tension 5V → 3.3V** : R1=2.2kΩ, R2=3.3kΩ

### 2. Drivers de sortie (relais, moteurs)
```
Pin Giga D22 ──── Résistance 1kΩ ──── Base transistor 2N2222
                                       Collecteur → Relais/moteur
                                       Émetteur → GND
```
**Courant max pin = 8mA** → Toujours utiliser transistors/MOSFETs !

### 3. Connexion GPS (Serial2)
```
GPS Module:
  VCC → 3.3V ou 5V (selon module)
  GND → GND
  TXD → D17 (RX2) directement si GPS 3.3V
  RXD → D16 (TX2) optionnel
```

### 4. Écran Nextion (Serial1)
```
Nextion:
  +5V → 5V (alimentation externe recommandée > 500mA)
  GND → GND
  TX → D0 (RX1) via level shifter 5V→3.3V
  RX → D1 (TX1) via level shifter 3.3V→5V
```

### 5. Potentiomètres analogiques
```
Potentiomètre 10kΩ:
  Pin 1 → 3.3V (PAS 5V!)
  Pin 2 (wiper) → A0/A1/A2
  Pin 3 → GND
```

---

## 🛠️ Activation de cette configuration

Pour utiliser cette configuration de pins :

1. Ouvrir [include/rotator_hardware.h](include/rotator_hardware.h)

2. Définir le hardware :
```cpp
#define HARDWARE_GIGA_R1   // Nouvelle définition
```

3. Dans [include/rotator_pins.h](include/rotator_pins.h), ajouter :
```cpp
#ifdef HARDWARE_GIGA_R1
  #include "rotator_pins_giga_r1.h"
#endif
```

4. Compiler avec l'environment `giga_r1_m7` :
```bash
pio run -e giga_r1_m7
```

---

## 📚 Références

- [Arduino Giga R1 WiFi](https://docs.arduino.cc/hardware/giga-r1-wifi/)
- [Giga R1 Pinout PDF](https://docs.arduino.cc/resources/datasheets/ABX00063-datasheet.pdf)
- [STM32H747XI Datasheet](https://www.st.com/resource/en/datasheet/stm32h747xi.pdf)
- [K3NG Rotator Wiki](https://github.com/k3ng/k3ng_rotator_controller/wiki)

---

**Version** : 1.0
**Dernière mise à jour** : Octobre 2025
**Auteur** : ON7KGK Michael
**Branche Git** : GigaR1_Test
