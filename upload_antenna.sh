#!/bin/bash
# Script d'upload pour ANTENNA Unit (Master) - Arduino Nano R4 Minima
# Configuration: No display, GPS, Motor control, RS485 Master
# Usage: ./upload_antenna.sh

echo "=========================================="
echo "  ANTENNA Unit (Master) Upload Script"
echo "=========================================="
echo ""
echo "Hardware: Arduino Nano R4 Minima"
echo "Display: None (display is on Remote)"
echo "Role: RS485 Master, Motor Control, GPS"
echo ""

echo "🔨 Compilation du firmware antenna_unit..."
~/.platformio/penv/bin/pio run -e antenna_unit

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Compilation réussie!"
    echo ""
    echo "=========================================="
    echo "⚡ DOUBLE-CLIQUE SUR LE BOUTON RESET MAINTENANT!"
    echo "=========================================="
    echo ""
    echo "Instructions:"
    echo "1. Appuie DEUX FOIS rapidement sur le bouton RESET"
    echo "2. La LED orange devrait pulser LENTEMENT"
    echo "3. Appuie sur ENTRÉE quand la LED pulse lentement"
    echo ""
    read -p "Appuie sur ENTRÉE quand c'est fait... "

    echo ""
    echo "📤 Upload en cours via DFU..."
    ~/.platformio/packages/tool-dfuutil-arduino/dfu-util --device 0x2341:0x0374 -D .pio/build/antenna_unit/firmware.bin -a0 -Q

    if [ $? -eq 0 ]; then
        echo ""
        echo "=========================================="
        echo "🎉 Upload réussi!"
        echo "=========================================="
        echo ""
        echo "Next steps:"
        echo "1. Connect GPS module to Serial2 (A4/A5)"
        echo "2. Connect motor control outputs"
        echo "3. Connect SSI encoders (or potentiometers)"
        echo "4. Connect RS485 (D0/D1 + D8/D9)"
        echo "5. Power up and check Serial Monitor"
        echo ""
    else
        echo ""
        echo "=========================================="
        echo "❌ Upload échoué"
        echo "=========================================="
        echo ""
        echo "Troubleshooting:"
        echo "1. Réessaie le double-clic RESET"
        echo "2. Vérifie que la LED pulse lentement"
        echo "3. Vérifie la connexion USB"
        echo "4. Essaie sur un autre port USB"
        echo ""
    fi
else
    echo ""
    echo "=========================================="
    echo "❌ Erreur de compilation"
    echo "=========================================="
    echo ""
    echo "Vérifie les erreurs ci-dessus et corrige-les"
    echo ""
fi
