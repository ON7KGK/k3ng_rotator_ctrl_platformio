#!/bin/bash
# Simple monitor using cat (no PTY required)
# Usage: ./monitor_simple.sh [port]

PORT=${1:-/dev/cu.usbmodem1101}

echo "======================================"
echo "  K3NG RS485 Simple Monitor"
echo "======================================"
echo "Port: $PORT"
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

# Configure port settings
stty -f "$PORT" 9600 cs8 -cstopb -parenb

# Read from port
cat "$PORT"
