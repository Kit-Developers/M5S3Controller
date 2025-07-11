#include "wifi_manager.h"

// WiFi設定（実体）
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// WiFi状態管理（実体）
bool wifi_connected = false;
String wifi_ip = "";

void initWiFi() {
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECT_TIMEOUT) {
        delay(500);
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        wifi_connected = true;
        wifi_ip = WiFi.localIP().toString();
    } else {
        wifi_connected = false;
    }
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void reconnectWiFi() {
    if (!isWiFiConnected() && wifi_connected) {
        wifi_connected = false;
        
        // 再接続試行
        WiFi.reconnect();
        
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < WIFI_RECONNECT_TIMEOUT) {
            delay(500);
            attempts++;
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            wifi_connected = true;
            wifi_ip = WiFi.localIP().toString();
        }
    }
} 