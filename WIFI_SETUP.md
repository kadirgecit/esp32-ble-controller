# 📶 WiFi Setup Guide

This guide explains how to configure WiFi on your ESP32 BLE Controller using the built-in Access Point mode.

## 🚀 Quick Setup (No Configuration Required!)

The ESP32 BLE Controller features automatic WiFi setup - no need to hardcode credentials!

### Step 1: First Boot
1. Upload the sketch to your ESP32
2. Upload the web files to SPIFFS
3. Power on your ESP32
4. Open Serial Monitor (115200 baud) to see status messages

### Step 2: Access Point Mode
If no WiFi credentials are configured, the ESP32 will automatically:
- Create a WiFi Access Point named: `ESP32-BLE-Controller`
- Set the password to: `12345678`
- Display connection instructions in the Serial Monitor

### Step 3: Connect to Setup Network
1. On your phone/computer, connect to the WiFi network: `ESP32-BLE-Controller`
2. Use password: `12345678`
3. Open a web browser and navigate to: `http://192.168.4.1`

### Step 4: Configure WiFi
1. The WiFi setup page will load automatically
2. Click "🔍 Scan for Networks" to discover available WiFi networks
3. Click on your desired network from the list (or manually enter SSID)
4. Enter your WiFi password
5. Click "💾 Save & Connect"

### Step 5: Automatic Connection
1. The ESP32 will save your credentials and restart
2. It will connect to your WiFi network automatically
3. Check your router's admin panel or Serial Monitor for the new IP address
4. Access the BLE Controller at: `http://[NEW_IP_ADDRESS]`

## 🔧 Configuration Options

### Access Point Settings
You can customize the AP settings in `config.h`:

```cpp
// Access Point Settings
#define AP_SSID "ESP32-BLE-Controller"    // AP network name
#define AP_PASSWORD "12345678"            // AP password (min 8 chars)
#define AP_TIMEOUT 300000                 // 5 minutes timeout
#define AP_IP_ADDRESS IPAddress(192, 168, 4, 1)  // AP IP address
```

### WiFi Connection Settings
```cpp
// WiFi Settings
#define WIFI_TIMEOUT 30000                // Connection timeout (30 seconds)
#define WIFI_MAX_RETRIES 5               // Maximum retry attempts
```

## 🔄 Reconnection Behavior

The ESP32 will automatically:
1. Try to connect to saved WiFi credentials on boot
2. Fall back to Access Point mode if connection fails
3. Store credentials in non-volatile memory (survives power cycles)
4. Retry connection after configuration changes

## 🛠️ Manual Configuration (Alternative)

If you prefer to hardcode WiFi credentials:

1. Open `config.h`
2. Update the WiFi settings:
   ```cpp
   #define WIFI_SSID "Your_Network_Name"
   #define WIFI_PASSWORD "Your_Network_Password"
   ```
3. Upload the sketch
4. The ESP32 will connect directly to your network

## 🚨 Troubleshooting

### ESP32 Stuck in AP Mode
- **Cause**: WiFi credentials are incorrect or network is unavailable
- **Solution**: Reconnect to the AP and update credentials

### Can't Connect to Access Point
- **Check**: Ensure you're connecting to `ESP32-BLE-Controller`
- **Password**: Make sure you're using `12345678`
- **Range**: Move closer to the ESP32

### Access Point Timeout
- **Default**: AP mode times out after 5 minutes
- **Solution**: Power cycle the ESP32 to restart AP mode
- **Customize**: Change `AP_TIMEOUT` in `config.h`

### Forgot WiFi Credentials
The ESP32 stores credentials in flash memory. To reset:
1. Connect to AP mode
2. Configure new credentials
3. Or use the factory reset feature (if enabled)

## 🔒 Security Notes

- The default AP password is `12345678` - change it in `config.h` for better security
- WiFi credentials are stored encrypted in ESP32's NVS (Non-Volatile Storage)
- The setup page is only accessible in AP mode for security

## 📱 Mobile Setup

The WiFi setup page is fully mobile-responsive:
- Works on smartphones and tablets
- Touch-friendly interface
- Automatic network scanning
- One-tap network selection

## 🌐 Network Requirements

- **WiFi Standard**: 802.11 b/g/n (2.4GHz only)
- **Security**: Open, WEP, WPA, WPA2 supported
- **Range**: Standard WiFi range (varies by environment)

## 💡 Tips

1. **Signal Strength**: The setup page shows signal strength for each network
2. **Network Security**: Locked (🔒) and open (🔓) networks are clearly marked
3. **Auto-Refresh**: The interface updates in real-time during setup
4. **Error Handling**: Clear error messages guide you through any issues
5. **Backup Access**: If you lose the IP address, check your router's connected devices list

---

**Need Help?** Check the main [README.md](README.md) or [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for more information.