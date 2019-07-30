#pragma once
#include "define.h"
#include "skill.h"

struct pokemon {
	char* name;					//名前
	bool isLive = true;			//生きているかどうか
	
	int Max_ex = 80;
	int level = 1;				//レベル
	int experience = 0;			//経験値(自分の場合:敵の経験値を加算する　敵の場合:自分に加算するための経験値を持つ)
	int maxHp = 0;				//レベル増加による体力最大値の変化に対応(させる予定)
	int hp = 0;					//体力
	int speed = 0;				//速さ
	int type;					//タイプ
	int attackNum = 0;			//現在選んでいる技の種類
	int baseAttack = 0;			//基礎攻撃力
	int moveTexture[4][2];		//入力ファイルからinitファイルに書き出し、使用する予定
	
	int voice = 0;				//鳴き声のファイル取得用(initファイルから読み込み予定)
	
	skill skill[4];

	int direction = DOWN;		//向いている方向
	int x, y;					//位置座標
};