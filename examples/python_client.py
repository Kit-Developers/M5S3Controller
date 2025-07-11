#!/usr/bin/env python3
"""
Nintendo Switch Controller Web API - Python Client Sample
M5AtomS3 SwitchController WebサーバーにPOSTリクエストを送信するサンプル
"""

import requests
import json
import time
import sys

# M5AtomS3のIPアドレスを設定してください
CONTROLLER_IP = "192.168.1.100"  # ← M5AtomS3のIPアドレスに変更
CONTROLLER_URL = f"http://{CONTROLLER_IP}/controller"

def send_controller_input(buttons=None, lstick=None, rstick=None, shoulder=None, system=None):
    """
    コントローラー入力をM5AtomS3に送信
    
    Args:
        buttons (dict): ボタン状態 {"A": bool, "B": bool, "X": bool, "Y": bool}
        lstick (dict): 左スティック {"x": int(-100~100), "y": int(-100~100)}
        rstick (dict): 右スティック {"x": int(-100~100), "y": int(-100~100)}
        shoulder (dict): ショルダーボタン {"L": bool, "R": bool, "ZL": bool, "ZR": bool}
        system (dict): システムボタン {"plus": bool, "minus": bool, "home": bool}
    
    Returns:
        bool: 送信成功時True
    """
    
    # デフォルト値設定
    payload = {
        "buttons": buttons if buttons else {"A": False, "B": False, "X": False, "Y": False},
        "lstick": lstick if lstick else {"x": 0, "y": 0},
        "rstick": rstick if rstick else {"x": 0, "y": 0},
        "shoulder": shoulder if shoulder else {"L": False, "R": False, "ZL": False, "ZR": False},
        "system": system if system else {"plus": False, "minus": False, "home": False}
    }
    
    try:
        response = requests.post(
            CONTROLLER_URL,
            json=payload,
            headers={"Content-Type": "application/json"},
            timeout=5
        )
        
        if response.status_code == 200:
            print(f"✓ 送信成功: {payload}")
            return True
        else:
            print(f"✗ 送信失敗: {response.status_code} - {response.text}")
            return False
            
    except requests.exceptions.RequestException as e:
        print(f"✗ 接続エラー: {e}")
        return False

def interactive_mode():
    """インタラクティブモード"""
    print("=== インタラクティブモード ===")
    print("コマンド:")
    print("  A, B, X, Y    - メインボタン押下")
    print("  l, r, zl, zr  - ショルダーボタン押下")
    print("  +, -, h       - システムボタン (plus, minus, home)")
    print("  w,a,s,d       - 左スティック移動 (上,左,下,右)")
    print("  i,j,k,l       - 右スティック移動 (上,左,下,右)")
    print("  reset         - すべてのスティックをリセット(中央)")
    print("  quit          - 終了")
    print()
    
    while True:
        try:
            cmd = input("コマンド入力 > ").strip()
            
            if cmd.lower() == "quit":
                break
            # メインボタン（大文字のみ）
            elif cmd in ["A", "B", "X", "Y"]:
                send_controller_input(buttons={cmd: True})
                time.sleep(1)
                send_controller_input(buttons={cmd: False})
            # ショルダーボタン
            elif cmd == "l":
                send_controller_input(shoulder={"L": True})
                time.sleep(1)
                send_controller_input(shoulder={"L": False})
            elif cmd == "r":
                send_controller_input(shoulder={"R": True})
                time.sleep(1)
                send_controller_input(shoulder={"R": False})
            elif cmd == "zl":
                send_controller_input(shoulder={"ZL": True})
                time.sleep(1)
                send_controller_input(shoulder={"ZL": False})
            elif cmd == "zr":
                send_controller_input(shoulder={"ZR": True})
                time.sleep(1)
                send_controller_input(shoulder={"ZR": False})
            # システムボタン
            elif cmd == "+":
                send_controller_input(system={"plus": True})
                time.sleep(1)
                send_controller_input(system={"plus": False})
            elif cmd == "-":
                send_controller_input(system={"minus": True})
                time.sleep(1)
                send_controller_input(system={"minus": False})
            elif cmd == "h":
                send_controller_input(system={"home": True})
                time.sleep(1)
                send_controller_input(system={"home": False})
            # 左スティック
            elif cmd == "w":
                send_controller_input(lstick={"x": 0, "y": 100})  # 上方向（修正: -100 → 100）
            elif cmd == "a":
                send_controller_input(lstick={"x": -100, "y": 0})
            elif cmd == "s":
                send_controller_input(lstick={"x": 0, "y": -100})  # 下方向（修正: 100 → -100）
            elif cmd == "d":
                send_controller_input(lstick={"x": 100, "y": 0})
            # 右スティック
            elif cmd == "i":
                send_controller_input(rstick={"x": 0, "y": 100})  # 上方向（修正: -100 → 100）
            elif cmd == "j":
                send_controller_input(rstick={"x": -100, "y": 0})
            elif cmd == "k":
                send_controller_input(rstick={"x": 0, "y": -100})  # 下方向（修正: 100 → -100）
            elif cmd == "l":
                send_controller_input(rstick={"x": 100, "y": 0})
            # リセット
            elif cmd == "reset":
                send_controller_input()
            else:
                print("無効なコマンドです")
                
        except KeyboardInterrupt:
            break
    
    # 終了時にすべてリセット
    send_controller_input()
    print("プログラム終了")

def main():
    """メイン関数"""
    print("Nintendo Switch Controller - Python Client")
    print(f"接続先: {CONTROLLER_URL}")
    print()
    
    # 接続テスト
    print("接続テスト中...")
    if not send_controller_input():
        print("M5AtomS3への接続に失敗しました。")
        print("1. M5AtomS3が起動しているか確認してください")
        print("2. WiFi接続が正常か確認してください")
        print(f"3. IPアドレス {CONTROLLER_IP} が正しいか確認してください")
        sys.exit(1)
    
    print("接続成功!")
    print()
    
    interactive_mode()

if __name__ == "__main__":
    main() 