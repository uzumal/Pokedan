#include "all.h"
#include "allVariableName.h"
#include "Generic.h"
#include "Enemy.h"
#include "FileIO.h"


/*音声ファイルメモリ用配列*/
int slap;
int bgm;
int down;
int main;
int start;
int click;

/*画像ファイルメモリ用配列*/
int skillBox;
int stairs_down;
int stairs_up;
int load;
int load2;
int load3;
int ball[2];
int tempTime = 0;
int bl;
int title[2];
int over[3];

int init();
void charaInit();
void nameSet(pokemon*, char*);
void charaSet(pokemon*);

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

	m->floor = 0;

	z[0]->name = "ピカチュウ";
	z[1]->name = "ヒノアラシ";
	z[2]->name = "ゼニガメ";

	for (int i = 0; i < 3; i++) {
		charaSet(z[i]);
	}


	/*階段の画像*/
	stairs_down = LoadGraph("画像/stairs.png");
	stairs_up = LoadGraph("画像/stairs2.png");

	/*画像のロード*/
	SetTransColor(255, 255, 255);
	ball[0] = LoadGraph("画像/モンスターボール.png");
	//ball[1] = LoadGraph("画像/ピカチュウd_1.png");
	title[0] = LoadGraph("画像/back.png");
	title[1] = LoadGraph("画像/title2.png");

	over[0] = LoadGraph("画像/over.png");
	bl = LoadGraph("画像/black.png");

	/*白色の透過*/
	SetTransColor(255, 255, 255);
	/*メッセージボックス*/
	messageBox = LoadGraph("画像/messageBox.png");
	SetTransColor(255, 0, 0);
	/*わざ選択ボックス*/
	skillBox = LoadGraph("画像/skillBox.png");

	/*音声をロード*/
	slap = LoadSoundMem("音楽/slap1.mp3");
	bgm = LoadSoundMem("音楽/bgm.mp3");
	down = LoadSoundMem("音楽/down1.mp3");

	load  = LoadGraph("画像/load.png");
	load2 = LoadGraph("画像/load2.png");
	load3 = LoadGraph("画像/load3.png");

	main = LoadSoundMem("音楽/main.mp3");
	start = LoadSoundMem("音楽/start.mp3");

	return 0;
}

void charaInit() {
	
	charaSet(c);

	enemy[2][1]->isLive = false;
	enemy[2][2]->isLive = false;

	enemy[0][0]->name = "フシギダネ";
	enemy[0][1]->name = "ディグダ";
	enemy[0][2]->name = "ディグダ";
	enemy[1][0]->name = "ダークライ";
	enemy[1][1]->name = "ボスゴドラ";
	enemy[1][2]->name = "スイクン";
	enemy[2][0]->name = "ミュウツー";

	for (int i = 0; i < FLOORNUM - 1; i++) {
		for (int j = 0; j < ENEMYNUM; j++) {
			charaSet(enemy[i][j]);
		}
	}

	charaSet(enemy[2][0]);

	/*マップ初期化*/
	m->floor = 0;
	m->x = 0;
	m->y = 0;

	for (int i = 0; i < FLOORNUM; i++) {
		for (int j = 0; j < MAP_YNUM; j++) {
			for (int k = 0; k < MAP_XNUM; k++) {
				miniMapFlag[i][j][k] = 0;
			}
		}
	}

	/*初期座標位置*/
	c->x = CHIP_SIZE * 3;
	c->y = CHIP_SIZE * 3;

	randomEnemyPut(enemy[m->floor]);

}

void nameSet(pokemon* me,char* s) {
	me->name = s;
}

void charaSet(pokemon* me) {
	char* dir[4] = { "d_","u_","l_","r_" };
	for (int i = 0; i < 4; i++) {
		sprintf_s(s, 256, "画像/%s/%s%s1.png", me->name,me->name,dir[i]);
		me->moveTexture[i][0] = LoadGraph(s);
		sprintf_s(s, 256, "画像/%s/%s%s2.png", me->name,me->name, dir[i]);
		me->moveTexture[i][1] = LoadGraph(s);
	}
	sprintf_s(s, 256, "音楽/%svoice.mp3",me->name);
	me->level = 1;
	me->Max_ex = 80;
	me->experience = 0;
	me->voice = LoadSoundMem(s);
	setPoke(me);
	me->direction = DOWN;
	me->isLive = true;
}