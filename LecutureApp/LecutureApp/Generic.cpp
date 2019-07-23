#pragma once
#include "all.h"
#include <random>
#include "Map.cpp"
#include "Character.cpp"

char keyState[256];			//押されているキーを格納

/*音声ファイルメモリ用配列*/
int slap;
int bgm;
int down;

/*画像ファイルメモリ用配列*/
int skillBox;
int stairs_down;
int stairs_up;
int load;
int load2;

/*キーが押されているフレーム数を数える*/
int getCountFrame() {
	char tmpKey[256];								// 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey);						// 全てのキーの入力状態を得る
	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) {						// i番のキーコードに対応するキーが押されていたら
			keyState[i]++;							// 加算
			if (keyState[i] == 60)keyState[i] = 2;	//移動時の画像処理に使用
		}
		else {										// 押されていなければ
			keyState[i] = 0;						// 0にする
		}
	}
	return 0;
}

void w_press() {
	while (getCountFrame() == 0) {
		if (keyState[KEY_INPUT_K] == 1) {
			break;
		}
	}
}

void wait(int ms, char* s) {
	int tmp = GetNowCount();
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && GetNowCount() - tmp < ms) {
		DrawFormatString(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2, GetColor(255, 255, 255), "%s", s);
	}
}

void wait(int ms) {
	int tmp = GetNowCount();
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && GetNowCount() - tmp < ms) {

	}
}

/*0～3の乱数を得る*/
int getRandom(int min, int max) {

	std::random_device rd;

	std::mt19937 mt(rd());

	std::uniform_int_distribution<int> dice(min, max);

	return dice(mt);
}


bool isPutMoveKey() {

	if (keyState[KEY_INPUT_Q] == 1 || keyState[KEY_INPUT_W] == 1 || keyState[KEY_INPUT_E] == 1 || keyState[KEY_INPUT_D] == 1 || keyState[KEY_INPUT_C] == 1 || keyState[KEY_INPUT_X] == 1 || keyState[KEY_INPUT_Z] == 1 || keyState[KEY_INPUT_A] == 1) {
		return true;
	}
	else {
		return false;
	}

}


int init() {
	/*ウインドウの大きさ指定*/
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16);
	/*背景色描画(後にマップ)*/
	SetBackgroundColor(20, 20, 20);
	/*ウィンドウモードに指定*/
	ChangeWindowMode(TRUE);
	if (DxLib_Init() != 0) { return -1; }	// DXライブラリ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);			//描画先を裏画面に設定
	SetUseDirectDrawFlag(FALSE);			//処理を軽くするために使用
	SetWaitVSyncFlag(FALSE);

	/*後々自分の名前を入力させる*/
	c->name = "ピカチュウ";
	for (int i = 0; i < FLOORNUM - 1; i++) {
		enemy[i][0]->name = "ダークライ";
		enemy[i][1]->name = "フシギダネ";
		enemy[i][2]->name = "スイクン";
	}

	/*画像のロード*/
	c->moveTexture[DOWN][0] = LoadGraph("画像/ピカチュウd_1.png");
	c->moveTexture[DOWN][1] = LoadGraph("画像/ピカチュウd_2.png");
	c->moveTexture[UP][0] = LoadGraph("画像/ピカチュウu_1.png");
	c->moveTexture[UP][1] = LoadGraph("画像/ピカチュウu_2.png");
	c->moveTexture[LEFT][0] = LoadGraph("画像/ピカチュウl_1.png");
	c->moveTexture[LEFT][1] = LoadGraph("画像/ピカチュウl_2.png");
	c->moveTexture[RIGHT][0] = LoadGraph("画像/ピカチュウr_1.png");
	c->moveTexture[RIGHT][1] = LoadGraph("画像/ピカチュウr_2.png");

	c->skill[0].name = "でんこうせっか";
	c->skill[1].name = "10まんボルト";
	c->skill[2].name = "かみなり";
	c->skill[3].name = "ひっかく";

	c->skill[0].value = 2;
	c->skill[1].value = 7;
	c->skill[2].value = 10;
	c->skill[3].value = 3;

	c->skill[0].maxCount = 20;
	c->skill[1].maxCount = 10;
	c->skill[2].maxCount = 5;
	c->skill[3].maxCount = 20;

	c->skill[0].count = c->skill[0].maxCount;
	c->skill[1].count = c->skill[1].maxCount;
	c->skill[2].count = c->skill[2].maxCount;
	c->skill[3].count = c->skill[3].maxCount;

	c->maxHp = 30;
	c->hp = 30;

	skillfull(0);
	for (int i = 0; i < FLOORNUM - 1; i++) {
		enemy[i][0]->moveTexture[DOWN][0] = LoadGraph("画像/ダークライd_1.png");
		enemy[i][0]->moveTexture[DOWN][1] = LoadGraph("画像/ダークライd_2.png");
		enemy[i][0]->moveTexture[UP][0] = LoadGraph("画像/ダークライu_1.png");
		enemy[i][0]->moveTexture[UP][1] = LoadGraph("画像/ダークライu_2.png");
		enemy[i][0]->moveTexture[LEFT][0] = LoadGraph("画像/ダークライl_1.png");
		enemy[i][0]->moveTexture[LEFT][1] = LoadGraph("画像/ダークライl_2.png");
		enemy[i][0]->moveTexture[RIGHT][0] = LoadGraph("画像/ダークライr_1.png");
		enemy[i][0]->moveTexture[RIGHT][1] = LoadGraph("画像/ダークライr_2.png");

		enemy[i][0]->skill[0].name = "でんこうせっか";
		enemy[i][0]->skill[1].name = "だましうち";
		enemy[i][0]->skill[2].name = "あくのはどう";
		enemy[i][0]->skill[3].name = "れいとうビーム";

		enemy[i][0]->skill[0].value = 2;
		enemy[i][0]->skill[1].value = 7;
		enemy[i][0]->skill[2].value = 10;
		enemy[i][0]->skill[3].value = 3;


		enemy[i][0]->skill[0].maxCount = 20;
		enemy[i][0]->skill[1].maxCount = 10;
		enemy[i][0]->skill[2].maxCount = 20;
		enemy[i][0]->skill[3].maxCount = 10;

		enemy[i][0]->skill[0].count = c->skill[0].maxCount;
		enemy[i][0]->skill[1].count = c->skill[1].maxCount;
		enemy[i][0]->skill[2].count = c->skill[2].maxCount;
		enemy[i][0]->skill[3].count = c->skill[3].maxCount;

		enemy[i][0]->voice = LoadSoundMem("音楽/ダークライvoice.mp3");

		enemy[i][0]->experience = 10000;

		enemy[i][0]->maxHp = 70;
		enemy[i][0]->hp = 70;

		enemy[i][1]->moveTexture[DOWN][0] = LoadGraph("画像/フシギダネd_1.png");
		enemy[i][1]->moveTexture[DOWN][1] = LoadGraph("画像/フシギダネd_2.png");
		enemy[i][1]->moveTexture[UP][0] = LoadGraph("画像/フシギダネu_1.png");
		enemy[i][1]->moveTexture[UP][1] = LoadGraph("画像/フシギダネu_2.png");
		enemy[i][1]->moveTexture[LEFT][0] = LoadGraph("画像/フシギダネl_1.png");
		enemy[i][1]->moveTexture[LEFT][1] = LoadGraph("画像/フシギダネl_2.png");
		enemy[i][1]->moveTexture[RIGHT][0] = LoadGraph("画像/フシギダネr_1.png");
		enemy[i][1]->moveTexture[RIGHT][1] = LoadGraph("画像/フシギダネr_2.png");

		enemy[i][1]->skill[0].name = "はっぱカッター";
		enemy[i][1]->skill[1].name = "でんこうせっか";
		enemy[i][1]->skill[2].name = "あくのはどう";
		enemy[i][1]->skill[3].name = "たいあたり";

		enemy[i][1]->skill[0].value = 5;
		enemy[i][1]->skill[1].value = 10;
		enemy[i][1]->skill[2].value = 4;
		enemy[i][1]->skill[3].value = 11;


		enemy[i][1]->skill[0].maxCount = 10;
		enemy[i][1]->skill[1].maxCount = 5;
		enemy[i][1]->skill[2].maxCount = 10;
		enemy[i][1]->skill[3].maxCount = 10;

		enemy[i][1]->skill[0].count = c->skill[0].maxCount;
		enemy[i][1]->skill[1].count = c->skill[1].maxCount;
		enemy[i][1]->skill[2].count = c->skill[2].maxCount;
		enemy[i][1]->skill[3].count = c->skill[3].maxCount;

		enemy[i][1]->voice = LoadSoundMem("音楽/フシギダネvoice.mp3");

		enemy[i][1]->experience = 10000;

		enemy[i][1]->maxHp = 70;
		enemy[i][1]->hp = 70;

		enemy[i][2]->moveTexture[DOWN][0] = LoadGraph("画像/スイクンd_1.png");
		enemy[i][2]->moveTexture[DOWN][1] = LoadGraph("画像/スイクンd_2.png");
		enemy[i][2]->moveTexture[UP][0] = LoadGraph("画像/スイクンu_1.png");
		enemy[i][2]->moveTexture[UP][1] = LoadGraph("画像/スイクンu_2.png");
		enemy[i][2]->moveTexture[LEFT][0] = LoadGraph("画像/スイクンl_1.png");
		enemy[i][2]->moveTexture[LEFT][1] = LoadGraph("画像/スイクンl_2.png");
		enemy[i][2]->moveTexture[RIGHT][0] = LoadGraph("画像/スイクンr_1.png");
		enemy[i][2]->moveTexture[RIGHT][1] = LoadGraph("画像/スイクンr_2.png");

		enemy[i][2]->skill[0].name = "みずでっぽう";
		enemy[i][2]->skill[1].name = "ハイドロポンプ";
		enemy[i][2]->skill[2].name = "あくのはどう";
		enemy[i][2]->skill[3].name = "あわ";

		enemy[i][2]->skill[0].value = 5;
		enemy[i][2]->skill[1].value = 10;
		enemy[i][2]->skill[2].value = 4;
		enemy[i][2]->skill[3].value = 11;


		enemy[i][2]->skill[0].maxCount = 10;
		enemy[i][2]->skill[1].maxCount = 5;
		enemy[i][2]->skill[2].maxCount = 10;
		enemy[i][2]->skill[3].maxCount = 10;

		enemy[i][2]->skill[0].count = c->skill[0].maxCount;
		enemy[i][2]->skill[1].count = c->skill[1].maxCount;
		enemy[i][2]->skill[2].count = c->skill[2].maxCount;
		enemy[i][2]->skill[3].count = c->skill[3].maxCount;

		enemy[i][2]->voice = LoadSoundMem("音楽/スイクンvoice.mp3");

		enemy[i][2]->experience = 10000;

		enemy[i][2]->maxHp = 70;
		enemy[i][2]->hp = 70;
	}
	/*階段の画像*/
	stairs_down = LoadGraph("画像/stairs.png");
	stairs_up = LoadGraph("画像/stairs2.png");

	/*白色の透過*/
	SetTransColor(255, 255, 255);
	/*メッセージボックス*/
	messageBox = LoadGraph("画像/messageBox.png");
	SetTransColor(255, 0, 0);
	/*わざ選択ボックス*/
	skillBox = LoadGraph("画像/skillBox.png");
	/*座標位置*/
	c->x = CHIP_SIZE * 3;
	c->y = CHIP_SIZE * 3;

	randomEnemyPut(enemy[m->floor]);

	/*音声をロード*/
	slap = LoadSoundMem("音楽/slap1.mp3");
	bgm = LoadSoundMem("音楽/bgm.mp3");
	down = LoadSoundMem("音楽/down1.mp3");

	load = LoadGraph("画像/load.png");
	load2 = LoadGraph("画像/load2.png");

	return 0;
}

//技威力設定
void skillfull(int level) {
	c->skill[0].min = 2 + level * 2;
	c->skill[0].max = 4 + level * 2;
	c->skill[1].min = 5 + level * 2;
	c->skill[1].max = 8 + level * 2;
	c->skill[2].min = 8 + level * 2;
	c->skill[2].max = 14 + level * 2;
	c->skill[3].min = 1 + level * 2;
	c->skill[3].max = 3 + level * 2;

	for (int j = 0; j < FLOORNUM - 1; j++) {
		for (int i = 0; i < ENEMYNUM; i++) {
			enemy[j][i]->skill[0].min = 2;
			enemy[j][i]->skill[0].max = 4;
			enemy[j][i]->skill[1].min = 3;
			enemy[j][i]->skill[1].max = 6;
			enemy[j][i]->skill[2].min = 8;
			enemy[j][i]->skill[2].max = 14;
			enemy[j][i]->skill[3].min = 4;
			enemy[j][i]->skill[3].max = 8;
		}
	}
}


void drawCharacter(pokemon* me, int d_num) {
	//生きていれば表示
	if (me->isLive) {
		/*座標は画像の真ん中に持つ*/
		/*DrawRotaGraph(x座標,y座標,縮尺度,角度,描画する画像ハンドル,背景透過処理のON,OFF)*/
		DrawRotaGraph(me->x + 20, me->y + 20, 1.5, 0, me->moveTexture[me->direction][d_num], true);
	}
}

