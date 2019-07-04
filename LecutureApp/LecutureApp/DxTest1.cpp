#include "DxLib.h"

char keyState[256];

/*キーが押されているフレーム数を数える*/
int gpUpdateKey() {
	char tmpKey[256]; // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
	for (int i = 0; i<256; i++) {
		if (tmpKey[i] != 0) { // i番のキーコードに対応するキーが押されていたら
			keyState[i]++;     // 加算
		}
		else {              // 押されていなければ
			keyState[i] = 0;   // 0にする
		}
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE);
	DxLib_Init();   // DXライブラリ初期化処理
	//SettingAfterInit();
	SetDrawScreen(DX_SCREEN_BACK); //描画先を裏画面に設定

	int Handle;     // 画像格納用ハンドル
	Handle = LoadGraph("画像/chara.jpg"); // 画像のロード

	int x = 100;
	int	y = 100;
	bool flag = false;

	DrawRotaGraph(x, y, 0.01, 0, Handle, TRUE);

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && gpUpdateKey() == 0) {

		DrawRotaGraph(x,y,0.2, 0,Handle, TRUE);

		if (keyState[KEY_INPUT_Q]!=0) {flag = true;}
		else if(keyState[KEY_INPUT_D]>2&&keyState[KEY_INPUT_D]<20){ }
		else if (keyState[KEY_INPUT_B] && keyState[KEY_INPUT_D]) { x += 3; }
		else if (keyState[KEY_INPUT_B] && keyState[KEY_INPUT_A]) { x -= 3; }
		else if (keyState[KEY_INPUT_B] && keyState[KEY_INPUT_W]) { y -= 3; }
		else if (keyState[KEY_INPUT_B] && keyState[KEY_INPUT_X]) { y += 3; }
		else if (keyState[KEY_INPUT_D]) { x++; }
		else if (keyState[KEY_INPUT_A]) { x--; }
		else if (keyState[KEY_INPUT_W]) { y--; }
		else if (keyState[KEY_INPUT_X]) { y++; }

		//switch (*keyState) {
		//case KEY_INPUT_D: x++; break;
		//case KEY_INPUT_W: y++; break;
		//case KEY_INPUT_A: x--; break;
		//case KEY_INPUT_X: y--; break;
		//case KEY_INPUT_Q: flag = true; break;
		//default: break;
		//}

		if (flag) {
			break;
		}
	}
	DxLib_End();    // DXライブラリ終了処理
	return 0;
}