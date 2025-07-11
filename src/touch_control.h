#ifndef TOUCH_CONTROL_H
#define TOUCH_CONTROL_H

#include "types.h"

// タッチ状態
extern lgfx::touch_point_t touch_point;
extern bool touch_detected;

/**
 * タッチ状態更新
 */
void updateTouch();

/**
 * ポイントがボタン内かチェック
 */
bool isPointInButton(int x, int y, TouchButton &btn);

/**
 * タッチ制御初期化
 */
void initTouchControl();

#endif // TOUCH_CONTROL_H 