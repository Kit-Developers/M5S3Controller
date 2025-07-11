/**
 * Nintendo Switch Controller Web API - JavaScript Client Sample
 * M5AtomS3 SwitchController WebサーバーにPOSTリクエストを送信するサンプル
 * Node.js環境で動作
 */

// M5AtomS3のIPアドレスを設定してください
const CONTROLLER_IP = "192.168.1.100";  // ← M5AtomS3のIPアドレスに変更
const CONTROLLER_URL = `http://${CONTROLLER_IP}/controller`;

/**
 * コントローラー入力をM5AtomS3に送信
 * @param {Object} buttons - ボタン状態 {A: bool, B: bool, X: bool, Y: bool}
 * @param {Object} lstick - 左スティック {x: number(-100~100), y: number(-100~100)}
 * @param {Object} rstick - 右スティック {x: number(-100~100), y: number(-100~100)}
 * @param {Object} shoulder - ショルダーボタン {L: bool, R: bool, ZL: bool, ZR: bool}
 * @param {Object} system - システムボタン {plus: bool, minus: bool, home: bool}
 * @returns {Promise<boolean>} 送信成功時true
 */
async function sendControllerInput(buttons = {}, lstick = {}, rstick = {}, shoulder = {}, system = {}) {
    // デフォルト値設定
    const payload = {
        buttons: { A: false, B: false, X: false, Y: false, ...buttons },
        lstick: { x: 0, y: 0, ...lstick },
        rstick: { x: 0, y: 0, ...rstick },
        shoulder: { L: false, R: false, ZL: false, ZR: false, ...shoulder },
        system: { plus: false, minus: false, home: false, ...system }
    };

    try {
        const response = await fetch(CONTROLLER_URL, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(payload)
        });

        if (response.ok) {
            console.log('✓ 送信成功:', payload);
            return true;
        } else {
            console.error('✗ 送信失敗:', response.status, await response.text());
            return false;
        }
    } catch (error) {
        console.error('✗ 接続エラー:', error.message);
        return false;
    }
}

/**
 * 指定時間待機
 * @param {number} ms - 待機時間（ミリ秒）
 */
function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

/**
 * インタラクティブモード
 */
async function interactiveMode() {
    const readline = require('readline');
    
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    });

    console.log('=== インタラクティブモード ===');
    console.log('コマンド:');
    console.log('  A, B, X, Y    - メインボタン押下');
    console.log('  l, r, zl, zr  - ショルダーボタン押下');
    console.log('  +, -, h       - システムボタン (plus, minus, home)');
    console.log('  w,a,s,d       - 左スティック移動 (上,左,下,右)');
    console.log('  i,j,k,l       - 右スティック移動 (上,左,下,右)');
    console.log('  reset         - すべてのスティックをリセット(中央)');
    console.log('  quit          - 終了');
    console.log();

    const askQuestion = () => {
        rl.question('コマンド入力 > ', async (cmd) => {
            cmd = cmd.trim();
            
            if (cmd.toLowerCase() === 'quit') {
                // 終了時にすべてリセット
                await sendControllerInput();
                console.log('プログラム終了');
                rl.close();
                return;
            }
            
            // メインボタン（大文字のみ）
            if (['A', 'B', 'X', 'Y'].includes(cmd)) {
                await sendControllerInput({ [cmd]: true });
                await sleep(1000);
                await sendControllerInput({ [cmd]: false });
            }
            // ショルダーボタン
            else if (cmd.toLowerCase() === 'l') {
                await sendControllerInput({}, {}, {}, { L: true });
                await sleep(1000);
                await sendControllerInput({}, {}, {}, { L: false });
            }
            else if (cmd.toLowerCase() === 'r') {
                await sendControllerInput({}, {}, {}, { R: true });
                await sleep(1000);
                await sendControllerInput({}, {}, {}, { R: false });
            }
            else if (cmd.toLowerCase() === 'zl') {
                await sendControllerInput({}, {}, {}, { ZL: true });
                await sleep(1000);
                await sendControllerInput({}, {}, {}, { ZL: false });
            }
            else if (cmd.toLowerCase() === 'zr') {
                await sendControllerInput({}, {}, {}, { ZR: true });
                await sleep(1000);
                await sendControllerInput({}, {}, {}, { ZR: false });
            }
            // システムボタン
            else if (cmd === '+') {
                await sendControllerInput({}, {}, {}, {}, { plus: true });
                await sleep(1000);
                await sendControllerInput({}, {}, {}, {}, { plus: false });
            }
            else if (cmd === '-') {
                await sendControllerInput({}, {}, {}, {}, { minus: true });
                await sleep(1000);
                await sendControllerInput({}, {}, {}, {}, { minus: false });
            }
            else if (cmd.toLowerCase() === 'h') {
                await sendControllerInput({}, {}, {}, {}, { home: true });
                await sleep(1000);
                await sendControllerInput({}, {}, {}, {}, { home: false });
            }
            // 左スティック
            else if (cmd.toLowerCase() === 'w') {
                await sendControllerInput({}, { x: 0, y: 100 });  // 上方向（修正: -100 → 100）
            }
            else if (cmd.toLowerCase() === 'a') {
                await sendControllerInput({}, { x: -100, y: 0 });
            }
            else if (cmd.toLowerCase() === 's') {
                await sendControllerInput({}, { x: 0, y: -100 });  // 下方向（修正: 100 → -100）
            }
            else if (cmd.toLowerCase() === 'd') {
                await sendControllerInput({}, { x: 100, y: 0 });
            }
            // 右スティック
            else if (cmd.toLowerCase() === 'i') {
                await sendControllerInput({}, {}, { x: 0, y: 100 });  // 上方向（修正: -100 → 100）
            }
            else if (cmd.toLowerCase() === 'j') {
                await sendControllerInput({}, {}, { x: -100, y: 0 });
            }
            else if (cmd.toLowerCase() === 'k') {
                await sendControllerInput({}, {}, { x: 0, y: -100 });  // 下方向（修正: 100 → -100）
            }
            else if (cmd.toLowerCase() === 'l') {
                await sendControllerInput({}, {}, { x: 100, y: 0 });
            }
            // リセット
            else if (cmd.toLowerCase() === 'reset') {
                await sendControllerInput();
            }
            else {
                console.log('無効なコマンドです');
            }
            
            askQuestion();
        });
    };

    askQuestion();
}

/**
 * 接続テスト
 */
async function testConnection() {
    console.log('接続テスト中...');
    const success = await sendControllerInput();
    
    if (!success) {
        console.error('M5AtomS3への接続に失敗しました。');
        console.error('1. M5AtomS3が起動しているか確認してください');
        console.error('2. WiFi接続が正常か確認してください');
        console.error(`3. IPアドレス ${CONTROLLER_IP} が正しいか確認してください`);
        return false;
    }
    
    console.log('接続成功!');
    return true;
}

/**
 * メイン関数（Node.js用）
 */
async function main() {
    console.log('Nintendo Switch Controller - JavaScript Client');
    console.log(`接続先: ${CONTROLLER_URL}`);
    console.log();
    
    // 接続テスト
    if (!(await testConnection())) {
        process.exit(1);
    }
    
    console.log();
    await interactiveMode();
}

// Node.js環境でのみ実行
if (typeof require !== 'undefined' && require.main === module) {
    main().catch(console.error);
}

// エクスポート（Node.js用）
if (typeof module !== 'undefined' && module.exports) {
    module.exports = {
        sendControllerInput,
        interactiveMode,
        testConnection
    };
} 