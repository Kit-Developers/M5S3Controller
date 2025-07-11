# Nintendo Switch Controller - サンプルクライアント

M5AtomS3コントローラのWebサーバーAPIを使用するクライアントサンプル集です。  
4つの異なる言語・環境でのクライアント実装例を提供しています。

> 📖 **プロジェクト全体の概要については [メインREADME](../README.md) をご覧ください**

## 📋 目次

- [事前準備](#事前準備)
- [サンプル一覧](#サンプル一覧)
- [Python Client](#python-client)
- [JavaScript Client](#javascript-client)
- [Arduino Client](#arduino-client)
- [Web Controller](#web-controller)
- [共通操作コマンド](#共通操作コマンド)
- [カスタマイズ例](#カスタマイズ例)

## ⚙️ 事前準備

### AtomS3側の準備
1. **WiFi設定**: `src/env-base.h` を `src/env.h` にコピーしてWiFi設定を編集
2. **ファームウェア書き込み**: 本プロジェクトのコードをAtomS3に書き込み
3. **WiFi接続**: AtomS3がWiFiに接続されていることを確認
4. **IPアドレス確認**: AtomS3のIPアドレスをメモ（例: `192.168.1.100`）
5. **Switch接続**: AtomS3とSwitchがUSB接続されていることを確認

### クライアント側の共通設定
各サンプルコード内の以下の部分を、実際のAtomS3のIPアドレスに変更してください：

```
CONTROLLER_IP = "192.168.1.100"  ← AtomS3の実際のIPアドレス
```

## 📁 サンプル一覧

| ファイル | 言語 | 説明 | 推奨用途 | 実行方法 |
|----------|------|------|----------|----------|
| `python_client.py` | Python | インタラクティブ制御 | 開発・テスト・自動化 | `python python_client.py` |
| `javascript_client.js` | JavaScript/Node.js | インタラクティブ制御 | Web開発・サーバーサイド | `node javascript_client.js` |
| `arduino_client.cpp` | C++/Arduino | インタラクティブ制御 | IoT・組み込み開発 | Arduino IDEで書き込み |
| `web_controller.html` | HTML/JavaScript | ブラウザUI制御 | エンドユーザー・GUI操作 | ブラウザで開く |

## 🐍 Python Client

### インストール
```bash
pip install requests
```

### 実行
```bash
python python_client.py
```

### 操作方法
```
コマンド一覧:
  A, B, X, Y    - メインボタン押下（大文字のみ）
  l, r, zl, zr  - ショルダーボタン押下（小文字）
  +, -, h       - システムボタン (plus, minus, home)
  w,a,s,d       - 左スティック移動（上,左,下,右）
  i,j,k,l       - 右スティック移動（上,左,下,右）
  reset         - すべてリセット
  quit          - 終了
```

### コード例
```python
import requests
import time

# 基本設定
CONTROLLER_IP = "192.168.1.100"
url = f"http://{CONTROLLER_IP}/controller"

# 基本的なボタン入力
def press_button(button_name):
    payload = {
        "buttons": {button_name: True, "A": False, "B": False, "X": False, "Y": False},
        "lstick": {"x": 0, "y": 0},
        "rstick": {"x": 0, "y": 0},
        "shoulder": {"L": False, "R": False, "ZL": False, "ZR": False},
        "system": {"plus": False, "minus": False, "home": False}
    }
    # 必要なボタンのみTrueに設定
    if button_name in ["A", "B", "X", "Y"]:
        payload["buttons"] = {k: k == button_name for k in ["A", "B", "X", "Y"]}
    
    response = requests.post(url, json=payload)
    return response.json()

# スティック操作
def move_stick(stick_type, x, y):
    payload = {
        "buttons": {"A": False, "B": False, "X": False, "Y": False},
        "lstick": {"x": x if stick_type == "left" else 0, "y": y if stick_type == "left" else 0},
        "rstick": {"x": x if stick_type == "right" else 0, "y": y if stick_type == "right" else 0},
        "shoulder": {"L": False, "R": False, "ZL": False, "ZR": False},
        "system": {"plus": False, "minus": False, "home": False}
    }
    response = requests.post(url, json=payload)
    return response.json()

# 使用例
press_button("A")  # Aボタンを押す
time.sleep(0.1)
move_stick("left", 100, 0)  # 左スティックを右に倒す
```

## 🌐 JavaScript Client (Node.js)

### 前提条件
- Node.js 18以上（fetch API対応）

### 実行
```bash
node javascript_client.js
```

### 操作方法
Python Clientと同じインタラクティブ操作

### コード例
```javascript
const CONTROLLER_IP = "192.168.1.100";
const url = `http://${CONTROLLER_IP}/controller`;

// 基本的なコントローラ入力
async function sendControllerInput(options = {}) {
    const payload = {
        buttons: options.buttons || { A: false, B: false, X: false, Y: false },
        lstick: options.lstick || { x: 0, y: 0 },
        rstick: options.rstick || { x: 0, y: 0 },
        shoulder: options.shoulder || { L: false, R: false, ZL: false, ZR: false },
        system: options.system || { plus: false, minus: false, home: false }
    };
    
    try {
        const response = await fetch(url, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(payload)
        });
        
        return await response.json();
    } catch (error) {
        console.error('送信エラー:', error);
        return { error: error.message };
    }
}

// 使用例
async function example() {
    // Aボタンを押す
    await sendControllerInput({ buttons: { A: true, B: false, X: false, Y: false } });
    
    // 1秒待機
    await new Promise(resolve => setTimeout(resolve, 1000));
    
    // 左スティックを右に倒す
    await sendControllerInput({ lstick: { x: 100, y: 0 } });
}
```

## 🔧 Arduino Client

### 必要なライブラリ
```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
```

### WiFi設定
```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* controllerIP = "192.168.1.100";
```

### 実行方法
1. Arduino IDEでコードを開く
2. ESP32ボードを選択
3. コンパイル＆書き込み
4. シリアルモニタ（115200bps）を開く
5. インタラクティブコマンドを入力

### 操作方法
Python/JavaScript Clientと同じコマンド体系

### コード例
```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* controllerIP = "192.168.1.100";

void sendControllerInput(bool btnA = false, bool btnB = false, bool btnX = false, bool btnY = false,
                        int lstickX = 0, int lstickY = 0, int rstickX = 0, int rstickY = 0,
                        bool shoulderL = false, bool shoulderR = false, bool shoulderZL = false, bool shoulderZR = false,
                        bool sysPlus = false, bool sysMinus = false, bool sysHome = false) {
    
    HTTPClient http;
    String url = "http://" + String(controllerIP) + "/controller";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    // JSON作成
    StaticJsonDocument<512> doc;
    doc["buttons"]["A"] = btnA;
    doc["buttons"]["B"] = btnB;
    doc["buttons"]["X"] = btnX;
    doc["buttons"]["Y"] = btnY;
    doc["lstick"]["x"] = lstickX;
    doc["lstick"]["y"] = lstickY;
    doc["rstick"]["x"] = rstickX;
    doc["rstick"]["y"] = rstickY;
    doc["shoulder"]["L"] = shoulderL;
    doc["shoulder"]["R"] = shoulderR;
    doc["shoulder"]["ZL"] = shoulderZL;
    doc["shoulder"]["ZR"] = shoulderZR;
    doc["system"]["plus"] = sysPlus;
    doc["system"]["minus"] = sysMinus;
    doc["system"]["home"] = sysHome;
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    int httpResponseCode = http.POST(jsonString);
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("レスポンス: " + response);
    } else {
        Serial.println("エラー: " + String(httpResponseCode));
    }
    http.end();
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("WiFi接続中...");
    }
    Serial.println("WiFi接続完了");
    Serial.println("コマンドを入力してください（A, B, X, Y, w, a, s, d等）:");
}

void loop() {
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        
        // コマンド処理（Python/JavaScriptと同様）
        if (cmd == "A") {
            sendControllerInput(true, false, false, false);
        } else if (cmd == "w") {
            sendControllerInput(false, false, false, false, 0, 100);
        }
        // ... 他のコマンド処理
        
        delay(1000);  // 1秒間隔
    }
}
```

## 🎮 Web Controller

### 使用方法
1. `web_controller.html` をWebブラウザで開く
2. AtomS3のIPアドレスを入力欄に設定
3. 「接続テスト」ボタンで接続確認
4. 画面上のボタンやスライダーで操作

### 特徴
- **4パネル構成**: System|Left Stick|Right Stick|Main Buttons
- **リアルタイム制御**: クリック即座にSwitch側に反映
- **精密スライダー**: スティック座標の微調整可能
- **ログ表示**: 送信履歴をリアルタイム表示
- **レスポンシブ**: PC・タブレット・スマホ対応

### カスタマイズ例
```html
<!-- 独自ボタンを追加 -->
<button onclick="customCombo()">必殺技コンボ</button>

<script>
async function customCombo() {
    // カスタムコンボ実行
    await sendControllerInput({ buttons: { X: true } });
    await sleep(100);
    await sendControllerInput({ buttons: { Y: true } });
    await sleep(100);
    await sendControllerInput({ buttons: { A: true } });
    log('必殺技コンボ実行');
}
</script>
```

## 🎯 共通操作コマンド

### インタラクティブモード共通コマンド
```
【メインボタン（大文字のみ）】
A        - Aボタン押下
B        - Bボタン押下  
X        - Xボタン押下
Y        - Yボタン押下

【ショルダーボタン（小文字）】
l        - Lボタン押下
r        - Rボタン押下
zl       - ZLボタン押下
zr       - ZRボタン押下

【システムボタン】
+        - Plusボタン押下
-        - Minusボタン押下
h        - HOMEボタン押下

【左スティック（小文字）】
w        - 上移動 (y: 100)
a        - 左移動 (x: -100)
s        - 下移動 (y: -100)  
d        - 右移動 (x: 100)

【右スティック（小文字）】
i        - 上移動 (y: 100)
j        - 左移動 (x: -100)
k        - 下移動 (y: -100)
l        - 右移動 (x: 100)

【制御コマンド】
reset    - 全ての入力をリセット
quit     - プログラム終了（Python/JavaScript）
```

### 大文字小文字の区別
- **大文字必須**: A, B, X, Y（メインボタン）
- **小文字必須**: w,a,s,d,i,j,k,l（スティック）, l,r,zl,zr（ショルダー）
- **その他**: +, -, h（システムボタン）

## 🛠️ カスタマイズ例

### 1. 自動操作スクリプト（Python）
```python
import time
import requests

def auto_farm_routine():
    """自動アイテム収集ルーチン"""
    commands = [
        ("A", 0.5),      # アイテム取得
        ("d", 2.0),      # 右移動
        ("A", 0.5),      # アイテム取得
        ("a", 2.0),      # 左移動（戻る）
    ]
    
    for cmd, duration in commands:
        execute_command(cmd)
        time.sleep(duration)

def execute_command(cmd):
    """コマンド実行（例：python_client.pyの関数を流用）"""
    # 実際の送信処理
    pass
```

### 2. ゲームパッド連携（JavaScript）
```javascript
// Gamepad APIを使用してゲームパッドからAtomS3へ
function pollGamepad() {
    const gamepad = navigator.getGamepads()[0];
    if (!gamepad) return;
    
    // アナログスティック
    const leftX = Math.round(gamepad.axes[0] * 100);
    const leftY = Math.round(-gamepad.axes[1] * 100);  // Y軸反転
    
    // ボタン状態
    const buttons = {
        A: gamepad.buttons[0].pressed,
        B: gamepad.buttons[1].pressed,
        X: gamepad.buttons[2].pressed,
        Y: gamepad.buttons[3].pressed
    };
    
    sendControllerInput({ buttons, lstick: { x: leftX, y: leftY } });
}

setInterval(pollGamepad, 50);  // 20fps
```

### 3. センサー連携（Arduino）
```cpp
// 加速度センサーでスティック制御
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void sensorControl() {
    Vector rawAccel = mpu.readRawAccel();
    
    // 加速度をスティック座標に変換
    int stickX = constrain(rawAccel.XAxis / 100, -100, 100);
    int stickY = constrain(rawAccel.YAxis / 100, -100, 100);
    
    sendControllerInput(false, false, false, false, stickX, stickY);
}
```

### 4. 音声認識連携（Python）
```python
import speech_recognition as sr
import threading

def voice_control_thread():
    """音声認識による操作"""
    r = sr.Recognizer()
    mic = sr.Microphone()
    
    with mic as source:
        r.adjust_for_ambient_noise(source)
    
    while True:
        try:
            with mic as source:
                audio = r.listen(source, timeout=1)
            
            command = r.recognize_google(audio, language='ja-JP')
            
            if "ジャンプ" in command:
                execute_command("A")
            elif "移動" in command:
                execute_command("d")
            elif "攻撃" in command:
                execute_command("X")
                
        except sr.UnknownValueError:
            pass  # 音声認識失敗
        except sr.RequestError as e:
            print(f"音声認識エラー: {e}")

# バックグラウンドで実行
voice_thread = threading.Thread(target=voice_control_thread, daemon=True)
voice_thread.start()
```

---

**サンプルコードを参考に、あなたの用途に合わせたクライアントを作成してみてください！ 🎮** 