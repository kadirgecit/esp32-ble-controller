# ESP32 BLE Controller - Usage Examples

This document provides practical examples of how to use the ESP32 BLE Controller with various types of BLE devices.

## Common BLE Device Examples

### 1. LED Strip Controller (WS2812B BLE)

Many BLE LED controllers use similar command structures:

**Device Connection:**
- Scan for devices named "BLE-LED" or similar
- Connect to the device

**Common Commands:**
```
Service UUID: 0000ffe0-0000-1000-8000-00805f9b34fb
Characteristic UUID: 0000ffe1-0000-1000-8000-00805f9b34fb

Turn On:  56FF00F0AA
Turn Off: 56FF0000AA
Red:      56FF0001AA
Green:    56FF0002AA
Blue:     56FF0003AA
White:    56FF0004AA
```

### 2. Smart Light Bulb

**Philips Hue or similar smart bulbs:**
```
Service UUID: 932c32bd-0000-47a2-835a-a8d455b859dd
Characteristic UUID: 932c32bd-0001-47a2-835a-a8d455b859dd

Power On:     01
Power Off:    00
Brightness:   02[VALUE] (VALUE: 00-FF)
Color Temp:   03[TEMP] (TEMP: warm=01, cool=FF)
```

### 3. BLE Heart Rate Monitor

**Standard Heart Rate Service:**
```
Service UUID: 0000180d-0000-1000-8000-00805f9b34fb
Characteristic UUID: 00002a37-0000-1000-8000-00805f9b34fb

Note: This is typically a notification characteristic.
Enable notifications to receive heart rate data.
```

### 4. Arduino/ESP32 Custom BLE Device

**Example custom service:**
```
Service UUID: 12345678-1234-1234-1234-123456789abc
Characteristic UUID: 87654321-4321-4321-4321-cba987654321

Custom Commands:
Hello:        48656C6C6F (hex for "Hello")
LED On:       4C4544204F4E (hex for "LED ON")
LED Off:      4C4544204F4646 (hex for "LED OFF")
Get Status:   53544154 (hex for "STAT")
```

### 5. Fitness Tracker

**Generic fitness tracker commands:**
```
Service UUID: 0000fee0-0000-1000-8000-00805f9b34fb
Characteristic UUID: 0000fee1-0000-1000-8000-00805f9b34fb

Sync Time:    01[TIMESTAMP]
Get Steps:    02
Get Battery:  03
Start Workout: 04
Stop Workout: 05
```

## Step-by-Step Usage Examples

### Example 1: Controlling a BLE LED Strip

1. **Scan and Connect:**
   - Click "Start Scan"
   - Look for device named "BLE-LED" or similar
   - Click on the device and select "Connect"

2. **Save the Device:**
   - After connecting, click "Save Device"
   - Name it "Living Room LED Strip"

3. **Create Commands:**
   - Go to Commands tab
   - Service UUID: `0000ffe0-0000-1000-8000-00805f9b34fb`
   - Characteristic UUID: `0000ffe1-0000-1000-8000-00805f9b34fb`
   - Create these commands:
     - Name: "Turn On", Data: `56FF00F0AA`
     - Name: "Turn Off", Data: `56FF0000AA`
     - Name: "Red", Data: `56FF0001AA`
     - Name: "Blue", Data: `56FF0003AA`

4. **Use Saved Commands:**
   - Click "Execute" next to any saved command
   - The LED strip should respond immediately

### Example 2: Custom Arduino BLE Device

If you have an Arduino with BLE capability, here's a simple sketch to test with:

```cpp
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-cba987654321"
#define LED_PIN 2

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String value = pCharacteristic->getValue().c_str();
        
        if (value == "LED_ON") {
            digitalWrite(LED_PIN, HIGH);
            Serial.println("LED turned ON");
        }
        else if (value == "LED_OFF") {
            digitalWrite(LED_PIN, LOW);
            Serial.println("LED turned OFF");
        }
        else if (value == "STATUS") {
            String status = digitalRead(LED_PIN) ? "LED is ON" : "LED is OFF";
            pCharacteristic->setValue(status.c_str());
            pCharacteristic->notify();
        }
    }
};

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    
    BLEDevice::init("Arduino BLE Test");
    pServer = BLEDevice::createServer();
    
    BLEService *pService = pServer->createService(SERVICE_UUID);
    
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_WRITE |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
    
    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->addDescriptor(new BLE2902());
    
    pService->start();
    pServer->getAdvertising()->start();
    Serial.println("BLE device ready!");
}

void loop() {
    delay(1000);
}
```

**Commands to test:**
- Service UUID: `12345678-1234-1234-1234-123456789abc`
- Characteristic UUID: `87654321-4321-4321-4321-cba987654321`
- Turn LED On: `LED_ON`
- Turn LED Off: `LED_OFF`
- Get Status: `STATUS`

## Data Format Examples

### Hex vs String Data

**Hex Format:**
- Use for binary data or when device expects specific byte values
- Example: `01FF00` (3 bytes: 0x01, 0xFF, 0x00)

**String Format:**
- Use for text-based commands
- Example: `HELLO` (sends ASCII characters)

### Common Data Patterns

**RGB Color (3 bytes):**
```
Red:   FF0000
Green: 00FF00
Blue:  0000FF
White: FFFFFF
```

**Temperature (2 bytes, little-endian):**
```
25°C: 1900 (0x0019 = 25 in decimal)
30°C: 1E00 (0x001E = 30 in decimal)
```

**Boolean Values:**
```
True/On:  01
False/Off: 00
```

## Troubleshooting Common Issues

### Device Won't Connect
1. Ensure device is in pairing mode
2. Check if device is already connected elsewhere
3. Try moving closer to the device
4. Restart both ESP32 and target device

### Commands Not Working
1. Verify Service and Characteristic UUIDs
2. Check data format (hex vs string)
3. Ensure characteristic supports write operations
4. Some devices require specific command sequences

### Finding UUIDs
1. Use the Services tab after connecting
2. Use smartphone apps like "BLE Scanner" (Android) or "LightBlue" (iOS)
3. Check device documentation
4. Use Nordic's nRF Connect app

## Advanced Usage

### Creating Command Sequences
For devices that require multiple commands:

1. Save individual commands first
2. Execute them in sequence manually
3. Note the timing requirements between commands

### Monitoring Responses
Some devices send responses back:

1. Check the Activity Log for any received data
2. Use the Services tab to identify notification characteristics
3. Enable notifications on characteristics that provide feedback

### Batch Operations
For controlling multiple similar devices:

1. Save each device with descriptive names
2. Create identical command sets for each device
3. Switch between devices as needed

## Security Considerations

1. **Pairing**: Some devices require pairing before accepting commands
2. **Authentication**: Enterprise devices may require authentication
3. **Encryption**: Sensitive commands should use encrypted characteristics
4. **Access Control**: Be cautious with devices that control physical systems

## Performance Tips

1. **Connection Management**: Disconnect when not in use to save battery
2. **Command Timing**: Some devices need delays between commands
3. **Range**: Stay within 10-30 feet for reliable communication
4. **Interference**: Avoid areas with heavy WiFi traffic

---

**Note**: Always refer to your specific device's documentation for exact command formats and UUIDs. The examples above are common patterns but may not work with all devices.