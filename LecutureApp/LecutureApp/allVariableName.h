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

/*画像ファイルメモリ用配列*/
extern int skillBox;
extern int stairs_down;
extern int stairs_up;
extern int load;
extern int load2;

/*コンソール表示用変数*/
extern char message1[256];
extern char message2[256];
extern char message3[256];
extern char s[256];				//sprintf_s用の配列

/*ポケモン構造体*/
extern pokemon* b;
extern pokemon* enemy[FLOORNUM - 1][ENEMYNUM];

extern pokemon* c;

extern int messageBox;

extern bool messageflag;	//メッセージを表示するか

extern bool menuflag;

extern maps* m;

extern char keyState[256];			//押されているキーを格納
