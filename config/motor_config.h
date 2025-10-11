#ifndef MOTOR_CONFIG_H
#define MOTOR_CONFIG_H

// Motor Type Selection (uncomment ONE option)
#define SERVO_MOTOR 1
#define STEPPER_MOTOR 2
#define DC_MOTOR 3

// Set your motor type here
#define MOTOR_TYPE STEPPER_MOTOR

// =====================================
// SERVO MOTOR CONFIGURATION
// =====================================
#if MOTOR_TYPE == SERVO_MOTOR
  #define SERVO_PIN 18
  #define SERVO_LOCK_POSITION 0      // Angle for locked position (0-180)
  #define SERVO_UNLOCK_POSITION 90   // Angle for unlocked position (0-180)
#endif

// =====================================
// STEPPER MOTOR CONFIGURATION (Nema 17 + A4988/DRV8825)
// =====================================
#if MOTOR_TYPE == STEPPER_MOTOR
  #define STEPPER_STEP_PIN 19      // Connect to STEP on driver
  #define STEPPER_DIR_PIN 21       // Connect to DIR on driver
  #define STEPPER_ENABLE_PIN 22    // (Optional) Connect to EN on driver
  #define STEPPER_UNLOCK_STEPS 200 // Number of steps to unlock (1 rev = 200 steps for 1.8°/step motor)
  #define STEPPER_LOCK_STEPS   200 // Number of steps to lock (reverse direction)
  #define STEPPER_STEP_DELAY_US 1000 // Microseconds between steps (controls speed)
#endif

// =====================================
// DC MOTOR CONFIGURATION
// =====================================
#if MOTOR_TYPE == DC_MOTOR
  #define DC_MOTOR_PIN1 16
  #define DC_MOTOR_PIN2 17
  #define DC_MOTOR_UNLOCK_TIME 1000  // Time in milliseconds to run motor
#endif

// =====================================
// GENERAL SETTINGS
// =====================================
// Auto-lock delay (milliseconds)
#define AUTO_LOCK_DELAY 5000

// Authentication timeout (milliseconds)
#define AUTH_TIMEOUT (30 * 60 * 1000)  // 30 minutes

#endif