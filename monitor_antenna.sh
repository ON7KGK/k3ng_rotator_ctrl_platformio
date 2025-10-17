#!/bin/bash
# Monitoring ANTENNA Unit (Master) seulement
# Usage: ./monitor_antenna.sh [port]

echo "=========================================="
echo "📡 MONITORING ANTENNA (Master) Unit"
echo "=========================================="
echo ""

# Auto-detect port or use specified one
if [ -z "$1" ]; then
    PORT=$(ls /dev/cu.usbmodem* 2>/dev/null | head -n 1)

    if [ -z "$PORT" ]; then
        echo "❌ No USB device found!"
        echo ""
        echo "Available ports:"
        ls -1 /dev/cu.usb* 2>/dev/null || echo "  No USB devices found"
        echo ""
        echo "Usage: ./monitor_antenna.sh [port]"
        echo "Example: ./monitor_antenna.sh /dev/cu.usbmodem1201"
        exit 1
    fi

    echo "✅ Auto-detected port: $PORT"
else
    PORT=$1
    echo "Using specified port: $PORT"
fi

echo ""
echo "Configuration:"
echo "  - Port: $PORT"
echo "  - Baud: 9600"
echo "  - Unit: ANTENNA (Master)"
echo ""
echo "Messages attendus:"
echo "  ✓ RS485 Master initialized"
echo "  ✓ Master processing command: 0x10"
echo "  ✓ Status update sent: AZ=... EL=..."
echo "  ✓ Frame sent: 23/23 bytes"
echo ""
echo "Commandes possibles:"
echo "  0x10 = REQ_STATUS (demande position)"
echo "  0x11 = REQ_MOVE_TO (rotation vers AZ/EL)"
echo "  0x12 = REQ_STOP (arrêt urgence)"
echo "  0x13 = REQ_PARK (retour park)"
echo ""
echo "Pour quitter: CTRL+C"
echo ""
echo "Appuie sur ENTRÉE pour démarrer..."
read

clear
echo "🔍 Monitoring en cours sur $PORT..."
echo "==========================================="
echo ""

~/.platformio/penv/bin/pio device monitor --baud 9600 --port "$PORT" --filter time
