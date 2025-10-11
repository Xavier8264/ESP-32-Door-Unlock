#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

// System lockout page
const char* LOCKOUT_PAGE = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>System Locked</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: linear-gradient(135deg, #e74c3c 0%, #c0392b 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 20px;
        }
        
        .lockout-container {
            background: white;
            padding: 40px;
            border-radius: 20px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.2);
            width: 100%;
            max-width: 400px;
            text-align: center;
        }
        
        .warning-icon {
            font-size: 80px;
            margin-bottom: 20px;
            color: #e74c3c;
            animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
            0%, 100% { transform: scale(1); }
            50% { transform: scale(1.1); }
        }
        
        h1 {
            color: #e74c3c;
            margin-bottom: 20px;
            font-size: 28px;
        }
        
        .message {
            color: #555;
            margin-bottom: 30px;
            line-height: 1.6;
        }
        
        .countdown {
            background: #fff5f5;
            padding: 20px;
            border-radius: 10px;
            border: 2px solid #fadbd8;
            margin-bottom: 20px;
        }
        
        .time-remaining {
            font-size: 24px;
            font-weight: bold;
            color: #e74c3c;
        }
    </style>
</head>
<body>
    <div class="lockout-container">
        <div class="warning-icon">🚨</div>
        <h1>System Locked</h1>
        <div class="message">
            The door control system has been temporarily locked due to multiple failed login attempts.
        </div>
        <div class="countdown">
            <div>Time remaining:</div>
            <div class="time-remaining" id="countdown">Loading...</div>
        </div>
        <div class="message">
            Please wait before attempting to access the system again. If you continue to experience issues, contact the system administrator.
        </div>
    </div>

    <script>
        // Auto-refresh page every 30 seconds
        setTimeout(() => {
            window.location.reload();
        }, 30000);
        
        document.getElementById('countdown').textContent = '15 minutes';
    </script>
</body>
</html>
)";

// Enhanced login page with security features
const char* LOGIN_PAGE = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Secure Door Access</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 20px;
        }
        
        .login-container {
            background: white;
            padding: 40px;
            border-radius: 20px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            width: 100%;
            max-width: 400px;
            text-align: center;
        }
        
        .security-badge {
            background: linear-gradient(135deg, #27ae60 0%, #2ecc71 100%);
            color: white;
            padding: 5px 15px;
            border-radius: 20px;
            font-size: 12px;
            font-weight: 600;
            display: inline-block;
            margin-bottom: 20px;
        }
        
        .lock-icon {
            font-size: 60px;
            margin-bottom: 20px;
            color: #667eea;
        }
        
        h1 {
            color: #333;
            margin-bottom: 30px;
            font-size: 28px;
        }
        
        .security-info {
            background: #f8f9fa;
            padding: 15px;
            border-radius: 10px;
            margin-bottom: 20px;
            text-align: left;
            font-size: 14px;
            color: #6c757d;
        }
        
        .form-group {
            margin-bottom: 20px;
            text-align: left;
        }
        
        label {
            display: block;
            margin-bottom: 8px;
            color: #555;
            font-weight: 500;
        }
        
        input {
            width: 100%;
            padding: 15px;
            border: 2px solid #e1e5e9;
            border-radius: 10px;
            font-size: 16px;
            transition: border-color 0.3s;
        }
        
        input:focus {
            outline: none;
            border-color: #667eea;
        }
        
        .login-btn {
            width: 100%;
            padding: 15px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 10px;
            font-size: 18px;
            font-weight: 600;
            cursor: pointer;
            transition: transform 0.2s;
        }
        
        .login-btn:hover {
            transform: translateY(-2px);
        }
        
        .login-btn:disabled {
            opacity: 0.6;
            cursor: not-allowed;
            transform: none;
        }
        
        .error-message {
            color: #e74c3c;
            margin-top: 15px;
            padding: 10px;
            background: #fff5f5;
            border-radius: 8px;
            border-left: 4px solid #e74c3c;
            display: none;
        }
        
        .loading {
            display: none;
            margin-top: 15px;
        }
        
        .spinner {
            border: 3px solid #f3f3f3;
            border-radius: 50%;
            border-top: 3px solid #667eea;
            width: 30px;
            height: 30px;
            animation: spin 1s linear infinite;
            margin: 0 auto;
        }
        
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
    </style>
</head>
<body>
    <div class="login-container">
        <div class="security-badge">🔒 SECURE ACCESS</div>
        <div class="lock-icon">🚪</div>
        <h1>Door Control System</h1>
        
        <div class="security-info">
            <strong>Security Features Active:</strong><br>
            • Session-based authentication<br>
            • Rate limiting protection<br>
            • Activity monitoring<br>
            • Auto-lockout after failed attempts
        </div>
        
        <form id="loginForm">
            <div class="form-group">
                <label for="username">Username</label>
                <input type="text" id="username" name="username" required autocomplete="username">
            </div>
            <div class="form-group">
                <label for="password">Password</label>
                <input type="password" id="password" name="password" required autocomplete="current-password">
            </div>
            <button type="submit" class="login-btn" id="loginBtn">Secure Sign In</button>
        </form>
        <div class="error-message" id="errorMessage"></div>
        <div class="loading" id="loading">
            <div class="spinner"></div>
        </div>
    </div>

    <script>
        let authToken = '';
        
        document.getElementById('loginForm').addEventListener('submit', async function(e) {
            e.preventDefault();
            
            const loginBtn = document.getElementById('loginBtn');
            const errorMessage = document.getElementById('errorMessage');
            const loading = document.getElementById('loading');
            
            // Reset UI
            errorMessage.style.display = 'none';
            loginBtn.disabled = true;
            loading.style.display = 'block';
            
            const formData = new FormData();
            formData.append('username', document.getElementById('username').value);
            formData.append('password', document.getElementById('password').value);
            
            try {
                const response = await fetch('/login', {
                    method: 'POST',
                    body: formData
                });
                
                const result = await response.json();
                
                if (result.success) {
                    authToken = result.token;
                    localStorage.setItem('authToken', authToken);
                    window.location.reload();
                } else {
                    errorMessage.textContent = result.message || 'Login failed';
                    errorMessage.style.display = 'block';
                    
                    if (response.status === 429) {
                        errorMessage.textContent = 'Too many requests. Please wait before trying again.';
                    } else if (response.status === 423) {
                        errorMessage.textContent = 'System is temporarily locked. Please wait.';
                        setTimeout(() => window.location.reload(), 30000);
                    }
                }
            } catch (error) {
                errorMessage.textContent = 'Connection error. Please check your network and try again.';
                errorMessage.style.display = 'block';
            }
            
            loginBtn.disabled = false;
            loading.style.display = 'none';
        });
    </script>
</body>
</html>
)";

// Enhanced main application page
const char* MAIN_PAGE = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Door Unlock - Login</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 20px;
        }
        
        .login-container {
            background: white;
            padding: 40px;
            border-radius: 20px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            width: 100%;
            max-width: 400px;
            text-align: center;
        }
        
        .lock-icon {
            font-size: 60px;
            margin-bottom: 20px;
            color: #667eea;
        }
        
        h1 {
            color: #333;
            margin-bottom: 30px;
            font-size: 28px;
        }
        
        .form-group {
            margin-bottom: 20px;
            text-align: left;
        }
        
        label {
            display: block;
            margin-bottom: 8px;
            color: #555;
            font-weight: 500;
        }
        
        input {
            width: 100%;
            padding: 15px;
            border: 2px solid #e1e5e9;
            border-radius: 10px;
            font-size: 16px;
            transition: border-color 0.3s;
        }
        
        input:focus {
            outline: none;
            border-color: #667eea;
        }
        
        .login-btn {
            width: 100%;
            padding: 15px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 10px;
            font-size: 18px;
            font-weight: 600;
            cursor: pointer;
            transition: transform 0.2s;
        }
        
        .login-btn:hover {
            transform: translateY(-2px);
        }
        
        .login-btn:disabled {
            opacity: 0.6;
            cursor: not-allowed;
            transform: none;
        }
        
        .error-message {
            color: #e74c3c;
            margin-top: 15px;
            padding: 10px;
            background: #fff5f5;
            border-radius: 8px;
            border-left: 4px solid #e74c3c;
            display: none;
        }
        
        .loading {
            display: none;
            margin-top: 15px;
        }
        
        .spinner {
            border: 3px solid #f3f3f3;
            border-radius: 50%;
            border-top: 3px solid #667eea;
            width: 30px;
            height: 30px;
            animation: spin 1s linear infinite;
            margin: 0 auto;
        }
        
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
    </style>
</head>
<body>
    <div class="login-container">
        <div class="lock-icon">🔒</div>
        <h1>Door Access</h1>
        <form id="loginForm">
            <div class="form-group">
                <label for="username">Username</label>
                <input type="text" id="username" name="username" required>
            </div>
            <div class="form-group">
                <label for="password">Password</label>
                <input type="password" id="password" name="password" required>
            </div>
            <button type="submit" class="login-btn" id="loginBtn">Sign In</button>
        </form>
        <div class="error-message" id="errorMessage"></div>
        <div class="loading" id="loading">
            <div class="spinner"></div>
        </div>
    </div>

    <script>
        document.getElementById('loginForm').addEventListener('submit', async function(e) {
            e.preventDefault();
            
            const loginBtn = document.getElementById('loginBtn');
            const errorMessage = document.getElementById('errorMessage');
            const loading = document.getElementById('loading');
            
            // Reset UI
            errorMessage.style.display = 'none';
            loginBtn.disabled = true;
            loading.style.display = 'block';
            
            const formData = new FormData();
            formData.append('username', document.getElementById('username').value);
            formData.append('password', document.getElementById('password').value);
            
            try {
                const response = await fetch('/login', {
                    method: 'POST',
                    body: formData
                });
                
                const result = await response.json();
                
                if (result.success) {
                    window.location.reload();
                } else {
                    errorMessage.textContent = result.message || 'Login failed';
                    errorMessage.style.display = 'block';
                }
            } catch (error) {
                errorMessage.textContent = 'Connection error. Please try again.';
                errorMessage.style.display = 'block';
            }
            
            loginBtn.disabled = false;
            loading.style.display = 'none';
        });
    </script>
</body>
</html>
)";

// Enhanced main application page
const char* MAIN_PAGE = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Secure Door Control</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 20px;
        }
        
        .control-container {
            background: white;
            padding: 40px;
            border-radius: 20px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            width: 100%;
            max-width: 400px;
            text-align: center;
        }
        
        .security-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 20px;
        }
        
        .security-badge {
            background: linear-gradient(135deg, #27ae60 0%, #2ecc71 100%);
            color: white;
            padding: 5px 12px;
            border-radius: 15px;
            font-size: 11px;
            font-weight: 600;
        }
        
        .logout-btn {
            background: #e74c3c;
            color: white;
            padding: 5px 12px;
            border: none;
            border-radius: 15px;
            font-size: 11px;
            font-weight: 600;
            cursor: pointer;
        }
        
        .door-icon {
            font-size: 80px;
            margin-bottom: 20px;
            transition: transform 0.3s;
        }
        
        .door-locked { color: #e74c3c; }
        .door-unlocked { color: #27ae60; }
        
        h1 {
            color: #333;
            margin-bottom: 10px;
            font-size: 28px;
        }
        
        .status {
            font-size: 18px;
            margin-bottom: 30px;
            padding: 15px;
            border-radius: 10px;
            font-weight: 500;
        }
        
        .status.locked {
            background: #fff5f5;
            color: #e74c3c;
            border: 2px solid #fadbd8;
        }
        
        .status.unlocked {
            background: #f0fff4;
            color: #27ae60;
            border: 2px solid #d5f4e6;
        }
        
        .unlock-btn {
            width: 100%;
            padding: 20px;
            margin-bottom: 20px;
            border: none;
            border-radius: 15px;
            font-size: 20px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s;
        }
        
        .unlock-btn.ready {
            background: linear-gradient(135deg, #27ae60 0%, #2ecc71 100%);
            color: white;
        }
        
        .unlock-btn.ready:hover {
            transform: translateY(-3px);
            box-shadow: 0 10px 25px rgba(39, 174, 96, 0.3);
        }
        
        .unlock-btn:disabled {
            background: #bdc3c7;
            color: #7f8c8d;
            cursor: not-allowed;
            transform: none;
            box-shadow: none;
        }
        
        .info-grid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 15px;
            margin-bottom: 20px;
        }
        
        .info-item {
            background: #f8f9fa;
            padding: 15px;
            border-radius: 10px;
            text-align: center;
        }
        
        .info-label {
            font-size: 12px;
            color: #6c757d;
            text-transform: uppercase;
            margin-bottom: 5px;
            letter-spacing: 0.5px;
        }
        
        .info-value {
            font-size: 16px;
            font-weight: 600;
            color: #333;
        }
        
        .security-info {
            background: #f8f9fa;
            padding: 15px;
            border-radius: 10px;
            text-align: left;
            font-size: 14px;
            margin-bottom: 20px;
        }
        
        .session-info {
            color: #6c757d;
            font-size: 12px;
            margin-top: 10px;
        }
        
        .success-message, .error-message {
            margin-top: 15px;
            padding: 12px;
            border-radius: 8px;
            display: none;
        }
        
        .success-message {
            background: #d4edda;
            color: #155724;
            border: 1px solid #c3e6cb;
        }
        
        .error-message {
            background: #f8d7da;
            color: #721c24;
            border: 1px solid #f5c6cb;
        }
        
        .loading {
            display: none;
        }
        
        .spinner {
            border: 3px solid #f3f3f3;
            border-radius: 50%;
            border-top: 3px solid #667eea;
            width: 25px;
            height: 25px;
            animation: spin 1s linear infinite;
            margin: 0 auto 10px;
        }
        
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
        
        @keyframes unlock {
            0% { transform: scale(1); }
            50% { transform: scale(1.1); }
            100% { transform: scale(1); }
        }
        
        .unlock-animation {
            animation: unlock 0.6s ease-in-out;
        }
    </style>
</head>
<body>
    <div class="control-container">
        <div class="security-header">
            <div class="security-badge">🔒 AUTHENTICATED</div>
            <button class="logout-btn" onclick="logout()">Logout</button>
        </div>
        
        <div class="door-icon" id="doorIcon">🚪</div>
        <h1>Secure Door Control</h1>
        <div class="status locked" id="doorStatus">🔒 Door is Locked</div>
        
        <button class="unlock-btn ready" id="unlockBtn">
            <span id="btnText">🔓 Unlock Door</span>
            <div class="loading" id="btnLoading">
                <div class="spinner"></div>
                Processing...
            </div>
        </button>
        
        <div class="info-grid">
            <div class="info-item">
                <div class="info-label">Status</div>
                <div class="info-value" id="statusValue">Locked</div>
            </div>
            <div class="info-item">
                <div class="info-label">Security</div>
                <div class="info-value" id="securityValue">Active</div>
            </div>
        </div>
        
        <div class="security-info">
            <strong>Security Status:</strong><br>
            • Session-based authentication active<br>
            • All actions are logged and monitored<br>
            • Auto-lock: 5 seconds after unlock<br>
            • Session timeout: 30 minutes
        </div>
        
        <div class="session-info" id="sessionInfo">
            Session active • Last activity: <span id="lastActivity">now</span>
        </div>
        
        <div class="success-message" id="successMessage"></div>
        <div class="error-message" id="errorMessage"></div>
    </div>

    <script>
        let doorLocked = true;
        let authToken = localStorage.getItem('authToken') || '';
        let sessionStartTime = Date.now();
        
        // Set authorization header for all requests
        function getAuthHeaders() {
            return {
                'Authorization': 'Bearer ' + authToken,
                'Content-Type': 'application/json'
            };
        }
        
        function updateUI(locked) {
            const doorIcon = document.getElementById('doorIcon');
            const doorStatus = document.getElementById('doorStatus');
            const statusValue = document.getElementById('statusValue');
            const unlockBtn = document.getElementById('unlockBtn');
            
            if (locked) {
                doorIcon.innerHTML = '🚪';
                doorIcon.className = 'door-icon door-locked';
                doorStatus.innerHTML = '🔒 Door is Locked';
                doorStatus.className = 'status locked';
                statusValue.textContent = 'Locked';
                unlockBtn.querySelector('#btnText').innerHTML = '🔓 Unlock Door';
            } else {
                doorIcon.innerHTML = '🚪';
                doorIcon.className = 'door-icon door-unlocked unlock-animation';
                doorStatus.innerHTML = '🔓 Door is Unlocked';
                doorStatus.className = 'status unlocked';
                statusValue.textContent = 'Unlocked';
                unlockBtn.querySelector('#btnText').innerHTML = '🔒 Auto-Locking Soon...';
            }
            
            doorLocked = locked;
        }
        
        function updateLastActivity() {
            const now = new Date();
            const timeStr = now.toLocaleTimeString();
            document.getElementById('lastActivity').textContent = timeStr;
        }
        
        function showMessage(message, isError = false) {
            const successEl = document.getElementById('successMessage');
            const errorEl = document.getElementById('errorMessage');
            
            if (isError) {
                errorEl.textContent = message;
                errorEl.style.display = 'block';
                successEl.style.display = 'none';
            } else {
                successEl.textContent = message;
                successEl.style.display = 'block';
                errorEl.style.display = 'none';
            }
            
            setTimeout(() => {
                successEl.style.display = 'none';
                errorEl.style.display = 'none';
            }, 5000);
        }
        
        async function unlockDoor() {
            const unlockBtn = document.getElementById('unlockBtn');
            const btnText = document.getElementById('btnText');
            const btnLoading = document.getElementById('btnLoading');
            
            unlockBtn.disabled = true;
            btnText.style.display = 'none';
            btnLoading.style.display = 'block';
            
            try {
                const response = await fetch('/unlock', {
                    method: 'POST',
                    headers: getAuthHeaders()
                });
                
                if (response.status === 401) {
                    showMessage('Session expired. Please login again.', true);
                    setTimeout(() => window.location.reload(), 2000);
                    return;
                }
                
                const result = await response.json();
                
                if (result.success) {
                    updateUI(false);
                    showMessage('Door unlocked successfully! Auto-lock in 5 seconds.');
                    updateLastActivity();
                    
                    // Auto-lock simulation
                    setTimeout(() => {
                        updateUI(true);
                        showMessage('Door automatically locked for security');
                    }, 5000);
                } else {
                    showMessage(result.message || 'Failed to unlock door', true);
                }
            } catch (error) {
                showMessage('Connection error. Please check your network.', true);
            }
            
            unlockBtn.disabled = false;
            btnText.style.display = 'block';
            btnLoading.style.display = 'none';
        }
        
        async function checkStatus() {
            try {
                const response = await fetch('/status', {
                    headers: getAuthHeaders()
                });
                
                if (response.status === 401) {
                    showMessage('Session expired. Redirecting to login...', true);
                    setTimeout(() => window.location.reload(), 2000);
                    return;
                }
                
                const result = await response.json();
                
                if (result.success) {
                    updateUI(result.doorStatus === 'locked');
                    document.getElementById('securityValue').textContent = 'Active';
                    updateLastActivity();
                }
            } catch (error) {
                document.getElementById('securityValue').textContent = 'Error';
                console.error('Status check failed:', error);
            }
        }
        
        async function logout() {
            try {
                await fetch('/logout', {
                    method: 'POST',
                    headers: getAuthHeaders()
                });
            } catch (error) {
                console.error('Logout error:', error);
            }
            
            localStorage.removeItem('authToken');
            window.location.reload();
        }
        
        // Event listeners
        document.getElementById('unlockBtn').addEventListener('click', unlockDoor);
        
        // Check status every 5 seconds (less frequent to reduce load)
        setInterval(checkStatus, 5000);
        checkStatus(); // Initial check
        
        // Update activity display every minute
        setInterval(updateLastActivity, 60000);
        updateLastActivity();
        
        // Auto-logout warning (5 minutes before session expires)
        setTimeout(() => {
            showMessage('Session will expire in 5 minutes. Activity will extend the session.', false);
        }, 25 * 60 * 1000); // 25 minutes
    </script>
</body>
</html>
)";

#endif