#ifndef MOTOR_CONFIG_H
#define MOTOR_CONFIG_H

// Motor Type Selection (uncomment ONE option)
#define SERVO_MOTOR 1
//#define STEPPER_MOTOR 2
//#define DC_MOTOR 3

// Set your motor type here
#define MOTOR_TYPE SERVO_MOTOR

// =====================================
// SERVO MOTOR CONFIGURATION
// =====================================
#if MOTOR_TYPE == SERVO_MOTOR
  #define SERVO_PIN 18
  #define SERVO_LOCK_POSITION 0      // Angle for locked position (0-180)
  #define SERVO_UNLOCK_POSITION 90   // Angle for unlocked position (0-180)
#endif

// =====================================
// STEPPER MOTOR CONFIGURATION
// =====================================
#if MOTOR_TYPE == STEPPER_MOTOR
  #define STEPPER_PIN1 19
  #define STEPPER_PIN2 21
  #define STEPPER_PIN3 22
  #define STEPPER_PIN4 23
  #define STEPPER_UNLOCK_STEPS 512   // Number of steps to unlock (512 = full rotation for 28BYJ-48)
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