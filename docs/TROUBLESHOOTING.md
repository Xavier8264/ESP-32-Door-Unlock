# Troubleshooting Guide - ESP32 Door Unlock System

## Common Issues and Solutions

### 🔧 Connection Issues

#### ESP32 Won't Connect to WiFi

**Symptoms:**
- Serial monitor shows "Connecting to WiFi..." indefinitely
- No IP address displayed

**Solutions:**
1. **Check WiFi Credentials**
   ```cpp
   // Verify in config/wifi_config.h
   const char* WIFI_SSID = "YourExactNetworkName";
   const char* WIFI_PASSWORD = "YourExactPassword";
   ```

2. **WiFi Network Compatibility**
   - ESP32 only supports 2.4GHz networks (not 5GHz)
   - Ensure network name doesn't contain special characters
   - Try connecting to a mobile hotspot for testing

3. **Signal Strength**
   - Move ESP32 closer to router
   - Check for interference from other devices
   - Ensure router is broadcasting SSID

4. **Reset Network Settings**
   ```cpp
   // Add this in setup() before WiFi.begin()
   WiFi.disconnect(true);
   delay(1000);
   ```

#### Cannot Access Web Interface

**Symptoms:**
- ESP32 connects to WiFi but website won't load
- Browser shows "This site can't be reached"

**Solutions:**
1. **Verify IP Address**
   - Check Serial Monitor for correct IP
   - Try accessing: `http://192.168.1.XXX` (not `https://`)

2. **Firewall Issues**
   - Temporarily disable phone's VPN
   - Connect phone to same WiFi network as ESP32
   - Try from different device

3. **Browser Cache**
   - Clear browser cache and cookies
   - Try private/incognito mode
   - Use different browser

### ⚡ Power and Hardware Issues

#### ESP32 Keeps Restarting

**Symptoms:**
- Continuous reset loop
- "Brownout detector" messages in Serial Monitor

**Solutions:**
1. **Power Supply Issues**
   - Use quality USB cable
   - Try different power source (computer USB port vs. wall adapter)
   - Check if motor is drawing too much current

2. **Wiring Problems**
   - Verify all connections are secure
   - Check for short circuits
   - Ensure proper grounding

3. **Code Issues**
   - Add delay in main loop: `delay(10);`
   - Comment out motor control code to isolate issue

#### Motor Not Working

**Symptoms:**
- Web interface loads but motor doesn't move
- No response when clicking unlock button

**Solutions:**
1. **Servo Motor Issues**
   ```cpp
   // Test servo with simple code
   #include <ESP32Servo.h>
   Servo testServo;
   
   void setup() {
     testServo.attach(18);
     testServo.write(90);
   }
   ```

2. **Power Requirements**
   - Servo motors may need external 5V power
   - Use common ground between ESP32 and servo power
   - Check voltage requirements in motor datasheet

3. **Pin Configuration**
   - Verify correct GPIO pins in `motor_config.h`
   - Some pins are reserved for internal use
   - Try different GPIO pins if current ones don't work

### 🔐 Authentication Issues

#### Login Not Working

**Symptoms:**
- "Invalid credentials" message
- Login form doesn't respond

**Solutions:**
1. **Check Credentials**
   - Verify username/password in `wifi_config.h`
   - Check for extra spaces or hidden characters
   - Case-sensitive authentication

2. **Form Data Issues**
   ```javascript
   // Check browser developer tools for network errors
   // Look for failed POST requests to /login
   ```

3. **Session Timeout**
   - Default timeout is 30 minutes
   - Refresh page and login again

#### Authentication Timeout

**Symptoms:**
- Automatically redirected to login page
- "Not authenticated" errors

**Solutions:**
1. **Extend Timeout**
   ```cpp
   // In main code, increase timeout
   unsigned long authTimeout = 60 * 60 * 1000; // 60 minutes
   ```

2. **Keep Session Active**
   - Web interface automatically checks status every 2 seconds
   - Don't close browser tab
   - Interact with interface periodically

### 📱 Mobile Interface Issues

#### Interface Not Mobile-Friendly

**Symptoms:**
- Text too small on phone
- Buttons hard to press
- Layout looks broken

**Solutions:**
1. **Viewport Settings**
   - Clear browser cache
   - Try landscape orientation
   - Zoom in if needed

2. **Browser Compatibility**
   - Try different mobile browser
   - Update browser to latest version
   - Use Chrome or Safari for best compatibility

#### Slow Response on Phone

**Symptoms:**
- Long delay when clicking unlock button
- Interface feels sluggish

**Solutions:**
1. **Network Issues**
   - Check WiFi signal strength on phone
   - Move closer to router
   - Restart WiFi on phone

2. **ESP32 Performance**
   - Increase delay in main loop if too aggressive
   - Check for memory leaks in Serial Monitor

### 🔧 Compilation Errors

#### Library Not Found

**Error Message:**
```
fatal error: ESP32Servo.h: No such file or directory
```

**Solution:**
1. Install ESP32Servo library:
   - Tools → Manage Libraries
   - Search "ESP32Servo"
   - Install by Kevin Harrington

#### Board Not Selected

**Error Message:**
```
Please select a valid board connected to your computer
```

**Solution:**
1. **Install ESP32 Board Support**
   - File → Preferences
   - Add board manager URL
   - Tools → Board → Boards Manager
   - Install ESP32 by Espressif Systems

2. **Select Correct Board**
   - Tools → Board → ESP32 Arduino → ESP32 Dev Module
   - Tools → Port → Select correct COM port

#### Compilation Timeout

**Error Message:**
```
Error compiling for board ESP32 Dev Module
```

**Solution:**
1. **File Path Issues**
   - Ensure no spaces in folder path
   - Move project to shorter path
   - Avoid special characters in folder names

2. **IDE Issues**
   - Restart Arduino IDE
   - Clear cache: Close IDE, delete temp files
   - Try different Arduino IDE version

### 🔄 System Reset and Recovery

#### Factory Reset ESP32

**When to Use:**
- System behaves unexpectedly
- Need to start fresh
- Corrupted configuration

**Steps:**
1. **Hardware Reset**
   - Hold BOOT button
   - Press and release RESET button
   - Release BOOT button

2. **Erase Flash Memory**
   ```bash
   # Using esptool (if installed)
   esptool.py --chip esp32 erase_flash
   ```

3. **Re-upload Firmware**
   - Upload code again through Arduino IDE

#### Recover from Bad Configuration

**If ESP32 won't connect after config changes:**

1. **Safe Mode Upload**
   - Hold BOOT button during upload
   - Upload minimal code first
   - Then upload full code

2. **Hardcoded Fallback**
   ```cpp
   // Add fallback WiFi in main code
   if (WiFi.status() != WL_CONNECTED) {
     WiFi.begin("fallback_network", "fallback_password");
   }
   ```

### 📊 Diagnostic Tools

#### Serial Monitor Debugging

**Enable Verbose Output:**
```cpp
// Add to setup()
Serial.setDebugOutput(true);
```

**Check Memory Usage:**
```cpp
// Add to loop()
Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
```

#### Network Diagnostics

**Check WiFi Status:**
```cpp
void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal: ");
  Serial.println(WiFi.RSSI());
}
```

#### Hardware Testing

**Test Individual Components:**
```cpp
// Test LED
digitalWrite(LED_BUILTIN, HIGH);
delay(1000);
digitalWrite(LED_BUILTIN, LOW);

// Test motor pins
digitalWrite(SERVO_PIN, HIGH);
delay(100);
digitalWrite(SERVO_PIN, LOW);
```

## Getting Help

### Before Asking for Help
1. Check Serial Monitor output
2. Verify all connections
3. Test with minimal code
4. Try different power source
5. Clear browser cache

### Information to Include
- ESP32 board model
- Arduino IDE version
- Complete error messages
- Serial Monitor output
- Hardware setup photos
- Network configuration

### Useful Resources
- ESP32 Arduino Core Documentation
- ESP32 Servo Library GitHub
- Arduino Community Forums
- ESP32 Technical Reference Manual

## Emergency Procedures

### Manual Door Override
- Always maintain manual key access
- Document emergency unlock procedure
- Test manual override regularly

### System Shutdown
1. Disconnect power to ESP32
2. Motor will stop immediately
3. Manual override should always work

### Recovery Mode
If system becomes completely unresponsive:
1. Power cycle ESP32
2. Check for hardware shorts
3. Re-upload firmware
4. Verify all connections