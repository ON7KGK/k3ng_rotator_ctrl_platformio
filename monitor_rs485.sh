#!/bin/bash
# Script de monitoring RS485 - Affiche les deux unités en parallèle
# Usage: ./monitor_rs485.sh

echo "=========================================="
echo "RS485 Dual Monitor - K3NG Rotator"
echo "=========================================="
echo ""

# Fonction pour détecter les ports USB
detect_ports() {
    PORTS=($(ls /dev/cu.usbmodem* 2>/dev/null))
    NUM_PORTS=${#PORTS[@]}

    if [ $NUM_PORTS -eq 0 ]; then
        echo "❌ Aucun Arduino Nano R4 détecté!"
        echo ""
        echo "Vérifie que:"
        echo "  1. Les deux Arduino sont branchés en USB"
        echo "  2. Les drivers sont installés"
        echo "  3. Les câbles USB fonctionnent"
        exit 1
    fi

    echo "📍 Port(s) USB détecté(s):"
    for i in "${!PORTS[@]}"; do
        echo "  [$i] ${PORTS[$i]}"
    done
    echo ""
}

# Détection des ports
detect_ports

# Si un seul port détecté
if [ $NUM_PORTS -eq 1 ]; then
    echo "⚠️  Un seul Arduino détecté: ${PORTS[0]}"
    echo ""
    echo "Mode: MONITORING UNIQUE"
    echo "Impossible de monitorer les deux unités simultanément."
    echo ""
    read -p "Appuie sur ENTRÉE pour monitorer cet Arduino..."

    echo ""
    echo "🔍 Monitoring ${PORTS[0]} à 9600 bauds..."
    echo "   (CTRL+C pour quitter)"
    echo ""

    ~/.platformio/penv/bin/pio device monitor --baud 9600 --port "${PORTS[0]}" --filter time
    exit 0
fi

# Si deux ports ou plus détectés
echo "Mode: MONITORING DUAL"
echo ""
echo "Sélectionne les ports pour chaque unité:"
echo ""

# Sélection port SHACK
echo "Quelle unité est branchée sur ${PORTS[0]}?"
echo "  1) SHACK (Remote)"
echo "  2) ANTENNA (Master)"
read -p "Choix: " choice1

if [ "$choice1" == "1" ]; then
    PORT_SHACK="${PORTS[0]}"
    PORT_ANTENNA="${PORTS[1]}"
    echo "✅ SHACK  → ${PORT_SHACK}"
    echo "✅ ANTENNA → ${PORT_ANTENNA}"
else
    PORT_SHACK="${PORTS[1]}"
    PORT_ANTENNA="${PORTS[0]}"
    echo "✅ SHACK  → ${PORT_SHACK}"
    echo "✅ ANTENNA → ${PORT_ANTENNA}"
fi

echo ""
echo "=========================================="
echo "Démarrage du monitoring..."
echo "=========================================="
echo ""
echo "Deux fenêtres de terminal vont s'ouvrir:"
echo "  - Terminal 1: SHACK (Remote)"
echo "  - Terminal 2: ANTENNA (Master)"
echo ""
echo "Pour arrêter: Ferme les fenêtres ou CTRL+C"
echo ""
read -p "Appuie sur ENTRÉE pour continuer..."

# Vérifier si on est sur macOS
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS - Utiliser Terminal.app

    echo "Ouverture des terminaux..."

    # Créer des scripts temporaires pour chaque terminal
    SCRIPT_SHACK="/tmp/monitor_shack_$$.sh"
    SCRIPT_ANTENNA="/tmp/monitor_antenna_$$.sh"

    # Script SHACK
    cat > "$SCRIPT_SHACK" <<'SHACKEOF'
#!/bin/bash
clear
echo "🏠 MONITORING SHACK (Remote)"
echo "=========================================="
echo "Port: PORT_SHACK_PLACEHOLDER"
echo "=========================================="
echo ""
~/.platformio/penv/bin/pio device monitor --baud 9600 --port "PORT_SHACK_PLACEHOLDER" --filter time
SHACKEOF
    sed -i '' "s|PORT_SHACK_PLACEHOLDER|${PORT_SHACK}|g" "$SCRIPT_SHACK"
    chmod +x "$SCRIPT_SHACK"

    # Script ANTENNA
    cat > "$SCRIPT_ANTENNA" <<'ANTENNAEOF'
#!/bin/bash
clear
echo "📡 MONITORING ANTENNA (Master)"
echo "=========================================="
echo "Port: PORT_ANTENNA_PLACEHOLDER"
echo "=========================================="
echo ""
~/.platformio/penv/bin/pio device monitor --baud 9600 --port "PORT_ANTENNA_PLACEHOLDER" --filter time
ANTENNAEOF
    sed -i '' "s|PORT_ANTENNA_PLACEHOLDER|${PORT_ANTENNA}|g" "$SCRIPT_ANTENNA"
    chmod +x "$SCRIPT_ANTENNA"

    # Ouvrir Terminal 1 - SHACK
    open -a Terminal "$SCRIPT_SHACK"

    # Attendre un peu
    sleep 0.5

    # Ouvrir Terminal 2 - ANTENNA
    open -a Terminal "$SCRIPT_ANTENNA"

    echo ""
    echo "✅ Deux terminaux devraient s'ouvrir!"
    echo ""
    echo "Si tu ne vois rien:"
    echo "  1. Vérifie dans le Dock si Terminal.app a des fenêtres cachées"
    echo "  2. Regarde dans Mission Control (F3)"
    echo "  3. Ou lance manuellement:"
    echo "     Terminal 1: $SCRIPT_SHACK"
    echo "     Terminal 2: $SCRIPT_ANTENNA"
    echo ""
    echo "Les scripts temporaires seront dans /tmp/"
    echo ""
    echo "Tu devrais voir:"
    echo "  SHACK   → 'RS485 Remote initialized' + requêtes REQ_STATUS"
    echo "  ANTENNA → 'RS485 Master initialized' + réponses RSP_STATUS"
    echo ""
    echo "Appuie sur ENTRÉE pour continuer (les terminaux resteront ouverts)..."
    read

else
    # Linux/Windows - Utiliser screen en mode split
    echo "Démarrage en mode split-screen..."
    echo ""

    # Créer un fichier de config screen temporaire
    cat > /tmp/rs485_screen.conf <<SCREENEOF
# Split horizontal
split
# Focus sur la fenêtre du haut
focus top
# Lancer monitoring SHACK
screen -t "SHACK" bash -c "clear && echo '🏠 MONITORING SHACK (Remote) - ${PORT_SHACK}' && echo '==========================================' && echo '' && ~/.platformio/penv/bin/pio device monitor --baud 9600 --port '${PORT_SHACK}' --filter time"
# Focus sur la fenêtre du bas
focus down
# Lancer monitoring ANTENNA
screen -t "ANTENNA" bash -c "clear && echo '📡 MONITORING ANTENNA (Master) - ${PORT_ANTENNA}' && echo '==========================================' && echo '' && ~/.platformio/penv/bin/pio device monitor --baud 9600 --port '${PORT_ANTENNA}' --filter time"
SCREENEOF

    # Lancer screen avec la config
    screen -c /tmp/rs485_screen.conf

    # Nettoyer
    rm /tmp/rs485_screen.conf
fi
