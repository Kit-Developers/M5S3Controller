#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "types.h"

// Nintendo Switch ボタン
extern TouchButton btnA;
extern TouchButton btnB;
extern TouchButton btnX;
extern TouchButton btnY;

// 左スティック
extern TouchButton lstickUp;
extern TouchButton lstickDown;
extern TouchButton lstickLeft;
extern TouchButton lstickRight;

// 追加ボタン
extern TouchButton btnPlus;
extern TouchButton btnMinus;
extern TouchButton btnHome;

// 表示状態管理
extern unsigned long lastDisplayUpdate;
extern String connection_status;
extern bool switch_connected;
extern int button_press_count;

/**
 * ディスプレイ更新（タッチ有効モード）
 */
void updateDisplayTouchMode();

/**
 * ディスプレイ更新（タッチ無効・シンプルモード）
 */
void updateDisplaySimpleMode();

/**
 * ディスプレイ更新（AtomS3小型LCD用）
 */
void updateDisplayAtomS3();

/**
 * LED表示更新（AtomS3用）
 */
void updateLEDDisplay();

/**
 * ディスプレイ更新
 */
void updateDisplay();

/**
 * ディスプレイ更新チェック・実行
 */
void checkAndUpdateDisplay();

/**
 * ボタン描画
 */
void drawButton(TouchButton &btn);

/**
 * ディスプレイ初期化
 */
void initDisplay();

/**
 * アクティブなWeb入力を取得
 */
String getActiveWebInput();

/**
 * ボタンインジケーター描画
 */
void drawButtonIndicator(int centerX, int centerY, String buttonName);

/**
 * スティックインジケーター描画
 */
void drawStickIndicator(int centerX, int centerY, String stickType);

/**
 * AtomS3用ボタンインジケーター描画
 */
void drawAtomS3ButtonIndicator(int centerX, int centerY, String buttonName);

/**
 * AtomS3用スティックインジケーター描画
 */
void drawAtomS3StickIndicator(int centerX, int centerY, String stickType);

#endif // LCD_DISPLAY_H 