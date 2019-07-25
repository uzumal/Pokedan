#pragma once
#include "define.h"

struct skill {

	int min;
	int max;
	char name[256];			//技名
	int count;			//わざを使える回数
	int maxCount;		//わざを使える最大回数
	int type;			//わざタイプ
};