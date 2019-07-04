#include "DxLib.h"

#define SCREEN_WIDTH     (1280)                          // 画面の横幅
#define SCREEN_HEIGHT    (720)                          // 画面の縦幅
#define CHIP_SIZE        (32)                           // 一つのチップのサイズ
#define MAP_WIDTH        (SCREEN_WIDTH / CHIP_SIZE)     // マップの横幅
#define MAP_HEIGHT       (SCREEN_HEIGHT / CHIP_SIZE)    // マップの縦幅


char keyState[256];

int FrameStartTime = 0;

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
	
	
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16);
	SetBackgroundColor(100, 0, 100);
	ChangeWindowMode(TRUE);
	DxLib_Init();					// DXライブラリ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);  //描画先を裏画面に設定

	
	/*画像ファイル読み込みの前に透過処理*/
	SetTransColor(255, 255, 255);
	
	
	int Mario;     // 画像格納用ハンドル
	Mario = LoadGraph("画像/Mario.png"); // 画像のロード
	

	double temp = 0;
	double y_temp = 0;
	double y_prev = 0;
	double f = 0;

	double x = 100;
	double	y = 100;
	bool endflag = false;
	bool onGround = false;
	bool jump = false;
	bool bigFlag = true;
	bool xVec = false;

	SetWaitVSyncFlag(FALSE);
	FrameStartTime = GetNowCount();
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && gpUpdateKey() == 0) {
		
		ClsDrawScreen();

		/*60フレーム固定*/
		while (GetNowCount() - FrameStartTime < 1000 / 60) {}

		FrameStartTime = GetNowCount();

		DrawRotaGraph((int)x,(int)y,0.2, 0, Mario, TRUE);

		if (bigFlag) {

			if (keyState[KEY_INPUT_Q] != 0) { endflag = true; }
			/*Bダッシュ*/
			else if (keyState[KEY_INPUT_B] && keyState[KEY_INPUT_D]) { if(x<SCREEN_WIDTH-30) x += 5; }
			else if (keyState[KEY_INPUT_B] && keyState[KEY_INPUT_A]) { if(x>30) x -= 5; }
			else if (keyState[KEY_INPUT_B] && keyState[KEY_INPUT_W]) { if(y>30) y -= 5; }
			else if (keyState[KEY_INPUT_B] && keyState[KEY_INPUT_X]) { if(y<SCREEN_HEIGHT-30) y += 5; }
			else if (keyState[KEY_INPUT_D]) { if(x<SCREEN_WIDTH-30) x++; }
			else if (keyState[KEY_INPUT_A]) { if(x>30) x--; }
			else if (keyState[KEY_INPUT_W]) { if(y>30) y--; }
			else if (keyState[KEY_INPUT_X]) { if(y<SCREEN_HEIGHT-30) y++; }
			
			/*ジャンプ処理*/
			if (keyState[KEY_INPUT_SPACE] == 1) { 
				y_prev = y; temp = y;
				xVec = true; onGround = true; bigFlag = false; 
			}

		}

		if (onGround) {
			if (y >= 260) {
				f = -40;
				y_temp = y;
				y += (y - y_prev) + f;
				y_prev = y_temp;
			}
			else {
				y_temp = 30;
				y = 30;
				y_prev = y_temp;
			}
			
			jump = true;
			onGround = false;
		}

		if (jump) {
			f = 3;
			y_temp = y;
			y += (y - y_prev) + f;
			y_prev = y_temp;
			if (y >= temp) {
				y = temp;
				bigFlag = true;
				jump = false;
				xVec = false;
			}
		}

		if (xVec) {
			if (keyState[KEY_INPUT_B] && keyState[KEY_INPUT_D]) { if(x<SCREEN_WIDTH-30) x += 5; }
			else if (keyState[KEY_INPUT_B] && keyState[KEY_INPUT_A]) { if(x>30) x -= 5; }
			else if (keyState[KEY_INPUT_D])if(x<SCREEN_WIDTH-30)x++;
			else if (keyState[KEY_INPUT_A])if(x>30)x--;
		}

		if (endflag) {
			break;
		}
	}
	DxLib_End();    // DXライブラリ終了処理
	return 0;
}