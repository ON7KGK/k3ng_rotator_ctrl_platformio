# RS485 Features Split - Phase 4 Planning

**Objectif :** Définir clairement quelles features sont actives sur chaque unité

**Architecture :**
- **ANTENNA Unit (Master)** : Contrôle physique du rotateur (capteurs + moteurs)
- **SHACK Unit (Remote)** : Interface utilisateur (display + commandes)

---

## 🔧 ANTENNA Unit (Master)

### ✅ Features ACTIVES

#### Contrôle Moteurs
- [X] `FEATURE_ELEVATION_CONTROL` - Contrôle élévation
- [X] Pins moteurs AZ: `rotate_cw` (A0), `rotate_ccw` (A1)
- [X] Pins moteurs EL: `rotate_up` (A2), `rotate_down` (A3)
- [X] Optionnel: PWM speed control
- [X] Optionnel: Brake control

#### Capteurs de Position
- [X] `FEATURE_AZ_POSITION_HH12_AS5045_SSI` - Encodeur absolu SSI Azimut
- [X] `FEATURE_EL_POSITION_HH12_AS5045_SSI` - Encodeur absolu SSI Élévation
- [X] Alternative: Potentiomètres (`rotator_analog_az`, `rotator_analog_el`)

#### GPS & Tracking
- [X] `FEATURE_GPS` - GPS sur Serial2 (pins A4/A5 sur Nano R4)
- [X] `FEATURE_CLOCK` - Horloge temps réel
- [X] `FEATURE_MOON_TRACKING` - Suivi Lune
- [X] `FEATURE_SUN_TRACKING` - Suivi Soleil
- [X] `FEATURE_SATELLITE_TRACKING` - Suivi satellites

#### Sécurité
- [X] End stops / fins de course
- [X] Rotation stall detection
- [X] Overlap detection (> 360°)

#### Communication
- [X] `FEATURE_RS485_MASTER` - Communication RS485 Master (NEW)
- [X] Serial1 dédié RS485 (TX1/RX1 + DE/RE pins 8/9)

### ❌ Features DÉSACTIVÉES

### note de ON7KGK pour Claude :" Dans cette section, je coche avec un X lorsque je souhaite que la feature ne soit pas possible sur le master" ###

#### Interface Utilisateur (déportée vers Remote)
- [X] `FEATURE_NEXTION_DISPLAY` - Display Nextion
- [X] `FEATURE_4_BIT_LCD_DISPLAY` - LCD 4 bits
- [X] `FEATURE_ADAFRUIT_I2C_LCD` - LCD I2C
- [X] Tous autres displays

#### Boutons Manuels (déportés vers Remote)
- [ ] `button_cw` - Bouton rotation CW manuel
- [ ] `button_ccw` - Bouton rotation CCW manuel
- [ ] `button_up` - Bouton élévation UP manuel
- [ ] `button_down` - Bouton élévation DOWN manuel
- [ ] `button_stop` - Bouton STOP
### note de ON7KGK pour Claude :"j'ai laissé actif la possibilité de connecter ces boutons au master car à 100m de distance du shack, si on souhaite prendre le contrôle manuellement des antennes, il faut laisser ces boutons."

#### Encodeurs Preset (déportés vers Remote)
- [X] `FEATURE_AZ_PRESET_ENCODER` - Encodeur preset azimut
- [X] `FEATURE_EL_PRESET_ENCODER` - Encodeur preset élévation
- [X] `az_preset_pot` - Potentiomètre preset
- [X] `preset_start_button` - Bouton start preset

#### Protocoles série (remplacés par RS485)
- [X] `FEATURE_YAESU_EMULATION` - Émulation Yaesu GS-232
- [X] `FEATURE_EASYCOM_EMULATION` - Émulation Easycom
- [X] `FEATURE_DCU_1_EMULATION` - Émulation DCU-1

#### Ethernet (optionnel, peut rester si besoin)
- [ ] `FEATURE_ETHERNET` - À décider : garder sur Master ou Remote ?
### note de ON7KGK pour Claude :"Je laisse la possibilité du côté du master d'activer ou non la feature ETHERNET, c'est normal d'utiliser cette fonctionnalité"
---

## 🏠 SHACK Unit (Remote)

### ✅ Features ACTIVES

#### Interface Utilisateur
- [X] `FEATURE_NEXTION_DISPLAY` - Display Nextion sur Serial1 **OU** RS485
  - **CONFLIT POTENTIEL :** Serial1 utilisé pour RS485 ET Nextion !
  - **Solutions possibles :**
    1. Nextion sur Software Serial
    2. LCD I2C à la place de Nextion
    3. Partage Serial1 (complexe)
- [ ] `FEATURE_4_BIT_LCD_DISPLAY` - Alternative LCD 4 bits
- [X] `FEATURE_ADAFRUIT_I2C_LCD` - Alternative LCD I2C (RECOMMANDÉ)
### note de ON7KGK pour Claude :"pour le port du Nextion, tu peux utiliser le Serial2 sur A4 et A5 car la feature GPS n'est pas activée côté shack (remote)"


#### Commandes Manuelles
- [X] `button_cw` - Bouton rotation CW
- [X] `button_ccw` - Bouton rotation CCW
- [X] `button_up` - Bouton élévation UP
- [X] `button_down` - Bouton élévation DOWN
- [X] `button_stop` - Bouton STOP d'urgence

#### Encodeurs Preset
- [X] `FEATURE_AZ_PRESET_ENCODER` - Encodeur preset azimut
- [X] `FEATURE_EL_PRESET_ENCODER` - Encodeur preset élévation
- [X] `az_rotary_preset_pin1/2` - Pins encodeur AZ
- [X] `el_rotary_preset_pin1/2` - Pins encodeur EL
- [X] `preset_start_button` - Bouton démarrage preset


### Communication
- [X] `FEATURE_RS485_REMOTE` - Communication RS485 Remote (NEW)
- [X] Serial1 dédié RS485 (TX1/RX1 + DE/RE pins 8/9)
- [ ] Optionnel: Ethernet pour contrôle réseau

### note de ON7KGK pour Claude :"la feature ETHERNET n'est possible que pour le master".

#### Protocoles série (pour contrôle externe)
- [X] `FEATURE_YAESU_EMULATION` - Sur Serial (USB) pour PC
- [X] `FEATURE_EASYCOM_EMULATION` - Alternative Easycom
- [ ] Commandes relayées vers Master via RS485

### note de ON7KGK pour Claude :"le programme utilisé avec le Serial USB est PSTRotator et il n'a surtout pas besoin que les commandes relayées vers le master en RS485 soit présente dans le Serial USB."

### ❌ Features DÉSACTIVÉES

#### Contrôle Moteurs (sur Master)
- [X] Pins moteurs AZ: `rotate_cw`, `rotate_ccw`
- [X] Pins moteurs EL: `rotate_up`, `rotate_down`
- [X] PWM speed control
- [X] Brake control

#### Capteurs de Position (sur Master)
- [X] `FEATURE_AZ_POSITION_HH12_AS5045_SSI`
- [X] `FEATURE_EL_POSITION_HH12_AS5045_SSI`
- [X] `rotator_analog_az`, `rotator_analog_el`

#### GPS & Tracking (sur Master)
- [X] `FEATURE_GPS` - GPS déporté sur Master
- [X] Données GPS reçues via RS485

#### Sécurité Matérielle (sur Master)
- [X] End stops
- [X] Stall detection
- [X] Overlap detection

---

## ⚠️ CONFLITS POTENTIELS À RÉSOUDRE

### 1. Serial1 sur Remote
**Problème :** Serial1 utilisé pour RS485 ET Nextion.    -> ON7KGK dit qu'il faut utiliser sur le Remote le Serial2 pour le Nextion (A4 et A5) car pas de GPS sur le Remote.
**Solutions :**
- **Option A :** LCD I2C à la place de Nextion (RECOMMANDÉ)
- **Option B :** Nextion sur Software Serial (pins libres)
- **Option C :** Partage temporel Serial1 (complexe, non recommandé)

### 2. Ethernet
**Problème :** Où placer l'Ethernet ?  uniquement sur le master mais il faut laisser la possibilité à l'utilisateur de l'activer ou pas.
**Options :**
- **Option A :** Sur Remote (contrôle réseau depuis PC)
- **Option B :** Sur Master (contrôle direct moteurs via réseau)
- **Option C :** Sur les deux (complexe)

### 3. Pins disponibles sur Nano R4
**Nano R4 Minima pins :**
- Digital: D0-D13 (D0/D1 = Serial1) 
- Analog: A0-A5 (A4/A5 = Serial2 GPS sur Master) et Nextion sur Remote
- A6/A7 disponibles mais sans pullup

**Allocation Master :**
- A0: rotate_cw
- A1: rotate_ccw
- A2: rotate_up
- A3: rotate_down
- A4/A5: GPS Serial2
- D0/D1: RS485 Serial1
- D8/D9: RS485 DE/RE
- D10/D11/D12/D13 pour la communication SPI : utilisées si la feature ETHERNET est active 
- Reste: Encodeurs SSI, end stops

**Allocation Remote :**
- D0/D1: RS485 Serial1
- D8/D9: RS485 DE/RE
- Reste: Boutons, encodeurs preset, display I2C

---

## 📝 DÉCISIONS À PRENDRE

### Questions pour l'utilisateur :

1. **Display sur Remote :**
   - [X] Nextion (conflit Serial1) ? -> Nextion sur Serial2 (A4 et A5) puisque pas GPS.
   - [X] LCD I2C (recommandé) ? On laisse la feature possible pour les utilisateurs qui n'ont pas de Nextion.
   - [ ] Autre ?

2. **Ethernet :**
   - [X] Sur Remote ?   non je ne veux pas que ce soit possible de l'activer sur le Remote.
   - [X] Sur Master ?  Uniquement sur Master
   - [X] Pas d'Ethernet ? au choix, on doit pouvoir l'activer ou pas mais uniquement sur le Master, cette feature est désactivée sur le remote.

3. **Boutons physiques sur Remote :**
   - [X] Combien de boutons prévus ? (CW, CCW, UP, DOWN, STOP, autres ?) On pourrait prévoir un bouton pour le traking de la lune et un bouton pour le soleil.
   - [ ] Pins disponibles ?

4. **Encodeurs preset sur Remote :**
   - [X] 1 encodeur (AZ seulement) ?
   - [X] 2 encodeurs (AZ + EL) ?
   - [X] Pas d'encodeur ? on laisse le choix à l'utilisateur d'activer ou non les preset qu'il a besoin.

5. **GPS sur Master :**
   - [X] GPS présent ?  oui
   - [X] Sur Serial2 (A4/A5) ?  oui car le Serial1 est utilisé pour le RS-485

6. **Capteurs position sur Master :**
   - [X] Encodeurs SSI HH-12 ?  on laisse le choix d'activer ou non suivant que l'utilisateur dispose du matériel HH-12 ou non.
   - [X] Potentiomètres ? on laisse le choix d'activer ou non suivant que l'utilisateur veut utiliser des potentiomètres ou non.
   - [ ] Autre ?  

---

## 🎯 PROCHAINES ÉTAPES Phase 4

1. Valider les choix ci-dessus avec l'utilisateur
2. Créer `rotator_features_master.h` et `rotator_features_remote.h`
3. Créer `rotator_pins_master.h` et `rotator_pins_remote.h`
4. Modifier `platformio.ini` pour 2 environnements distincts
5. Intégrer RS485 dans la boucle principale K3NG
6. Tester avec hardware réel

---

**À compléter par l'utilisateur : cochez [x] les features désirées et répondez aux questions.**
