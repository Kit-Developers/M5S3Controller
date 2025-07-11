#include "controller_input.h"
#include "lcd_display.h"

void updateSwitchController() {
    // Nintendo Switch ボタンの処理（SwitchControllerESP32ライブラリ使用）
    // タッチ入力またはWeb入力で判定
    
    // A ボタン
    bool btnA_active = btnA.current || btnA.web_input;
    if (btnA_active != btnA.previous) {
        if (btnA_active) {
            pushButton2(Button::A, 40, 0, 1);  // 40ms押下
            button_press_count++;
        }
        btnA.previous = btnA_active;
    }
    
    // B ボタン
    bool btnB_active = btnB.current || btnB.web_input;
    if (btnB_active != btnB.previous) {
        if (btnB_active) {
            pushButton2(Button::B, 40, 0, 1);  // 40ms押下
            button_press_count++;
        }
        btnB.previous = btnB_active;
    }
    
    // X ボタン
    bool btnX_active = btnX.current || btnX.web_input;
    if (btnX_active != btnX.previous) {
        if (btnX_active) {
            pushButton2(Button::X, 40, 0, 1);  // 40ms押下
            button_press_count++;
        }
        btnX.previous = btnX_active;
    }
    
    // Y ボタン
    bool btnY_active = btnY.current || btnY.web_input;
    if (btnY_active != btnY.previous) {
        if (btnY_active) {
            pushButton2(Button::Y, 40, 0, 1);  // 40ms押下
            button_press_count++;
        }
        btnY.previous = btnY_active;
    }
    
    // 左スティック操作
    // Web入力がある場合は精密制御、ない場合はタッチ入力
    if (webButtons.lstick_x != 0 || webButtons.lstick_y != 0) {
        // Web入力による精密制御
        tiltJoystick(webButtons.lstick_x, -webButtons.lstick_y, 0, 0, 40, 0);
    } else {
        // タッチ入力による方向制御
        bool lstickUp_active = lstickUp.current;
        bool lstickDown_active = lstickDown.current;
        bool lstickLeft_active = lstickLeft.current;
        bool lstickRight_active = lstickRight.current;
        
        int stick_x = 0, stick_y = 0;
        if (lstickLeft_active) stick_x = -100;
        if (lstickRight_active) stick_x = 100;
        if (lstickUp_active) stick_y = -100;
        if (lstickDown_active) stick_y = 100;
        
        tiltJoystick(stick_x, stick_y, 0, 0, 40, 0);
    }
    
    // 追加ボタン
    bool btnPlus_active = btnPlus.current || btnPlus.web_input;
    if (btnPlus_active != btnPlus.previous) {
        if (btnPlus_active) {
            pushButton2(Button::PLUS, 40, 0, 1);  // 40ms押下
            button_press_count++;
        }
        btnPlus.previous = btnPlus_active;
    }
    
    bool btnMinus_active = btnMinus.current || btnMinus.web_input;
    if (btnMinus_active != btnMinus.previous) {
        if (btnMinus_active) {
            pushButton2(Button::MINUS, 40, 0, 1);  // 40ms押下
            button_press_count++;
        }
        btnMinus.previous = btnMinus_active;
    }
    
    bool btnHome_active = btnHome.current || btnHome.web_input;
    if (btnHome_active != btnHome.previous) {
        if (btnHome_active) {
            pushButton2(Button::HOME, 40, 0, 1);  // 40ms押下
            button_press_count++;
        }
        btnHome.previous = btnHome_active;
    }
}

void initController() {
    // Nintendo Switchコントローラー初期化
    switchcontrolleresp32_init();
    
    // USB接続開始
    USB.begin();
    
    // コントローラーリセット
    switchcontrolleresp32_reset();
} 