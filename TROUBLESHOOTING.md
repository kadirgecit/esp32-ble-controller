# ESP32 BLE Controller - Troubleshooting Guide

This guide helps you resolve common issues when using the ESP32 BLE Controller.

## Installation Issues

### Arduino IDE Problems

**Problem: ESP32 board not showing in Board Manager**
```
Solution:
1. Check the Additional Board Manager URL is correct:
   https://dl.espressif.com/dl/package_esp32_index.json
2. Restart Arduino IDE
3. Try using the stable release URL if development version fails
```

**Problem: Library installation fails**
```
Solution:
1. Update Arduino IDE to latest version
2. Clear library cache: Delete Arduino/libraries folder contents
3. Install libraries one by one instead of all at once
4. Check internet connection and firewall settings
```

**Problem: SPIFFS upload fails**
```
Solution:
1. Ensure ESP32 Sketch Data Upload plugin is properly installed
2. Close Serial Monitor before uploading
3. Try different USB cable or port
4. Reduce SPIFFS partition size in Tools menu
```

### Compilation Errors

**Error: `'class BLEAdvertisedDevice' has no member named 'getServiceUUIDs'`**
```
Solution:
Update ESP32 Arduino Core to version 2.0.0 or later:
1. Go to Boards Manager
2. Search for ESP32
3. Update to latest version
```

**Error: `AsyncTCP.h: No such file or directory`**
```
Solution:
Install missing libraries:
1. Tools → Manage Libraries
2. Search and install: AsyncTCP, ESPAsyncWebServer
3. Restart Arduino IDE
```

**Error: `SPIFFS was not declared in this scope`**
```
Solution:
For newer ESP32 cores, replace SPIFFS with LittleFS:
1. Change #include <SPIFFS.h> to #include <LittleFS.h>
2. Change SPIFFS.begin() to LittleFS.begin()
3. Change SPIFFS.open() to LittleFS.open()
```

## Hardware Issues

### ESP32 Connection Problems

**Problem: ESP32 not detected by computer**
```
Diagnosis:
- Check Device Manager (Windows) or System Information (Mac)
- Look for COM ports or USB devices

Solutions:
1. Install CP2102 or CH340 drivers (depending on your ESP32 board)
2. Try different USB cable (data cable, not charging-only)
3. Try different USB port
4. Press and hold BOOT button while connecting
```

**Problem: Upload fails with "Failed to connect to ESP32"**
```
Solutions:
1. Hold BOOT button during upload
2. Lower upload speed (Tools → Upload Speed → 115200)
3. Check if another program is using the serial port
4. Try different ESP32 board variant in Tools menu
```

### Power Issues

**Problem: ESP32 resets randomly**
```
Possible causes and solutions:
1. Insufficient power supply - Use quality USB cable and power source
2. Brown-out detection - Add capacitors or use external power supply
3. WiFi power consumption - Add delays in main loop
```

## Network Issues

### WiFi Connection Problems

**Problem: ESP32 won't connect to WiFi**
```
Diagnosis steps:
1. Check Serial Monitor output at 115200 baud
2. Look for WiFi connection status messages

Solutions:
1. Verify SSID and password are correct (case-sensitive)
2. Ensure WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
3. Check WiFi signal strength - move ESP32 closer to router
4. Try different WiFi network
5. Check for special characters in WiFi credentials
```

**Problem: WiFi connects but web interface doesn't load**
```
Solutions:
1. Check IP address in Serial Monitor
2. Ensure you're on the same network as ESP32
3. Try accessing via IP address directly (not hostname)
4. Check firewall settings on your computer
5. Verify SPIFFS files were uploaded correctly
```

### Web Interface Issues

**Problem: Web page loads but buttons don't work**
```
Solutions:
1. Check browser console for JavaScript errors (F12)
2. Verify WebSocket connection in browser developer tools
3. Clear browser cache and reload page
4. Try different web browser
5. Check if CORS is blocking requests
```

**Problem: WebSocket connection fails**
```
Solutions:
1. Check if ESP32 is still connected to WiFi
2. Verify WebSocket URL in browser console
3. Try refreshing the page
4. Check for proxy or firewall blocking WebSocket connections
```

## BLE Issues

### Scanning Problems

**Problem: No BLE devices found during scan**
```
Diagnosis:
1. Check if target devices are in advertising/pairing mode
2. Verify devices are within range (typically 10-30 feet)

Solutions:
1. Restart target BLE devices
2. Move ESP32 closer to target devices
3. Check for interference from other 2.4GHz devices
4. Try scanning at different times (some devices have limited advertising)
5. Verify ESP32 BLE is working with known devices
```

**Problem: Some devices appear, others don't**
```
Possible causes:
1. Device-specific advertising intervals
2. Different BLE versions or profiles
3. Devices already connected to other controllers
4. Power-saving modes on target devices

Solutions:
1. Wait longer during scan (some devices advertise infrequently)
2. Disconnect devices from other controllers
3. Reset target devices
```

### Connection Issues

**Problem: Can't connect to discovered devices**
```
Solutions:
1. Ensure device is still in pairing/connectable mode
2. Check if device requires pairing or bonding
3. Try connecting immediately after discovery
4. Restart both ESP32 and target device
5. Check device documentation for connection requirements
```

**Problem: Connection drops frequently**
```
Solutions:
1. Reduce distance between ESP32 and target device
2. Check for interference sources
3. Verify power supply stability
4. Implement connection retry logic
5. Check target device battery level
```

### Command Issues

**Problem: Commands sent but device doesn't respond**
```
Diagnosis steps:
1. Verify Service and Characteristic UUIDs are correct
2. Check data format (hex vs string)
3. Confirm characteristic supports write operations

Solutions:
1. Use Services tab to verify UUIDs
2. Check device documentation for correct command format
3. Try different data formats (hex/string)
4. Verify write permissions on characteristic
5. Check if device requires specific command sequences
```

**Problem: Getting "Characteristic not found" errors**
```
Solutions:
1. Load services first using Services tab
2. Verify UUIDs are typed correctly (case-sensitive)
3. Check if device requires service discovery after connection
4. Some characteristics may not be immediately available
```

## Performance Issues

### Slow Response Times

**Problem: Web interface is slow or unresponsive**
```
Solutions:
1. Check WiFi signal strength
2. Reduce number of concurrent connections
3. Clear browser cache
4. Close unnecessary browser tabs
5. Check ESP32 memory usage in Serial Monitor
```

**Problem: BLE operations are slow**
```
Solutions:
1. Reduce BLE scan interval in code
2. Optimize command data size
3. Implement command queuing for multiple operations
4. Check for BLE interference
```

### Memory Issues

**Problem: ESP32 runs out of memory**
```
Symptoms:
- Random resets
- Failed operations
- "Guru Meditation Error" messages

Solutions:
1. Reduce JSON buffer sizes in code
2. Limit number of stored devices/commands
3. Implement memory cleanup routines
4. Use ESP32 with more RAM (ESP32-S3)
```

## Debugging Techniques

### Serial Monitor Debugging

**Enable detailed logging:**
```cpp
#define DEBUG_ESP_PORT Serial
#define DEBUG_ESP_BLE
#define CORE_DEBUG_LEVEL 4
```

**Monitor memory usage:**
```cpp
void printMemoryInfo() {
    Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("Largest free block: %d bytes\n", ESP.getMaxAllocHeap());
}
```

### Browser Developer Tools

**Check WebSocket connection:**
1. Open browser developer tools (F12)
2. Go to Network tab
3. Look for WebSocket connections
4. Check for connection errors or dropped connections

**Monitor JavaScript errors:**
1. Open Console tab in developer tools
2. Look for error messages
3. Check for failed API requests

### BLE Debugging Tools

**Recommended smartphone apps:**
- **Android**: BLE Scanner, nRF Connect
- **iOS**: LightBlue Explorer, BLE Scanner 4.0

**Use these apps to:**
1. Verify device advertising
2. Check service and characteristic UUIDs
3. Test commands independently
4. Monitor connection stability

## Common Error Messages

### ESP32 Serial Monitor Errors

```
"Guru Meditation Error: Core 1 panic'ed (LoadProhibited)"
Solution: Memory access violation - check array bounds and pointer usage

"Brownout detector was triggered"
Solution: Power supply issue - use better power source or add capacitors

"WiFi: no AP found"
Solution: WiFi network not found - check SSID and ensure 2.4GHz network

"BLE: Failed to start advertising"
Solution: BLE initialization issue - restart ESP32 or check BLE configuration
```

### Web Interface Errors

```
"WebSocket connection failed"
Solution: Check ESP32 WiFi connection and firewall settings

"Failed to fetch"
Solution: Network connectivity issue - verify ESP32 IP address and network

"Device not found"
Solution: BLE device not in range or not advertising
```

## Demo Mode Testing

### Using the Demo Server

Before troubleshooting hardware issues, test the web interface in demo mode:

```bash
cd esp32_ble_controller
python3 simple_demo_server.py
```

Then open http://localhost:8000 to test the interface without ESP32 hardware.

**Benefits of Demo Mode:**
- Isolates web interface issues from hardware problems
- Tests JavaScript functionality
- Verifies CSS styling and responsiveness
- Confirms API endpoint structure

### Demo Server vs Real ESP32

| Feature | Demo Server | Real ESP32 |
|---------|-------------|------------|
| Web Interface | ✅ Full functionality | ✅ Full functionality |
| BLE Scanning | ❌ Simulated only | ✅ Real BLE devices |
| Device Connection | ❌ Simulated only | ✅ Real connections |
| Command Sending | ❌ Simulated only | ✅ Real commands |
| Data Persistence | ❌ Session only | ✅ SPIFFS storage |

## Getting Help

### Before Asking for Help

1. **Test in demo mode first** to isolate hardware vs software issues
2. **Check Serial Monitor output** at 115200 baud rate
3. **Try basic functionality** (WiFi connection, web interface loading)
4. **Test with known working BLE devices**
5. **Review this troubleshooting guide**
6. **Check for similar issues** in project documentation

### Information to Include When Reporting Issues

1. **Hardware details**: ESP32 board type, power supply
2. **Software versions**: Arduino IDE, ESP32 core, library versions
3. **Network setup**: WiFi type, router model
4. **Target BLE devices**: Make, model, known working apps
5. **Error messages**: Complete error text from Serial Monitor
6. **Steps to reproduce**: Exact sequence that causes the issue

### Useful Diagnostic Commands

**Check ESP32 info:**
```cpp
Serial.println("ESP32 Chip model: " + String(ESP.getChipModel()));
Serial.println("ESP32 Chip revision: " + String(ESP.getChipRevision()));
Serial.println("ESP32 Flash size: " + String(ESP.getFlashChipSize()));
Serial.println("ESP32 Free heap: " + String(ESP.getFreeHeap()));
```

**Check WiFi info:**
```cpp
Serial.println("WiFi SSID: " + WiFi.SSID());
Serial.println("WiFi RSSI: " + String(WiFi.RSSI()));
Serial.println("WiFi IP: " + WiFi.localIP().toString());
Serial.println("WiFi Gateway: " + WiFi.gatewayIP().toString());
```

---

**Remember**: Most issues are related to basic setup problems like incorrect wiring, wrong library versions, or network configuration. Start with the basics and work your way up to more complex debugging.