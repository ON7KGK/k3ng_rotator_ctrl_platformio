#!/bin/bash
# Script de monitoring RS485 SIMPLE - Affiche les deux unités l'une après l'autre
# Usage: ./monitor_rs485_simple.sh

echo "=========================================="
echo "RS485 Simple Monitor - K3NG Rotator"
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
    echo "Impossible de monitorer les deux unités."
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
echo "Mode: MONITORING SÉQUENTIEL"
echo ""
echo "Les deux unités seront monitorées l'une après l'autre."
echo "Pour changer d'unité, appuie sur CTRL+C puis relance le script."
echo ""

# Sélection port SHACK
echo "Quelle unité est branchée sur ${PORTS[0]}?"
echo "  1) SHACK (Remote)"
echo "  2) ANTENNA (Master)"
read -p "Choix: " choice1

if [ "$choice1" == "1" ]; then
    PORT_SHACK="${PORTS[0]}"
    PORT_ANTENNA="${PORTS[1]}"
else
    PORT_SHACK="${PORTS[1]}"
    PORT_ANTENNA="${PORTS[0]}"
fi

echo ""
echo "Configuration:"
echo "  SHACK   → ${PORT_SHACK}"
echo "  ANTENNA → ${PORT_ANTENNA}"
echo ""

# Menu de sélection
while true; do
    echo ""
    echo "=========================================="
    echo "Quelle unité veux-tu monitorer?"
    echo "=========================================="
    echo "  1) SHACK (Remote) - ${PORT_SHACK}"
    echo "  2) ANTENNA (Master) - ${PORT_ANTENNA}"
    echo "  3) Les deux en alternance (5 sec chacune)"
    echo "  q) Quitter"
    echo ""
    read -p "Choix: " choice

    case $choice in
        1)
            echo ""
            echo "🏠 Monitoring SHACK (Remote)..."
            echo "Port: ${PORT_SHACK}"
            echo "(CTRL+C pour retourner au menu)"
            echo ""
            sleep 1
            ~/.platformio/penv/bin/pio device monitor --baud 9600 --port "${PORT_SHACK}" --filter time
            ;;
        2)
            echo ""
            echo "📡 Monitoring ANTENNA (Master)..."
            echo "Port: ${PORT_ANTENNA}"
            echo "(CTRL+C pour retourner au menu)"
            echo ""
            sleep 1
            ~/.platformio/penv/bin/pio device monitor --baud 9600 --port "${PORT_ANTENNA}" --filter time
            ;;
        3)
            echo ""
            echo "🔄 Monitoring alternance SHACK ↔ ANTENNA"
            echo "(CTRL+C pour retourner au menu)"
            echo ""

            while true; do
                clear
                echo "=========================================="
                echo "🏠 SHACK (Remote) - ${PORT_SHACK}"
                echo "=========================================="
                timeout 5s ~/.platformio/penv/bin/pio device monitor --baud 9600 --port "${PORT_SHACK}" --filter time 2>/dev/null

                clear
                echo "=========================================="
                echo "📡 ANTENNA (Master) - ${PORT_ANTENNA}"
                echo "=========================================="
                timeout 5s ~/.platformio/penv/bin/pio device monitor --baud 9600 --port "${PORT_ANTENNA}" --filter time 2>/dev/null
            done
            ;;
        q|Q)
            echo ""
            echo "Au revoir!"
            exit 0
            ;;
        *)
            echo "❌ Choix invalide"
            ;;
    esac
done
