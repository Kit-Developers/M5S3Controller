#ifndef ENV_H
#define ENV_H

// =================================
// ボード手動選択（どちらか一つをコメントアウト）
// =================================

// 使用するボードを選択してください（platformio.iniでの設定も忘れずに）
// #define TARGET_CORES3
#define TARGET_ATOMS3



// =================================
// 環境設定パラメータ
// =================================

// WiFi設定 - 実際の値に変更してください
#define WIFI_SSID "YOUR_WIFI_SSID"        // あなたのWiFi名に変更
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD" // あなたのWiFiパスワードに変更

// Webサーバー設定
#define WEB_SERVER_PORT 80

// ディスプレイ設定
#define DISPLAY_ROTATION 1
#define DISPLAY_BRIGHTNESS 128

// 制御設定
#define DISPLAY_UPDATE_INTERVAL 30  // ディスプレイ更新間隔（ms）
#define MAIN_LOOP_DELAY 5           // メインループ遅延（ms）

// WiFi接続設定
#define WIFI_CONNECT_TIMEOUT 30     // WiFi接続タイムアウト（試行回数）
#define WIFI_RECONNECT_TIMEOUT 10   // WiFi再接続タイムアウト（試行回数）

// 左スティック設定
#define LSTICK_THRESHOLD 50         // Web入力時の左スティック閾値




// =================================
// ボード設定（自動設定 - 編集不要）
// =================================

#ifdef TARGET_ATOMS3
    #define IS_ATOMS3 true
    #define HAS_LCD true       // AtomS3には128x128の小型LCD有り
    #define HAS_TOUCH false    // AtomS3にはタッチ無し
    #define LCD_WIDTH 128
    #define LCD_HEIGHT 128
#else
    #define IS_ATOMS3 false
    #define HAS_LCD true       // CoreS3には320x240のLCD有り
    #define HAS_TOUCH true     // CoreS3にはタッチ有り
    #define LCD_WIDTH 320
    #define LCD_HEIGHT 240
#endif


// LCD表示・タッチ制御設定
#if HAS_TOUCH
    #define ENABLE_TOUCH_CONTROL false   // タッチ操作有効/無効（true=有効, false=無効）
#else
    #define ENABLE_TOUCH_CONTROL false   // AtomS3では強制的に無効
#endif

// ボード別設定
#if IS_ATOMS3
    #define ENABLE_LED_INDICATOR false   // AtomS3では小型LCDを使用
#else
    #define ENABLE_LED_INDICATOR false   // CoreS3では大型LCDを使用
#endif

#endif // ENV_H 