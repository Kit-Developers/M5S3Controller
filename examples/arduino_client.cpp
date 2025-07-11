/*
 * Nintendo Switch Controller Web API - Arduino Client Sample
 * M5AtomS3 SwitchController WebサーバーにPOSTリクエストを送信するサンプル
 * ESP32系マイコン用（WiFi機能が必要）
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi設定
const char* ssid = "YOUR_WIFI_SSID";          // WiFi SSID
const char* password = "YOUR_WIFI_PASSWORD";   // WiFi パスワード

// M5AtomS3 コントローラーの設定
const char* controllerIP = "192.168.1.100";    // M5AtomS3のIPアドレス
String controllerURL = "http://" + String(controllerIP) + "/controller";

// HTTPクライアント
HTTPClient http;

// 関数の前方宣言
bool sendControllerInput(bool btnA = false, bool btnB = false, bool btnX = false, bool btnY = false,
                        int lstickX = 0, int lstickY = 0, int rstickX = 0, int rstickY = 0,
                        bool btnL = false, bool btnR = false, bool btnZL = false, bool btnZR = false,
                        bool btnPlus = false, bool btnMinus = false, bool btnHome = false);
void connectWiFi();
void interactiveMode();

/**
 * コントローラー入力をM5AtomS3に送信
 */
bool sendControllerInput(bool btnA, bool btnB, bool btnX, bool btnY,
                        int lstickX, int lstickY, int rstickX, int rstickY,
                        bool btnL, bool btnR, bool btnZL, bool btnZR,
                        bool btnPlus, bool btnMinus, bool btnHome) {
    
    // JSON作成
    DynamicJsonDocument doc(512);
    
    // ボタン情報
    JsonObject buttons = doc.createNestedObject("buttons");
    buttons["A"] = btnA;
    buttons["B"] = btnB;
    buttons["X"] = btnX;
    buttons["Y"] = btnY;
    
    // 左スティック情報
    JsonObject lstick = doc.createNestedObject("lstick");
    lstick["x"] = constrain(lstickX, -100, 100);
    lstick["y"] = constrain(lstickY, -100, 100);
    
    // 右スティック情報
    JsonObject rstick = doc.createNestedObject("rstick");
    rstick["x"] = constrain(rstickX, -100, 100);
    rstick["y"] = constrain(rstickY, -100, 100);
    
    // ショルダーボタン情報
    JsonObject shoulder = doc.createNestedObject("shoulder");
    shoulder["L"] = btnL;
    shoulder["R"] = btnR;
    shoulder["ZL"] = btnZL;
    shoulder["ZR"] = btnZR;
    
    // システムボタン情報
    JsonObject system = doc.createNestedObject("system");
    system["plus"] = btnPlus;
    system["minus"] = btnMinus;
    system["home"] = btnHome;
    
    // JSON文字列に変換
    String jsonString;
    serializeJson(doc, jsonString);
    
    // HTTP POST送信
    http.begin(controllerURL);
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST(jsonString);
    
    if (httpResponseCode == 200) {
        Serial.println("✓ 送信成功: " + jsonString);
        http.end();
        return true;
    } else {
        Serial.println("✗ 送信失敗: " + String(httpResponseCode));
        String response = http.getString();
        Serial.println("Response: " + response);
        http.end();
        return false;
    }
}

/**
 * WiFi接続
 */
void connectWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("WiFi接続中");
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("WiFi接続成功!");
        Serial.print("IPアドレス: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println();
        Serial.println("WiFi接続失敗");
        while (true) {
            delay(1000);
        }
    }
}

/**
 * インタラクティブモード
 */
void interactiveMode() {
    Serial.println("=== インタラクティブモード ===");
    Serial.println("コマンド:");
    Serial.println("  A, B, X, Y    - メインボタン押下");
    Serial.println("  l, r, zl, zr  - ショルダーボタン押下");
    Serial.println("  +, -, h       - システムボタン (plus, minus, home)");
    Serial.println("  w,a,s,d       - 左スティック移動 (上,左,下,右)");
    Serial.println("  i,j,k,l       - 右スティック移動 (上,左,下,右)");
    Serial.println("  reset         - すべてのスティックをリセット(中央)");
    Serial.println();
    
    while (true) {
        if (Serial.available()) {
            String cmd = Serial.readStringUntil('\n');
            cmd.trim();
            
            // メインボタン（大文字のみ）
            if (cmd.equals("A")) {
                sendControllerInput(true, false, false, false);
                delay(1000);
                sendControllerInput(false, false, false, false);
            }
            else if (cmd.equals("B")) {
                sendControllerInput(false, true, false, false);
                delay(1000);
                sendControllerInput(false, false, false, false);
            }
            else if (cmd.equals("X")) {
                sendControllerInput(false, false, true, false);
                delay(1000);
                sendControllerInput(false, false, false, false);
            }
            else if (cmd.equals("Y")) {
                sendControllerInput(false, false, false, true);
                delay(1000);
                sendControllerInput(false, false, false, false);
            }
            // ショルダーボタン
            else if (cmd.equalsIgnoreCase("l")) {
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, true, false, false, false);
                delay(1000);
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, false, false);
            }
            else if (cmd.equalsIgnoreCase("r")) {
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, true, false, false);
                delay(1000);
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, false, false);
            }
            else if (cmd.equalsIgnoreCase("zl")) {
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, true, false);
                delay(1000);
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, false, false);
            }
            else if (cmd.equalsIgnoreCase("zr")) {
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, false, true);
                delay(1000);
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, false, false);
            }
            // システムボタン
            else if (cmd == "+") {
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, false, false, true, false, false);
                delay(1000);
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, false, false, false, false, false);
            }
            else if (cmd == "-") {
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, false, false, false, true, false);
                delay(1000);
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, false, false, false, false, false);
            }
            else if (cmd.equalsIgnoreCase("h")) {
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, false, false, false, false, true);
                delay(1000);
                sendControllerInput(false, false, false, false, 0, 0, 0, 0, false, false, false, false, false, false, false);
            }
            // 左スティック
            else if (cmd.equalsIgnoreCase("w")) {
                sendControllerInput(false, false, false, false, 0, 100, 0, 0);  // 上方向（修正: -100 → 100）
            }
            else if (cmd.equalsIgnoreCase("a")) {
                sendControllerInput(false, false, false, false, -100, 0, 0, 0);
            }
            else if (cmd.equalsIgnoreCase("s")) {
                sendControllerInput(false, false, false, false, 0, -100, 0, 0);  // 下方向（修正: 100 → -100）
            }
            else if (cmd.equalsIgnoreCase("d")) {
                sendControllerInput(false, false, false, false, 100, 0, 0, 0);
            }
            // 右スティック
            else if (cmd.equalsIgnoreCase("i")) {
                sendControllerInput(false, false, false, false, 0, 0, 0, 100);  // 上方向（修正: -100 → 100）
            }
            else if (cmd.equalsIgnoreCase("j")) {
                sendControllerInput(false, false, false, false, 0, 0, -100, 0);
            }
            else if (cmd.equalsIgnoreCase("k")) {
                sendControllerInput(false, false, false, false, 0, 0, 0, -100);  // 下方向（修正: 100 → -100）
            }
            else if (cmd.equalsIgnoreCase("l")) {
                sendControllerInput(false, false, false, false, 0, 0, 100, 0);
            }
            // リセット
            else if (cmd.equalsIgnoreCase("reset")) {
                sendControllerInput();
            }
            else {
                Serial.println("無効なコマンドです");
            }
        }
        
        delay(100);  // CPU負荷軽減
    }
}

/**
 * 接続テスト
 */
bool testConnection() {
    Serial.println("接続テスト中...");
    
    if (sendControllerInput()) {
        Serial.println("接続成功!");
        return true;
    } else {
        Serial.println("M5AtomS3への接続に失敗しました。");
        Serial.println("1. M5AtomS3が起動しているか確認してください");
        Serial.println("2. WiFi接続が正常か確認してください");
        Serial.print("3. IPアドレス ");
        Serial.print(controllerIP);
        Serial.println(" が正しいか確認してください");
        return false;
    }
}

/**
 * セットアップ
 */
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("==================================");
    Serial.println("Nintendo Switch Controller - Arduino Client");
    Serial.println("M5AtomS3 SwitchController 制御サンプル");
    Serial.println("==================================");
    
    // WiFi接続
    connectWiFi();
    
    Serial.print("接続先: ");
    Serial.println(controllerURL);
    Serial.println();
    
    // 接続テスト
    if (!testConnection()) {
        Serial.println("プログラムを停止します。");
        while (true) {
            delay(1000);
        }
    }
    
    Serial.println("インタラクティブモード開始!");
    Serial.println();
}

/**
 * メインループ
 */
void loop() {
    interactiveMode();
} 