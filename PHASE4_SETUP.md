# Phase 4 Setup Guide - Production Environments

**Date:** 12 octobre 2025
**Status:** Ready for integration testing
**Hardware:** 2x Arduino Nano R4 Minima (Renesas RA4M1)

---

## 📋 Overview

Phase 4 introduces production-ready environments for the RS485 Master/Remote architecture:

- **ANTENNA Unit (Master)** - Physical rotator control at antenna site
- **SHACK Unit (Remote)** - User interface and PC control in shack

Each unit has dedicated:
- Feature configuration files (`rotator_features_*.h`)
- Pin allocation files (`rotator_pins_*.h`)
- PlatformIO environment (`antenna_unit`, `shack_unit`)
- Upload scripts (`upload_antenna.sh`, `upload_shack.sh`)

---

## 📁 New Files Created

### Configuration Files

| File | Purpose |
|------|---------|
| `include/rotator_features_master.h` | Features enabled on ANTENNA unit |
| `include/rotator_features_remote.h` | Features enabled on SHACK unit |
| `include/rotator_pins_master.h` | Pin allocation for ANTENNA unit |
| `include/rotator_pins_remote.h` | Pin allocation for SHACK unit |

### Build Environments

Added to `platformio.ini`:
- `[env:antenna_unit]` - ANTENNA Unit (Master)
- `[env:shack_unit]` - SHACK Unit (Remote)

### Upload Scripts

| Script | Purpose |
|--------|---------|
| `upload_antenna.sh` | Build and upload to ANTENNA unit |
| `upload_shack.sh` | Build and upload to SHACK unit |

---

## 🔧 ANTENNA Unit (Master) Configuration

### Features Enabled
- ✅ Motor control (AZ/EL)
- ✅ Position sensors (SSI encoders or potentiometers)
- ✅ GPS tracking (Serial2 on A4/A5)
- ✅ Moon/Sun/Satellite tracking
- ✅ RS485 Master communication
- ✅ Optional Ethernet (W5100/W5500)

### Pin Allocation
```
RS485 Communication:
  D0  - RX1 (Serial1)
  D1  - TX1 (Serial1)
  D8  - DE (Driver Enable)
  D9  - RE (Receiver Enable)

Motor Control:
  A0  - Azimuth CW
  A1  - Azimuth CCW
  A2  - Elevation UP
  A3  - Elevation DOWN

GPS:
  A4  - GPS TX (Serial2)
  A5  - GPS RX (Serial2)

Position Sensors (SSI):
  D2  - AZ SSI Clock
  D3  - AZ SSI CS
  D4  - AZ SSI Data
  D5  - EL SSI Clock
  D6  - EL SSI CS
  D7  - EL SSI Data

Ethernet (optional):
  D10 - CS
  D11 - MOSI
  D12 - MISO
  D13 - SCK
```

### Build & Upload
```bash
# Build only
~/.platformio/penv/bin/pio run -e antenna_unit

# Build and upload
./upload_antenna.sh /dev/cu.usbmodem1101
```

---

## 🏠 SHACK Unit (Remote) Configuration

### Features Enabled
- ✅ Nextion display (Serial2 on A4/A5) or LCD I2C
- ✅ Manual control buttons (5 buttons)
- ✅ Optional tracking buttons (Moon/Sun)
- ✅ Optional preset encoders (AZ/EL)
- ✅ RS485 Remote communication
- ✅ PC control (Yaesu/Easycom on USB Serial)

### Pin Allocation
```
RS485 Communication:
  D0  - RX1 (Serial1)
  D1  - TX1 (Serial1)
  D8  - DE (Driver Enable)
  D9  - RE (Receiver Enable)

Display:
  A4  - Nextion TX (Serial2) or I2C SDA
  A5  - Nextion RX (Serial2) or I2C SCL

Manual Buttons:
  D2  - CW
  D3  - CCW
  D4  - UP
  D5  - DOWN
  D6  - STOP
  D7  - Moon Track (optional)
  A0  - Sun Track (optional)

Preset Encoders (optional):
  D10 - AZ Encoder Pin 1
  D11 - AZ Encoder Pin 2
  D12 - EL Encoder Pin 1
  D13 - EL Encoder Pin 2
  A1  - Preset Start Button
```

### Build & Upload
```bash
# Build only
~/.platformio/penv/bin/pio run -e shack_unit

# Build and upload
./upload_shack.sh /dev/cu.usbmodem1101
```

---

## 🚀 Quick Start

### 1. Modify Main Code to Use Conditional Compilation

The main K3NG code needs to be updated to use the appropriate configuration files:

**In `rotator_hardware.h` or main `.ino` file:**

```cpp
#ifdef USE_ROTATOR_FEATURES_MASTER
  #include "rotator_features_master.h"
  #include "rotator_pins_master.h"
#elif defined(USE_ROTATOR_FEATURES_REMOTE)
  #include "rotator_features_remote.h"
  #include "rotator_pins_remote.h"
#else
  #include "rotator_features.h"
  #include "rotator_pins.h"
#endif
```

### 2. Integrate RS485 Communication

**In main loop:**

```cpp
#ifdef FEATURE_RS485_MASTER
  rs485_master_loop();  // Handle RS485 Master communication
#endif

#ifdef FEATURE_RS485_REMOTE
  rs485_remote_loop();  // Handle RS485 Remote communication
#endif
```

### 3. Build and Upload

**For ANTENNA Unit:**
```bash
./upload_antenna.sh /dev/cu.usbmodem1101
```

**For SHACK Unit:**
```bash
./upload_shack.sh /dev/cu.usbmodem1102
```

### 4. Test Communication

1. Power both units
2. Connect RS485 cable (A-A, B-B, 120Ω termination)
3. Open Serial Monitor on both units (115200 baud)
4. Verify:
   - Master broadcasts position every 100ms
   - Remote receives and displays position
   - Commands from Remote executed by Master

---

## 📊 Build Flags

Each environment uses specific build flags to select configuration:

**ANTENNA Unit:**
```ini
-DUSE_ROTATOR_FEATURES_MASTER
-DUSE_ROTATOR_PINS_MASTER
```

**SHACK Unit:**
```ini
-DUSE_ROTATOR_FEATURES_REMOTE
-DUSE_ROTATOR_PINS_REMOTE
```

---

## ⚠️ Important Notes

### Conditional Compilation Required

The existing K3NG code needs modification to:
1. Include the appropriate features/pins files based on build flags
2. Conditionally compile RS485 Master or Remote code
3. Exclude unnecessary features per unit

### Memory Considerations

- **ANTENNA Unit:** GPS + Tracking + RS485 = high RAM usage
- **SHACK Unit:** Display + UI + RS485 = moderate RAM usage
- Monitor RAM usage during testing and optimize if needed

### Serial Port Usage

**ANTENNA Unit:**
- Serial (USB): Debug output (115200)
- Serial1: RS485 communication (19200)
- Serial2: GPS input (9600 or GPS baud rate)

**SHACK Unit:**
- Serial (USB): PC control - Yaesu/Easycom (9600)
- Serial1: RS485 communication (19200)
- Serial2: Nextion display (9600) OR I2C LCD

---

## 🧪 Testing Checklist

### ANTENNA Unit
- [ ] Compiles without errors
- [ ] Motor control responds to RS485 commands
- [ ] Position sensors read correctly
- [ ] GPS data received on Serial2
- [ ] RS485 broadcasts position every 100ms
- [ ] Tracking calculations functional

### SHACK Unit
- [ ] Compiles without errors
- [ ] Display shows position from Master
- [ ] Buttons send RS485 commands to Master
- [ ] Preset encoders functional (if enabled)
- [ ] PC control via USB Serial works
- [ ] RS485 receives Master broadcasts

### Communication
- [ ] No RS485 collisions
- [ ] Latency < 10ms
- [ ] CRC errors = 0%
- [ ] Position updates smooth
- [ ] Commands executed correctly

---

## 📚 Related Documentation

- [RS485_FEATURES_SPLIT.md](RS485_FEATURES_SPLIT.md) - Feature allocation decisions
- [RS485_PINS_ALLOCATION.md](RS485_PINS_ALLOCATION.md) - Pin allocation details
- [PROGRESS.md](PROGRESS.md) - Development progress and phases

---

## 🔄 Next Steps

1. **Modify main K3NG code** to use conditional compilation
2. **Integrate RS485 classes** into main loop
3. **Test build** for both environments
4. **Debug compilation errors** if any
5. **Hardware testing** with real rotator
6. **Optimize RAM usage** if needed
7. **Field testing** over 100m RS485 cable

---

**Phase 4 Status:** Configuration files ready, integration pending
**Ready for:** Code integration and hardware testing
