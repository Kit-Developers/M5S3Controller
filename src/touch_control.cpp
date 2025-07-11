#include "touch_control.h"
#include "lcd_display.h"
#include "env.h"

// タッチ状態（実体）
lgfx::touch_point_t touch_point;
bool touch_detected = false;

bool isPointInButton(int x, int y, TouchButton &btn) {
    return (x >= btn.x && x <= btn.x + btn.w && 
            y >= btn.y && y <= btn.y + btn.h);
}

void updateTouch() {
    if (!HAS_TOUCH || !ENABLE_TOUCH_CONTROL) {
        // タッチ制御無効時は全てfalseに設定
        touch_detected = false;
        touch_point.x = 0;
        touch_point.y = 0;
        
        // 全ボタンのタッチ状態をfalseに
        btnA.current = false;
        btnB.current = false;
        btnX.current = false;
        btnY.current = false;
        lstickUp.current = false;
        lstickDown.current = false;
        lstickLeft.current = false;
        lstickRight.current = false;
        btnPlus.current = false;
        btnMinus.current = false;
        btnHome.current = false;
        return;
    }
    
    // タッチ制御有効時の処理（CoreS3のみ）
    // タッチ状態取得
    touch_detected = M5.Display.getTouch(&touch_point);
    
    // 各ボタンのタッチ状態更新
    btnA.current = touch_detected && isPointInButton(touch_point.x, touch_point.y, btnA);
    btnB.current = touch_detected && isPointInButton(touch_point.x, touch_point.y, btnB);
    btnX.current = touch_detected && isPointInButton(touch_point.x, touch_point.y, btnX);
    btnY.current = touch_detected && isPointInButton(touch_point.x, touch_point.y, btnY);
    
    // 左スティックのタッチ状態更新
    lstickUp.current = touch_detected && isPointInButton(touch_point.x, touch_point.y, lstickUp);
    lstickDown.current = touch_detected && isPointInButton(touch_point.x, touch_point.y, lstickDown);
    lstickLeft.current = touch_detected && isPointInButton(touch_point.x, touch_point.y, lstickLeft);
    lstickRight.current = touch_detected && isPointInButton(touch_point.x, touch_point.y, lstickRight);
    
    // 追加ボタンのタッチ状態更新
    btnPlus.current = touch_detected && isPointInButton(touch_point.x, touch_point.y, btnPlus);
    btnMinus.current = touch_detected && isPointInButton(touch_point.x, touch_point.y, btnMinus);
    btnHome.current = touch_detected && isPointInButton(touch_point.x, touch_point.y, btnHome);
}

void initTouchControl() {
    // タッチ制御初期化（必要に応じて設定を追加）
    touch_detected = false;
    touch_point.x = 0;
    touch_point.y = 0;
} 