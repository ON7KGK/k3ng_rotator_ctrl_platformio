#!/bin/bash
# Script d'upload pour RS485 REMOTE (SHACK) - Phase 3
# Branche: NanoR4_Test_RS485
# Usage: ./upload_remote.sh

echo "🔨 Compilation RS485 Remote (SHACK Unit)..."
~/.platformio/penv/bin/pio run -e test_rs485_remote

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
    ~/.platformio/packages/tool-dfuutil-arduino/dfu-util --device 0x2341:0x0374 -D .pio/build/test_rs485_remote/firmware.bin -a0 -Q

    if [ $? -eq 0 ]; then
        echo ""
        echo "🎉 Upload réussi sur REMOTE (SHACK)!"
        echo ""
        echo "📟 Pour voir les messages:"
        echo "   pio device monitor --baud 115200"
        echo ""
        echo "🏠 Ce Remote:"
        echo "   - Interroge le Master toutes les 100ms"
        echo "   - Affiche les positions reçues"
        echo "   - Envoie des commandes test toutes les 15s"
    else
        echo ""
        echo "❌ Upload échoué. Réessaie le double-clic RESET."
    fi
else
    echo "❌ Erreur de compilation"
fi
