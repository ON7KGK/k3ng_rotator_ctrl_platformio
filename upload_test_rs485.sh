#!/bin/bash
# Script d'upload pour test RS485 sur Nano R4
# Branche: NanoR4_Test_RS485
# Usage: ./upload_test_rs485.sh

echo "🔨 Compilation du test RS485..."
~/.platformio/penv/bin/pio run -e test_rs485

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
    ~/.platformio/packages/tool-dfuutil-arduino/dfu-util --device 0x2341:0x0374 -D .pio/build/test_rs485/firmware.bin -a0 -Q

    if [ $? -eq 0 ]; then
        echo ""
        echo "🎉 Upload réussi!"
        echo ""
        echo "📟 Pour voir les messages, ouvre le Serial Monitor:"
        echo "   pio device monitor --baud 115200"
    else
        echo ""
        echo "❌ Upload échoué. Réessaie le double-clic RESET."
    fi
else
    echo "❌ Erreur de compilation"
fi
