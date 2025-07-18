# Changelog

All notable changes to the ESP32 BLE Controller project will be documented in this file.

## [2.0.0] - 2024-07-18

### 🆕 Major Features Added
- **WiFi Access Point Mode**: Automatic AP mode for easy WiFi configuration without hardcoding credentials
- **WiFi Configuration Interface**: Beautiful, responsive web interface for WiFi setup
- **Captive Portal**: Seamless WiFi setup experience with automatic redirection
- **Persistent WiFi Storage**: WiFi credentials stored in ESP32's non-volatile memory
- **Network Scanning**: Real-time WiFi network discovery with signal strength indicators

### ✨ Enhancements
- **Automatic Fallback**: ESP32 automatically creates AP when WiFi connection fails
- **Mobile-Responsive Setup**: WiFi configuration page optimized for mobile devices
- **Enhanced Configuration**: Extended `config.h` with comprehensive WiFi and AP settings
- **Status LED Integration**: Visual feedback for WiFi connection status
- **Timeout Management**: Configurable AP mode timeout with automatic restart
- **Security Indicators**: Clear visual indicators for secured vs. open networks

### 📚 Documentation
- **WiFi Setup Guide**: Comprehensive guide for WiFi configuration (`WIFI_SETUP.md`)
- **Updated README**: Enhanced installation and usage instructions
- **Configuration Reference**: Detailed configuration options documentation

### 🔧 Technical Improvements
- **DNS Server**: Built-in DNS server for captive portal functionality
- **Preferences Library**: Secure credential storage using ESP32 Preferences
- **Error Handling**: Improved error handling and user feedback
- **Code Organization**: Better separation of WiFi management functions

### 🛠️ Configuration Changes
- **New Dependencies**: Added DNSServer and Preferences libraries
- **Extended Config**: New WiFi and AP configuration options in `config.h`
- **Backward Compatibility**: Maintains compatibility with manual WiFi configuration

## [1.0.0] - 2024-07-18

### 🎉 Initial Release
- **BLE Device Scanning**: Discover nearby BLE devices with real-time updates
- **Device Connection Management**: Connect to and manage BLE device connections
- **Service Discovery**: Explore device services and characteristics
- **Command Management**: Send custom commands to connected BLE devices
- **Persistent Storage**: Save devices and commands using SPIFFS
- **Modern Web Interface**: Responsive web UI with real-time WebSocket updates
- **Mobile Support**: Fully responsive design for smartphones and tablets
- **Demo Mode**: Local demo server for testing without hardware
- **Comprehensive Documentation**: Complete setup and usage guides
- **PlatformIO Support**: Multi-platform development environment support
- **MIT License**: Open source with permissive licensing

### 📦 Core Components
- **ESP32 Arduino Sketch**: Main controller firmware
- **Web Interface**: HTML/CSS/JavaScript frontend
- **Configuration System**: Flexible configuration management
- **Documentation**: README, examples, troubleshooting guides
- **Demo Server**: Python-based demo server for testing

### 🔧 Supported Hardware
- ESP32-WROOM-32
- ESP32-DevKit
- ESP32-S3
- ESP32-C3
- Other ESP32 variants

---

## Version Numbering

This project follows [Semantic Versioning](https://semver.org/):
- **MAJOR**: Incompatible API changes
- **MINOR**: New functionality in a backwards compatible manner
- **PATCH**: Backwards compatible bug fixes

## Contributing

See [README.md](README.md) for contribution guidelines and development setup instructions.