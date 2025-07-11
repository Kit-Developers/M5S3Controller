#ifndef TYPES_H
#define TYPES_H

#include "env.h"

// ボード別ライブラリinclude
#ifdef TARGET_ATOMS3
    #include <M5AtomS3.h>
#else
    #include <M5CoreS3.h>
#endif

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// WebServer
extern WebServer server;

// タッチボタン定義
struct TouchButton {
    int x, y, w, h;           // 位置とサイズ
    uint32_t color;           // ボタンカラー
    uint32_t pressed_color;   // 押下時カラー
    String label;             // ボタンラベル
    bool current = false;     // 現在の状態
    bool previous = false;    // 前回の状態
    bool web_input = false;   // Web入力フラグ
    
    TouchButton(int _x, int _y, int _w, int _h, uint32_t _color, uint32_t _pressed_color, String _label) {
        x = _x; y = _y; w = _w; h = _h;
        color = _color; pressed_color = _pressed_color;
        label = _label;
        current = false; previous = false; web_input = false;
    }
    
    bool changed() { return current != previous; }
    void update() { previous = current; }
};

// Web入力用のボタン状態構造体
struct WebButtonState {
    bool A = false;
    bool B = false; 
    bool X = false;
    bool Y = false;
    bool plus = false;
    bool minus = false;
    bool home = false;
    bool L = false;
    bool R = false;
    bool ZL = false;
    bool ZR = false;
    int lstick_x = 0;  // -100 to 100
    int lstick_y = 0;  // -100 to 100
    int rstick_x = 0;  // -100 to 100
    int rstick_y = 0;  // -100 to 100
    
    // 最新入力追跡用（タッチ操作無効時の表示用）
    String last_active_input = "";         // 最新のアクティブ入力名
    unsigned long last_update_time = 0;    // 最終更新時刻
};

// グローバル変数のextern宣言
extern WebButtonState webButtons;

#endif // TYPES_H 