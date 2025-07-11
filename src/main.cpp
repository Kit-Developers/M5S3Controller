#include "types.h"
#include "env.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "controller_input.h"
#include "touch_control.h"
#include "lcd_display.h"

// Nintendo Switch Controller - M5CoreS3タッチスクリーン実装 + Webサーバー機能
// SwitchControllerESP32ライブラリ使用（Nintendo Switch専用）

// WebServer（実体）
WebServer server(WEB_SERVER_PORT);

// Web入力状態（実体）
WebButtonState webButtons;

void setup() {
    // M5デバイス初期化（ボード別設定）
#ifdef TARGET_ATOMS3
    // M5AtomS3専用の初期化設定
    auto cfg = M5.config();
    cfg.serial_baudrate = 115200;
    cfg.clear_display = true;
    cfg.output_power = true;
    cfg.internal_imu = true;
    cfg.internal_rtc = false;
    cfg.internal_spk = false;
    cfg.internal_mic = false;
    cfg.external_imu = false;
    cfg.external_rtc = false;
    cfg.external_spk = false;
    M5.begin(cfg);
#else
    // M5CoreS3用の通常初期化
    M5.begin();
#endif
    
    delay(1000);
    
    // ディスプレイ初期化
    initDisplay();
    
    connection_status = "Nintendo Switch初期化中...";
    updateDisplay();
    
    // タッチ制御初期化
    initTouchControl();
    
    // WiFi初期化
    initWiFi();
    
    // Nintendo Switchコントローラー初期化
    initController();
    
    // Webサーバー初期化
    initWebServer();
    
    connection_status = "Nintendo Switch接続準備完了!";
    switch_connected = true;
    
    // 初期画面表示
    updateDisplay();
}

void loop() {
    // WiFi接続チェック・再接続
    reconnectWiFi();
    
    // Webサーバー処理
    handleWebServer();
    
    // Web入力更新
    updateWebInput();
    
    // タッチ状態更新
    updateTouch();
    
    // Nintendo Switchコントローラー更新
    updateSwitchController();
    
    // ディスプレイ更新チェック・実行
    checkAndUpdateDisplay();
    
    delay(MAIN_LOOP_DELAY);  // Nintendo Switch用の最適化された遅延
}