#include "all.h"
#include "allVariableName.h"
#include "Generic.h"
#include "Enemy.h"
#include "FileIO.h"
#include "Message.h"

/*音声ファイルメモリ用配列*/
int slap;
int bgm;
int down;
int main;
int click;
int boss_bgm;
int startSound;
int levelUP;

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

	nameSet(z[0], MAINCHARANAME1);
	nameSet(z[1], MAINCHARANAME2);
	nameSet(z[2],MAINCHARANAME3);
	

	for (int i = 0; i < 3; i++) {
		charaSet(z[i]);
	}


	/*階段の画像*/
	stairs_down = LoadGraph("画像/stairs.png");
	stairs_up = LoadGraph("画像/stairs2.png");

	/*画像のロード*/
	SetTransColor(255, 255, 255);
	ball[0] = LoadGraph("画像/モンスターボール.png");
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

	load  = LoadGraph("画像/load.png");
	load2 = LoadGraph("画像/load2.png");
	load3 = LoadGraph("画像/load3.png");

	/*音声をロード*/
	slap = LoadSoundMem("音楽/slap1.mp3");
	bgm = LoadSoundMem("音楽/bgm.mp3");
	down = LoadSoundMem("音楽/down1.mp3");
	boss_bgm = LoadSoundMem("音楽/bossBGM.mp3");
	main = LoadSoundMem("音楽/main.mp3");
	startSound = LoadSoundMem("音楽/start.mp3");
	levelUP = LoadSoundMem("音楽/レベルアップ.mp3");

	return 0;
}

void charaInit() {
	
	/*マップ初期化*/
	m->floor = 0;
	m->x = 0;
	m->y = 0;

	enemy[0][0]->name = ENEMYNAME1;
	enemy[0][1]->name =	ENEMYNAME2;
	enemy[0][2]->name = ENEMYNAME3;
	enemy[1][0]->name = ENEMYNAME4;
	enemy[1][1]->name = ENEMYNAME5;
	enemy[1][2]->name = ENEMYNAME6;
	
	lastboss->name = BOSSNAME;

	if (continueflag) {
		if (readData() == -1) {
			setMessage("データを読み込めませんでした");
			outMessage();
			ScreenFlip();
		}
	}
	else {
		charaSet(c);
		//主人公は経験値0から始める
		c->experience = 0;
		/*初期座標位置*/
		c->x = CHIP_SIZE * 3;
		c->y = CHIP_SIZE * 3;
		for (int i = 0; i < FLOORNUM - 1; i++) {
			for (int j = 0; j < ENEMYNUM; j++) {
				charaSet(enemy[i][j]);
			}
		}
		charaSet(lastboss);
		for (int i = 0; i < FLOORNUM; i++) {
			for (int j = 0; j < MAP_YNUM; j++) {
				for (int k = 0; k < MAP_XNUM; k++) {
					miniMapFlag[i][j][k] = 0;
				}
			}
		}
	}

	if (!continueflag) {
		randomEnemyPut(enemy[m->floor]);
	}
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