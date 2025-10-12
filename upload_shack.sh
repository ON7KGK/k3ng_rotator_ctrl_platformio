#!/bin/bash
# Upload script for SHACK Unit (Remote) - Phase 4
# Hardware: Arduino Nano R4 Minima in shack
# Role: RS485 Remote, user interface, PC control

echo "=========================================="
echo "SHACK Unit (Remote) Upload Script"
echo "=========================================="
echo ""
echo "Hardware: Arduino Nano R4 Minima"
echo "Role: RS485 Remote"
echo "Features:"
echo "  - User interface (Nextion/LCD)"
echo "  - Manual control buttons"
echo "  - Preset rotary encoders"
echo "  - PC control (Yaesu/Easycom)"
echo "  - RS485 communication"
echo ""
echo "Pins allocation:"
echo "  RS485: D0/D1 (Serial1), D8/D9 (DE/RE)"
echo "  Display: A4/A5 (Nextion Serial2 or I2C LCD)"
echo "  Buttons: D2-D6 (CW/CCW/UP/DOWN/STOP)"
echo "  Encoders: D10-D13 (if enabled)"
echo ""

# Check if port is specified
if [ -z "$1" ]; then
    echo "Usage: ./upload_shack.sh <port>"
    echo ""
    echo "Example: ./upload_shack.sh /dev/cu.usbmodem1101"
    echo ""
    echo "Available ports:"
    ls -1 /dev/cu.usb* 2>/dev/null || echo "  No USB devices found"
    exit 1
fi

PORT=$1
echo "Upload port: $PORT"
echo ""
echo "Building and uploading..."
echo ""

# Build and upload using PlatformIO
~/.platformio/penv/bin/pio run -e shack_unit --target upload --upload-port "$PORT"

if [ $? -eq 0 ]; then
    echo ""
    echo "=========================================="
    echo "Upload successful!"
    echo "=========================================="
    echo ""
    echo "Next steps:"
    echo "1. Open Serial Monitor at 115200 baud"
    echo "2. Check RS485 communication with Master"
    echo "3. Verify display output (Nextion/LCD)"
    echo "4. Test manual control buttons"
    echo "5. Test PC control (PSTRotator)"
    echo "6. Verify position data from Master"
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
