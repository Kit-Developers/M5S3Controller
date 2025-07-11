# Nintendo Switch Controller with M5AtomS3

M5AtomS3を使ったNintendo Switchコントローラのプロジェクです。  
AtomS3上でWebサーバーが動作し、HTTP APIを通じてコントローラ入力を受け付け、USBでSwitchに送信します。

## 📋 目次

- [プロジェクト概要](#プロジェクト概要)
- [システム構成](#システム構成)
- [対応ボタン・操作](#対応ボタン操作)
- [前提条件](#前提条件)
- [AtomS3セットアップ](#atoms3セットアップ)
- [API仕様](#api仕様)
- [サンプルコード](#サンプルコード)
- [トラブルシューティング](#トラブルシューティング)
- [応用例](#応用例)
- [ライセンス](#ライセンス)

## 🎮 プロジェクト概要

このプロジェクトは、M5AtomS3をNintendo Switchの代替コントローラとして使用するシステムです。  
AtomS3上でWebサーバーが動作し、HTTP APIを通じてコントローラ入力を受け付けます。

### 主な特徴
- **Webサーバー**: AtomS3内蔵のHTTP APIサーバー
- **USB接続**: SwitchとAtomS3をUSBケーブルで接続
- **多言語対応**: Python, JavaScript, Arduino, HTMLクライアント
- **リアルタイム制御**: 低遅延でのコントローラ入力
- **タッチ操作**: AtomS3本体のタッチボタンでも操作可能

## 🔗 システム構成

```
[クライアント] --HTTP POST--> [AtomS3 WebServer] --USB--> [Nintendo Switch]
```

### データフロー
1. **クライアント**: コントローラ入力をJSON形式でHTTP POST
2. **AtomS3**: WebサーバーでHTTPリクエストを受信
3. **AtomS3**: JSONをSwitchコントローラ信号に変換
4. **Nintendo Switch**: USB経由でコントローラ入力を受信

## 🎮 対応ボタン・操作

### メインボタン
- **A, B, X, Y**: 基本操作ボタン

### ショルダーボタン
- **L, R**: 左右ショルダーボタン
- **ZL, ZR**: 左右トリガーボタン

### システムボタン
- **+（Plus）**: メニュー・一時停止
- **-（Minus）**: マイナス・戻る
- **HOME**: ホームボタン

### スティック
- **左スティック（lstick）**: キャラクター移動等
- **右スティック（rstick）**: カメラ操作等
- **座標範囲**: X,Y軸ともに -100〜100

## 🔧 前提条件

### ハードウェア
- **M5AtomS3** (M5Stack製マイコン)
- **Nintendo Switch本体**
- **USBケーブル** (Type-C to Type-C)
- **WiFiネットワーク環境**

### ソフトウェア
- **AtomS3側**: 本プロジェクトのファームウェアが書き込み済み
- **開発環境**: 以下のいずれか
  - Python 3.7以上 + requests ライブラリ
  - Node.js 14以上
  - Arduino IDE + ESP32ボード
  - Webブラウザ（Chrome, Firefox, Safari等）

## ⚙️ AtomS3セットアップ

### 1. 開発環境準備
```bash
# PlatformIOを使用する場合
pip install platformio
cd BleController
pio run --target upload

# Arduino IDEを使用する場合
# src/フォルダ内のコードをArduino IDEで開いて書き込み
```

### 2. WiFi設定
WiFi設定ファイルを作成します：

```bash
# 設定テンプレートをコピー
cp src/env-base.h src/env.h

# env.hファイルを編集
```

`src/env.h` ファイル内のWiFi設定を実際の値に変更してください：

```cpp
// WiFi設定 - 実際の値に変更してください
#define WIFI_SSID "YOUR_WIFI_SSID"        // ← あなたのWiFi名に変更
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD" // ← あなたのWiFiパスワードに変更
```

> ⚠️ **重要**: `env.h`ファイルは`.gitignore`で除外されています。  
> 実際のWiFi設定はGitHubにプッシュされませんので、安全です。

### 3. ファームウェア書き込み
```bash
# PlatformIOの場合
pio run --target upload

# シリアルモニタで動作確認
pio device monitor
```

### 4. IPアドレス確認
AtomS3起動後、シリアルモニタまたはLCD画面でIPアドレスを確認してください。  
例: `192.168.1.100`

### 5. Nintendo Switchとの接続
AtomS3が自動的にSwitchとUSB接続を試みます。  
Switch側で「設定」→「コントローラーとセンサー」→「Proコントローラーの持ちかた/順番を変える」で接続してください。

## 🔌 API仕様

### エンドポイント
```
POST http://[AtomS3のIP]/controller
Content-Type: application/json
```

### リクエスト形式
```json
{
  "buttons": {
    "A": false,
    "B": false,
    "X": false,
    "Y": false
  },
  "lstick": {
    "x": 0,
    "y": 0
  },
  "rstick": {
    "x": 0,
    "y": 0
  },
  "shoulder": {
    "L": false,
    "R": false,
    "ZL": false,
    "ZR": false
  },
  "system": {
    "plus": false,
    "minus": false,
    "home": false
  }
}
```

### パラメータ詳細
- **buttons**: メインボタンの状態（true=押下, false=離す）
- **lstick/rstick**: スティック座標（-100〜100の範囲）
- **shoulder**: ショルダーボタンの状態
- **system**: システムボタンの状態

### レスポンス
```json
{"status": "OK"}
```

### 使用例
```bash
# curlでのテスト例
curl -X POST http://192.168.1.100/controller \
  -H "Content-Type: application/json" \
  -d '{
    "buttons": {"A": true, "B": false, "X": false, "Y": false},
    "lstick": {"x": 0, "y": 0},
    "rstick": {"x": 0, "y": 0},
    "shoulder": {"L": false, "R": false, "ZL": false, "ZR": false},
    "system": {"plus": false, "minus": false, "home": false}
  }'
```

## 📁 サンプルコード

詳細なサンプルコードと使用方法については、**[examples/README.md](examples/README.md)** をご覧ください。

### 提供サンプル
| ファイル | 言語 | 説明 |
|----------|------|------|
| `python_client.py` | Python | インタラクティブ制御 |
| `javascript_client.js` | JavaScript/Node.js | インタラクティブ制御 |
| `arduino_client.cpp` | C++/Arduino | インタラクティブ制御 |
| `web_controller.html` | HTML/JavaScript | ブラウザUI制御 |

## 🔧 トラブルシューティング

### 接続エラー
```
✗ 接続エラー: Connection refused
```

**原因と対処法:**
1. **IPアドレス確認**: AtomS3の実際のIPアドレスを確認
2. **WiFi接続**: AtomS3とクライアントが同じネットワークにいるか確認
3. **ファイアウォール**: ポート80がブロックされていないか確認
4. **AtomS3再起動**: 電源を入れ直してみる

### Switch接続問題
```
USB接続に失敗
```

**対処法:**
1. USBケーブルの接続を確認
2. Switch側のコントローラ設定を確認
3. 他のコントローラを切断
4. AtomS3を再起動してペアリングし直し

### 応答が遅い
```
操作が反映されない
```

**原因:**
- WiFiの電波強度が弱い
- ネットワーク負荷が高い

**対処法:**
- AtomS3をルーターに近づける
- 2.4GHz WiFiを使用する

### 操作が効かない
```
ボタンが反応しない
```

**確認点:**
1. JSON形式が正しいか確認
2. ボタン名の大文字小文字（A, B, X, Y は大文字）
3. スティック値の範囲（-100〜100）

## 💡 応用例

### 1. ゲーム自動化
```python
import time
import requests

def auto_play_sequence():
    # 自動でゲーム操作を実行
    commands = [
        {"buttons": {"+": True}},  # メニューを開く
        {"lstick": {"x": 100, "y": 0}},  # 右に移動
        {"buttons": {"B": True}},  # ジャンプ
    ]
    
    for cmd in commands:
        send_controller_input(**cmd)
        time.sleep(1)
```

### 2. 音声制御
```python
import speech_recognition as sr

def voice_control():
    r = sr.Recognizer()
    with sr.Microphone() as source:
        audio = r.listen(source)
        command = r.recognize_google(audio, language='ja-JP')
        
        if "ジャンプ" in command:
            send_controller_input(buttons={"A": True})
        elif "攻撃" in command:
            send_controller_input(buttons={"X": True})
```

### 3. IoTセンサー連携
```cpp
// 加速度センサーでスティック制御
void gyro_control() {
    float gyroX = getGyroX();
    float gyroY = getGyroY();
    
    int stickX = constrain(gyroX * 100, -100, 100);
    int stickY = constrain(gyroY * 100, -100, 100);
    
    sendControllerInput(false, false, false, false, 
                       stickX, stickY, 0, 0);
}
```

### 4. 外部API連携
```javascript
// Discord Botからの制御
discord.on('message', async (msg) => {
    if (msg.content === '!jump') {
        await sendControllerInput({ buttons: { A: true } });
        setTimeout(() => 
            sendControllerInput({ buttons: { A: false } }), 100);
    }
});
```

### 5. AI制御
```python
import cv2
import numpy as np

def ai_play():
    # ゲーム画面を解析
    screenshot = capture_switch_screen()
    action = ai_model.predict(screenshot)
    
    # AI判定に基づいて操作
    if action == "move_right":
        send_controller_input(lstick={"x": 100, "y": 0})
    elif action == "attack":
        send_controller_input(buttons={"X": True})
```

### 6. 定期処理・マクロ
```python
# 定期的にアイテム収集
def auto_farm():
    while True:
        # アイテム収集の繰り返し処理
        send_controller_input(buttons={"A": True})  # アイテム取得
        time.sleep(0.5)
        send_controller_input(lstick={"x": 100, "y": 0})  # 移動
        time.sleep(2)
        send_controller_input(lstick={"x": -100, "y": 0})  # 戻る
        time.sleep(2)
```

### 7. ストリーミング連携
```javascript
// Twitch チャット連動
twitchClient.on('message', (channel, tags, message, self) => {
    if (message.startsWith('!move')) {
        const direction = message.split(' ')[1];
        switch(direction) {
            case 'up': sendControllerInput({lstick: {x: 0, y: 100}}); break;
            case 'down': sendControllerInput({lstick: {x: 0, y: -100}}); break;
            case 'left': sendControllerInput({lstick: {x: -100, y: 0}}); break;
            case 'right': sendControllerInput({lstick: {x: 100, y: 0}}); break;
        }
    }
});
```

## 📂 プロジェクト構成

```
BleController/
├── src/                    # AtomS3ファームウェア
│   ├── main.cpp           # メインプログラム
│   ├── web_server.cpp     # Webサーバー機能
│   ├── controller_input.cpp # コントローラ入力処理
│   └── ...
├── examples/              # クライアントサンプル
│   ├── python_client.py   # Python クライアント
│   ├── javascript_client.js # JavaScript クライアント
│   ├── arduino_client.cpp # Arduino クライアント
│   ├── web_controller.html # Web UI
│   └── README.md          # サンプル詳細説明
├── platformio.ini         # PlatformIO設定
└── README.md             # 本ファイル
```

## 📝 ライセンス

このプロジェクトはMITライセンスの下で公開されています。  
自由に改変・商用利用していただけます。

## 🤝 コントリビューション

バグ報告や機能改善の提案は、GitHubのIssueまでお願いします。  
プルリクエストも歓迎いたします。

### 開発に参加する場合
1. このリポジトリをフォーク
2. 機能ブランチを作成 (`git checkout -b feature/amazing-feature`)
3. 変更をコミット (`git commit -m 'Add amazing feature'`)
4. ブランチにプッシュ (`git push origin feature/amazing-feature`)
5. プルリクエストを作成

---

**Happy Coding! 🎮✨** 