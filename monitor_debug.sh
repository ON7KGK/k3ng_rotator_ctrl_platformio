#!/bin/bash
# Script to monitor debug output from Arduino via USB Serial
# Usage: ./monitor_debug.sh [port]

PORT=${1:-/dev/cu.usbmodem1101}

echo "======================================"
echo "  K3NG RS485 Debug Monitor"
echo "======================================"
echo "Port: $PORT"
echo "Baud: 9600 (USB Serial debug)"
echo ""
echo "Press Ctrl+C to exit"
echo "======================================"
echo ""

# Check if port exists
if [ ! -e "$PORT" ]; then
    echo "ERROR: Port $PORT not found!"
    echo ""
    echo "Available ports:"
    ls -1 /dev/cu.usb* 2>/dev/null || echo "No USB ports found"
    exit 1
fi

# Monitor the serial port
screen $PORT 9600
