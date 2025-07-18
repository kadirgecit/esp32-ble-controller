#!/usr/bin/env python3
"""
Simple demo server to showcase the ESP32 BLE Controller web interface
This simulates the ESP32 responses for demonstration purposes
"""

import json
import time
import os
from http.server import HTTPServer, SimpleHTTPRequestHandler
from urllib.parse import parse_qs, urlparse
import threading

# Change to the data directory
os.chdir('data')

class BLEControllerHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
    
    def do_GET(self):
        parsed_path = urlparse(self.path)
        
        if parsed_path.path == '/api/scan':
            self.handle_scan()
        elif parsed_path.path == '/api/stop-scan':
            self.handle_stop_scan()
        elif parsed_path.path == '/api/disconnect':
            self.handle_disconnect()
        elif parsed_path.path == '/api/get-services':
            self.handle_get_services()
        elif parsed_path.path == '/api/devices':
            self.handle_get_devices()
        elif parsed_path.path == '/api/commands':
            self.handle_get_commands()
        elif parsed_path.path == '/api/wifi/status':
            self.handle_wifi_status()
        elif parsed_path.path == '/ws':
            self.handle_websocket()
        else:
            super().do_GET()
    
    def do_POST(self):
        parsed_path = urlparse(self.path)
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length).decode('utf-8')
        
        if parsed_path.path == '/api/connect':
            self.handle_connect(post_data)
        elif parsed_path.path == '/api/send-command':
            self.handle_send_command(post_data)
        elif parsed_path.path == '/api/devices':
            self.handle_save_device(post_data)
        elif parsed_path.path == '/api/commands':
            self.handle_save_command(post_data)
    
    def do_DELETE(self):
        parsed_path = urlparse(self.path)
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length).decode('utf-8')
        
        if parsed_path.path == '/api/commands':
            self.handle_delete_command(post_data)
    
    def send_json_response(self, data, status=200):
        self.send_response(status)
        self.send_header('Content-type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, DELETE, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()
        self.wfile.write(json.dumps(data).encode())
    
    def do_OPTIONS(self):
        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, DELETE, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()
    
    def handle_scan(self):
        self.send_json_response({"status": "scanning"})
    
    def handle_stop_scan(self):
        self.send_json_response({"status": "stopped"})
    
    def handle_connect(self, post_data):
        params = parse_qs(post_data)
        address = params.get('address', [''])[0]
        self.send_json_response({"status": "connecting"})
    
    def handle_disconnect(self):
        self.send_json_response({"status": "disconnected"})
    
    def handle_send_command(self, post_data):
        self.send_json_response({"status": "command_sent"})
    
    def handle_get_services(self):
        # Simulate device services
        services = [
            {
                "uuid": "12345678-1234-1234-1234-123456789abc",
                "characteristics": [
                    {"uuid": "87654321-4321-4321-4321-cba987654321", "properties": 10},
                    {"uuid": "11111111-2222-3333-4444-555555555555", "properties": 8}
                ]
            },
            {
                "uuid": "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee",
                "characteristics": [
                    {"uuid": "ffffffff-eeee-dddd-cccc-bbbbbbbbbbbb", "properties": 12}
                ]
            }
        ]
        self.send_json_response(services)
    
    def handle_get_devices(self):
        # Load or create demo devices
        try:
            with open('devices.json', 'r') as f:
                devices = json.load(f)
        except FileNotFoundError:
            devices = [
                {"name": "Demo Heart Rate Monitor", "address": "AA:BB:CC:DD:EE:FF", "saved_at": int(time.time() * 1000)},
                {"name": "Demo Temperature Sensor", "address": "11:22:33:44:55:66", "saved_at": int(time.time() * 1000)}
            ]
        self.send_json_response(devices)
    
    def handle_save_device(self, post_data):
        params = parse_qs(post_data)
        name = params.get('name', [''])[0]
        address = params.get('address', [''])[0]
        
        # Load existing devices
        try:
            with open('devices.json', 'r') as f:
                devices = json.load(f)
        except FileNotFoundError:
            devices = []
        
        # Add new device
        devices.append({
            "name": name,
            "address": address,
            "saved_at": int(time.time() * 1000)
        })
        
        # Save back
        with open('devices.json', 'w') as f:
            json.dump(devices, f)
        
        self.send_json_response({"status": "device_saved"})
    
    def handle_get_commands(self):
        try:
            with open('commands.json', 'r') as f:
                commands = json.load(f)
        except FileNotFoundError:
            commands = [
                {
                    "name": "Turn On LED",
                    "serviceUUID": "12345678-1234-1234-1234-123456789abc",
                    "characteristicUUID": "87654321-4321-4321-4321-cba987654321",
                    "data": "01",
                    "saved_at": int(time.time() * 1000)
                },
                {
                    "name": "Get Temperature",
                    "serviceUUID": "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee",
                    "characteristicUUID": "ffffffff-eeee-dddd-cccc-bbbbbbbbbbbb",
                    "data": "TEMP",
                    "saved_at": int(time.time() * 1000)
                }
            ]
        self.send_json_response(commands)
    
    def handle_save_command(self, post_data):
        params = parse_qs(post_data)
        name = params.get('name', [''])[0]
        service_uuid = params.get('serviceUUID', [''])[0]
        characteristic_uuid = params.get('characteristicUUID', [''])[0]
        data = params.get('data', [''])[0]
        
        # Load existing commands
        try:
            with open('commands.json', 'r') as f:
                commands = json.load(f)
        except FileNotFoundError:
            commands = []
        
        # Add new command
        commands.append({
            "name": name,
            "serviceUUID": service_uuid,
            "characteristicUUID": characteristic_uuid,
            "data": data,
            "saved_at": int(time.time() * 1000)
        })
        
        # Save back
        with open('commands.json', 'w') as f:
            json.dump(commands, f)
        
        self.send_json_response({"status": "command_saved"})
    
    def handle_delete_command(self, post_data):
        params = parse_qs(post_data)
        index = int(params.get('index', ['0'])[0])
        
        try:
            with open('commands.json', 'r') as f:
                commands = json.load(f)
            
            if 0 <= index < len(commands):
                commands.pop(index)
                
                with open('commands.json', 'w') as f:
                    json.dump(commands, f)
                
                self.send_json_response({"status": "command_deleted"})
            else:
                self.send_json_response({"error": "Invalid index"}, 400)
        except FileNotFoundError:
            self.send_json_response({"error": "Commands file not found"}, 404)
    
    def handle_wifi_status(self):
        """Handle WiFi status request - demo mode is not in AP mode"""
        response = {
            "isAPMode": False,
            "wifiConnected": True,
            "currentSSID": "Demo-Network",
            "ipAddress": "127.0.0.1",
            "apIP": ""
        }
        self.send_json_response(response)
    
    def handle_websocket(self):
        """Handle WebSocket connection attempt - return 404 for demo mode"""
        self.send_response(404)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.wfile.write(b'WebSocket not supported in demo mode')

if __name__ == "__main__":
    # Start HTTP server
    server = HTTPServer(('0.0.0.0', 12000), BLEControllerHandler)
    print("Demo server running at http://localhost:12000")
    print("Note: This is a demo version without real-time WebSocket updates")
    print("Press Ctrl+C to stop")
    
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down server...")
        server.shutdown()