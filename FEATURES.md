# 🎛️ Configuration des Features - Nano R4 Rotator

Ce document explique comment activer/désactiver les fonctionnalités optionnelles, **à la manière de K3NG** avec des `#define`.

## 📋 Configuration SHACK Unit

Toutes les features se configurent dans : **[src/shack_unit/include/config.h](src/shack_unit/include/config.h)**

### 🔧 Section FEATURE ACTIVATION

```cpp
// ===================================================================
// FEATURE ACTIVATION (comment/uncomment to enable/disable)
// ===================================================================
// Like K3NG style: comment out features you don't have/want

#define FEATURE_MCP23017_BUTTONS      // MCP23017 I/O expander for buttons
// #define FEATURE_DIRECT_GPIO_BUTTONS   // Use Nano R4 GPIO pins for buttons (not implemented yet)
// #define FEATURE_ETHERNET              // Ethernet interface (not implemented yet)
// #define FEATURE_NEXTION_DISPLAY       // Nextion display on Serial2 (not implemented yet)
```

### ✅ Activer une feature

**Enlève les `//` devant le `#define`** :

```cpp
#define FEATURE_MCP23017_BUTTONS      // Feature ACTIVÉE ✅
```

### ❌ Désactiver une feature

**Ajoute `//` devant le `#define`** :

```cpp
// #define FEATURE_MCP23017_BUTTONS   // Feature DÉSACTIVÉE ❌
```

---

## 🎯 Features disponibles

### 1️⃣ **FEATURE_MCP23017_BUTTONS** (Phase 1 - Implémenté)

**Quoi :** Utilise le MCP23017 I/O expander pour les boutons (CW, CCW, UP, DOWN, STOP, etc.)

**Matériel nécessaire :**
- MCP23017 I/O expander (I2C address 0x20)
- Boutons connectés aux pins du MCP23017
- Pull-up interne activé

**Quand désactiver :**
- ❌ Tu n'as pas encore branché le MCP23017
- ❌ Tu veux tester le système sans boutons
- ❌ Tu préfères utiliser les GPIO du Nano R4 directement

**Impact si désactivé :**
- ✅ Pas d'erreur "MCP23017 ERROR!" au boot
- ✅ Le LCD et RS485 fonctionnent normalement
- ❌ Impossible d'envoyer des commandes manuelles (pas de boutons)
- ℹ️ Tu peux quand même envoyer des commandes via le moniteur série

**Configuration :**
```cpp
// Désactiver si pas de MCP23017 branché
// #define FEATURE_MCP23017_BUTTONS
```

---

### 2️⃣ **FEATURE_DIRECT_GPIO_BUTTONS** (Phase 2 - Pas encore implémenté)

**Quoi :** Utilise les GPIO du Nano R4 directement pour les boutons

**Matériel nécessaire :**
- Boutons connectés directement aux pins GPIO du Nano R4
- Résistances pull-up externes ou pull-up interne

**Avantages :**
- Pas besoin de MCP23017 (économie ~2€)
- Moins de composants
- Plus simple à câbler

**Quand l'implémenter :**
- Si tu veux un système plus simple
- Si tu as peu de boutons (< 10)

**État actuel :** ⏳ Non implémenté (prévu Phase 2)

---

### 3️⃣ **FEATURE_ETHERNET** (Phase 3 - Pas encore implémenté)

**Quoi :** Interface Ethernet pour contrôle distant (Hamlib, web interface)

**Matériel nécessaire :**
- Module Ethernet W5100 ou W5500 (SPI)

**Fonctionnalités prévues :**
- Hamlib rotctld compatible
- Interface web pour contrôle via navigateur
- Telnet interface
- API REST

**État actuel :** ⏳ Non implémenté (prévu Phase 3)

---

### 4️⃣ **FEATURE_NEXTION_DISPLAY** (Phase 2 - Pas encore implémenté)

**Quoi :** Écran tactile Nextion pour interface graphique avancée

**Matériel nécessaire :**
- Écran Nextion (2.4" à 7")
- Connexion Serial2 (pins A4/A5)

**Fonctionnalités prévues :**
- Interface graphique tactile
- Graphiques de position
- Configuration via écran
- Presets de positions
- Moon/Sun tracking visuel

**État actuel :** ⏳ Non implémenté (prévu Phase 2)

---

## 📋 Configuration ANTENNA Unit

Toutes les features se configurent dans : **[src/antenna_unit/include/config.h](src/antenna_unit/include/config.h)**

### Features côté ANTENNA (à venir) :

- `FEATURE_HH12_ENCODERS` - Encodeurs absolus HH12/AS5045 (déjà implémenté, toujours actif)
- `FEATURE_RELAY_MOTORS` - Moteurs via relais (déjà implémenté, toujours actif)
- `FEATURE_STEPPER_MOTORS` - Moteurs pas-à-pas (à implémenter)
- `FEATURE_LIMIT_SWITCHES` - Fins de course électroniques (à implémenter)
- `FEATURE_GPS` - GPS pour moon/sun tracking (à implémenter)
- `FEATURE_WIND_SENSOR` - Anémomètre pour sécurité vent (à implémenter)

---

## 🔄 Workflow de test progressif

### Phase 1 : Test de base (actuel)
```cpp
// SHACK: Désactiver MCP23017 pour tester LCD + RS485 seulement
// #define FEATURE_MCP23017_BUTTONS
```

**Test :**
- ✅ LCD affiche splash screen
- ✅ RS485 communique avec ANTENNA
- ✅ Position s'affiche sur LCD
- ❌ Pas de contrôle manuel (normal, pas de boutons)

### Phase 2 : Ajouter les boutons
```cpp
// SHACK: Activer MCP23017
#define FEATURE_MCP23017_BUTTONS
```

**Test :**
- ✅ LCD + RS485 fonctionnent
- ✅ Boutons CW/CCW envoient des commandes
- ✅ Moteurs réagissent aux boutons
- ✅ Contrôle manuel complet

### Phase 3 : Fonctionnalités avancées
```cpp
// SHACK: Activer Nextion ou Ethernet
#define FEATURE_NEXTION_DISPLAY
// #define FEATURE_ETHERNET
```

---

## 🐛 Dépannage

### "MCP23017 ERROR!" au boot

**Cause :** Le code cherche le MCP23017 mais ne le trouve pas.

**Solutions :**
1. ✅ **Désactiver la feature** (si MCP23017 pas branché) :
   ```cpp
   // #define FEATURE_MCP23017_BUTTONS
   ```

2. ✅ **Vérifier le câblage I2C** :
   - SDA → SDA
   - SCL → SCL
   - VCC → 5V
   - GND → GND

3. ✅ **Vérifier l'adresse I2C** :
   - Par défaut : `0x20`
   - Scanner I2C pour trouver l'adresse réelle

4. ✅ **Recompiler et uploader** après modification du config.h

### Comment scanner l'adresse I2C du MCP23017 ?

Utilise ce sketch Arduino :
```cpp
#include <Wire.h>

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Serial.println("I2C Scanner");
}

void loop() {
    for (byte addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.print("Found device at 0x");
            Serial.println(addr, HEX);
        }
    }
    delay(5000);
}
```

---

## 📚 Référence K3NG

Ce système s'inspire du style K3NG Rotator Controller où toutes les features sont activables/désactivables avec des `#define` dans le fichier de configuration.

**Avantages de cette approche :**
- ✅ Pas besoin de modifier le code principal
- ✅ Configuration centralisée dans config.h
- ✅ Facile à comprendre (comme K3NG)
- ✅ Code compilé seulement ce qui est nécessaire (économie RAM/Flash)

---

**73 de ON7KGK** 📻
