/* ---------------------- K3NG RS485 Integration Wrapper - Implementation ---------------------- */

#include "k3ng_rs485_integration.h"

// Include RS485 configuration only if features are enabled
#if defined(FEATURE_RS485_MASTER) || defined(FEATURE_RS485_REMOTE)
  #include "rs485_config.h"
  #include "rs485_protocol.h"
#endif

#ifdef FEATURE_RS485_MASTER
  #include "rs485_master.h"

  // Access to K3NG global variables (read positions)
  extern float azimuth;
  extern float elevation;
  extern float target_azimuth;
  extern float target_elevation;
  extern byte azimuth_button_was_pushed;
  extern byte el_button_was_pushed;

  // Global RS485 Master state
  extern RotatorState rotatorState;
  extern unsigned long lastBroadcastTime;
#endif

#ifdef FEATURE_RS485_REMOTE
  #include "rs485_remote.h"

  // Access to K3NG global variables (write commands)
  extern float target_azimuth;
  extern float target_elevation;
  extern byte azimuth_button_was_pushed;
  extern byte el_button_was_pushed;

  // Global RS485 Remote state
  extern RotatorState remoteRotatorState;
  extern unsigned long lastRemoteUpdate;
#endif

/* ========== Setup Function ========== */

void rs485_wrapper_setup() {

  #ifdef FEATURE_RS485_MASTER
    // Initialize RS485 Master
    rs485_master_init();

    #ifdef DEBUG_RS485
      Serial.println(F("RS485 Master initialized"));
    #endif
  #endif

  #ifdef FEATURE_RS485_REMOTE
    // Initialize RS485 Remote
    rs485_remote_init();

    #ifdef DEBUG_RS485
      Serial.println(F("RS485 Remote initialized"));
    #endif
  #endif

}

/* ========== Loop Function ========== */

void rs485_wrapper_loop() {

  #ifdef FEATURE_RS485_MASTER
    // Update rotator state from K3NG variables
    rotatorState.azimuth = azimuth;
    rotatorState.elevation = elevation;
    rotatorState.target_azimuth = target_azimuth;
    rotatorState.target_elevation = target_elevation;
    rotatorState.status = 0;

    // Set status flags
    if (azimuth_button_was_pushed) {
      rotatorState.status |= STATUS_ROTATING_AZ;
    }
    if (el_button_was_pushed) {
      rotatorState.status |= STATUS_ROTATING_EL;
    }

    // Service RS485 Master (broadcasts position, handles commands)
    rs485_master_process();
  #endif

  #ifdef FEATURE_RS485_REMOTE
    // Service RS485 Remote (receives position, sends commands)
    rs485_remote_process();

    // Update K3NG target positions from RS485 (if remote control is active)
    // Note: This would need additional logic to determine when to accept
    // remote commands vs local control. For now, we just listen.
    // The actual command sending is done from K3NG's button/preset handlers
    // which would need to call RS485 send functions.
  #endif

}
