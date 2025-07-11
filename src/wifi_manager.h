#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "types.h"
#include "env.h"

// WiFi状態管理
extern bool wifi_connected;
extern String wifi_ip;

/**
 * WiFi接続機能
 */
void initWiFi();

/**
 * WiFi接続状態チェック
 */
bool isWiFiConnected();

/**
 * WiFi再接続試行
 */
void reconnectWiFi();

#endif // WIFI_MANAGER_H 