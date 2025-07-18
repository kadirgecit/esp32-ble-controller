# ESP32 BLE Controller

A comprehensive ESP32 application with web interface for scanning, connecting to, and controlling BLE devices. This project provides a complete solution for BLE device management with persistent storage, real-time updates, and an intuitive web interface.

## 🌟 Features

- 🔍 **BLE Device Scanning**: Discover nearby BLE devices with real-time updates and RSSI information
- 🔗 **Device Connection Management**: Connect to discovered or saved BLE devices with automatic reconnection
- 🛠️ **Service Discovery**: Explore device services and characteristics with detailed property information
- 📡 **Command Management**: Send custom commands to connected devices via specific characteristics
- 💾 **Persistent Storage**: Save devices and commands for quick reuse with SPIFFS filesystem
- 🌐 **Modern Web Interface**: Responsive web UI accessible from any device on the network
- ⚡ **Real-time Updates**: WebSocket communication for live status updates and device notifications
- 📱 **Mobile Friendly**: Fully responsive design that works on smartphones and tablets
- 🔧 **Configurable**: Easy-to-modify configuration file for customization
- 📊 **Activity Logging**: Comprehensive logging system with timestamped entries
- 🎯 **One-Click Commands**: Execute saved commands instantly with a single click
- 📶 **WiFi Access Point Mode**: Automatic AP mode for easy WiFi configuration without hardcoding credentials
- 🔄 **Auto WiFi Setup**: Captive portal for seamless WiFi network configuration

## 🎮 Try the Demo

Before setting up the hardware, you can try the web interface in demo mode:

```bash
cd esp32_ble_controller
python3 simple_demo_server.py
```

Then open http://localhost:8000 in your browser to explore the interface.

## 🔧 Hardware Requirements

- ESP32 development board (ESP32-WROOM-32, ESP32-DevKit, ESP32-S3, ESP32-C3, etc.)
- USB cable for programming and power
- WiFi network access (2.4GHz)
- BLE devices to interact with (smartphones, LED controllers, sensors, etc.)

## Software Requirements

- Arduino IDE 1.8.19+ or PlatformIO
- ESP32 Arduino Core 2.0.0+
- Required libraries (see installation section)

## 📦 Installation

### Method 1: Arduino IDE (Recommended for Beginners)

#### 1. Arduino IDE Setup

1. **Download and Install Arduino IDE**
   - Download from [arduino.cc](https://www.arduino.cc/en/software)
   - Install version 1.8.19 or later

2. **Add ESP32 Board Support**
   - Open Arduino IDE
   - Go to `File` → `Preferences`
   - Add this URL to "Additional Board Manager URLs":
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to `Tools` → `Board` → `Boards Manager`
   - Search for "ESP32" and install "ESP32 by Espressif Systems" (version 2.0.0+)

#### 2. Install Required Libraries

Open `Sketch` → `Include Library` → `Manage Libraries` and install:

- **ArduinoJson** by Benoit Blanchon (version 6.21.3+)
- **ESPAsyncWebServer** by lacamera (version 1.2.3+)
- **AsyncTCP** by dvarrel (version 1.1.1+)

#### 3. Install SPIFFS Upload Tool

1. Download [ESP32 Sketch Data Upload](https://github.com/me-no-dev/arduino-esp32fs-plugin/releases)
2. Extract to `Arduino/tools/ESP32FS/tool/esp32fs.jar`
3. Restart Arduino IDE

### Method 2: PlatformIO (Recommended for Advanced Users)

1. **Install PlatformIO**
   - Install [VS Code](https://code.visualstudio.com/)
   - Install PlatformIO extension

2. **Use the provided platformio.ini**
   - The project includes configurations for ESP32, ESP32-S3, and ESP32-C3
   - All dependencies are automatically managed

### 3. Upload Web Files

1. Install the ESP32 SPIFFS upload tool:
   - Download from: https://github.com/me-no-dev/arduino-esp32fs-plugin
   - Extract to `Arduino/tools/ESP32FS/tool/esp32fs.jar`
   - Restart Arduino IDE

2. Upload the web interface files:
   - Select your ESP32 board and port
   - Go to Tools → ESP32 Sketch Data Upload
   - Wait for upload to complete

### 4. Configure and Upload Sketch

1. Open `esp32_ble_controller.ino`
2. **WiFi Configuration (Choose one method):**
   
   **Method A: Automatic Setup (Recommended)**
   - No configuration needed! The ESP32 will create an Access Point for WiFi setup
   - Skip to step 3
   
   **Method B: Manual Configuration**
   - Open `config.h` and update WiFi credentials:
     ```cpp
     #define WIFI_SSID "YOUR_WIFI_SSID"
     #define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
     ```

3. Select your ESP32 board and port
4. Upload the sketch

## Usage

### 1. Initial Setup

**If using Automatic WiFi Setup:**
1. Power on your ESP32
2. Open Serial Monitor (115200 baud) to see status messages
3. If no WiFi is configured, the ESP32 will create an Access Point:
   - Network: `ESP32-BLE-Controller`
   - Password: `12345678`
4. Connect your device to this network
5. Open a browser and go to `http://192.168.4.1`
6. Use the WiFi setup page to scan and connect to your network
7. The ESP32 will restart and connect to your WiFi
8. Check your router or Serial Monitor for the new IP address

**If using Manual WiFi Setup:**
1. Power on your ESP32
2. Open Serial Monitor (115200 baud) to see the IP address
3. Connect to the web interface using the displayed IP address

### 2. Scanning for Devices

1. Navigate to the "Scan & Connect" tab
2. Click "Start Scan" to discover nearby BLE devices
3. Devices will appear in real-time with name, address, and signal strength
4. Click on a device to view details and connect

### 3. Connecting to Devices

1. From the scan results, click on a device to open details
2. Click "Connect" to establish a BLE connection
3. Optionally click "Save Device" to store it for future use
4. Connection status is shown in the header

### 4. Exploring Services

1. After connecting, go to the "Services" tab
2. Click "Load Services" to discover device services and characteristics
3. Each service shows its UUID and available characteristics
4. Click "Use" next to a characteristic to auto-fill the command form

### 5. Sending Commands

1. Go to the "Commands" tab
2. Fill in the required fields:
   - **Service UUID**: The target service UUID
   - **Characteristic UUID**: The target characteristic UUID
   - **Data**: Command data (hex format like "01FF" or plain text)
   - **Save as**: Optional name to save this command
3. Click "Send Command" to execute

### 6. Managing Saved Items

- **Saved Devices**: Quick connect to previously saved devices
- **Saved Commands**: One-click execution of frequently used commands
- Commands can be deleted individually

## Web Interface Overview

### Tabs

1. **Scan & Connect**: Device discovery and connection
2. **Saved Devices**: Quick access to saved devices
3. **Commands**: Send commands and manage saved commands
4. **Services**: Explore connected device services

### Status Indicators

- **Connection Status**: Shows current device connection
- **Scan Status**: Indicates scanning state
- **Activity Log**: Real-time log of all operations

## API Endpoints

The ESP32 exposes these REST API endpoints:

- `GET /api/scan` - Start BLE scanning
- `GET /api/stop-scan` - Stop BLE scanning
- `POST /api/connect` - Connect to device (requires `address` parameter)
- `GET /api/disconnect` - Disconnect from current device
- `POST /api/send-command` - Send BLE command
- `GET /api/get-services` - Get connected device services
- `GET /api/devices` - Get saved devices
- `POST /api/devices` - Save a device
- `GET /api/commands` - Get saved commands
- `POST /api/commands` - Save a command
- `DELETE /api/commands` - Delete a command

## 📁 Project Structure

```
esp32_ble_controller/
├── 📄 esp32_ble_controller.ino    # Main Arduino sketch
├── 📄 config.h                    # Configuration file for easy customization
├── 📄 platformio.ini              # PlatformIO configuration for multiple ESP32 variants
├── 📁 data/                       # Web interface files (uploaded to SPIFFS)
│   ├── 📄 index.html             # Main web interface HTML
│   ├── 📄 style.css              # Modern responsive CSS styling
│   └── 📄 script.js              # JavaScript for real-time functionality
├── 📄 simple_demo_server.py       # Python demo server for testing interface
├── 📄 README.md                   # Comprehensive documentation (this file)
├── 📄 QUICK_START.md              # Quick setup guide for beginners
├── 📄 EXAMPLES.md                 # Usage examples with common BLE devices
├── 📄 TROUBLESHOOTING.md          # Detailed troubleshooting guide
└── 📄 libraries.txt               # List of required Arduino libraries
```

### Key Files Explained

- **esp32_ble_controller.ino**: Main Arduino code with BLE and web server functionality
- **config.h**: Centralized configuration file for WiFi, BLE, and other settings
- **data/**: Web interface files that get uploaded to ESP32's SPIFFS filesystem
- **simple_demo_server.py**: Python server to test the web interface without ESP32 hardware
- **platformio.ini**: Configuration for PlatformIO with support for multiple ESP32 variants

## 🔧 Configuration

The project includes a `config.h` file for easy customization:

```cpp
// WiFi Settings
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// BLE Settings
#define BLE_DEVICE_NAME "ESP32_BLE_Controller"
#define BLE_SCAN_DURATION 10

// Web Server Settings
#define WEB_SERVER_PORT 80
#define MAX_WS_CLIENTS 4
```

## 🚀 Usage Examples

### Basic LED Controller
```cpp
Service UUID: 0000ffe0-0000-1000-8000-00805f9b34fb
Characteristic UUID: 0000ffe1-0000-1000-8000-00805f9b34fb
Commands:
- Turn On: 56FF00F0AA
- Turn Off: 56FF0000AA
- Red: 56FF0001AA
```

For more examples, see [EXAMPLES.md](EXAMPLES.md).

## 🛠️ Troubleshooting

### Common Issues

1. **WiFi Connection Failed**
   - Verify SSID and password are correct
   - Check WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)

2. **Web Interface Not Loading**
   - Ensure SPIFFS data was uploaded successfully
   - Check serial monitor for IP address
   - Try accessing via IP address directly

3. **BLE Connection Issues**
   - Ensure target device is in pairing/discoverable mode
   - Check device is within range (typically 10-30 feet)
   - Some devices require specific pairing procedures

4. **Command Not Working**
   - Verify service and characteristic UUIDs are correct
   - Check data format (some devices expect hex, others plain text)
   - Ensure characteristic supports write operations

### Detailed Help

For comprehensive troubleshooting, see [TROUBLESHOOTING.md](TROUBLESHOOTING.md) which includes:
- Installation issues and solutions
- Hardware connection problems  
- Network configuration issues
- BLE debugging techniques
- Performance optimization tips
- Common error messages and fixes

### Getting Support

1. Check the [TROUBLESHOOTING.md](TROUBLESHOOTING.md) guide
2. Review the Serial Monitor output (115200 baud)
3. Try the demo mode to isolate hardware issues
4. Check the [EXAMPLES.md](EXAMPLES.md) for device-specific guidance

## Data Formats

### Command Data

Commands can be sent in two formats:

1. **Hex Format**: `01FF` (even number of hex characters)
2. **String Format**: `Hello` (plain text)

The system automatically detects the format based on string length and content.

### UUIDs

UUIDs can be in various formats:
- Full: `12345678-1234-1234-1234-123456789abc`
- Short: `1234` (for standard Bluetooth services)

## Security Considerations

- The web interface is unprotected - use only on trusted networks
- BLE communications are not encrypted by this application
- Consider implementing authentication for production use

## Contributing

Feel free to submit issues and enhancement requests!

## License

This project is open source and available under the MIT License.