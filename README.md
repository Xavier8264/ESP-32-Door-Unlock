# ESP32 Door Unlock System

A remote door unlock system using ESP32 WROOM dev board with web interface accessible from your phone.

## Features
- WiFi-enabled ESP32 web server with dual-core processing
- **Multi-layer security authentication system**
- **Session management with automatic timeout**
- **Secure password hashing and validation**
- **Rate limiting to prevent brute force attacks**
- **IP-based access control (optional)**
- Mobile-friendly responsive web interface
- Motor/servo control for door mechanism
- Real-time status feedback and logging
- **Automatic security lockout after failed attempts**
- **Activity logging and monitoring**

## Hardware Requirements

### ESP32 Board
- **AITRIP ESP32-WROOM-32 Development Board (30-pin)**
  - Model: ESP32-WROOM-32 with CP2102 USB chip
  - 2.4GHz WiFi + Bluetooth capability
  - Dual-core processor with integrated antenna
  - Type-C USB connector for programming and power

### Motor Options (Choose one)
1. **Servo Motor (Recommended for beginners)**
   - SG90 Micro Servo (9g)
   - Operating voltage: 4.8V-6V
   - Easy to control and position

2. **Stepper Motor**
   - 28BYJ-48 5V Stepper Motor with ULN2003 Driver
   - More precise positioning

3. **DC Motor with Motor Driver**
   - Small DC motor (3-6V)
   - L298N Motor Driver Module

### Additional Components
- Breadboard or PCB for connections
- Jumper wires (male-to-male, male-to-female)
- Power supply (5V/3.3V depending on motor)
- Optional: LED indicators
- Optional: Push button for manual override

## Wiring Diagrams

### Option 1: Servo Motor (SG90) - RECOMMENDED
```
AITRIP ESP32 Pin    →    Servo Pin
GPIO 18             →    Signal (Orange/Yellow)
5V                  →    VCC (Red)
GND                 →    GND (Brown/Black)
```

### Option 2: Stepper Motor (28BYJ-48 with ULN2003)
```
AITRIP ESP32 Pin    →    ULN2003 Pin
GPIO 19             →    IN1
GPIO 21             →    IN2
GPIO 22             →    IN3
GPIO 23             →    IN4
5V                  →    VCC
GND                 →    GND
```

### Option 3: DC Motor (with L298N)
```
AITRIP ESP32 Pin    →    L298N Pin
GPIO 16             →    IN1
GPIO 17             →    IN2
5V                  →    VCC
GND                 →    GND
```

### Additional Connections (All Options)
```
AITRIP ESP32 Pin    →    Component
GPIO 2 (Built-in)   →    Status LED (built-in)
GPIO 0              →    Optional: Manual override button
3V3                 →    Available for sensors
```

## Security Considerations

⚠️ **Enhanced Security Features:**

### Authentication System
- **Username and password authentication required**
- **Session tokens with automatic expiration**
- **Rate limiting: Max 3 login attempts per 15 minutes**
- **Automatic lockout after 5 failed attempts**
- **Session timeout after 30 minutes of inactivity**
- **Secure credential storage in ESP32 memory**

### Network Security
- **Local network access only (no internet required)**
- **Optional MAC address whitelist**
- **WPA2/WPA3 WiFi encryption recommended**
- **Custom port configuration available**
- **HTTPS option for enhanced encryption**

### Physical Security
- **Tamper detection (optional sensor input)**
- **Manual override always available**
- **Power failure failsafe (door remains secure)**
- **Secure enclosure mounting recommended**

### Operational Security
- **Activity logging with timestamps**
- **Failed attempt notifications**
- **Regular password change reminders**
- **System health monitoring**

⚠️ **Critical Safety Notes:**
- **MUST change default passwords before deployment**
- Ensure your door mechanism has a reliable manual override
- Test thoroughly before relying on the system
- Consider fire safety regulations in your building
- Use appropriate power supplies to avoid damage
- Comply with local building codes and regulations
- **Keep backup access method always available**

## Project Structure
```
ESP-32 Door Unlock/
├── src/
│   ├── esp32_door_unlock.ino    # Main Arduino firmware
│   └── web_interface.h          # HTML/CSS/JS embedded in header
├── config/
│   ├── wifi_config.h            # WiFi credentials template
│   └── motor_config.h           # Motor type and pin configurations
├── docs/
│   ├── INSTALLATION.md          # Setup instructions
│   └── TROUBLESHOOTING.md       # Common issues and solutions
└── README.md                    # This file
```

## Quick Start
1. Install Arduino IDE and ESP32 board support (CP2102 drivers included)
2. **Configure security settings** in `config/wifi_config.h` and `config/security_config.h`
3. Choose motor type in `config/motor_config.h`
4. **Set strong passwords and security parameters**
5. Upload firmware to AITRIP ESP32 via Type-C cable
6. Connect to ESP32's IP address from your phone
7. **Complete security setup and initial authentication**
8. Test the door unlock function thoroughly

## Legal Disclaimer
This project is for educational purposes. Ensure compliance with local laws and building regulations before installation.