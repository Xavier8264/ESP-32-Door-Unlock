#ifndef SECURITY_CONFIG_H
#define SECURITY_CONFIG_H

// =====================================
// Enhanced Security Configuration
// =====================================

// Session Management
#define SESSION_TIMEOUT_MINUTES 30          // Session expires after 30 minutes
#define MAX_CONCURRENT_SESSIONS 2           // Maximum simultaneous logins
#define SESSION_REFRESH_INTERVAL 300000     // Refresh session every 5 minutes (ms)

// Rate Limiting and Brute Force Protection
#define MAX_LOGIN_ATTEMPTS 3                // Max attempts before temporary lockout
#define LOCKOUT_DURATION_MINUTES 15         // Lockout duration after max attempts
#define MAX_FAILED_ATTEMPTS_PERMANENT 5     // Permanent lockout after this many failures
#define RATE_LIMIT_WINDOW_SECONDS 60        // Time window for rate limiting
#define MAX_REQUESTS_PER_WINDOW 10          // Max requests per time window

// Password Security
#define MIN_PASSWORD_LENGTH 8               // Minimum password length
#define REQUIRE_STRONG_PASSWORDS true       // Require uppercase, lowercase, numbers, symbols
#define PASSWORD_CHANGE_REMINDER_DAYS 90    // Remind to change password after 90 days

// Access Control
#define ENABLE_IP_WHITELIST false           // Set to true to enable IP restrictions
#ifdef ENABLE_IP_WHITELIST
// Add trusted IP addresses (modify for your network)
const char* TRUSTED_IPS[] = {
  "192.168.1.100",  // Example: Your phone's static IP
  "192.168.1.101",  // Example: Another trusted device
  // Add more IPs as needed
};
const int TRUSTED_IP_COUNT = 2;
#endif

// Logging and Monitoring
#define ENABLE_ACTIVITY_LOGGING true        // Log all access attempts
#define LOG_FAILED_ATTEMPTS true            // Log failed login attempts
#define LOG_SUCCESSFUL_UNLOCKS true         // Log successful door unlocks
#define MAX_LOG_ENTRIES 100                 // Maximum log entries to store
#define ENABLE_SERIAL_SECURITY_LOGS true    // Output security logs to Serial

// Auto-Lock and Timing
#define AUTO_LOCK_DELAY_SECONDS 5           // Auto-lock after 5 seconds
#define UNLOCK_DURATION_SECONDS 10          // Maximum unlock duration
#define ENABLE_SCHEDULED_ACCESS false       // Enable time-based access control

#ifdef ENABLE_SCHEDULED_ACCESS
// Access schedule (24-hour format)
#define ACCESS_START_HOUR 6                 // Allow access from 6 AM
#define ACCESS_END_HOUR 23                  // Allow access until 11 PM
#define WEEKEND_ACCESS_DIFFERENT false      // Different schedule for weekends
#endif

// Hardware Security
#define ENABLE_TAMPER_DETECTION false       // Enable if tamper sensor connected
#define TAMPER_SENSOR_PIN 15                // GPIO pin for tamper sensor
#define ENABLE_DOOR_SENSOR false            // Enable door position sensor
#define DOOR_SENSOR_PIN 4                   // GPIO pin for door sensor

// Network Security
#define ENABLE_HTTPS false                  // Enable HTTPS (requires certificates)
#define CUSTOM_WEB_PORT 80                  // Custom port (80 = default HTTP)
#define ENABLE_CORS_PROTECTION true        // Enable Cross-Origin Request protection
#define HIDE_SERVER_SIGNATURE true         // Hide server information in headers

// Emergency Features
#define ENABLE_PANIC_MODE false             // Emergency lockdown mode
#define PANIC_MODE_PIN 12                   // GPIO pin for panic button
#define ENABLE_POWER_FAILURE_SAFE true     // Secure behavior on power loss
#define ENABLE_MANUAL_OVERRIDE_DETECT true // Detect manual key usage

// Notification Settings (for future expansion)
#define ENABLE_EMAIL_ALERTS false          // Email notifications (requires internet)
#define ENABLE_SMS_ALERTS false            // SMS notifications (requires service)
#define ENABLE_PUSH_NOTIFICATIONS false    // Push notifications to phone

// System Health Monitoring
#define ENABLE_WATCHDOG true                // Enable system watchdog
#define WATCHDOG_TIMEOUT_SECONDS 30        // Reset if system hangs for 30 seconds
#define ENABLE_MEMORY_MONITORING true      // Monitor memory usage
#define MIN_FREE_HEAP 10000                 // Minimum free heap memory (bytes)

// Security Token Configuration
#define TOKEN_LENGTH 32                     // Length of session tokens
#define TOKEN_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
#define ENABLE_TOKEN_ENCRYPTION true       // Encrypt session tokens

// Debug and Development
#define SECURITY_DEBUG_MODE false          // Enable detailed security debugging
#define ALLOW_WEAK_PASSWORDS_DEBUG false   // Allow weak passwords in debug mode
#define DISABLE_LOCKOUTS_DEBUG false       // Disable lockouts in debug mode

#endif