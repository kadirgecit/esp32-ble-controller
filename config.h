/**
 * ESP32 BLE Controller Configuration File
 * 
 * Modify these settings to customize the behavior of your ESP32 BLE Controller
 */

#ifndef CONFIG_H
#define CONFIG_H

// =============================================================================
// WIFI CONFIGURATION
// =============================================================================

// WiFi credentials - CHANGE THESE TO YOUR NETWORK
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// WiFi connection timeout (milliseconds)
#define WIFI_TIMEOUT 30000

// WiFi reconnection attempts
#define WIFI_MAX_RETRIES 5

// =============================================================================
// WEB SERVER CONFIGURATION
// =============================================================================

// Web server port
#define WEB_SERVER_PORT 80

// WebSocket path
#define WEBSOCKET_PATH "/ws"

// Maximum number of WebSocket clients
#define MAX_WS_CLIENTS 4

// =============================================================================
// BLE CONFIGURATION
// =============================================================================

// BLE device name (how this ESP32 appears to other devices)
#define BLE_DEVICE_NAME "ESP32_BLE_Controller"

// BLE scan duration (seconds)
#define BLE_SCAN_DURATION 10

// BLE scan interval (1.25ms units, range: 0x0004 to 0x4000)
#define BLE_SCAN_INTERVAL 1349

// BLE scan window (1.25ms units, range: 0x0004 to 0x4000, must be <= interval)
#define BLE_SCAN_WINDOW 449

// Enable active scanning (requests scan response data)
#define BLE_ACTIVE_SCAN true

// Connection timeout (milliseconds)
#define BLE_CONNECTION_TIMEOUT 10000

// Maximum number of discovered devices to track
#define MAX_DISCOVERED_DEVICES 50

// =============================================================================
// STORAGE CONFIGURATION
// =============================================================================

// File paths for persistent storage
#define DEVICES_FILE "/devices.json"
#define COMMANDS_FILE "/commands.json"

// Maximum number of saved devices
#define MAX_SAVED_DEVICES 20

// Maximum number of saved commands
#define MAX_SAVED_COMMANDS 50

// JSON document size for device storage (bytes)
#define DEVICES_JSON_SIZE 4096

// JSON document size for command storage (bytes)
#define COMMANDS_JSON_SIZE 8192

// =============================================================================
// LOGGING CONFIGURATION
// =============================================================================

// Serial baud rate
#define SERIAL_BAUD_RATE 115200

// Enable debug logging
#define ENABLE_DEBUG_LOG true

// Log levels: 0=None, 1=Error, 2=Warn, 3=Info, 4=Debug, 5=Verbose
#define LOG_LEVEL 3

// Enable BLE debug logging
#define ENABLE_BLE_DEBUG false

// Enable WiFi debug logging
#define ENABLE_WIFI_DEBUG false

// =============================================================================
// PERFORMANCE CONFIGURATION
// =============================================================================

// Main loop delay (milliseconds)
#define MAIN_LOOP_DELAY 10

// WebSocket cleanup interval (milliseconds)
#define WS_CLEANUP_INTERVAL 1000

// Memory check interval (milliseconds)
#define MEMORY_CHECK_INTERVAL 30000

// Minimum free heap before warning (bytes)
#define MIN_FREE_HEAP 10000

// =============================================================================
// SECURITY CONFIGURATION
// =============================================================================

// Enable basic authentication (not implemented in base version)
#define ENABLE_AUTH false

// Default username and password (if auth is enabled)
#define DEFAULT_USERNAME "admin"
#define DEFAULT_PASSWORD "esp32ble"

// Enable HTTPS (requires certificates)
#define ENABLE_HTTPS false

// =============================================================================
// HARDWARE CONFIGURATION
// =============================================================================

// Status LED pin (set to -1 to disable)
#define STATUS_LED_PIN 2

// LED blink patterns
#define LED_BLINK_WIFI_CONNECTING 500    // ms
#define LED_BLINK_BLE_SCANNING 200       // ms
#define LED_BLINK_BLE_CONNECTED 1000     // ms

// Button pin for factory reset (set to -1 to disable)
#define RESET_BUTTON_PIN -1

// Button press duration for factory reset (milliseconds)
#define RESET_BUTTON_DURATION 5000

// =============================================================================
// ADVANCED CONFIGURATION
// =============================================================================

// Enable over-the-air (OTA) updates
#define ENABLE_OTA false

// OTA password
#define OTA_PASSWORD "esp32update"

// Enable mDNS (allows access via hostname.local)
#define ENABLE_MDNS true

// mDNS hostname
#define MDNS_HOSTNAME "esp32ble"

// Enable CORS headers for cross-origin requests
#define ENABLE_CORS true

// Watchdog timer timeout (seconds, 0 to disable)
#define WATCHDOG_TIMEOUT 30

// =============================================================================
// FEATURE FLAGS
// =============================================================================

// Enable device auto-reconnection
#define ENABLE_AUTO_RECONNECT true

// Auto-reconnect interval (milliseconds)
#define AUTO_RECONNECT_INTERVAL 5000

// Enable command history
#define ENABLE_COMMAND_HISTORY true

// Maximum command history entries
#define MAX_COMMAND_HISTORY 100

// Enable device statistics
#define ENABLE_DEVICE_STATS true

// Enable notification support
#define ENABLE_NOTIFICATIONS true

// Enable characteristic read operations
#define ENABLE_CHARACTERISTIC_READ true

// =============================================================================
// VALIDATION MACROS
// =============================================================================

// Validate configuration values
#if BLE_SCAN_WINDOW > BLE_SCAN_INTERVAL
#error "BLE_SCAN_WINDOW must be <= BLE_SCAN_INTERVAL"
#endif

#if MAX_WS_CLIENTS > 8
#warning "High number of WebSocket clients may cause memory issues"
#endif

#if DEVICES_JSON_SIZE < 1024
#warning "DEVICES_JSON_SIZE may be too small for multiple devices"
#endif

#if COMMANDS_JSON_SIZE < 2048
#warning "COMMANDS_JSON_SIZE may be too small for multiple commands"
#endif

// =============================================================================
// HELPER MACROS
// =============================================================================

// Debug logging macro
#if ENABLE_DEBUG_LOG
#define DEBUG_LOG(level, format, ...) \
    if (level <= LOG_LEVEL) { \
        Serial.printf("[%s] " format "\n", #level, ##__VA_ARGS__); \
    }
#else
#define DEBUG_LOG(level, format, ...)
#endif

// Memory check macro
#define CHECK_MEMORY() \
    if (ESP.getFreeHeap() < MIN_FREE_HEAP) { \
        DEBUG_LOG(WARN, "Low memory warning: %d bytes free", ESP.getFreeHeap()); \
    }

// Status LED control macro
#if STATUS_LED_PIN >= 0
#define SET_STATUS_LED(state) digitalWrite(STATUS_LED_PIN, state)
#define INIT_STATUS_LED() pinMode(STATUS_LED_PIN, OUTPUT)
#else
#define SET_STATUS_LED(state)
#define INIT_STATUS_LED()
#endif

#endif // CONFIG_H