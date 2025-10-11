#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <EEPROM.h>
#include <esp_system.h>
#include <esp_random.h>
#include "config/wifi_config.h"
#include "config/motor_config.h"
#include "config/security_config.h"
#include "web_interface.h"

// Create server and servo objects
WebServer server(CUSTOM_WEB_PORT);
Servo doorServo;

// Enhanced Security Structures
struct LoginAttempt {
  unsigned long timestamp;
  String ipAddress;
  bool successful;
};

struct Session {
  String token;
  String ipAddress;
  unsigned long createdAt;
  unsigned long lastActivity;
  bool isActive;
};

struct SecurityLog {
  unsigned long timestamp;
  String event;
  String ipAddress;
  String details;
};

// Security variables
Session activeSessions[MAX_CONCURRENT_SESSIONS];
LoginAttempt recentAttempts[MAX_LOGIN_ATTEMPTS * 2];
SecurityLog securityLogs[MAX_LOG_ENTRIES];
int logIndex = 0;
int attemptIndex = 0;

// Rate limiting
unsigned long lastRequestTime[10];  // Track recent requests
String lastRequestIPs[10];
int requestIndex = 0;

// System state
bool systemLocked = false;
unsigned long systemLockTime = 0;
bool doorLocked = true;
unsigned long lastUnlockTime = 0;
int failedAttemptCount = 0;
unsigned long lastFailedAttempt = 0;

// Enhanced function prototypes
void handleRoot();
void handleLogin();
void handleUnlock();
void handleStatus();
void handleLogout();
void handleNotFound();
bool checkAuth();
bool checkRateLimit(String ipAddress);
bool isValidCredentials(String username, String password);
String generateSessionToken();
bool addSession(String token, String ipAddress);
void removeSession(String token);
Session* findSession(String token);
void cleanExpiredSessions();
void logSecurityEvent(String event, String ipAddress, String details = "");
void logLoginAttempt(String ipAddress, bool successful);
bool isSystemLocked();
void lockSystem();
void unlockSystem();
bool isIPTrusted(String ipAddress);
void unlockDoor();
void lockDoor();
void setupMotor();
void blinkLED(int times);
void initializeSecurity();
void handleSecurityCheck();
String hashPassword(String password);
bool verifyPassword(String input, String stored);

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Door Unlock System Starting...");
  Serial.println("Model: AITRIP ESP32-WROOM-32 (30-pin, CP2102)");
  
  // Initialize EEPROM for persistent storage
  EEPROM.begin(512);
  
  // Initialize security system
  initializeSecurity();
  
  // Initialize built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Setup motor
  setupMotor();
  
  // Initialize optional security sensors
  #if ENABLE_TAMPER_DETECTION
    pinMode(TAMPER_SENSOR_PIN, INPUT_PULLUP);
    Serial.println("Tamper detection enabled");
  #endif
  
  #if ENABLE_DOOR_SENSOR
    pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);
    Serial.println("Door sensor enabled");
  #endif
  
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 30) {
    delay(500);
    Serial.print(".");
    blinkLED(1);
    wifiAttempts++;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to WiFi!");
    Serial.println("Please check credentials in wifi_config.h");
    return;
  }
  
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Web server port: ");
  Serial.println(CUSTOM_WEB_PORT);
  
  // Log system startup
  logSecurityEvent("SYSTEM_START", WiFi.localIP().toString(), "ESP32 Door System Initialized");
  
  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/unlock", HTTP_POST, handleUnlock);
  server.on("/status", handleStatus);
  server.on("/logout", HTTP_POST, handleLogout);
  server.onNotFound(handleNotFound);
  
  // Start server
  server.begin();
  Serial.println("Secure web server started");
  Serial.println("Security features enabled:");
  Serial.printf("- Session timeout: %d minutes\n", SESSION_TIMEOUT_MINUTES);
  Serial.printf("- Max login attempts: %d\n", MAX_LOGIN_ATTEMPTS);
  Serial.printf("- Rate limiting: %d requests per %d seconds\n", MAX_REQUESTS_PER_WINDOW, RATE_LIMIT_WINDOW_SECONDS);
  
  // Initial door state (locked)
  lockDoor();
  
  // Signal ready with LED pattern
  for(int i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
  
  Serial.println("System ready - Enhanced security active");
}

void loop() {
  server.handleClient();
  
  // Security maintenance tasks
  cleanExpiredSessions();
  handleSecurityCheck();
  
  // Check for system lockout expiration
  if (isSystemLocked() && (millis() - systemLockTime > (LOCKOUT_DURATION_MINUTES * 60000))) {
    unlockSystem();
  }
  
  // Auto-lock door after delay
  if (!doorLocked && (millis() - lastUnlockTime > (AUTO_LOCK_DELAY_SECONDS * 1000))) {
    lockDoor();
    logSecurityEvent("AUTO_LOCK", "SYSTEM", "Door automatically locked");
    Serial.println("Auto-locking door");
  }
  
  // Security sensor monitoring
  #if ENABLE_TAMPER_DETECTION
    if (digitalRead(TAMPER_SENSOR_PIN) == LOW) {
      logSecurityEvent("TAMPER_DETECTED", "SENSOR", "Tamper sensor triggered");
      Serial.println("SECURITY ALERT: Tamper detected!");
      blinkLED(10);  // Alert pattern
    }
  #endif
  
  // Memory monitoring
  #if ENABLE_MEMORY_MONITORING
    if (ESP.getFreeHeap() < MIN_FREE_HEAP) {
      Serial.printf("WARNING: Low memory - Free heap: %d bytes\n", ESP.getFreeHeap());
      logSecurityEvent("LOW_MEMORY", "SYSTEM", "Free heap: " + String(ESP.getFreeHeap()));
    }
  #endif
  
  delay(10);
}

void setupMotor() {
  #if MOTOR_TYPE == SERVO_MOTOR
    doorServo.attach(SERVO_PIN);
    doorServo.write(SERVO_LOCK_POSITION);
    Serial.println("Servo motor initialized");
    
  #elif MOTOR_TYPE == STEPPER_MOTOR
    pinMode(STEPPER_PIN1, OUTPUT);
    pinMode(STEPPER_PIN2, OUTPUT);
    pinMode(STEPPER_PIN3, OUTPUT);
    pinMode(STEPPER_PIN4, OUTPUT);
    Serial.println("Stepper motor initialized");
    
  #elif MOTOR_TYPE == DC_MOTOR
    pinMode(DC_MOTOR_PIN1, OUTPUT);
    pinMode(DC_MOTOR_PIN2, OUTPUT);
    digitalWrite(DC_MOTOR_PIN1, LOW);
    digitalWrite(DC_MOTOR_PIN2, LOW);
    Serial.println("DC motor initialized");
  #endif
}

void handleRoot() {
  String clientIP = server.client().remoteIP().toString();
  
  // Check rate limiting
  if (!checkRateLimit(clientIP)) {
    server.send(429, "application/json", "{\"success\":false,\"message\":\"Rate limit exceeded\"}");
    return;
  }
  
  // Check system lockout
  if (isSystemLocked()) {
    server.send(423, "text/html", LOCKOUT_PAGE);
    return;
  }
  
  // Check for valid session
  String token = server.header("Authorization");
  if (token.startsWith("Bearer ")) {
    token = token.substring(7);
  }
  
  Session* session = findSession(token);
  if (session != nullptr) {
    server.send(200, "text/html", MAIN_PAGE);
    logSecurityEvent("PAGE_ACCESS", clientIP, "Authenticated access");
  } else {
    server.send(200, "text/html", LOGIN_PAGE);
    logSecurityEvent("LOGIN_PAGE_ACCESS", clientIP, "Unauthenticated access");
  }
}

void handleLogin() {
  String clientIP = server.client().remoteIP().toString();
  
  // Check rate limiting
  if (!checkRateLimit(clientIP)) {
    server.send(429, "application/json", "{\"success\":false,\"message\":\"Too many requests\"}");
    return;
  }
  
  // Check system lockout
  if (isSystemLocked()) {
    server.send(423, "application/json", "{\"success\":false,\"message\":\"System temporarily locked\"}");
    return;
  }
  
  String username = server.arg("username");
  String password = server.arg("password");
  
  // Validate credentials
  if (isValidCredentials(username, password)) {
    // Generate session token
    String token = generateSessionToken();
    
    if (addSession(token, clientIP)) {
      logSecurityEvent("LOGIN_SUCCESS", clientIP, "User: " + username);
      logLoginAttempt(clientIP, true);
      
      String response = "{\"success\":true,\"message\":\"Login successful\",\"token\":\"" + token + "\"}";
      server.send(200, "application/json", response);
      Serial.printf("Successful login from %s (User: %s)\n", clientIP.c_str(), username.c_str());
    } else {
      server.send(500, "application/json", "{\"success\":false,\"message\":\"Session creation failed\"}");
    }
  } else {
    logSecurityEvent("LOGIN_FAILED", clientIP, "User: " + username);
    logLoginAttempt(clientIP, false);
    server.send(401, "application/json", "{\"success\":false,\"message\":\"Invalid credentials\"}");
    Serial.printf("Failed login attempt from %s (User: %s)\n", clientIP.c_str(), username.c_str());
  }
}

void handleUnlock() {
  String clientIP = server.client().remoteIP().toString();
  
  // Check rate limiting
  if (!checkRateLimit(clientIP)) {
    server.send(429, "application/json", "{\"success\":false,\"message\":\"Rate limit exceeded\"}");
    return;
  }
  
  // Check system lockout
  if (isSystemLocked()) {
    server.send(423, "application/json", "{\"success\":false,\"message\":\"System locked\"}");
    return;
  }
  
  // Verify session
  String token = server.header("Authorization");
  if (token.startsWith("Bearer ")) {
    token = token.substring(7);
  }
  
  Session* session = findSession(token);
  if (session == nullptr) {
    server.send(401, "application/json", "{\"success\":false,\"message\":\"Invalid session\"}");
    return;
  }
  
  // Verify IP matches session
  if (session->ipAddress != clientIP) {
    logSecurityEvent("IP_MISMATCH", clientIP, "Session IP: " + session->ipAddress);
    removeSession(token);
    server.send(401, "application/json", "{\"success\":false,\"message\":\"Security violation\"}");
    return;
  }
  
  // Execute unlock
  unlockDoor();
  logSecurityEvent("DOOR_UNLOCK", clientIP, "Door unlocked successfully");
  server.send(200, "application/json", "{\"success\":true,\"message\":\"Door unlocked\"}");
  Serial.printf("Door unlock command from %s\n", clientIP.c_str());
}

void handleStatus() {
  String clientIP = server.client().remoteIP().toString();
  
  // Check rate limiting (more lenient for status checks)
  if (!checkRateLimit(clientIP)) {
    server.send(429, "application/json", "{\"success\":false,\"message\":\"Rate limit exceeded\"}");
    return;
  }
  
  // Verify session
  String token = server.header("Authorization");
  if (token.startsWith("Bearer ")) {
    token = token.substring(7);
  }
  
  Session* session = findSession(token);
  if (session == nullptr) {
    server.send(401, "application/json", "{\"success\":false,\"message\":\"Invalid session\"}");
    return;
  }
  
  String status = doorLocked ? "locked" : "unlocked";
  String json = "{\"success\":true,\"doorStatus\":\"" + status + "\",\"ip\":\"" + WiFi.localIP().toString() + 
                "\",\"uptime\":" + String(millis()) + ",\"freeHeap\":" + String(ESP.getFreeHeap()) + "}";
  server.send(200, "application/json", json);
}

void handleLogout() {
  String clientIP = server.client().remoteIP().toString();
  
  String token = server.header("Authorization");
  if (token.startsWith("Bearer ")) {
    token = token.substring(7);
  }
  
  removeSession(token);
  logSecurityEvent("LOGOUT", clientIP, "User logged out");
  server.send(200, "application/json", "{\"success\":true,\"message\":\"Logged out\"}");
}

void handleNotFound() {
  String clientIP = server.client().remoteIP().toString();
  logSecurityEvent("404_ACCESS", clientIP, "Path: " + server.uri());
  server.send(404, "text/plain", "Not Found");
}

bool checkAuth() {
  // This function is now replaced by individual session validation in handlers
  // Kept for compatibility but should not be used
  return false;
}

void unlockDoor() {
  doorLocked = false;
  lastUnlockTime = millis();
  
  #if MOTOR_TYPE == SERVO_MOTOR
    doorServo.write(SERVO_UNLOCK_POSITION);
    
  #elif MOTOR_TYPE == STEPPER_MOTOR
    // Rotate stepper motor to unlock position
    for(int i = 0; i < STEPPER_UNLOCK_STEPS; i++) {
      stepperStep(i % 4);
      delay(2);
    }
    
  #elif MOTOR_TYPE == DC_MOTOR
    // Run DC motor for unlock duration
    digitalWrite(DC_MOTOR_PIN1, HIGH);
    digitalWrite(DC_MOTOR_PIN2, LOW);
    delay(DC_MOTOR_UNLOCK_TIME);
    digitalWrite(DC_MOTOR_PIN1, LOW);
  #endif
  
  // Blink LED to indicate unlock
  blinkLED(3);
  Serial.println("Door unlocked");
}

void lockDoor() {
  doorLocked = true;
  
  #if MOTOR_TYPE == SERVO_MOTOR
    doorServo.write(SERVO_LOCK_POSITION);
    
  #elif MOTOR_TYPE == STEPPER_MOTOR
    // Rotate stepper motor back to lock position
    for(int i = 0; i < STEPPER_UNLOCK_STEPS; i++) {
      stepperStep(3 - (i % 4));
      delay(2);
    }
    
  #elif MOTOR_TYPE == DC_MOTOR
    // Run DC motor in reverse for lock duration
    digitalWrite(DC_MOTOR_PIN1, LOW);
    digitalWrite(DC_MOTOR_PIN2, HIGH);
    delay(DC_MOTOR_UNLOCK_TIME);
    digitalWrite(DC_MOTOR_PIN1, LOW);
    digitalWrite(DC_MOTOR_PIN2, LOW);
  #endif
  
  Serial.println("Door locked");
}

#if MOTOR_TYPE == STEPPER_MOTOR
void stepperStep(int step) {
  switch(step) {
    case 0:
      digitalWrite(STEPPER_PIN1, HIGH);
      digitalWrite(STEPPER_PIN2, LOW);
      digitalWrite(STEPPER_PIN3, LOW);
      digitalWrite(STEPPER_PIN4, LOW);
      break;
    case 1:
      digitalWrite(STEPPER_PIN1, LOW);
      digitalWrite(STEPPER_PIN2, HIGH);
      digitalWrite(STEPPER_PIN3, LOW);
      digitalWrite(STEPPER_PIN4, LOW);
      break;
    case 2:
      digitalWrite(STEPPER_PIN1, LOW);
      digitalWrite(STEPPER_PIN2, LOW);
      digitalWrite(STEPPER_PIN3, HIGH);
      digitalWrite(STEPPER_PIN4, LOW);
      break;
    case 3:
      digitalWrite(STEPPER_PIN1, LOW);
      digitalWrite(STEPPER_PIN2, LOW);
      digitalWrite(STEPPER_PIN3, LOW);
      digitalWrite(STEPPER_PIN4, HIGH);
      break;
  }
}
#endif

void blinkLED(int times) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
  }
}

// =====================================
// Enhanced Security Functions
// =====================================

void initializeSecurity() {
  // Initialize session array
  for(int i = 0; i < MAX_CONCURRENT_SESSIONS; i++) {
    activeSessions[i].isActive = false;
  }
  
  // Initialize rate limiting arrays
  for(int i = 0; i < 10; i++) {
    lastRequestTime[i] = 0;
    lastRequestIPs[i] = "";
  }
  
  // Initialize security logs
  for(int i = 0; i < MAX_LOG_ENTRIES; i++) {
    securityLogs[i].timestamp = 0;
  }
  
  Serial.println("Security system initialized");
}

bool checkRateLimit(String ipAddress) {
  unsigned long currentTime = millis();
  int requestCount = 0;
  
  // Count recent requests from this IP
  for(int i = 0; i < 10; i++) {
    if (lastRequestIPs[i] == ipAddress && 
        (currentTime - lastRequestTime[i]) < (RATE_LIMIT_WINDOW_SECONDS * 1000)) {
      requestCount++;
    }
  }
  
  if (requestCount >= MAX_REQUESTS_PER_WINDOW) {
    logSecurityEvent("RATE_LIMIT_EXCEEDED", ipAddress, "Requests: " + String(requestCount));
    return false;
  }
  
  // Record this request
  lastRequestTime[requestIndex] = currentTime;
  lastRequestIPs[requestIndex] = ipAddress;
  requestIndex = (requestIndex + 1) % 10;
  
  return true;
}

bool isValidCredentials(String username, String password) {
  // Check primary user
  if (username == AUTH_USERNAME && password == AUTH_PASSWORD) {
    return true;
  }
  
  #ifdef ENABLE_SECOND_USER
  // Check secondary user
  if (username == AUTH_USERNAME2 && password == AUTH_PASSWORD2) {
    return true;
  }
  #endif
  
  // Check admin override
  if (password == ADMIN_OVERRIDE_PASSWORD) {
    return true;
  }
  
  return false;
}

String generateSessionToken() {
  String token = "";
  const char* alphabet = TOKEN_ALPHABET;
  int alphabetLength = strlen(alphabet);
  
  for(int i = 0; i < TOKEN_LENGTH; i++) {
    token += alphabet[esp_random() % alphabetLength];
  }
  
  return token;
}

bool addSession(String token, String ipAddress) {
  // Find empty session slot
  for(int i = 0; i < MAX_CONCURRENT_SESSIONS; i++) {
    if (!activeSessions[i].isActive) {
      activeSessions[i].token = token;
      activeSessions[i].ipAddress = ipAddress;
      activeSessions[i].createdAt = millis();
      activeSessions[i].lastActivity = millis();
      activeSessions[i].isActive = true;
      return true;
    }
  }
  
  // No empty slots, remove oldest session
  int oldestIndex = 0;
  unsigned long oldestTime = activeSessions[0].createdAt;
  
  for(int i = 1; i < MAX_CONCURRENT_SESSIONS; i++) {
    if (activeSessions[i].createdAt < oldestTime) {
      oldestTime = activeSessions[i].createdAt;
      oldestIndex = i;
    }
  }
  
  // Replace oldest session
  activeSessions[oldestIndex].token = token;
  activeSessions[oldestIndex].ipAddress = ipAddress;
  activeSessions[oldestIndex].createdAt = millis();
  activeSessions[oldestIndex].lastActivity = millis();
  activeSessions[oldestIndex].isActive = true;
  
  return true;
}

void removeSession(String token) {
  for(int i = 0; i < MAX_CONCURRENT_SESSIONS; i++) {
    if (activeSessions[i].isActive && activeSessions[i].token == token) {
      activeSessions[i].isActive = false;
      break;
    }
  }
}

Session* findSession(String token) {
  for(int i = 0; i < MAX_CONCURRENT_SESSIONS; i++) {
    if (activeSessions[i].isActive && activeSessions[i].token == token) {
      activeSessions[i].lastActivity = millis();  // Update activity
      return &activeSessions[i];
    }
  }
  return nullptr;
}

void cleanExpiredSessions() {
  unsigned long currentTime = millis();
  unsigned long sessionTimeout = SESSION_TIMEOUT_MINUTES * 60000;
  
  for(int i = 0; i < MAX_CONCURRENT_SESSIONS; i++) {
    if (activeSessions[i].isActive && 
        (currentTime - activeSessions[i].lastActivity) > sessionTimeout) {
      logSecurityEvent("SESSION_EXPIRED", activeSessions[i].ipAddress, "Token: " + activeSessions[i].token.substring(0, 8) + "...");
      activeSessions[i].isActive = false;
    }
  }
}

void logSecurityEvent(String event, String ipAddress, String details) {
  #if ENABLE_ACTIVITY_LOGGING
  securityLogs[logIndex].timestamp = millis();
  securityLogs[logIndex].event = event;
  securityLogs[logIndex].ipAddress = ipAddress;
  securityLogs[logIndex].details = details;
  
  logIndex = (logIndex + 1) % MAX_LOG_ENTRIES;
  
  #if ENABLE_SERIAL_SECURITY_LOGS
  Serial.printf("[SECURITY] %s from %s: %s\n", event.c_str(), ipAddress.c_str(), details.c_str());
  #endif
  #endif
}

void logLoginAttempt(String ipAddress, bool successful) {
  recentAttempts[attemptIndex].timestamp = millis();
  recentAttempts[attemptIndex].ipAddress = ipAddress;
  recentAttempts[attemptIndex].successful = successful;
  
  attemptIndex = (attemptIndex + 1) % (MAX_LOGIN_ATTEMPTS * 2);
  
  if (!successful) {
    failedAttemptCount++;
    lastFailedAttempt = millis();
    
    // Check for permanent lockout
    if (failedAttemptCount >= MAX_FAILED_ATTEMPTS_PERMANENT) {
      lockSystem();
      logSecurityEvent("PERMANENT_LOCKOUT", ipAddress, "Too many failed attempts");
    }
  }
}

bool isSystemLocked() {
  if (systemLocked) return true;
  
  // Check for temporary lockout based on recent failed attempts
  unsigned long currentTime = millis();
  int recentFailures = 0;
  
  for(int i = 0; i < (MAX_LOGIN_ATTEMPTS * 2); i++) {
    if (!recentAttempts[i].successful && 
        (currentTime - recentAttempts[i].timestamp) < (LOCKOUT_DURATION_MINUTES * 60000)) {
      recentFailures++;
    }
  }
  
  return recentFailures >= MAX_LOGIN_ATTEMPTS;
}

void lockSystem() {
  systemLocked = true;
  systemLockTime = millis();
  logSecurityEvent("SYSTEM_LOCKED", "SECURITY", "System entering lockout mode");
  Serial.println("SECURITY: System locked due to multiple failed attempts");
}

void unlockSystem() {
  systemLocked = false;
  logSecurityEvent("SYSTEM_UNLOCKED", "SECURITY", "Lockout period expired");
  Serial.println("SECURITY: System lockout expired");
}

void handleSecurityCheck() {
  // Periodic security checks can be added here
  static unsigned long lastSecurityCheck = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastSecurityCheck > 60000) {  // Check every minute
    lastSecurityCheck = currentTime;
    
    // Log system health
    if (ENABLE_MEMORY_MONITORING) {
      if (ESP.getFreeHeap() < MIN_FREE_HEAP) {
        logSecurityEvent("LOW_MEMORY_WARNING", "SYSTEM", "Free heap: " + String(ESP.getFreeHeap()));
      }
    }
  }
}