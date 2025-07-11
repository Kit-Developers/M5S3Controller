#include "web_server.h"
#include "wifi_manager.h"
#include "lcd_display.h"
#include "env.h"

void initWebServer() {
    if (!wifi_connected) return;
    
    server.on("/", handleRoot);
    server.on("/controller", HTTP_POST, handleControllerPOST);
    
    // CORS対応
    server.enableCORS(true);
    
    server.begin();
}

void handleWebServer() {
    if (wifi_connected) {
        server.handleClient();
    }
}

void handleRoot() {
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<title>Nintendo Switch Controller</title></head><body>";
    html += "<h1>Nintendo Switch Controller Web Interface</h1>";
    html += "<p>POST /controller endpoint ready</p>";
    html += "<h2>JSON Format:</h2>";
    html += "<pre>{<br>";
    html += "  \"buttons\": {\"A\": true, \"B\": false, \"X\": false, \"Y\": false},<br>";
    html += "  \"lstick\": {\"x\": 0, \"y\": 0},<br>";
    html += "  \"rstick\": {\"x\": 0, \"y\": 0},<br>";
    html += "  \"shoulder\": {\"L\": false, \"R\": false, \"ZL\": false, \"ZR\": false},<br>";
    html += "  \"system\": {\"plus\": false, \"minus\": false, \"home\": false}<br>";
    html += "}</pre>";
    html += "<p>Sticks: x,y range -100 to 100</p>";
    html += "</body></html>";
    
    server.send(200, "text/html", html);
}

void handleControllerPOST() {
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        JsonDocument doc;
        
        DeserializationError error = deserializeJson(doc, body);
        if (error) {
            server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            return;
        }
        
        unsigned long currentTime = millis();
        String latestInput = "";
        
        // ボタン状態更新
        if (doc["buttons"].is<JsonObject>()) {
            JsonObject buttons = doc["buttons"];
            
            // 各ボタンの状態更新と最新入力追跡
            if (buttons["A"] | false) { webButtons.A = true; latestInput = "A"; }
            else { webButtons.A = false; }
            
            if (buttons["B"] | false) { webButtons.B = true; latestInput = "B"; }
            else { webButtons.B = false; }
            
            if (buttons["X"] | false) { webButtons.X = true; latestInput = "X"; }
            else { webButtons.X = false; }
            
            if (buttons["Y"] | false) { webButtons.Y = true; latestInput = "Y"; }
            else { webButtons.Y = false; }
        }
        
        // 左スティック状態更新
        if (doc["lstick"].is<JsonObject>()) {
            JsonObject lstick = doc["lstick"];
            int new_x = lstick["x"] | 0;
            int new_y = lstick["y"] | 0;
            
            // 範囲制限
            new_x = constrain(new_x, -100, 100);
            new_y = constrain(new_y, -100, 100);
            
            webButtons.lstick_x = new_x;
            webButtons.lstick_y = new_y;
            
            // スティック入力がある場合（閾値10以上）
            if (abs(new_x) > 10 || abs(new_y) > 10) {
                latestInput = "STICK_L";
            }
        }
        
        // 右スティック状態更新
        if (doc["rstick"].is<JsonObject>()) {
            JsonObject rstick = doc["rstick"];
            int new_x = rstick["x"] | 0;
            int new_y = rstick["y"] | 0;
            
            // 範囲制限
            new_x = constrain(new_x, -100, 100);
            new_y = constrain(new_y, -100, 100);
            
            webButtons.rstick_x = new_x;
            webButtons.rstick_y = new_y;
            
            // スティック入力がある場合（閾値10以上）
            if (abs(new_x) > 10 || abs(new_y) > 10) {
                latestInput = "STICK_R";
            }
        }
        
        // ショルダーボタン状態更新
        if (doc["shoulder"].is<JsonObject>()) {
            JsonObject shoulder = doc["shoulder"];
            
            if (shoulder["L"] | false) { webButtons.L = true; latestInput = "L"; }
            else { webButtons.L = false; }
            
            if (shoulder["R"] | false) { webButtons.R = true; latestInput = "R"; }
            else { webButtons.R = false; }
            
            if (shoulder["ZL"] | false) { webButtons.ZL = true; latestInput = "ZL"; }
            else { webButtons.ZL = false; }
            
            if (shoulder["ZR"] | false) { webButtons.ZR = true; latestInput = "ZR"; }
            else { webButtons.ZR = false; }
        }
        
        // システムボタン状態更新
        if (doc["system"].is<JsonObject>()) {
            JsonObject system = doc["system"];
            
            if (system["plus"] | false) { webButtons.plus = true; latestInput = "+"; }
            else { webButtons.plus = false; }
            
            if (system["minus"] | false) { webButtons.minus = true; latestInput = "-"; }
            else { webButtons.minus = false; }
            
            if (system["home"] | false) { webButtons.home = true; latestInput = "HOME"; }
            else { webButtons.home = false; }
        }
        
        // 最新入力を記録（アクティブな入力がある場合のみ）
        if (latestInput != "") {
            webButtons.last_active_input = latestInput;
            webButtons.last_update_time = currentTime;
        }
        
        server.send(200, "application/json", "{\"status\":\"OK\"}");
    } else {
        server.send(400, "application/json", "{\"error\":\"No JSON body\"}");
    }
}

void updateWebInput() {
    // Web入力をボタン状態に反映
    btnA.web_input = webButtons.A;
    btnB.web_input = webButtons.B;
    btnX.web_input = webButtons.X;
    btnY.web_input = webButtons.Y;
    btnPlus.web_input = webButtons.plus;
    btnMinus.web_input = webButtons.minus;
    btnHome.web_input = webButtons.home;
    
    // 左スティックの計算
    lstickUp.web_input = (webButtons.lstick_y < -LSTICK_THRESHOLD);
    lstickDown.web_input = (webButtons.lstick_y > LSTICK_THRESHOLD);
    lstickLeft.web_input = (webButtons.lstick_x < -LSTICK_THRESHOLD);
    lstickRight.web_input = (webButtons.lstick_x > LSTICK_THRESHOLD);
} 