#!/bin/bash
# Script d'upload pour RS485 MASTER (ANTENNA) - Phase 3
# Branche: NanoR4_Test_RS485
# Usage: ./upload_master.sh

echo "🔨 Compilation RS485 Master (ANTENNA Unit)..."
~/.platformio/penv/bin/pio run -e test_rs485_master

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
    ~/.platformio/packages/tool-dfuutil-arduino/dfu-util --device 0x2341:0x0374 -D .pio/build/test_rs485_master/firmware.bin -a0 -Q

    if [ $? -eq 0 ]; then
        echo ""
        echo "🎉 Upload réussi sur MASTER (ANTENNA)!"
        echo ""
        echo "📟 Pour voir les messages:"
        echo "   pio device monitor --baud 115200"
        echo ""
        echo "📡 Ce Master:"
        echo "   - Écoute les requêtes du Remote"
        echo "   - Envoie les positions toutes les 100ms"
        echo "   - Répond aux commandes (Move, Stop, Park)"
    else
        echo ""
        echo "❌ Upload échoué. Réessaie le double-clic RESET."
    fi
else
    echo "❌ Erreur de compilation"
fi
