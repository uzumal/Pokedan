#pragma once
#include "define.h"

struct skill {

	int value;				//攻撃の値(initファイルから読み込み予定)
	char* name;			//技名
	int count;			//わざを使える回数
	int maxCount;		//わざを使える最大回数
	int type;			//わざタイプ
};