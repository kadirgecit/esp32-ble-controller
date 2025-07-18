#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEClient.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

// WiFi credentials - Update these
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Web server
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// BLE variables
BLEScan* pBLEScan;
BLEClient* pClient;
std::map<std::string, BLEAdvertisedDevice*> discoveredDevices;
bool isScanning = false;
bool isConnected = false;
std::string connectedDeviceAddress = "";

// Device and command storage
const char* DEVICES_FILE = "/devices.json";
const char* COMMANDS_FILE = "/commands.json";

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        String deviceInfo = "{";
        deviceInfo += "\"address\":\"" + String(advertisedDevice.getAddress().toString().c_str()) + "\",";
        deviceInfo += "\"name\":\"" + String(advertisedDevice.getName().c_str()) + "\",";
        deviceInfo += "\"rssi\":" + String(advertisedDevice.getRSSI()) + ",";
        deviceInfo += "\"serviceUUIDs\":[";
        
        if (advertisedDevice.haveServiceUUID()) {
            for (int i = 0; i < advertisedDevice.getServiceUUIDs().getCount(); i++) {
                if (i > 0) deviceInfo += ",";
                deviceInfo += "\"" + String(advertisedDevice.getServiceUUIDs().getNth(i).toString().c_str()) + "\"";
            }
        }
        deviceInfo += "]}";
        
        // Store device for later connection
        discoveredDevices[advertisedDevice.getAddress().toString()] = new BLEAdvertisedDevice(advertisedDevice);
        
        // Send to web interface
        ws.textAll("SCAN_RESULT:" + deviceInfo);
    }
};

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
        isConnected = true;
        ws.textAll("CONNECTED:" + String(connectedDeviceAddress.c_str()));
    }

    void onDisconnect(BLEClient* pclient) {
        isConnected = false;
        connectedDeviceAddress = "";
        ws.textAll("DISCONNECTED");
    }
};

void setup() {
    Serial.begin(115200);
    
    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    
    // Initialize BLE
    BLEDevice::init("ESP32_BLE_Controller");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Setup WebSocket
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
    
    // Serve static files
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    
    // API endpoints
    server.on("/api/scan", HTTP_GET, handleStartScan);
    server.on("/api/stop-scan", HTTP_GET, handleStopScan);
    server.on("/api/connect", HTTP_POST, handleConnect);
    server.on("/api/disconnect", HTTP_GET, handleDisconnect);
    server.on("/api/send-command", HTTP_POST, handleSendCommand);
    server.on("/api/get-services", HTTP_GET, handleGetServices);
    server.on("/api/devices", HTTP_GET, handleGetDevices);
    server.on("/api/devices", HTTP_POST, handleSaveDevice);
    server.on("/api/commands", HTTP_GET, handleGetCommands);
    server.on("/api/commands", HTTP_POST, handleSaveCommand);
    server.on("/api/commands", HTTP_DELETE, handleDeleteCommand);
    
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    ws.cleanupClients();
    delay(10);
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
               void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            // Handle WebSocket data if needed
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void handleStartScan(AsyncWebServerRequest *request) {
    if (!isScanning) {
        discoveredDevices.clear();
        pBLEScan->start(10, false);
        isScanning = true;
        ws.textAll("SCAN_STARTED");
    }
    request->send(200, "application/json", "{\"status\":\"scanning\"}");
}

void handleStopScan(AsyncWebServerRequest *request) {
    if (isScanning) {
        pBLEScan->stop();
        isScanning = false;
        ws.textAll("SCAN_STOPPED");
    }
    request->send(200, "application/json", "{\"status\":\"stopped\"}");
}

void handleConnect(AsyncWebServerRequest *request) {
    if (request->hasParam("address", true)) {
        String address = request->getParam("address", true)->value();
        connectToDevice(address.c_str());
        request->send(200, "application/json", "{\"status\":\"connecting\"}");
    } else {
        request->send(400, "application/json", "{\"error\":\"Missing address parameter\"}");
    }
}

void handleDisconnect(AsyncWebServerRequest *request) {
    if (pClient && isConnected) {
        pClient->disconnect();
    }
    request->send(200, "application/json", "{\"status\":\"disconnected\"}");
}

void handleSendCommand(AsyncWebServerRequest *request) {
    if (!isConnected) {
        request->send(400, "application/json", "{\"error\":\"Not connected to any device\"}");
        return;
    }
    
    if (request->hasParam("serviceUUID", true) && 
        request->hasParam("characteristicUUID", true) && 
        request->hasParam("data", true)) {
        
        String serviceUUID = request->getParam("serviceUUID", true)->value();
        String characteristicUUID = request->getParam("characteristicUUID", true)->value();
        String data = request->getParam("data", true)->value();
        
        bool success = sendBLECommand(serviceUUID.c_str(), characteristicUUID.c_str(), data.c_str());
        
        if (success) {
            request->send(200, "application/json", "{\"status\":\"command_sent\"}");
        } else {
            request->send(500, "application/json", "{\"error\":\"Failed to send command\"}");
        }
    } else {
        request->send(400, "application/json", "{\"error\":\"Missing required parameters\"}");
    }
}

void handleGetServices(AsyncWebServerRequest *request) {
    if (!isConnected || !pClient) {
        request->send(400, "application/json", "{\"error\":\"Not connected to any device\"}");
        return;
    }
    
    String servicesJson = "[";
    std::map<std::string, BLERemoteService*>* services = pClient->getServices();
    bool first = true;
    
    for (auto& service : *services) {
        if (!first) servicesJson += ",";
        first = false;
        
        servicesJson += "{";
        servicesJson += "\"uuid\":\"" + String(service.first.c_str()) + "\",";
        servicesJson += "\"characteristics\":[";
        
        std::map<std::string, BLERemoteCharacteristic*>* characteristics = service.second->getCharacteristics();
        bool firstChar = true;
        
        for (auto& characteristic : *characteristics) {
            if (!firstChar) servicesJson += ",";
            firstChar = false;
            
            servicesJson += "{";
            servicesJson += "\"uuid\":\"" + String(characteristic.first.c_str()) + "\",";
            servicesJson += "\"properties\":" + String(characteristic.second->getProperties());
            servicesJson += "}";
        }
        
        servicesJson += "]}";
    }
    
    servicesJson += "]";
    request->send(200, "application/json", servicesJson);
}

void handleGetDevices(AsyncWebServerRequest *request) {
    File file = SPIFFS.open(DEVICES_FILE, "r");
    if (!file) {
        request->send(200, "application/json", "[]");
        return;
    }
    
    String content = file.readString();
    file.close();
    request->send(200, "application/json", content);
}

void handleSaveDevice(AsyncWebServerRequest *request) {
    if (request->hasParam("name", true) && request->hasParam("address", true)) {
        String name = request->getParam("name", true)->value();
        String address = request->getParam("address", true)->value();
        
        // Load existing devices
        DynamicJsonDocument doc(4096);
        File file = SPIFFS.open(DEVICES_FILE, "r");
        if (file) {
            deserializeJson(doc, file);
            file.close();
        }
        
        // Add new device
        JsonObject device = doc.createNestedObject();
        device["name"] = name;
        device["address"] = address;
        device["saved_at"] = millis();
        
        // Save back to file
        file = SPIFFS.open(DEVICES_FILE, "w");
        serializeJson(doc, file);
        file.close();
        
        request->send(200, "application/json", "{\"status\":\"device_saved\"}");
    } else {
        request->send(400, "application/json", "{\"error\":\"Missing required parameters\"}");
    }
}

void handleGetCommands(AsyncWebServerRequest *request) {
    File file = SPIFFS.open(COMMANDS_FILE, "r");
    if (!file) {
        request->send(200, "application/json", "[]");
        return;
    }
    
    String content = file.readString();
    file.close();
    request->send(200, "application/json", content);
}

void handleSaveCommand(AsyncWebServerRequest *request) {
    if (request->hasParam("name", true) && 
        request->hasParam("serviceUUID", true) && 
        request->hasParam("characteristicUUID", true) && 
        request->hasParam("data", true)) {
        
        String name = request->getParam("name", true)->value();
        String serviceUUID = request->getParam("serviceUUID", true)->value();
        String characteristicUUID = request->getParam("characteristicUUID", true)->value();
        String data = request->getParam("data", true)->value();
        
        // Load existing commands
        DynamicJsonDocument doc(4096);
        File file = SPIFFS.open(COMMANDS_FILE, "r");
        if (file) {
            deserializeJson(doc, file);
            file.close();
        }
        
        // Add new command
        JsonObject command = doc.createNestedObject();
        command["name"] = name;
        command["serviceUUID"] = serviceUUID;
        command["characteristicUUID"] = characteristicUUID;
        command["data"] = data;
        command["saved_at"] = millis();
        
        // Save back to file
        file = SPIFFS.open(COMMANDS_FILE, "w");
        serializeJson(doc, file);
        file.close();
        
        request->send(200, "application/json", "{\"status\":\"command_saved\"}");
    } else {
        request->send(400, "application/json", "{\"error\":\"Missing required parameters\"}");
    }
}

void handleDeleteCommand(AsyncWebServerRequest *request) {
    if (request->hasParam("index", true)) {
        int index = request->getParam("index", true)->value().toInt();
        
        // Load existing commands
        DynamicJsonDocument doc(4096);
        File file = SPIFFS.open(COMMANDS_FILE, "r");
        if (file) {
            deserializeJson(doc, file);
            file.close();
            
            // Remove command at index
            doc.remove(index);
            
            // Save back to file
            file = SPIFFS.open(COMMANDS_FILE, "w");
            serializeJson(doc, file);
            file.close();
            
            request->send(200, "application/json", "{\"status\":\"command_deleted\"}");
        } else {
            request->send(404, "application/json", "{\"error\":\"Commands file not found\"}");
        }
    } else {
        request->send(400, "application/json", "{\"error\":\"Missing index parameter\"}");
    }
}

bool connectToDevice(const char* address) {
    if (discoveredDevices.find(address) == discoveredDevices.end()) {
        return false;
    }
    
    pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback());
    
    BLEAdvertisedDevice* device = discoveredDevices[address];
    
    if (pClient->connect(device)) {
        connectedDeviceAddress = address;
        return true;
    }
    
    return false;
}

bool sendBLECommand(const char* serviceUUID, const char* characteristicUUID, const char* data) {
    if (!pClient || !isConnected) {
        return false;
    }
    
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
        return false;
    }
    
    BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(characteristicUUID);
    if (pRemoteCharacteristic == nullptr) {
        return false;
    }
    
    // Convert hex string to bytes if needed
    std::vector<uint8_t> bytes;
    if (strlen(data) % 2 == 0) {
        for (size_t i = 0; i < strlen(data); i += 2) {
            std::string byteString = std::string(data).substr(i, 2);
            uint8_t byte = (uint8_t) strtol(byteString.c_str(), NULL, 16);
            bytes.push_back(byte);
        }
        pRemoteCharacteristic->writeValue(bytes.data(), bytes.size());
    } else {
        // Send as string
        pRemoteCharacteristic->writeValue((uint8_t*)data, strlen(data));
    }
    
    return true;
}