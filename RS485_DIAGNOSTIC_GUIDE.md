# Guide de Diagnostic RS485

## Problème: Les valeurs du potentiomètre A1 (Master) ne s'affichent pas sur le Remote

### Étape 1: Vérifier que le debug RS485 est activé

Le debug RS485 est activé dans `rs485_config.h`:
```cpp
#define RS485_DEBUG_ENABLED     // Ligne 86
```

Les messages de debug s'affichent sur le **port USB Serial** (pas sur RS485).

---

### Étape 2: Monitorer les deux unités simultanément

#### Terminal 1 - Unité ANTENNA (Master)
```bash
# Trouver le port USB de l'unité Antenna
ls /dev/cu.usb*

# Monitorer l'Antenna (remplacer par votre port)
./monitor_debug.sh /dev/cu.usbmodem1101
```

**Ce que vous devriez voir:**
- Au démarrage: `RS485 Master initialized`
- Toutes les 100ms: Messages de broadcast de position
  - `Broadcasting AZ: XXX.XX EL: YY.YY`
  - `TX Frame: AA 02 01 ...`

#### Terminal 2 - Unité SHACK (Remote)
```bash
# Trouver le port USB de l'unité Shack
ls /dev/cu.usb*

# Monitorer le Shack (remplacer par votre port)
./monitor_debug.sh /dev/cu.usbmodem2201
```

**Ce que vous devriez voir:**
- Au démarrage: `RS485 Remote initialized`
- Toutes les 100ms: Messages de réception
  - `RX Position: AZ: XXX.XX EL: YY.YY`
  - `RX Frame: AA 02 01 ...`

---

### Étape 3: Vérifier le potentiomètre A1

#### Sur l'unité ANTENNA, ajoutez un debug dans le code:

Ouvrez `src/k3ng_rotator_controller.cpp` et cherchez la fonction `read_azimuth()`.

Ajoutez:
```cpp
#ifdef FEATURE_AZ_POSITION_POTENTIOMETER
  int raw_value = analogRead(rotator_analog_az);
  Serial.print("AZ Pot A1 raw: ");
  Serial.println(raw_value);
#endif
```

Recompilez et uploadez. Vous devriez voir:
```
AZ Pot A1 raw: 512
```

La valeur devrait changer quand vous tournez le potentiomètre (0-1023).

---

### Étape 4: Vérifier le câblage RS485

#### Connexions Master (Antenna):
- **D0 (RX1)** → RO du module RS485
- **D1 (TX1)** → DI du module RS485
- **D8** → DE du module RS485
- **D9** → RE du module RS485
- **A** et **B** → Câble torsadé vers Remote

#### Connexions Remote (Shack):
- **D0 (RX1)** → RO du module RS485
- **D1 (TX1)** → DI du module RS485
- **D8** → DE du module RS485
- **D9** → RE du module RS485
- **A** et **B** → Câble torsadé vers Master

⚠️ **IMPORTANT**: A et B doivent être croisés entre les deux modules!
- Master A → Remote A
- Master B → Remote B

---

### Étape 5: Test LED de diagnostic

#### Sur les modules RS485, vous devriez voir:
- **LED TX** clignote sur Master (broadcast toutes les 100ms)
- **LED RX** clignote sur Remote (réception toutes les 100ms)

Si aucune LED ne clignote:
1. Vérifier l'alimentation 5V des modules RS485
2. Vérifier les connexions DE/RE (D8/D9)
3. Vérifier que Serial1 n'est pas utilisé ailleurs

---

### Étape 6: Test de bouclage (Loopback)

#### Test Master seul:
Débranchez le câble A-B et court-circuitez **DI** et **RO** sur le module Master.

Vous devriez voir les trames envoyées être reçues (écho).

#### Test Remote seul:
Même chose sur le Remote.

---

### Étape 7: Vérifier les paramètres RS485

Dans `rs485_config.h`:
```cpp
#define RS485_BAUD_RATE     19200   // Ligne 28
#define RS485_SERIAL        Serial1  // Ligne 21
```

Dans `rotator_settings_master.h` ET `rotator_settings_remote.h`:
```cpp
#define CONTROL_PORT_MAPPED_TO &Serial1
#define CONTROL_PORT_BAUD_RATE 9600
```

⚠️ **ATTENTION**: Il y a un conflit ici!
- rs485_config.h dit 19200 bauds
- rotator_settings dit 9600 bauds

**Solution**: Les deux doivent être synchronisés!

---

### Étape 8: Mesures avec multimètre

#### Vérifier les niveaux logiques:
- **DE (D8)**: Doit passer à HIGH pendant TX
- **RE (D9)**: Doit être LOW pendant RX
- **A-B**: Tension différentielle ~2-5V pendant transmission

#### Test de continuité du câble:
- Continuité entre Master-A et Remote-A
- Continuité entre Master-B et Remote-B
- PAS de court-circuit entre A et B!

---

### Problèmes courants

#### 1. Aucun message de debug
→ Vérifier que `RS485_DEBUG_ENABLED` est défini
→ Vérifier que le USB Serial est à 9600 bauds
→ Vérifier que le port COM est le bon

#### 2. Messages Master mais pas de réception Remote
→ Vérifier le câblage A-B
→ Vérifier que les bauds sont identiques (19200 ou 9600)
→ Vérifier la polarité A/B (peut-être inversée)

#### 3. Potentiomètre A1 lit toujours 0
→ Vérifier que le pot est alimenté (3.3V ou 5V)
→ Vérifier la connection du curseur à A1
→ Mesurer la tension sur A1 avec multimètre (0-3.3V)

#### 4. Display I2C ne montre rien
→ Vérifier l'adresse I2C (0x3F ou 0x27)
→ Scanner I2C: `i2cdetect` si disponible
→ Vérifier SDA/SCL sur A4/A5

#### 5. Conflit de baud rate
→ **CRITIQUE**: Harmoniser rs485_config.h et rotator_settings!

---

### Commandes utiles

```bash
# Lister tous les ports USB
ls -l /dev/cu.usb*

# Monitorer avec pio
pio device monitor -e antenna_unit
pio device monitor -e shack_unit

# Envoyer des commandes de test via USB Serial
echo "C" > /dev/cu.usbmodem1101  # Commande status

# Compiler avec verbose
pio run -e antenna_unit -v
pio run -e shack_unit -v
```

---

### Next Steps

1. ✅ Vérifier que RS485_DEBUG_ENABLED est actif
2. ✅ Monitorer les deux unités simultanément
3. ⚠️ **CORRIGER le baud rate** (19200 vs 9600)
4. ✅ Vérifier le câblage physique A-B
5. ✅ Vérifier que le potentiomètre A1 est lu correctement
6. ✅ Vérifier les LEDs TX/RX sur les modules RS485

---

**73 de ON7KGK!**
