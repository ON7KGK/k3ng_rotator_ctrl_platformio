#!/bin/bash
# Script d'upload pour Nano R4 (branche NanoR4_Test)
# Usage: ./upload_nano_r4.sh

echo "🔨 Compilation du firmware Nano R4..."
~/.platformio/penv/bin/pio run -e nano_r4

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
    ~/.platformio/packages/tool-dfuutil-arduino/dfu-util --device 0x2341:0x0374 -D .pio/build/nano_r4/firmware.bin -a0 -Q

    if [ $? -eq 0 ]; then
        echo ""
        echo "🎉 Upload réussi!"
    else
        echo ""
        echo "❌ Upload échoué. Réessaie le double-clic RESET."
    fi
else
    echo "❌ Erreur de compilation"
fi
