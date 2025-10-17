#!/bin/bash
# Monitoring SHACK Unit (Remote) seulement
# Usage: ./monitor_shack.sh [port]

echo "=========================================="
echo "🏠 MONITORING SHACK (Remote) Unit"
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
        echo "Usage: ./monitor_shack.sh [port]"
        echo "Example: ./monitor_shack.sh /dev/cu.usbmodem1101"
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
echo "  - Unit: SHACK (Remote)"
echo ""
echo "Messages attendus:"
echo "  ✓ RS485 Remote initialized"
echo "  ✓ Frame sent: 7/7 bytes"
echo "  ✓ Position updated: AZ=... EL=..."
echo "  ✓ Remote: ACK received"
echo ""
echo "Erreurs possibles:"
echo "  ✗ Remote: timeout waiting response"
echo "    → Le SHACK envoie mais ne reçoit rien du Master"
echo "  ✗ Parse error: CRC mismatch"
echo "    → Problème de câblage ou d'interférences"
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
