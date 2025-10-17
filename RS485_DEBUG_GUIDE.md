# Guide de Debug RS485 - K3NG Rotator Controller

## Comment voir le dialogue RS485 entre les deux unités

### 1. Configuration actuelle

Le debug RS485 est **DÉJÀ ACTIVÉ** dans le code :
- Fichier : `include/rs485_config.h` ligne 86
- `#define RS485_DEBUG_ENABLED` est actif

Tous les messages RS485 sont envoyés sur **Serial (USB)** à **9600 bauds**.

---

## 2. Méthode 1 : Serial Monitor PlatformIO (RECOMMANDÉ)

### Sur l'unité SHACK (Remote) :

```bash
# Terminal 1 - Moniteur SHACK
pio device monitor --baud 9600 --filter send_on_enter --port /dev/cu.usbmodem1101
```

### Sur l'unité ANTENNA (Master) :

```bash
# Terminal 2 - Moniteur ANTENNA
pio device monitor --baud 9600 --filter send_on_enter --port /dev/cu.usbmodem1201
```

**Remplace les ports** par ceux détectés automatiquement :
```bash
# Lister les ports disponibles
pio device list
```

---

## 3. Messages de debug attendus

### Côté SHACK (Remote) :

```
RS485 Remote initialized
Frame sent: 7/7 bytes
Remote: timeout waiting response
Position updated: AZ=180.5 EL=45.2 Status=0x3
Remote: ACK received
```

### Côté ANTENNA (Master) :

```
RS485 Master initialized
Master processing command: 0x1
Move to AZ: 180.5 EL: 45.2
Status update sent: AZ=180.5 EL=45.2
Frame sent: 23/23 bytes
```

---

## 4. Messages d'erreur possibles

| Message | Signification | Solution |
|---------|---------------|----------|
| `RX timeout (partial frame)` | Trame incomplète reçue | Vérifier câblage RS485 |
| `Parse error: CRC mismatch` | Erreur de transmission | Vérifier blindage câble, réduire vitesse |
| `Parse error: invalid start byte` | Désynchronisation | Reset les deux Arduino |
| `Remote: max retries reached` | Aucune réponse du Master | Vérifier que Master est alimenté |
| `RX error: buffer overflow` | Trop de données | Bug logiciel, signaler |

---

## 5. Détails des trames RS485

Chaque trame affiche :
```
Frame built: addr=0x1 cmd=0x1 len=8 crc=0xAB12
Frame sent: 15/15 bytes
```

**Décodage** :
- `addr=0x1` : Adresse Master (0x1) ou Remote (0x2)
- `cmd=0x1` : Commande (voir rs485_protocol.h)
- `len=8` : Longueur des données (en bytes)
- `crc=0xAB12` : Checksum CRC16 pour validation
- `15/15 bytes` : Nombre d'octets envoyés/attendus

---

## 6. Commandes RS485 (rs485_protocol.h)

| Code | Nom | Direction | Description |
|------|-----|-----------|-------------|
| 0x10 | REQ_STATUS | Remote → Master | Demande position actuelle |
| 0x11 | REQ_MOVE_TO | Remote → Master | Demande rotation vers AZ/EL |
| 0x12 | REQ_STOP | Remote → Master | Arrêt d'urgence |
| 0x13 | REQ_PARK | Remote → Master | Retour position park |
| 0x20 | RSP_STATUS | Master → Remote | Réponse avec position |
| 0xF0 | RSP_ACK | Master → Remote | Commande acceptée |
| 0xF1 | RSP_NAK | Master → Remote | Commande refusée |

---

## 7. Activation de debug ENCORE PLUS VERBEUX (optionnel)

Si tu veux voir **CHAQUE BYTE** qui passe sur RS485 :

### Éditer `include/rs485_config.h` :

Ajouter après la ligne 86 :
```cpp
#define RS485_DEBUG_ENABLED
#define RS485_DEBUG_RAW_BYTES    // Nouveau - debug byte par byte
```

### Puis dans `src/rs485_common.cpp` ligne 193, ajouter :

```cpp
// Dans rs485_send_frame(), après digitalWrite(RS485_DE_PIN, HIGH);
#ifdef RS485_DEBUG_RAW_BYTES
    RS485_DEBUG("TX RAW [");
    for (size_t i = 0; i < frame_size; i++) {
        if (tx_buffer[i] < 0x10) RS485_DEBUG("0");
        RS485_DEBUG_HEX(tx_buffer[i]);
        RS485_DEBUG(" ");
    }
    RS485_DEBUGLN("]");
#endif
```

### Et dans `rs485_receive_frame()` ligne 231 :

```cpp
// Après uint8_t byte = RS485_SERIAL.read();
#ifdef RS485_DEBUG_RAW_BYTES
    if (byte < 0x10) RS485_DEBUG("0");
    RS485_DEBUG_HEX(byte);
    RS485_DEBUG(" ");
#endif
```

---

## 8. Test de communication RS485

### Test simple - Loopback :

1. **Débrancher** le câble RS485 entre les deux unités
2. Sur l'unité SHACK, **court-circuiter** les pins A et B du module RS485
3. Lancer le Serial Monitor
4. Tu devrais voir les trames **envoyées ET reçues** (écho)

Si l'écho fonctionne → Le module RS485 SHACK est OK
Si pas d'écho → Problème hardware (module, pins DE/RE, alimentation)

### Test communication bidirectionnelle :

1. **Rebrancher** le câble RS485
2. Alimenter les **DEUX** unités (SHACK + ANTENNA)
3. Ouvrir **DEUX** Serial Monitor (un par unité)
4. Observer les messages synchronisés

**Ce que tu devrais voir** :
- SHACK envoie `REQ_STATUS` toutes les 100ms
- ANTENNA répond `RSP_STATUS` avec position
- SHACK affiche `Position updated: AZ=... EL=...`

---

## 9. LED témoin RS485 (à ajouter si besoin)

Si tu veux une LED qui clignote à chaque trame RS485 :

### Matériel :
- LED + résistance 220Ω sur pin D13 (LED onboard du Nano R4)

### Code à ajouter dans `rs485_common.cpp` :

```cpp
// Dans rs485_send_frame()
digitalWrite(LED_BUILTIN, HIGH);
size_t written = RS485_SERIAL.write(tx_buffer, frame_size);
delay(10);  // Flash visible
digitalWrite(LED_BUILTIN, LOW);
```

---

## 10. Checklist de dépannage

- [ ] Les deux Arduino sont alimentés
- [ ] Le câble RS485 A-A et B-B est bien connecté
- [ ] Les pins DE (D8) et RE (D9) sont bien câblés
- [ ] Le module RS485 a bien VCC et GND
- [ ] `RS485_DEBUG_ENABLED` est défini dans rs485_config.h
- [ ] Serial Monitor est à 9600 bauds
- [ ] Les deux Arduino sont flashés avec le bon firmware (shack_unit / antenna_unit)
- [ ] Le Nextion est maintenant sur D0/D1 (Serial1) sur SHACK
- [ ] Le RS485 est maintenant sur A4/A5 (Serial2) sur SHACK

---

## 11. Commandes utiles

```bash
# Compiler et uploader SHACK
./upload_shack.sh

# Compiler et uploader ANTENNA
./upload_antenna.sh

# Moniteur série avec timestamp
pio device monitor --baud 9600 --filter time

# Moniteur série avec log dans fichier
pio device monitor --baud 9600 | tee rs485_log.txt

# Moniteur série colorisé
pio device monitor --baud 9600 --filter colorize
```

---

## 12. Contact en cas de problème

Si après tous ces tests le RS485 ne communique pas :

1. Poste les logs Serial Monitor des DEUX unités
2. Vérifie avec un multimètre la tension sur A et B (devrait varier entre 0-5V)
3. Essaie de réduire la vitesse RS485 : `RS485_BAUD_RATE 9600` au lieu de 19200
4. Vérifie que les modules RS485 ont bien des résistances de terminaison (120Ω)

Bon debug !
