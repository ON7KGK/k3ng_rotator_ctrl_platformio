# RS485 Pins Allocation - Arduino Nano R4 Minima

**Hardware :** 2x Arduino Nano R4 Minima (Renesas RA4M1)
**Pins disponibles :** D0-D13, A0-A7 (attention: A6/A7 sans pullup interne)

---

## 📌 Pins Réservées RS485 (identiques sur les 2 unités)

| Pin | Fonction | Description |
|-----|----------|-------------|
| D0 (RX1) | RS485 RX | Réception Serial1 |
| D1 (TX1) | RS485 TX | Transmission Serial1 |
| D8 | RS485 DE | Driver Enable |
| D9 | RS485 RE | Receiver Enable |

**⚠️ Ces pins sont INTERDITES pour autres usages !**

---

## 🔧 ANTENNA Unit (Master) - Allocation Pins

### Pins Moteurs (4 pins obligatoires)
| Pin | Fonction | Description | Status |
|-----|----------|-------------|--------|
| A0 | `rotate_cw` | Rotation Azimut CW | ✅ Assigné |
| A1 | `rotate_ccw` | Rotation Azimut CCW | ✅ Assigné |
| A2 | `rotate_up` | Rotation Élévation UP | ✅ Assigné |
| A3 | `rotate_down` | Rotation Élévation DOWN | ✅ Assigné |

### Pins GPS (Serial2 matériel)
| Pin | Fonction | Description | Status |
|-----|----------|-------------|--------|
| A4 (TX2) | GPS TX | Transmission GPS | ✅ Assigné |
| A5 (RX2) | GPS RX | Réception GPS | ✅ Assigné |

**Note :** GPS actif sur Master uniquement

### Pins Encodeurs SSI (HH-12 AS5045) - Optionnel

**Azimut SSI :**
| Pin | Fonction | Suggestion | Status |
|-----|----------|------------|--------|
| D2 | AZ_SSI_CLK | Clock SSI | 💡 Suggéré |
| D3 | AZ_SSI_CS | Chip Select | 💡 Suggéré |
| D4 | AZ_SSI_DO | Data Out | 💡 Suggéré |

**Élévation SSI :**
| Pin | Fonction | Suggestion | Status |
|-----|----------|------------|--------|
| D5 | EL_SSI_CLK | Clock SSI | 💡 Suggéré |
| D6 | EL_SSI_CS | Chip Select | 💡 Suggéré |
| D7 | EL_SSI_DO | Data Out | 💡 Suggéré |

**⚠️ Feature optionnelle :** Activable uniquement si matériel HH-12 présent

### Pins Fins de Course - Optionnel
| Pin | Fonction | Suggestion | Status |
|-----|----------|------------|--------|
| A6 | AZ_LIMIT_CW | Fin course AZ CW | 💡 Suggéré |
| A7 | AZ_LIMIT_CCW | Fin course AZ CCW | 💡 Suggéré |
| - | EL_LIMIT_UP | Fin course EL UP | ⚠️ À définir |
| - | EL_LIMIT_DOWN | Fin course EL DOWN | ⚠️ À définir |

**Note :** A6/A7 sans pullup interne (nécessite pullup externe)

### Pins Ethernet (SPI) - Optionnel
| Pin | Fonction | Description | Status |
|-----|----------|-------------|--------|
| D10 | ETH_CS | Chip Select | ✅ Si ETHERNET actif |
| D11 | ETH_MOSI | SPI MOSI | ✅ Si ETHERNET actif |
| D12 | ETH_MISO | SPI MISO | ✅ Si ETHERNET actif |
| D13 | ETH_SCK | SPI Clock | ✅ Si ETHERNET actif |

**⚠️ Feature optionnelle :** Uniquement sur Master (non disponible sur Remote)

### Pins Boutons Locaux - Optionnel
| Pin | Fonction | Description | Status |
|-----|----------|-------------|--------|
| - | `button_cw` | Rotation CW local | 💡 À définir |
| - | `button_ccw` | Rotation CCW local | 💡 À définir |
| - | `button_up` | Élévation UP local | 💡 À définir |
| - | `button_down` | Élévation DOWN local | 💡 À définir |
| - | `button_stop` | STOP urgence local | 💡 À définir |

**Note utilisateur :** Boutons locaux utiles pour contrôle manuel à 100m du shack

### Résumé Master (utilisation maximale)

**Pins utilisées :**
- RS485: D0, D1, D8, D9 (4 pins)
- Moteurs: A0, A1, A2, A3 (4 pins)
- GPS: A4, A5 (2 pins)
- Encodeurs SSI: D2-D7 (6 pins) - optionnel
- Ethernet: D10-D13 (4 pins) - optionnel
- Fins course: A6, A7 + ? (2+ pins) - optionnel

**Total :** 10 pins obligatoires + jusqu'à 12 pins optionnels

---

## 🏠 SHACK Unit (Remote) - Allocation Pins

### Pins Display

#### Option A : Nextion Display (Serial2)
| Pin | Fonction | Description | Status |
|-----|----------|-------------|--------|
| A4 (TX2) | NEXTION_TX | TX vers Nextion | ✅ RECOMMANDÉ |
| A5 (RX2) | NEXTION_RX | RX depuis Nextion | ✅ RECOMMANDÉ |

**✅ Avantage :**
- Serial2 matériel (A4/A5) disponible car PAS de GPS sur Remote
- Performant et fiable
- Seulement 2 pins utilisées

#### Option B : LCD I2C (alternative)
| Pin | Fonction | Description | Status |
|-----|----------|-------------|--------|
| A4 (SDA) | I2C Data | Données I2C | ✅ Alternative |
| A5 (SCL) | I2C Clock | Horloge I2C | ✅ Alternative |

**✅ Avantage :** Pour utilisateurs sans Nextion

#### Option C : LCD 4 bits (non recommandé)
| Pin | Fonction | Description | Status |
|-----|----------|-------------|--------|
| D10 | LCD_RS | Register Select | ⚠️ Non recommandé |
| D11 | LCD_EN | Enable | ⚠️ Non recommandé |
| D12 | LCD_D4 | Data 4 | ⚠️ Non recommandé |
| D13 | LCD_D5 | Data 5 | ⚠️ Non recommandé |
| - | LCD_D6 | Data 6 | ⚠️ Non recommandé |
| - | LCD_D7 | Data 7 | ⚠️ Non recommandé |

**Inconvénient :** 6 pins utilisées, moins de pins disponibles

### Pins Boutons Manuels
| Pin | Fonction | Description | Status |
|-----|----------|-------------|--------|
| D2 | `button_cw` | Rotation CW | ✅ Suggéré |
| D3 | `button_ccw` | Rotation CCW | ✅ Suggéré |
| D4 | `button_up` | Élévation UP | ✅ Suggéré |
| D5 | `button_down` | Élévation DOWN | ✅ Suggéré |
| D6 | `button_stop` | STOP urgence | ✅ Obligatoire |
| D7 | `button_moon_track` | Tracking Lune | 💡 Optionnel |
| A0 | `button_sun_track` | Tracking Soleil | 💡 Optionnel |

**Note :** Boutons normalement ouverts vers GND (pullup interne)

### Pins Encodeurs Preset - Optionnel

#### Encodeur Azimut
| Pin | Fonction | Description | Status |
|-----|----------|-------------|--------|
| D10 | `az_rotary_preset_pin1` | Encodeur AZ A | ✅ Si activé |
| D11 | `az_rotary_preset_pin2` | Encodeur AZ B | ✅ Si activé |

#### Encodeur Élévation
| Pin | Fonction | Description | Status |
|-----|----------|-------------|--------|
| D12 | `el_rotary_preset_pin1` | Encodeur EL A | ✅ Si activé |
| D13 | `el_rotary_preset_pin2` | Encodeur EL B | ✅ Si activé |

#### Bouton Start Preset
| Pin | Fonction | Description | Status |
|-----|----------|-------------|--------|
| A1 | `preset_start_button` | Démarrage preset | ✅ Si encodeurs actifs |

**⚠️ Feature optionnelle :** Au choix de l'utilisateur

### ❌ Pins NON disponibles sur Remote

**Ethernet :** NON SUPPORTÉ sur Remote (uniquement sur Master)

### Résumé Remote (utilisation recommandée)

**Configuration RECOMMANDÉE : Nextion + Boutons + Encodeurs Preset**

**Pins utilisées :**
- RS485: D0, D1, D8, D9 (4 pins)
- Display Nextion: A4, A5 (2 pins)
- Boutons: D2-D7, A0 (7 pins)
- Encodeurs preset: D10-D13, A1 (5 pins)

**Total :** 18 pins utilisées / 20 pins disponibles
**Disponible :** A2, A3 (2 pins libres)

---

## 📊 Tableau Récapitulatif Final

### ANTENNA (Master) - Configuration Complète

| Pin | Fonction | Type | Feature |
|-----|----------|------|---------|
| D0 | RS485 RX | Serial1 | RS485 Master |
| D1 | RS485 TX | Serial1 | RS485 Master |
| D8 | RS485 DE | Digital Out | RS485 Master |
| D9 | RS485 RE | Digital Out | RS485 Master |
| A0 | Moteur AZ CW | Digital Out | Contrôle Moteurs |
| A1 | Moteur AZ CCW | Digital Out | Contrôle Moteurs |
| A2 | Moteur EL UP | Digital Out | Contrôle Moteurs |
| A3 | Moteur EL DOWN | Digital Out | Contrôle Moteurs |
| A4 | GPS TX | Serial2 | GPS Tracking |
| A5 | GPS RX | Serial2 | GPS Tracking |
| D2 | AZ SSI CLK | Digital Out | Encodeur SSI (opt.) |
| D3 | AZ SSI CS | Digital Out | Encodeur SSI (opt.) |
| D4 | AZ SSI DO | Digital In | Encodeur SSI (opt.) |
| D5 | EL SSI CLK | Digital Out | Encodeur SSI (opt.) |
| D6 | EL SSI CS | Digital Out | Encodeur SSI (opt.) |
| D7 | EL SSI DO | Digital In | Encodeur SSI (opt.) |
| D10 | ETH CS | SPI | Ethernet (opt.) |
| D11 | ETH MOSI | SPI | Ethernet (opt.) |
| D12 | ETH MISO | SPI | Ethernet (opt.) |
| D13 | ETH SCK | SPI | Ethernet (opt.) |
| A6 | AZ Limit CW | Digital In | Fins course (opt.) |
| A7 | AZ Limit CCW | Digital In | Fins course (opt.) |

**⚠️ Note :** Encodeurs SSI et Ethernet ne peuvent pas être actifs simultanément (conflit D2-D7 vs D10-D13)

### SHACK (Remote) - Configuration RECOMMANDÉE

| Pin | Fonction | Type | Feature |
|-----|----------|------|---------|
| D0 | RS485 RX | Serial1 | RS485 Remote |
| D1 | RS485 TX | Serial1 | RS485 Remote |
| D8 | RS485 DE | Digital Out | RS485 Remote |
| D9 | RS485 RE | Digital Out | RS485 Remote |
| A4 | Nextion TX | Serial2 | Display Nextion |
| A5 | Nextion RX | Serial2 | Display Nextion |
| D2 | Button CW | Digital In | Contrôle Manuel |
| D3 | Button CCW | Digital In | Contrôle Manuel |
| D4 | Button UP | Digital In | Contrôle Manuel |
| D5 | Button DOWN | Digital In | Contrôle Manuel |
| D6 | Button STOP | Digital In | Sécurité |
| D7 | Button Moon | Digital In | Tracking (opt.) |
| A0 | Button Sun | Digital In | Tracking (opt.) |
| D10 | Encodeur AZ A | Digital In | Preset (opt.) |
| D11 | Encodeur AZ B | Digital In | Preset (opt.) |
| D12 | Encodeur EL A | Digital In | Preset (opt.) |
| D13 | Encodeur EL B | Digital In | Preset (opt.) |
| A1 | Preset Start | Digital In | Preset (opt.) |
| A2 | **LIBRE** | - | - |
| A3 | **LIBRE** | - | - |

---

## ✅ DÉCISIONS VALIDÉES

### ANTENNA (Master)
- ✅ GPS sur Serial2 (A4/A5)
- ✅ Moteurs sur A0-A3
- ✅ Encodeurs SSI optionnels (HH-12)
- ✅ Potentiomètres optionnels (alternative SSI)
- ✅ Ethernet optionnel (D10-D13)
- ✅ Boutons locaux possibles (pins à définir selon config)
- ✅ Fins de course optionnels

### SHACK (Remote)
- ✅ Display Nextion sur Serial2 (A4/A5) - RECOMMANDÉ
- ✅ Alternative LCD I2C possible (A4/A5)
- ✅ 5 boutons de base (D2-D6)
- ✅ 2 boutons tracking optionnels (D7, A0)
- ✅ Encodeurs preset optionnels (D10-D13, A1)
- ❌ PAS de GPS (sur Master uniquement)
- ❌ PAS d'Ethernet (sur Master uniquement)
- ❌ PAS de contrôle moteurs direct (sur Master uniquement)

### Communication
- ✅ Protocole Yaesu/Easycom sur Serial USB (Remote → PC)
- ✅ Commandes PSTRotator sur USB uniquement
- ✅ PAS de relay USB → RS485 (communication séparée)

---

## 🎯 PROCHAINES ACTIONS

1. ✅ Documentation pins complète
2. ⏭️ Créer `rotator_pins_master.h` avec cette allocation
3. ⏭️ Créer `rotator_pins_remote.h` avec cette allocation
4. ⏭️ Adapter `rotator_features_master.h` et `rotator_features_remote.h`
5. ⏭️ Modifier `platformio.ini` pour les 2 environnements
6. ⏭️ Intégrer RS485 dans loop principale
7. ⏭️ Tests hardware

---

**Document mis à jour selon les choix utilisateur dans RS485_FEATURES_SPLIT.md**
