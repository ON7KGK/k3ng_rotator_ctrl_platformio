#!/bin/bash
# Upload script for ANTENNA Unit (Master) - Phase 4
# Hardware: Arduino Nano R4 Minima at antenna site
# Role: RS485 Master, motor control, position sensors, GPS tracking

echo "=========================================="
echo "ANTENNA Unit (Master) Upload Script"
echo "=========================================="
echo ""
echo "Hardware: Arduino Nano R4 Minima"
echo "Role: RS485 Master"
echo "Features:"
echo "  - Motor control (AZ/EL)"
echo "  - Position sensors (SSI/Potentiometer)"
echo "  - GPS tracking (Serial2)"
echo "  - Moon/Sun/Satellite tracking"
echo "  - Optional Ethernet"
echo ""
echo "Pins allocation:"
echo "  RS485: D0/D1 (Serial1), D8/D9 (DE/RE)"
echo "  Motors: A0-A3"
echo "  GPS: A4/A5 (Serial2)"
echo "  Encoders: D2-D7 (if enabled)"
echo ""

# Auto-detect port or use specified one
if [ -z "$1" ]; then
    # Auto-detect first available USB port
    PORT=$(ls /dev/cu.usbmodem* 2>/dev/null | head -n 1)

    if [ -z "$PORT" ]; then
        echo "❌ No USB device found!"
        echo ""
        echo "Available ports:"
        ls -1 /dev/cu.usb* 2>/dev/null || echo "  No USB devices found"
        echo ""
        echo "Usage: ./upload_antenna.sh [port]"
        echo "Example: ./upload_antenna.sh /dev/cu.usbmodem1101"
        exit 1
    fi

    echo "✅ Auto-detected port: $PORT"
else
    PORT=$1
    echo "Using specified port: $PORT"
fi
echo ""
echo "=========================================="
echo "⚠️  IMPORTANT: Arduino Nano R4 Bootloader"
echo "=========================================="
echo ""
echo "1. Press RESET button 2x QUICKLY on your Nano R4"
echo "2. Wait for the LED to blink SLOWLY (bootloader mode)"
echo "3. Press ENTER to start upload"
echo ""
read -p "Press ENTER when ready..." dummy
echo ""
echo "Building and uploading..."
echo ""

# Build and upload using PlatformIO
~/.platformio/penv/bin/pio run -e antenna_unit --target upload --upload-port "$PORT"

if [ $? -eq 0 ]; then
    echo ""
    echo "=========================================="
    echo "Upload successful!"
    echo "=========================================="
    echo ""
    echo "Next steps:"
    echo "1. Open Serial Monitor at 115200 baud"
    echo "2. Verify GPS data reception (Serial2)"
    echo "3. Check RS485 communication"
    echo "4. Test motor control"
    echo "5. Verify position sensor readings"
else
    echo ""
    echo "=========================================="
    echo "Upload failed!"
    echo "=========================================="
    echo ""
    echo "Troubleshooting:"
    echo "1. Check USB connection"
    echo "2. Verify correct port"
    echo "3. Try resetting the board"
    echo "4. Check for compilation errors above"
fi
