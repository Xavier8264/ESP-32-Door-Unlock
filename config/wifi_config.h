#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// =====================================
// WiFi Configuration for AITRIP ESP32
// =====================================
// Replace with your WiFi network credentials
const char* WIFI_SSID = "YOUR_WIFI_NETWORK_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// =====================================
// Primary Authentication Configuration
// =====================================
// ⚠️ CRITICAL: Change these default credentials for security!
// Use strong passwords with mix of letters, numbers, and symbols
const char* AUTH_USERNAME = "admin";
const char* AUTH_PASSWORD = "DoorUnlock2025!@#";  // CHANGE THIS!

// =====================================
// Secondary User (Optional)
// =====================================
// Uncomment to enable second user account
//#define ENABLE_SECOND_USER
#ifdef ENABLE_SECOND_USER
const char* AUTH_USERNAME2 = "user";
const char* AUTH_PASSWORD2 = "UserPass2025!@#";  // CHANGE THIS!
#endif

// =====================================
// Admin Override (Emergency Access)
// =====================================
// Master password for emergency access (use very strong password)
const char* ADMIN_OVERRIDE_PASSWORD = "EmergencyAccess2025!@#$%";  // CHANGE THIS!

#endif