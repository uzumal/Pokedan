#pragma once

#define DOWN	0
#define UP		1
#define LEFT	2
#define RIGHT	3

struct pokemon {
	char* name;				//名前
	int level = 1;				//レベル
	int experience = 0;			//経験値
	int maxHp = 30;				//レベル増加による体力最大値の変化に対応
	int hp = maxHp;					//体力
	int moveTexture[4][2];	//入力ファイルからinitファイルに書き出し、使用する予定
	int voice;				//鳴き声のファイル取得用(initファイルから読み込み予定)
	int attack[4] = { 1,1,1,1 };			//技の種類(initファイルから読み込み予定)
	int direction = DOWN;
	char* skillName[4];		//技名
	int x, y;				//位置座標
	bool isLive = true;     //生きているかどうか
};