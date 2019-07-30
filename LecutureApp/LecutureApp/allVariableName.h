#pragma once
#include "all.h"

#define GX (c->x/CHIP_SIZE - m->x)
#define GY (c->y/CHIP_SIZE - m->y)
#define SX(a) (a->x/CHIP_SIZE - m->x)
#define SY(a) (a->y/CHIP_SIZE - m->y)

/*音声ファイルメモリ用配列*/
extern int slap;
extern int bgm;
extern int down;
extern int main;
extern int click;
extern int boss_bgm;
extern int startSound;
extern int levelUP;

/*画像ファイルメモリ用配列*/
extern int skillBox;			//わざメニュー
extern int stairs_down;			//下り階段
extern int stairs_up;			//上り階段
extern int load;				//1階の道
extern int load2;				//2階の道
extern int load3;				//3階の道
extern int messageBox;			//下のメッセージボックス
extern int ball[2];
extern int tempTime;
extern int bl;
extern int title[2];
extern int over[3];

/*コンソール表示用変数*/
extern char message1[256];		//メッセージボックスンの上段
extern char message2[256];		//中段
extern char message3[256];		//下段
extern char s[256];				//sprintf_s用の配列

/*ポケモン構造体*/
extern pokemon* lastboss;						//ボスを格納する
extern pokemon* enemy[FLOORNUM-1][ENEMYNUM];	//敵を格納する

extern pokemon* c;								//ポケモン主人公ポインタ
extern pokemon* z[3];
extern maps* m;									//マップ構造体ポインタ

extern int recovery;						//歩いて回復するかどうか


/*ミニマップ表示フラグ*/
extern int miniMapFlag[FLOORNUM][MAP_YNUM][MAP_XNUM];

extern char keyState[256];						//押されているキーを格納する配列


extern bool messageflag;						//メッセージを表示するか

extern bool menuflag;							//メニューを表示するか
extern bool endflag;							//終わりかどうか
extern bool continueflag;						//続きからかどうか
extern bool isReturn;							//タイトルに戻るかどうか