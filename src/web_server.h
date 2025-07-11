#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "types.h"
#include "wifi_manager.h"

/**
 * Webサーバー初期化
 */
void initWebServer();

/**
 * Webサーバー処理（リクエスト処理）
 */
void handleWebServer();

/**
 * ルートページ処理
 */
void handleRoot();

/**
 * コントローラーPOST処理
 */
void handleControllerPOST();

/**
 * Web入力の更新
 */
void updateWebInput();

#endif // WEB_SERVER_H 