#include "lcd_display.h"
#include "wifi_manager.h"
#include "env.h"

// Nintendo Switch ボタン（実体）
TouchButton btnA(240, 120, 70, 50, GREEN, 0x00AA00, "A");
TouchButton btnB(240, 180, 70, 50, RED, 0xAA0000, "B");
TouchButton btnX(160, 120, 70, 50, BLUE, 0x0000AA, "X");
TouchButton btnY(160, 180, 70, 50, YELLOW, 0xAAAA00, "Y");

// 左スティック（実体）
TouchButton lstickUp(60, 100, 50, 40, 0x00CED1, 0x008B8D, "↑");
TouchButton lstickDown(60, 180, 50, 40, 0x00CED1, 0x008B8D, "↓");
TouchButton lstickLeft(10, 140, 40, 50, 0x00CED1, 0x008B8D, "←");
TouchButton lstickRight(110, 140, 40, 50, 0x00CED1, 0x008B8D, "→");

// 追加ボタン（実体）
TouchButton btnPlus(280, 60, 35, 25, 0x9C27B0, 0x6A1B9A, "+");
TouchButton btnMinus(5, 60, 35, 25, 0x9C27B0, 0x6A1B9A, "-");
TouchButton btnHome(280, 30, 35, 25, 0x607D8B, 0x455A64, "H");

// 表示状態管理（実体）
unsigned long lastDisplayUpdate = 0;
String connection_status = "Nintendo Switch初期化中...";
bool switch_connected = false;
int button_press_count = 0;

void drawButton(TouchButton &btn) {
    // タッチ入力またはWeb入力で押下状態を判定
    bool isPressed = btn.current || btn.web_input;
    uint32_t color = isPressed ? btn.pressed_color : btn.color;
    
    // ボタン背景
    M5.Display.fillRoundRect(btn.x, btn.y, btn.w, btn.h, 5, color);
    
    // ボタン枠（Web入力時は異なる色で表示）
    uint32_t border_color = btn.web_input ? CYAN : WHITE;
    M5.Display.drawRoundRect(btn.x, btn.y, btn.w, btn.h, 5, border_color);
    
    // ボタンラベル
    M5.Display.setTextColor(WHITE);
    M5.Display.setTextSize(2);
    int text_x = btn.x + (btn.w - M5.Display.textWidth(btn.label)) / 2;
    int text_y = btn.y + (btn.h - 16) / 2;
    M5.Display.setCursor(text_x, text_y);
    M5.Display.print(btn.label);
    
    // Web入力インジケーター
    if (btn.web_input) {
        M5.Display.fillCircle(btn.x + btn.w - 8, btn.y + 8, 3, CYAN);
    }
}

void updateDisplayTouchMode() {
    M5.Display.clear(BLACK);
    
    // ヘッダー
    M5.Display.setCursor(10, 10);
    M5.Display.setTextColor(WHITE);
    M5.Display.setTextSize(2);
    M5.Display.println("Nintendo Switch Controller");
    
    // 状態表示
    M5.Display.setCursor(10, 40);
    M5.Display.setTextSize(1);
    M5.Display.println("Status: " + connection_status);
    
    // 接続状態
    M5.Display.setCursor(10, 55);
    if (switch_connected) {
        M5.Display.setTextColor(GREEN);
        M5.Display.println("★ SWITCH CONNECTED ★");
    } else {
        M5.Display.setTextColor(ORANGE);
        M5.Display.println("USB READY - Connect to Switch");
    }
    
    // WiFi状態表示
    M5.Display.setCursor(200, 55);
    if (wifi_connected) {
        M5.Display.setTextColor(CYAN);
        M5.Display.println("WiFi: " + wifi_ip);
    } else {
        M5.Display.setTextColor(RED);
        M5.Display.println("WiFi: Disconnected");
    }
    
    // 統計情報
    M5.Display.setTextColor(WHITE);
    M5.Display.setCursor(10, 70);
    M5.Display.setTextSize(1);
    M5.Display.print("Button presses: ");
    M5.Display.println(button_press_count);
    
    // メインボタン描画
    drawButton(btnA);
    drawButton(btnB);
    drawButton(btnX);
    drawButton(btnY);
    
    // 左スティック描画
    drawButton(lstickUp);
    drawButton(lstickDown);
    drawButton(lstickLeft);
    drawButton(lstickRight);
    
    // 追加ボタン描画
    drawButton(btnPlus);
    drawButton(btnMinus);
    drawButton(btnHome);
    
    // ボタン状態表示
    M5.Display.setTextColor(WHITE);
    M5.Display.setTextSize(1);
    M5.Display.setCursor(10, 250);
    String status = "Active: ";
    if (btnA.current || btnA.web_input) status += "[A] ";
    if (btnB.current || btnB.web_input) status += "[B] ";
    if (btnX.current || btnX.web_input) status += "[X] ";
    if (btnY.current || btnY.web_input) status += "[Y] ";
    if (lstickUp.current || lstickUp.web_input) status += "[L↑] ";
    if (lstickDown.current || lstickDown.web_input) status += "[L↓] ";
    if (lstickLeft.current || lstickLeft.web_input) status += "[L←] ";
    if (lstickRight.current || lstickRight.web_input) status += "[L→] ";
    if (btnPlus.current || btnPlus.web_input) status += "[+] ";
    if (btnMinus.current || btnMinus.web_input) status += "[-] ";
    if (btnHome.current || btnHome.web_input) status += "[H] ";
    M5.Display.println(status);
    
    // 操作説明
    M5.Display.setCursor(10, 270);
    M5.Display.setTextSize(1);
    M5.Display.println("Touch: L-Stick(Left) + ABXY(Right) + System(Top)");
    M5.Display.setCursor(10, 285);
    M5.Display.println("Web: POST /controller (JSON) - Blue border = Web input");
    
    // タッチ座標表示はtouch_control.cppに移動
}

void updateDisplayAtomS3() {
    M5.Display.clear(BLACK);
    
    // 小型画面用のレイアウト（128x128）
    M5.Display.setTextSize(1);
    
    // WiFi状態表示（上部）- 接続成功時は暗い灰色でIPアドレス、失敗時は赤で「No Wifi」
    M5.Display.setCursor(2, 2);
    if (wifi_connected) {
        // 接続成功時：暗い灰色でIPアドレス表示
        M5.Display.setTextColor(M5.Display.color565(40, 40, 40));  // より暗い灰色
        M5.Display.print(wifi_ip);
    } else {
        // 接続失敗時：赤で「No Wifi」表示
        M5.Display.setTextColor(M5.Display.color565(255, 0, 0));  // 赤色
        M5.Display.print("No Wifi");
    }
    
    // Web入力状態表示（中央）- アクティブ入力のみ表示
    String activeInput = getActiveWebInput();
    if (activeInput != "") {
        int centerX = LCD_WIDTH / 2;
        int centerY = LCD_HEIGHT / 2;
        
        if (activeInput.startsWith("STICK_")) {
            // 小型スティック表示
            drawAtomS3StickIndicator(centerX, centerY, activeInput);
        } else {
            // 小型ボタン表示
            drawAtomS3ButtonIndicator(centerX, centerY, activeInput);
        }
    }
    // デフォルト時は何も表示しない（ルール仕様）
}

void drawAtomS3ButtonIndicator(int centerX, int centerY, String buttonName) {
    // 小型画面用ボタン表示（半径25）
    int radius = 25;
    M5.Display.fillCircle(centerX, centerY, radius, WHITE);
    M5.Display.drawCircle(centerX, centerY, radius, BLACK);
    
    // ボタン名を表示
    M5.Display.setTextColor(BLACK);
    M5.Display.setTextSize(2);
    int textWidth = M5.Display.textWidth(buttonName);
    int textX = centerX - textWidth / 2;
    int textY = centerY - 8;
    M5.Display.setCursor(textX, textY);
    M5.Display.print(buttonName);
}

void drawAtomS3StickIndicator(int centerX, int centerY, String stickType) {
    // 小型画面用スティック表示（半径30）- 細い白い円の枠線
    int radius = 30;
    M5.Display.drawCircle(centerX, centerY, radius, WHITE);
    
    // カーソル位置計算
    int stickX, stickY;
    if (stickType == "STICK_L") {
        stickX = map(webButtons.lstick_x, -100, 100, centerX - radius + 5, centerX + radius - 5);
        stickY = map(webButtons.lstick_y, -100, 100, centerY + radius - 5, centerY - radius + 5);
    } else { // STICK_R
        stickX = map(webButtons.rstick_x, -100, 100, centerX - radius + 5, centerX + radius - 5);
        stickY = map(webButtons.rstick_y, -100, 100, centerY + radius - 5, centerY - radius + 5);
    }
    
    // 「+」カーソルの描画（ルール仕様）
    int crossSize = 6;
    // 横線
    M5.Display.drawLine(stickX - crossSize, stickY, stickX + crossSize, stickY, WHITE);
    // 縦線  
    M5.Display.drawLine(stickX, stickY - crossSize, stickX, stickY + crossSize, WHITE);
    // 中央を少し太く
    M5.Display.drawLine(stickX - crossSize + 1, stickY, stickX + crossSize - 1, stickY, WHITE);
    M5.Display.drawLine(stickX, stickY - crossSize + 1, stickX, stickY + crossSize - 1, WHITE);
}

void updateDisplaySimpleMode() {
    M5.Display.clear(BLACK);
    
    // WiFi状態表示（上部）
    M5.Display.setCursor(10, 10);
    M5.Display.setTextColor(WHITE);
    M5.Display.setTextSize(1);
    if (wifi_connected) {
        M5.Display.setTextColor(CYAN);
        M5.Display.print("WiFi: Connected | IP: " + wifi_ip);
    } else {
        M5.Display.setTextColor(RED);
        M5.Display.print("WiFi: Disconnected");
    }
    
    // Web入力状態の確認と表示（中央）
    String activeInput = getActiveWebInput();
    if (activeInput != "") {
        // 中央に大きく表示
        int centerX = M5.Display.width() / 2;
        int centerY = M5.Display.height() / 2;
        
        if (activeInput.startsWith("STICK_")) {
            // スティック表示
            drawStickIndicator(centerX, centerY, activeInput);
        } else {
            // ボタン表示
            drawButtonIndicator(centerX, centerY, activeInput);
        }
    }
}

String getActiveWebInput() {
    // 最新入力追跡機能を使用（複数入力時は最新のものを1つ表示）
    if (webButtons.last_active_input != "") {
        // 現在もその入力がアクティブかチェック
        if (webButtons.last_active_input == "A" && webButtons.A) return "A";
        if (webButtons.last_active_input == "B" && webButtons.B) return "B";
        if (webButtons.last_active_input == "X" && webButtons.X) return "X";
        if (webButtons.last_active_input == "Y" && webButtons.Y) return "Y";
        if (webButtons.last_active_input == "L" && webButtons.L) return "L";
        if (webButtons.last_active_input == "R" && webButtons.R) return "R";
        if (webButtons.last_active_input == "ZL" && webButtons.ZL) return "ZL";
        if (webButtons.last_active_input == "ZR" && webButtons.ZR) return "ZR";
        if (webButtons.last_active_input == "+" && webButtons.plus) return "+";
        if (webButtons.last_active_input == "-" && webButtons.minus) return "-";
        if (webButtons.last_active_input == "HOME" && webButtons.home) return "HOME";
        if (webButtons.last_active_input == "STICK_L" && (abs(webButtons.lstick_x) > 10 || abs(webButtons.lstick_y) > 10)) return "STICK_L";
        if (webButtons.last_active_input == "STICK_R" && (abs(webButtons.rstick_x) > 10 || abs(webButtons.rstick_y) > 10)) return "STICK_R";
        
        // 最新入力がもうアクティブでない場合はクリア
        webButtons.last_active_input = "";
    }
    
    return ""; // デフォルト状態（何も表示しない）
}

void drawButtonIndicator(int centerX, int centerY, String buttonName) {
    // 白い円の描画（大きめ）
    int radius = 60;
    M5.Display.fillCircle(centerX, centerY, radius, WHITE);
    M5.Display.drawCircle(centerX, centerY, radius, BLACK);
    
    // 黒字でボタン名を表示
    M5.Display.setTextColor(BLACK);
    M5.Display.setTextSize(4);
    int textWidth = M5.Display.textWidth(buttonName);
    int textX = centerX - textWidth / 2;
    int textY = centerY - 16; // フォントサイズ4の場合の調整
    M5.Display.setCursor(textX, textY);
    M5.Display.print(buttonName);
}

void drawStickIndicator(int centerX, int centerY, String stickType) {
    // スティックの枠（細く白い円）
    int radius = 80;
    M5.Display.drawCircle(centerX, centerY, radius, WHITE);
    M5.Display.drawCircle(centerX, centerY, radius - 1, WHITE); // 少し太く
    
    // 十字カーソルの位置計算
    int stickX, stickY;
    if (stickType == "STICK_L") {
        stickX = map(webButtons.lstick_x, -100, 100, centerX - radius + 10, centerX + radius - 10);
        stickY = map(webButtons.lstick_y, -100, 100, centerY + radius - 10, centerY - radius + 10); // Y軸反転
    } else { // STICK_R
        stickX = map(webButtons.rstick_x, -100, 100, centerX - radius + 10, centerX + radius - 10);
        stickY = map(webButtons.rstick_y, -100, 100, centerY + radius - 10, centerY - radius + 10); // Y軸反転
    }
    
    // 十字カーソルの描画
    int crossSize = 8;
    M5.Display.drawLine(stickX - crossSize, stickY, stickX + crossSize, stickY, WHITE); // 横線
    M5.Display.drawLine(stickX, stickY - crossSize, stickX, stickY + crossSize, WHITE); // 縦線
    
    // 中央に小さい円
    M5.Display.fillCircle(stickX, stickY, 3, WHITE);
    
    // スティック名表示
    M5.Display.setTextColor(WHITE);
    M5.Display.setTextSize(2);
    String displayName = (stickType == "STICK_L") ? "L-Stick" : "R-Stick";
    int textWidth = M5.Display.textWidth(displayName);
    M5.Display.setCursor(centerX - textWidth / 2, centerY + radius + 15);
    M5.Display.print(displayName);
}

void updateDisplay() {
    if (!HAS_LCD) {
        // LED表示のみの場合
        updateLEDDisplay();
        return;
    }
    
    if (IS_ATOMS3) {
        // AtomS3の小型LCD表示
        updateDisplayAtomS3();
    } else if (ENABLE_TOUCH_CONTROL) {
        // CoreS3のタッチモード
        updateDisplayTouchMode();
    } else {
        // CoreS3のシンプルモード
        updateDisplaySimpleMode();
    }
}

void checkAndUpdateDisplay() {
    // ディスプレイ更新（設定値間隔）
    if (millis() - lastDisplayUpdate > DISPLAY_UPDATE_INTERVAL) {
        updateDisplay();
        lastDisplayUpdate = millis();
    }
}

void updateLEDDisplay() {
    // AtomS3には小型LCDがあるため、LED表示は使用しない
    // この関数は互換性のためのみ残存
}

void initDisplay() {
    if (!HAS_LCD) {
        // LCD無しの場合はスキップ
        return;
    }
    
#ifdef TARGET_ATOMS3
    // M5AtomS3専用のディスプレイ初期化
    M5.Display.init();
    M5.Display.setRotation(0);  // 90度反時計回りに回転
    M5.Display.setColorDepth(16);
    M5.Display.clear(BLACK);
    M5.Display.setBrightness(200); // 小型画面なので明るめに
    M5.Display.setTextSize(1);
    M5.Display.setTextColor(WHITE);
    
    // 初期化確認テスト表示
    M5.Display.setCursor(0, 0);
    M5.Display.println("AtomS3 Init...");
    delay(1000);
#else
    // M5CoreS3用の通常初期化
    M5.Display.setRotation(DISPLAY_ROTATION);
    M5.Display.clear(BLACK);
    M5.Display.setBrightness(DISPLAY_BRIGHTNESS);
#endif
} 