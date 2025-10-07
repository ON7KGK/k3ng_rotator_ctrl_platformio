# 📤 Guide d'Upload pour Arduino Nano R4

Ce document explique comment uploader le firmware sur les Arduino Nano R4 avec PlatformIO.

## 🔧 Pourquoi le double-clic sur RESET ?

Le Nano R4 (Renesas RA4M1) utilise le mode **DFU (Device Firmware Update)** pour l'upload. Contrairement à l'Arduino IDE qui peut forcer automatiquement le reset, PlatformIO nécessite un **double-clic manuel** sur le bouton RESET pour entrer en mode bootloader.

## ✅ Méthode 1 : Scripts automatiques (Recommandé)

### Upload ANTENNA unit :
```bash
./upload_antenna.sh
```

### Upload SHACK unit :
```bash
./upload_shack.sh
```

**Procédure :**
1. Le script compile le firmware
2. Il demande de double-cliquer sur RESET
3. Double-clique rapidement sur le bouton RESET du Nano R4
4. La LED orange devrait **pulser lentement** (mode bootloader)
5. Appuie sur ENTRÉE
6. L'upload se fait automatiquement

## 🖥️ Méthode 2 : VSCode PlatformIO Extension

1. Dans VSCode, ouvre la barre latérale **PlatformIO** (icône alien à gauche)
2. Sous **PROJECT TASKS** → **antenna_unit** (ou **shack_unit**) → **General**
3. Clique sur **"Upload"**
4. **Pendant la compilation**, prépare-toi à double-cliquer sur RESET
5. Dès que la compilation se termine, **double-clique sur RESET**
6. L'upload devrait se lancer automatiquement

## 🔍 Vérification du mode DFU

Pour vérifier si le Nano R4 est bien en mode DFU (après double-clic RESET) :

```bash
~/.platformio/packages/tool-dfuutil-arduino/dfu-util -l
```

Tu devrais voir :
```
Found DFU: [2341:0374] ver=0100, devnum=1, cfg=1, intf=0, path="1-1", alt=0, name="@CodeFlash /0x00000000/8*2Ka,120*2Kg"
```

Si tu vois `No DFU capable USB device available`, réessaie le double-clic RESET.

## 🎯 Timing du double-clic

- **Trop lent** : La carte ne rentre pas en mode bootloader
- **Trop rapide** : Ça peut être interprété comme un simple reset
- **Idéal** : Deux clics rapides avec ~200ms entre eux

**Astuce** : Entraîne-toi à double-cliquer sur RESET jusqu'à voir la LED orange pulser lentement.

## 🐛 Dépannage

### "No DFU capable USB device available"
- Réessaie le double-clic RESET (timing différent)
- Vérifie que le câble USB est bien branché
- Essaie un autre port USB

### "Upload échoué" mais le device DFU est détecté
- Le timeout est trop court, relance l'upload immédiatement
- Vérifie que tu es sur la bonne branche Git : `NanoR4_FromScratch`

### L'Arduino IDE fonctionne mais pas PlatformIO
- C'est normal ! Arduino IDE utilise un mécanisme de reset automatique
- PlatformIO nécessite le double-clic manuel
- Utilise les scripts `upload_*.sh` pour simplifier

## 📊 Utilisation mémoire

### ANTENNA unit :
- RAM: 12.5% (4104 bytes / 32KB)
- Flash: 17.6% (46268 bytes / 256KB)

### SHACK unit :
- RAM: 15.8% (5180 bytes / 32KB)
- Flash: 19.0% (49780 bytes / 262KB)

## 🔗 Liens utiles

- [PlatformIO Renesas RA Platform](https://docs.platformio.org/en/latest/platforms/renesas-ra.html)
- [Arduino Nano R4 Documentation](https://docs.arduino.cc/hardware/uno-r4-minima/)
- [DFU-Util Documentation](http://dfu-util.sourceforge.net/)

---

**73 de ON7KGK** 📻
