# Installation Guide - ESP32 Door Unlock System

## Prerequisites

### Software Requirements
1. **Arduino IDE** (version 1.8.19 or later)
   - Download from: https://www.arduino.cc/en/software

2. **ESP32 Board Support**
   - Open Arduino IDE
   - Go to File → Preferences
   - Add this URL to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to Tools → Board → Boards Manager
   - Search for "ESP32" and install "ESP32 by Espressif Systems"

3. **Required Libraries**
   - ESP32Servo (for servo motor support)
   - WebServer (usually included with ESP32 board package)

### Hardware Requirements
- ESP32 WROOM Dev Board
- Motor (servo, stepper, or DC motor - see README.md for options)
- Breadboard and jumper wires
- Power supply appropriate for your motor
- USB cable for programming

## Step-by-Step Installation

### Step 1: Hardware Setup

Choose your motor type and follow the corresponding wiring diagram from the README.md file.

**Important Safety Notes:**
- Double-check all connections before powering on
- Ensure proper voltage levels for your motor
- Test the motor separately before connecting to ESP32

### Step 2: Enhanced Security Configuration

1. **Configure WiFi and Authentication**
   - Open `config/wifi_config.h`
   - Replace WiFi credentials with your network details
   - **CRITICAL:** Change all default passwords to strong, unique passwords
   
   ```cpp
   const char* WIFI_SSID = "YourWiFiName";
   const char* WIFI_PASSWORD = "YourWiFiPassword";
   
   // Use strong passwords with 12+ characters, mixed case, numbers, symbols
   const char* AUTH_USERNAME = "admin";
   const char* AUTH_PASSWORD = "YourStrongPassword123!@#";
   const char* ADMIN_OVERRIDE_PASSWORD = "EmergencyAccess456$%^";
   ```

2. **Configure Security Settings**
   - Open `config/security_config.h`
   - Review and adjust security parameters:
     - Session timeout duration
     - Maximum login attempts
     - Rate limiting settings
     - Enable/disable advanced features
   
   ```cpp
   #define SESSION_TIMEOUT_MINUTES 30
   #define MAX_LOGIN_ATTEMPTS 3
   #define LOCKOUT_DURATION_MINUTES 15
   #define ENABLE_IP_WHITELIST false  // Set true for IP restrictions
   ```

3. **Configure Motor Settings**
   - Open `config/motor_config.h`
   - Select your motor type (SERVO_MOTOR recommended for AITRIP ESP32)
   - Adjust pin assignments and motor parameters

### Step 3: Install Required Libraries

1. **Install ESP32Servo Library**
   - Open Arduino IDE
   - Go to Tools → Manage Libraries
   - Search for "ESP32Servo"
   - Install "ESP32Servo by Kevin Harrington"

### Step 4: Upload Firmware

1. **Connect ESP32**
   - Connect ESP32 to computer via USB
   - Select correct board: Tools → Board → ESP32 Arduino → ESP32 Dev Module
   - Select correct port: Tools → Port → (select your ESP32 port)

2. **Open and Upload Code**
   - Open `src/esp32_door_unlock.ino` in Arduino IDE
   - Click Upload button (or Ctrl+U)
   - Wait for compilation and upload to complete

3. **Monitor Serial Output**
   - Open Serial Monitor (Tools → Serial Monitor)
   - Set baud rate to 115200
   - You should see:
     ```
     ESP32 Door Unlock System Starting...
     Connecting to WiFi....
     Connected! IP address: 192.168.1.XXX
     Web server started
     ```

### Step 5: Test the System

1. **Note the IP Address**
   - From Serial Monitor, note the IP address displayed
   - Example: `192.168.1.123`

2. **Access Web Interface**
   - On your phone, open a web browser
   - Navigate to the IP address: `http://192.168.1.123`
   - You should see the login page

3. **Login and Test**
   - Use the credentials from `wifi_config.h`
   - Default: Username: `admin`, Password: `doorunlock123`
   - After login, test the unlock button
   - Verify motor movement and auto-lock functionality

## Initial Testing Checklist

- [ ] ESP32 connects to WiFi successfully
- [ ] Web interface loads on phone
- [ ] Login system works
- [ ] Motor responds to unlock command
- [ ] Auto-lock timer functions correctly
- [ ] LED indicators work properly
- [ ] Serial monitor shows expected output

## Network Configuration

### Finding ESP32 IP Address
If you lose the IP address, you can:

1. **Check Serial Monitor**
   - Reconnect to ESP32 via USB
   - Reset the board
   - IP address will be displayed on startup

2. **Check Router Admin Panel**
   - Access your router's admin interface
   - Look for connected devices
   - Find device named "ESP32" or similar

3. **Use Network Scanner**
   - Use apps like "Fing" on your phone
   - Scan your network for ESP32 device

### Static IP Configuration (Optional)
To assign a static IP address, add this to your Arduino code in the `setup()` function:

```cpp
IPAddress local_IP(192, 168, 1, 184);  // Desired static IP
IPAddress gateway(192, 168, 1, 1);     // Your router IP
IPAddress subnet(255, 255, 0, 0);      // Subnet mask

if (!WiFi.config(local_IP, gateway, subnet)) {
  Serial.println("STA Failed to configure");
}
```

## Security Considerations

### Change Default Credentials
**CRITICAL:** Always change the default username and password!

### WiFi Security
- Use WPA2 or WPA3 encrypted WiFi
- Consider using a guest network
- Regularly update WiFi passwords

### Physical Security
- Secure the ESP32 in an enclosure
- Protect wiring from tampering
- Ensure manual override mechanism

### Firewall Configuration
- Consider restricting access to local network only
- Use MAC address filtering if needed

## Mechanical Installation

### Door Lock Mechanism
1. **Servo Motor Installation**
   - Mount servo to door frame
   - Connect servo arm to door lock mechanism
   - Ensure smooth operation without binding
   - Test range of motion

2. **Power Supply**
   - Use appropriate voltage for your motor
   - Consider battery backup for power outages
   - Ensure stable power delivery

3. **Enclosure**
   - Use weatherproof enclosure if needed
   - Ensure adequate ventilation
   - Provide access to reset button

## Final System Test

1. **Complete Function Test**
   - Test from multiple devices
   - Verify timeout functionality
   - Test under various network conditions
   - Ensure mechanical reliability

2. **Emergency Procedures**
   - Document manual override procedure
   - Test emergency unlock methods
   - Ensure compliance with safety regulations

## Troubleshooting

If you encounter issues, refer to `TROUBLESHOOTING.md` for common problems and solutions.