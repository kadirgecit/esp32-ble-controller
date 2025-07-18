// Global variables
let websocket;
let selectedDevice = null;
let isConnected = false;
let isScanning = false;

// Initialize the application
document.addEventListener('DOMContentLoaded', function() {
    checkWiFiStatus();
    initWebSocket();
    loadSavedDevices();
    loadSavedCommands();
    
    // Setup command form
    document.getElementById('command-form').addEventListener('submit', handleCommandSubmit);
});

// WebSocket connection
function initWebSocket() {
    // Check if we're running in demo mode (port 12000 or contains 'demo' or 'runtime')
    const isDemo = window.location.port === '12000' || 
                   window.location.hostname === 'localhost' ||
                   window.location.hostname.includes('runtime') ||
                   window.location.hostname.includes('demo');
    
    console.log('Demo mode check:', {
        port: window.location.port,
        hostname: window.location.hostname,
        isDemo: isDemo
    });
    
    if (isDemo) {
        log('Running in demo mode - WebSocket disabled', 'info');
        return;
    }
    
    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const wsUrl = `${protocol}//${window.location.host}/ws`;
    
    websocket = new WebSocket(wsUrl);
    
    websocket.onopen = function(event) {
        log('WebSocket connected', 'success');
    };
    
    websocket.onmessage = function(event) {
        handleWebSocketMessage(event.data);
    };
    
    websocket.onclose = function(event) {
        log('WebSocket disconnected', 'error');
        // Only reconnect if not in demo mode
        const isDemo = window.location.port === '12000' || 
                       window.location.hostname === 'localhost' ||
                       window.location.hostname.includes('runtime') ||
                       window.location.hostname.includes('demo');
        if (!isDemo) {
            setTimeout(initWebSocket, 3000); // Reconnect after 3 seconds
        }
    };
    
    websocket.onerror = function(error) {
        log('WebSocket error: ' + error, 'error');
    };
}

// Handle WebSocket messages
function handleWebSocketMessage(message) {
    if (message.startsWith('SCAN_RESULT:')) {
        const deviceData = JSON.parse(message.substring(12));
        addDiscoveredDevice(deviceData);
    } else if (message === 'SCAN_STARTED') {
        updateScanStatus(true);
        log('BLE scan started', 'info');
    } else if (message === 'SCAN_STOPPED') {
        updateScanStatus(false);
        log('BLE scan stopped', 'info');
    } else if (message.startsWith('CONNECTED:')) {
        const address = message.substring(10);
        updateConnectionStatus(true, address);
        log(`Connected to device: ${address}`, 'success');
    } else if (message === 'DISCONNECTED') {
        updateConnectionStatus(false);
        log('Disconnected from device', 'warning');
    }
}

// Tab management
function openTab(evt, tabName) {
    const tabcontent = document.getElementsByClassName('tab-content');
    for (let i = 0; i < tabcontent.length; i++) {
        tabcontent[i].classList.remove('active');
    }
    
    const tablinks = document.getElementsByClassName('tab-button');
    for (let i = 0; i < tablinks.length; i++) {
        tablinks[i].classList.remove('active');
    }
    
    document.getElementById(tabName).classList.add('active');
    evt.currentTarget.classList.add('active');
}

// BLE scanning functions
function startScan() {
    fetch('/api/scan')
        .then(response => response.json())
        .then(data => {
            document.getElementById('discovered-devices').innerHTML = '';
            log('Starting BLE scan...', 'info');
        })
        .catch(error => {
            log('Error starting scan: ' + error, 'error');
        });
}

function stopScan() {
    fetch('/api/stop-scan')
        .then(response => response.json())
        .then(data => {
            log('Stopping BLE scan...', 'info');
        })
        .catch(error => {
            log('Error stopping scan: ' + error, 'error');
        });
}

function updateScanStatus(scanning) {
    isScanning = scanning;
    const scanBtn = document.getElementById('scan-btn');
    const stopBtn = document.getElementById('stop-scan-btn');
    const statusElement = document.getElementById('scan-status');
    
    if (scanning) {
        scanBtn.disabled = true;
        stopBtn.disabled = false;
        statusElement.textContent = 'Scanning...';
        statusElement.className = 'status scanning';
    } else {
        scanBtn.disabled = false;
        stopBtn.disabled = true;
        statusElement.textContent = 'Ready';
        statusElement.className = 'status';
    }
}

// Device management
function addDiscoveredDevice(device) {
    const container = document.getElementById('discovered-devices');
    const deviceElement = document.createElement('div');
    deviceElement.className = 'device-item';
    deviceElement.onclick = () => showDeviceDetails(device);
    
    deviceElement.innerHTML = `
        <div class="device-name">${device.name || 'Unknown Device'}</div>
        <div class="device-address">${device.address}</div>
        <div class="device-rssi">RSSI: ${device.rssi} dBm</div>
    `;
    
    container.appendChild(deviceElement);
}

function showDeviceDetails(device) {
    selectedDevice = device;
    const modal = document.getElementById('device-modal');
    const details = document.getElementById('device-details');
    
    let servicesList = '';
    if (device.serviceUUIDs && device.serviceUUIDs.length > 0) {
        servicesList = '<h4>Services:</h4><ul>';
        device.serviceUUIDs.forEach(uuid => {
            servicesList += `<li>${uuid}</li>`;
        });
        servicesList += '</ul>';
    }
    
    details.innerHTML = `
        <p><strong>Name:</strong> ${device.name || 'Unknown'}</p>
        <p><strong>Address:</strong> ${device.address}</p>
        <p><strong>RSSI:</strong> ${device.rssi} dBm</p>
        ${servicesList}
    `;
    
    modal.style.display = 'block';
}

function closeModal() {
    document.getElementById('device-modal').style.display = 'none';
    selectedDevice = null;
}

function connectToSelectedDevice() {
    if (!selectedDevice) return;
    
    const formData = new FormData();
    formData.append('address', selectedDevice.address);
    
    fetch('/api/connect', {
        method: 'POST',
        body: formData
    })
    .then(response => response.json())
    .then(data => {
        log(`Connecting to ${selectedDevice.address}...`, 'info');
        closeModal();
    })
    .catch(error => {
        log('Error connecting to device: ' + error, 'error');
    });
}

function saveSelectedDevice() {
    if (!selectedDevice) return;
    
    const name = prompt('Enter a name for this device:', selectedDevice.name || selectedDevice.address);
    if (!name) return;
    
    const formData = new FormData();
    formData.append('name', name);
    formData.append('address', selectedDevice.address);
    
    fetch('/api/devices', {
        method: 'POST',
        body: formData
    })
    .then(response => response.json())
    .then(data => {
        log(`Device saved as: ${name}`, 'success');
        loadSavedDevices();
        closeModal();
    })
    .catch(error => {
        log('Error saving device: ' + error, 'error');
    });
}

function loadSavedDevices() {
    fetch('/api/devices')
        .then(response => response.json())
        .then(devices => {
            const container = document.getElementById('saved-devices');
            container.innerHTML = '';
            
            devices.forEach(device => {
                const deviceElement = document.createElement('div');
                deviceElement.className = 'device-item';
                deviceElement.onclick = () => connectToSavedDevice(device.address);
                
                deviceElement.innerHTML = `
                    <div class="device-name">${device.name}</div>
                    <div class="device-address">${device.address}</div>
                `;
                
                container.appendChild(deviceElement);
            });
        })
        .catch(error => {
            log('Error loading saved devices: ' + error, 'error');
        });
}

function connectToSavedDevice(address) {
    const formData = new FormData();
    formData.append('address', address);
    
    fetch('/api/connect', {
        method: 'POST',
        body: formData
    })
    .then(response => response.json())
    .then(data => {
        log(`Connecting to ${address}...`, 'info');
    })
    .catch(error => {
        log('Error connecting to device: ' + error, 'error');
    });
}

function disconnect() {
    fetch('/api/disconnect')
        .then(response => response.json())
        .then(data => {
            log('Disconnecting...', 'info');
        })
        .catch(error => {
            log('Error disconnecting: ' + error, 'error');
        });
}

function updateConnectionStatus(connected, address = '') {
    isConnected = connected;
    const statusElement = document.getElementById('connection-status');
    const disconnectBtn = document.getElementById('disconnect-btn');
    
    if (connected) {
        statusElement.textContent = `Connected to ${address}`;
        statusElement.className = 'status connected';
        disconnectBtn.disabled = false;
    } else {
        statusElement.textContent = 'Disconnected';
        statusElement.className = 'status disconnected';
        disconnectBtn.disabled = true;
    }
}

// Command management
function handleCommandSubmit(event) {
    event.preventDefault();
    
    const serviceUUID = document.getElementById('service-uuid').value;
    const characteristicUUID = document.getElementById('characteristic-uuid').value;
    const data = document.getElementById('command-data').value;
    const name = document.getElementById('command-name').value;
    
    sendCommand(serviceUUID, characteristicUUID, data, name);
}

function sendCommand(serviceUUID, characteristicUUID, data, name = '') {
    if (!isConnected) {
        log('Not connected to any device', 'error');
        return;
    }
    
    const formData = new FormData();
    formData.append('serviceUUID', serviceUUID);
    formData.append('characteristicUUID', characteristicUUID);
    formData.append('data', data);
    
    fetch('/api/send-command', {
        method: 'POST',
        body: formData
    })
    .then(response => response.json())
    .then(result => {
        log(`Command sent: ${data}`, 'success');
        
        // Save command if name provided
        if (name) {
            saveCommand(name, serviceUUID, characteristicUUID, data);
        }
    })
    .catch(error => {
        log('Error sending command: ' + error, 'error');
    });
}

function saveCommand(name, serviceUUID, characteristicUUID, data) {
    const formData = new FormData();
    formData.append('name', name);
    formData.append('serviceUUID', serviceUUID);
    formData.append('characteristicUUID', characteristicUUID);
    formData.append('data', data);
    
    fetch('/api/commands', {
        method: 'POST',
        body: formData
    })
    .then(response => response.json())
    .then(result => {
        log(`Command saved as: ${name}`, 'success');
        loadSavedCommands();
        document.getElementById('command-name').value = '';
    })
    .catch(error => {
        log('Error saving command: ' + error, 'error');
    });
}

function loadSavedCommands() {
    fetch('/api/commands')
        .then(response => response.json())
        .then(commands => {
            const container = document.getElementById('saved-commands-list');
            container.innerHTML = '';
            
            commands.forEach((command, index) => {
                const commandElement = document.createElement('div');
                commandElement.className = 'command-item';
                
                commandElement.innerHTML = `
                    <div class="command-info">
                        <div class="command-name">${command.name}</div>
                        <div class="command-details">
                            Service: ${command.serviceUUID}<br>
                            Characteristic: ${command.characteristicUUID}<br>
                            Data: ${command.data}
                        </div>
                    </div>
                    <div class="command-actions">
                        <button class="execute-btn" onclick="executeCommand('${command.serviceUUID}', '${command.characteristicUUID}', '${command.data}')">Execute</button>
                        <button class="delete-btn" onclick="deleteCommand(${index})">Delete</button>
                    </div>
                `;
                
                container.appendChild(commandElement);
            });
        })
        .catch(error => {
            log('Error loading saved commands: ' + error, 'error');
        });
}

function executeCommand(serviceUUID, characteristicUUID, data) {
    sendCommand(serviceUUID, characteristicUUID, data);
}

function deleteCommand(index) {
    if (!confirm('Are you sure you want to delete this command?')) return;
    
    const formData = new FormData();
    formData.append('index', index);
    
    fetch('/api/commands', {
        method: 'DELETE',
        body: formData
    })
    .then(response => response.json())
    .then(result => {
        log('Command deleted', 'success');
        loadSavedCommands();
    })
    .catch(error => {
        log('Error deleting command: ' + error, 'error');
    });
}

// Services management
function loadServices() {
    if (!isConnected) {
        log('Not connected to any device', 'error');
        return;
    }
    
    fetch('/api/get-services')
        .then(response => response.json())
        .then(services => {
            const container = document.getElementById('services-list');
            container.innerHTML = '';
            
            services.forEach(service => {
                const serviceElement = document.createElement('div');
                serviceElement.className = 'service-item';
                
                let characteristicsHtml = '';
                service.characteristics.forEach(characteristic => {
                    characteristicsHtml += `
                        <div class="characteristic-item">
                            <span class="characteristic-uuid">${characteristic.uuid}</span>
                            <span class="characteristic-properties">Props: ${characteristic.properties}</span>
                            <button class="use-characteristic-btn" onclick="useCharacteristic('${service.uuid}', '${characteristic.uuid}')">Use</button>
                        </div>
                    `;
                });
                
                serviceElement.innerHTML = `
                    <div class="service-uuid">Service: ${service.uuid}</div>
                    <div class="characteristics">
                        <h4>Characteristics:</h4>
                        ${characteristicsHtml}
                    </div>
                `;
                
                container.appendChild(serviceElement);
            });
            
            log('Services loaded', 'success');
        })
        .catch(error => {
            log('Error loading services: ' + error, 'error');
        });
}

function useCharacteristic(serviceUUID, characteristicUUID) {
    // Switch to commands tab and fill in the UUIDs
    document.querySelector('[onclick="openTab(event, \'commands-tab\')"]').click();
    document.getElementById('service-uuid').value = serviceUUID;
    document.getElementById('characteristic-uuid').value = characteristicUUID;
    document.getElementById('command-data').focus();
}

// Logging
function log(message, type = 'info') {
    const logContent = document.getElementById('log-content');
    const timestamp = new Date().toLocaleTimeString();
    const logEntry = document.createElement('div');
    logEntry.className = `log-entry log-${type}`;
    logEntry.innerHTML = `<span class="log-timestamp">[${timestamp}]</span> ${message}`;
    
    logContent.appendChild(logEntry);
    logContent.scrollTop = logContent.scrollHeight;
}

function clearLog() {
    document.getElementById('log-content').innerHTML = '';
}

// Close modal when clicking outside
window.onclick = function(event) {
    const modal = document.getElementById('device-modal');
    if (event.target === modal) {
        closeModal();
    }
}

// WiFi Status Management
function checkWiFiStatus() {
    // Skip WiFi check in demo mode
    if (window.location.port === '12000') {
        return;
    }
    
    fetch('/api/wifi/status')
        .then(response => response.json())
        .then(data => {
            if (data.isAPMode) {
                showAPModeNotification();
            }
        })
        .catch(error => {
            console.log('WiFi status check failed:', error);
        });
}

function showAPModeNotification() {
    // Create AP mode notification
    const notification = document.createElement('div');
    notification.id = 'ap-notification';
    notification.style.cssText = `
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        background: linear-gradient(135deg, #ff6b6b, #ee5a24);
        color: white;
        padding: 15px;
        text-align: center;
        z-index: 1000;
        box-shadow: 0 2px 10px rgba(0,0,0,0.2);
        font-weight: 500;
    `;
    
    notification.innerHTML = `
        <div style="max-width: 800px; margin: 0 auto;">
            🔧 <strong>WiFi Setup Mode</strong> - 
            This ESP32 is not connected to WiFi. 
            <a href="/wifi-config.html" style="color: #fff; text-decoration: underline; margin-left: 10px;">
                Configure WiFi Settings
            </a>
        </div>
    `;
    
    document.body.insertBefore(notification, document.body.firstChild);
    
    // Adjust main content margin
    document.querySelector('.container').style.marginTop = '60px';
}