#!/bin/bash
# Script d'upload pour test RS485 PROTOCOLE (Phase 2) sur Nano R4
# Branche: NanoR4_Test_RS485
# Usage: ./upload_test_protocol.sh

echo "🔨 Compilation du test RS485 Protocol (Phase 2)..."
~/.platformio/penv/bin/pio run -e test_rs485_protocol

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Compilation réussie!"
    echo ""
    echo "⚡ DOUBLE-CLIQUE SUR LE BOUTON RESET MAINTENANT!"
    echo "   (La LED orange devrait pulser lentement)"
    echo ""
    echo "Appuie sur ENTRÉE quand c'est fait..."
    read

    echo "📤 Upload en cours..."
    ~/.platformio/packages/tool-dfuutil-arduino/dfu-util --device 0x2341:0x0374 -D .pio/build/test_rs485_protocol/firmware.bin -a0 -Q

    if [ $? -eq 0 ]; then
        echo ""
        echo "🎉 Upload réussi!"
        echo ""
        echo "📟 Pour voir les messages, ouvre le Serial Monitor:"
        echo "   pio device monitor --baud 115200"
        echo ""
        echo "🧪 Ce test envoie des trames structurées avec:"
        echo "   - CRC16 pour validation"
        echo "   - Données RotatorState complètes"
        echo "   - Statistiques TX/RX"
    else
        echo ""
        echo "❌ Upload échoué. Réessaie le double-clic RESET."
    fi
else
    echo "❌ Erreur de compilation"
fi
