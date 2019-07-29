#pragma once
#include <random>
#include "Enemy.h"
#include "Character.h"
#include "allVariableName.h"
#include "Message.h"

char keyState[256];

int miniMapFlag[FLOORNUM][MAP_YNUM][MAP_XNUM];

/*プロトタイプ宣言*/
void wait_key(int);
int getCountFrame();
void wait(int ms, char* s);
void wait(int ms);
int getRandom(int min, int max);
bool isPutMoveKey();
void levelUp(int level);

void wait_key(int key) {
	while (getCountFrame() == 0) {
		if (keyState[key] == 1) {
			break;
		}
	}
}

//技威力設定
void levelUp(int level) {

	c->skill[0].min = 2 + level * 2;
	c->skill[0].max = 4 + level * 2;
	c->skill[1].min = 5 + level * 2;
	c->skill[1].max = 8 + level * 2;
	c->skill[2].min = 8 + level * 2;
	c->skill[2].max = 14 + level * 2;
	c->skill[3].min = 1 + level * 2;
	c->skill[3].max = 3 + level * 2;
	
	for (int i = 0; i < 4; i++) {
		c->skill[i].count = c->skill[i].maxCount;
	}

	/*体力回復*/
	c->maxHp += 5;
	c->hp = c->maxHp;
}


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


//敵が死んでいるか判定
bool life(pokemon* enemy, pokemon* me) {

	int ex = 0;
	/*生きているか死んでいるか*/
	/*死亡確認*/
	if (enemy->hp == 0 && enemy->isLive) {
		PlaySoundMem(down, DX_PLAYTYPE_BACK);
		PlaySoundMem(enemy->voice, DX_PLAYTYPE_BACK);
		c->experience += enemy->experience;
		sprintf_s(s, 256, "%sは倒れた! 経験値%dを獲得!", enemy->name, enemy->experience);
		setMessage(s);
		outMessage();
		wait(400);
		while (me->Max_ex < enemy->experience && ScreenFlip()==0) {

			enemy->experience = enemy->experience - me->Max_ex;
			me->level++;
			me->Max_ex += 30;
			sprintf_s(s, "%sはレベル%dになった！", me->name, me->level);
			setMessage(s);
			outMessage();
			wait(400);
			levelUp(me->level);

		}
		enemy->isLive = false;
		return TRUE;
	}
	return FALSE;
}


