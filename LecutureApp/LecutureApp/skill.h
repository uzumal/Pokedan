#pragma once
#include "define.h"

struct skill {

	int min;
	int max;
	char name[256];		//技名
	int count;			//わざを使える回数
	int maxCount;		//わざを使える最大回数
	int type;			//わざタイプ

	//0:ノーマル,1:ほのお,2:みず,3:でんき,4:くさ,5:こおり,6:かくとう,7:どく,8:じめん,9:ひこう,10:エスパー,11:むし,12:いわ,13:ゴースト,14:ドラゴン,15:あく,16:はがね
};