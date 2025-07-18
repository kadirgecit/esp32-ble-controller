# ESP32 BLE Controller - Quick Start Guide

Get up and running with the ESP32 BLE Controller in just a few minutes!

## 🚀 Quick Setup (5 minutes)

### 1. Hardware Setup
- Connect your ESP32 to your computer via USB
- No additional wiring required for basic functionality

### 2. Software Installation
```bash
# Install Arduino IDE (if not already installed)
# Download from: https://www.arduino.cc/en/software

# Add ESP32 board support:
# File → Preferences → Additional Board Manager URLs:
https://dl.espressif.com/dl/package_esp32_index.json

# Install required libraries via Library Manager:
# - ArduinoJson (6.21.3+)
# - ESPAsyncWebServer (1.2.3+)
# - AsyncTCP (1.1.1+)
```

### 3. Configure WiFi
Edit `esp32_ble_controller.ino`:
```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

### 4. Upload Code
1. Open `esp32_ble_controller.ino` in Arduino IDE
2. Select your ESP32 board (Tools → Board → ESP32 Dev Module)
3. Select the correct port (Tools → Port)
4. Upload the sketch (Ctrl+U)

### 5. Upload Web Files
1. Install ESP32 Sketch Data Upload plugin
2. Tools → ESP32 Sketch Data Upload
3. Wait for upload to complete

### 6. Access Web Interface
1. Open Serial Monitor (115200 baud)
2. Reset ESP32 and note the IP address
3. Open browser and go to the IP address
4. Start using the BLE Controller!

## 🎯 First Test

### Test with a Smartphone
1. Install "BLE Scanner" app on your phone
2. Create a simple BLE peripheral on your phone
3. Use the ESP32 controller to scan and connect

### Test with Demo Mode
```bash
# Run the demo server to test the web interface
cd esp32_ble_controller
python3 simple_demo_server.py
# Open http://localhost:8000
```

## 📱 Basic Usage

### Scan for Devices
1. Click "Scan & Connect" tab
2. Click "Start Scan"
3. Wait for devices to appear
4. Click on any device to see details

### Connect to Device
1. Click on a discovered device
2. Click "Connect" in the popup
3. Wait for connection confirmation
4. Green status indicates successful connection

### Send Commands
1. Go to "Commands" tab
2. Fill in Service UUID and Characteristic UUID
3. Enter your command data
4. Click "Send Command"

### Save for Later
- Save devices: Click "Save Device" when viewing device details
- Save commands: Enter a name in the "Save as" field before sending

## 🔧 Common UUIDs

### Generic LED Controller
```
Service: 0000ffe0-0000-1000-8000-00805f9b34fb
Characteristic: 0000ffe1-0000-1000-8000-00805f9b34fb
Commands:
- Turn On: 56FF00F0AA
- Turn Off: 56FF0000AA
- Red: 56FF0001AA
```

### Battery Service
```
Service: 0000180f-0000-1000-8000-00805f9b34fb
Characteristic: 00002a19-0000-1000-8000-00805f9b34fb
Command: Read battery level
```

## 🆘 Quick Troubleshooting

**ESP32 won't connect to WiFi?**
- Check SSID/password spelling
- Ensure 2.4GHz network (not 5GHz)
- Move closer to router

**Web interface won't load?**
- Check Serial Monitor for IP address
- Verify SPIFFS upload completed
- Try different browser

**No BLE devices found?**
- Ensure target devices are advertising
- Check device distance (stay within 30 feet)
- Try restarting target devices

**Commands not working?**
- Verify UUIDs are correct
- Check data format (hex vs string)
- Ensure device is still connected

## 📚 Next Steps

1. **Read the full README.md** for detailed information
2. **Check EXAMPLES.md** for device-specific examples
3. **Review TROUBLESHOOTING.md** if you encounter issues
4. **Explore the Services tab** to discover device capabilities
5. **Customize config.h** for advanced settings

## 🎉 You're Ready!

Your ESP32 BLE Controller is now ready to scan, connect, and control BLE devices. Start with simple devices like LED controllers or fitness trackers, then move on to more complex applications.

**Happy BLE controlling!** 🚀