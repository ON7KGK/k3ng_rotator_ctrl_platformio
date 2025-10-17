#!/bin/bash
# Upload RS485 Master TEST to Antenna Unit - Arduino Nano R4 Minima
# This is a simple test that broadcasts position every 100ms
# Usage: ./upload_test_antenna.sh

echo "=========================================="
echo "  RS485 Master TEST Upload (Antenna)"
echo "=========================================="
echo ""
echo "Hardware: Arduino Nano R4 Minima"
echo "Test: Broadcast simulated AZ/EL every 100ms"
echo "RS485: Serial1 @ 9600 bauds"
echo "Debug: USB Serial @ 9600 bauds"
echo ""

echo "🔨 Compilation du test RS485 Master..."
~/.platformio/penv/bin/pio run -e test_rs485_master

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
    ~/.platformio/packages/tool-dfuutil-arduino/dfu-util --device 0x2341:0x0374 -D .pio/build/test_rs485_master/firmware.bin -a0 -Q

    if [ $? -eq 0 ]; then
        echo ""
        echo "=========================================="
        echo "🎉 Upload réussi!"
        echo "=========================================="
        echo ""
        echo "Next steps:"
        echo "1. Connect RS485 module to D0/D1 (Serial1)"
        echo "2. Connect DE/RE to D8/D9"
        echo "3. Monitor with: ./monitor_simple.sh /dev/cu.usbmodem101"
        echo ""
        echo "Expected output:"
        echo "  RS485 Master Test starting..."
        echo "  Broadcast: AZ=180.00 EL=45.00"
        echo "  Broadcast: AZ=180.50 EL=45.10"
        echo "  (repeating every 100ms)"
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
