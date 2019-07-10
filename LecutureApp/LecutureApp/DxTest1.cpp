#include "DxLib.h"
#include "pokemon.h"
#include <iostream>
#include <random>

#define SCREEN_WIDTH     (800)                          // 画面の横幅
#define SCREEN_HEIGHT    (600)                          // 画面の縦幅
#define CHIP_SIZE        (40)                           // 一つのチップのサイズ
#define MAP_HEIGHT		 (400)
#define MAP_WIDTH		 (720)
/*ジャンプ用変数*/
int temp = 0;
int y_temp = 0;
int y_prev = 0;

/*ジャンプ用フラグ*/
bool onGround = false;		//ジャンプする瞬間
bool jump = false;			//ジャンプしているかどうか	


/*コンソール表示用変数*/
char message1[256];
char message2[256];
char message3[256];
char s[256];				//sprintf_s用の配列
bool messageflag = false;	//メッセージを表示するか

char keyState[256];			//押されているキーを格納

/*フラグ*/
bool endflag = false;		//qを押すと終わり
bool menuflag = false;
/*ポケモン構造体*/
pokemon poke;
pokemon dark;
pokemon* c = &poke;
pokemon* d = &dark;

/*音声ファイルメモリ用配列*/
int slap;
int bgm;
int down;

/*画像ファイルメモリ用配列*/
int messageBox;
int skillBox;

int attackNum;

/*プロトタイプ宣言*/
int init();
void attack(pokemon*, pokemon*, int);			//攻撃
void turnToPokemon(pokemon*, pokemon*);			//ポケモンの方を向く
void moveJump(pokemon*);						//Jumpする
void initConsole();								//メッセージボックスを初期化する
void setMessage(char[]);						//表示したいメッセージをセットする
void outMessage();								//メッセージを表示する
bool isNearPokemon(pokemon*, pokemon*);			//敵が近くにいたら(攻撃圏内にいたら)true
bool findPokemon(pokemon*, pokemon*);
void enemyMove(pokemon*);						//敵の動き
int getRandom(int,int);

/*キーが押されているフレーム数によって表示する画像を変更する*/
/*
int getDnum(int key) {
	if (keyState[key] % 60 >= 1 && keyState[key] % 60 < 30) return 0;
	else return 1;
}
*/

/******************************************************************************/
/*キーが押されているフレーム数を数える*/
int getCountFrame() {
	char tmpKey[256];								// 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey);						// 全てのキーの入力状態を得る
	for (int i = 0; i<256; i++) {
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
/******************************************************************************/


/***********************      Mainの処理      *********************************/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	/*初期化処理*/
	if (init() == -1) { return -1; }

	int d_num = 0;
	int tempTime = 0;

	/*白色を格納*/
	const int white = GetColor(255, 255, 255);

	/*bgm再生開始*/
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);

	/*描画する*/
	// while(裏画面を表画面に反映, メッセージ処理, 画面クリア,フレームカウント)
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && getCountFrame() == 0) {

		/*500ms(0.5秒)に一度画像更新(歩いているように見える)*/
		if (GetNowCount() - tempTime> 500) 
		{	
			if (d_num == 1)d_num = 0;
			else d_num = 1;
			tempTime = GetNowCount(); 
		}

		/*階層、level、HP表示(固定)*/
		DrawFormatString(0	, 0, white, "B1F");
		DrawFormatString(50	, 0, white, "Lv: %d",c->level);
		DrawFormatString(120, 0, white, "HP: %d/ %d",c->hp,c->maxHp);
		DrawFormatString(220, 0, white, "セットわざ名 : %s",c->skill[attackNum].name);
		/*確認用座標(あとで消す)*/
		DrawFormatString(500, 0, white, "ピカ座標(%d,%d)", c->x, c->y);
		DrawFormatString(500, 20,white, "ダー座標(%d,%d)",d->x,d->y );

		/*DrawRotaGraph(x座標,y座標,縮尺度,角度,描画する画像ハンドル,背景透過処理のON,OFF)*/
		/*座標は画像の真ん中に持つ*/
		DrawRotaGraph(c->x, c->y, 1.5, 0, c->moveTexture[c->direction][d_num], true);
		/*生きてたら敵表示*/
		if (d->isLive) { DrawRotaGraph(d->x, d->y, 1.5, 0, d->moveTexture[d->direction][d_num], true); }

		/*メッセージ出力*/
		outMessage();

		/*qキーで終わり*/
		if (keyState[KEY_INPUT_Q] == 1) { endflag = true; }
		
		/*わざ表を消去*/
		if (menuflag && keyState[KEY_INPUT_I] == 1) {
			menuflag = false;
		}

		/*わざ表を表示*/
		else if (!menuflag && keyState[KEY_INPUT_I] == 1) {
			menuflag = true;
		}

		/*Attack*/
		if (!jump && !menuflag && keyState[KEY_INPUT_Z] == 1) {
			/*敵のいる方向に向きを変える*/
			if (isNearPokemon(c, d) && d->isLive) {
				attack(c, d, attackNum);
			}
			enemyMove(d);
		}

		/*斜め移動も入れるならelse ifじゃなくてifだけにする*/
		/*自分が動けば敵も動く*/
		/*Right*/
		else if (!menuflag && keyState[KEY_INPUT_RIGHT]==1) {
			c->direction = RIGHT;
			if (!keyState[KEY_INPUT_Y] && c->x < MAP_WIDTH && (!(d->x == c->x + CHIP_SIZE && d->y == c->y)||!d->isLive)) {
					c->x += CHIP_SIZE;
			}
		}

		/*Left*/
		else if (!menuflag && keyState[KEY_INPUT_LEFT]==1) { 
			c->direction = LEFT;
			if (!keyState[KEY_INPUT_Y] &&  c->x > CHIP_SIZE * 2 && (!(d->x == c->x - CHIP_SIZE && d->y == c->y)||!d->isLive)) {
				c->x-=CHIP_SIZE;
			}
		}

		/*Up*/
		if (!menuflag && !jump && keyState[KEY_INPUT_UP]==1) {
			c->direction = UP;
			if (!keyState[KEY_INPUT_Y] && c->y > CHIP_SIZE * 2 && (!(d->y == c->y - CHIP_SIZE && d->x == c->x)||!d->isLive)) {
				c->y-=CHIP_SIZE;
			}
		}

		/*Down*/
		else if (!menuflag && !jump && keyState[KEY_INPUT_DOWN]==1) {
			c->direction = DOWN;
			if (!keyState[KEY_INPUT_Y] && c->y < MAP_HEIGHT && (!(d->y == c->y + CHIP_SIZE && d->x == c->x)||!d->isLive)) {
				c->y+=CHIP_SIZE;
			}
		}

		if (!menuflag && !keyState[KEY_INPUT_Y]) {
			if (keyState[KEY_INPUT_RIGHT] == 1 || keyState[KEY_INPUT_LEFT] == 1 || keyState[KEY_INPUT_UP] == 1 || keyState[KEY_INPUT_DOWN] == 1){
				if(!(c->x == CHIP_SIZE * 2||c->x == MAP_WIDTH||c->y == CHIP_SIZE * 2 || c->y == MAP_HEIGHT) || !isNearPokemon(c,d))
					enemyMove(d);
			}
		}
		/*おまけのジャンプ処理*/
		//if (!jump && keyState[KEY_INPUT_SPACE] == 1) { 
		//	y_prev = c->y; temp = c->y;
		//	jump = true;  onGround = true;
		//}

		/*ジャンプ処理本体*/
		//if (jump) {
			//moveJump(c);
		//}

		if (menuflag) {
			DrawRotaGraph(550, 160, 1, 0, skillBox, true);
			DrawFormatString(530, 98,  white, "%s (%d/%d)", c->skill[0].name, c->skill[0].count, c->skill[0].maxCount);
			DrawFormatString(530, 149, white, "%s (%d/%d)", c->skill[1].name, c->skill[1].count, c->skill[1].maxCount);
			DrawFormatString(530, 199, white, "%s (%d/%d)", c->skill[2].name, c->skill[2].count, c->skill[2].maxCount);
			DrawFormatString(530, 250, white, "%s (%d/%d)", c->skill[3].name, c->skill[3].count, c->skill[3].maxCount);
			if (keyState[KEY_INPUT_1]) { attackNum = 0; menuflag = false; }
			else if (keyState[KEY_INPUT_2]) { attackNum = 1; menuflag = false;}
			else if (keyState[KEY_INPUT_3]) { attackNum = 2; menuflag = false;}
			else if (keyState[KEY_INPUT_4]) { attackNum = 3; menuflag = false;}
		}

		if (endflag) {
			break;
		}
	}

	DxLib::DxLib_End();    // DXライブラリ終了処理
	
	return 0;
}



int init() {
	/*ウインドウの大きさ指定*/
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16);
	/*背景色描画(後にマップ)*/
	SetBackgroundColor(100, 100, 100);
	/*ウィンドウモードに指定*/
	ChangeWindowMode(TRUE);
	if (DxLib_Init() != 0) { return -1; }	// DXライブラリ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);			//描画先を裏画面に設定
	SetUseDirectDrawFlag(FALSE);			//処理を軽くするために使用
	SetWaitVSyncFlag(FALSE);

	/*後々自分の名前を入力させる*/
	c->name = "ピカチュウ";
	d->name = "ダークライ";

	/*画像のロード*/
	c->moveTexture[DOWN][0] = LoadGraph("画像/ピカチュウd_1.png");
	c->moveTexture[DOWN][1] = LoadGraph("画像/ピカチュウd_2.png");
	c->moveTexture[UP][0] = LoadGraph("画像/ピカチュウu_1.png");
	c->moveTexture[UP][1] = LoadGraph("画像/ピカチュウu_2.png");
	c->moveTexture[LEFT][0] = LoadGraph("画像/ピカチュウl_1.png");
	c->moveTexture[LEFT][1] = LoadGraph("画像/ピカチュウl_2.png");
	c->moveTexture[RIGHT][0] = LoadGraph("画像/ピカチュウr_1.png");
	c->moveTexture[RIGHT][1] = LoadGraph("画像/ピカチュウr_2.png");

	c->skill[0].name = "でんこうせっか";
	c->skill[1].name = "10まんボルト";
	c->skill[2].name = "かみなり";
	c->skill[3].name = "ひっかく";

	c->skill[0].value = 2;
	c->skill[1].value = 7;
	c->skill[2].value = 10;
	c->skill[3].value = 3;

	c->skill[0].maxCount = 20;
	c->skill[1].maxCount = 10;
	c->skill[2].maxCount = 5;
	c->skill[3].maxCount = 20;

	c->skill[0].count = c->skill[0].maxCount;
	c->skill[1].count = c->skill[1].maxCount;
	c->skill[2].count = c->skill[2].maxCount;
	c->skill[3].count = c->skill[3].maxCount;

	c->maxHp = 30;
	c->hp = 30;

	d->moveTexture[DOWN][0] = LoadGraph("画像/ダークライd_1.png");
	d->moveTexture[DOWN][1] = LoadGraph("画像/ダークライd_2.png");
	d->moveTexture[UP][0] = LoadGraph("画像/ダークライu_1.png");
	d->moveTexture[UP][1] = LoadGraph("画像/ダークライu_2.png");
	d->moveTexture[LEFT][0] = LoadGraph("画像/ダークライl_1.png");
	d->moveTexture[LEFT][1] = LoadGraph("画像/ダークライl_2.png");
	d->moveTexture[RIGHT][0] = LoadGraph("画像/ダークライr_1.png");
	d->moveTexture[RIGHT][1] = LoadGraph("画像/ダークライr_2.png");

	d->skill[0].name = "でんこうせっか";
	d->skill[1].name = "だましうち";
	d->skill[2].name = "あくのはどう";
	d->skill[3].name = "れいとうビーム";

	d->skill[0].value = 2;
	d->skill[1].value = 7;
	d->skill[2].value = 10;
	d->skill[3].value = 3;


	d->skill[0].maxCount = 20;
	d->skill[1].maxCount = 10;
	d->skill[2].maxCount = 20;
	d->skill[3].maxCount = 10;

	d->skill[0].count = c->skill[0].maxCount;
	d->skill[1].count = c->skill[1].maxCount;
	d->skill[2].count = c->skill[2].maxCount;
	d->skill[3].count = c->skill[3].maxCount;


	d->maxHp = 70;
	d->hp = 70;

	/*白色の透過*/
	SetTransColor(255, 255, 255);
	/*メッセージボックス*/
	messageBox = LoadGraph("画像/messageBox.png");
	SetTransColor(255, 0, 0);
	/*わざ選択ボックス*/
	skillBox = LoadGraph("画像/skillBox.png");
	/*座標位置*/
	c->x = CHIP_SIZE * 3;
	c->y = CHIP_SIZE * 3;

	d->x = CHIP_SIZE * 10;
	d->y = CHIP_SIZE * 2;


	/*音声をロード*/
	slap = LoadSoundMem("音楽/slap1.mp3");
	bgm = LoadSoundMem("音楽/bgm.mp3");
	down = LoadSoundMem("音楽/down1.mp3");

}

/*attack(自分,敵,攻撃の種類)*/
void attack(pokemon* me, pokemon* enemy, int attackNum) {

	turnToPokemon(me, enemy);//敵の方を向く
	
	//ななめに居る時は攻撃しない
	if (!(((me->x - CHIP_SIZE == enemy->x) && (me->y + CHIP_SIZE == enemy->y || me->y - CHIP_SIZE == enemy->y)) || (me->x + CHIP_SIZE == enemy->x) && (me->y + CHIP_SIZE == enemy->y || me->y - CHIP_SIZE == enemy->y))) {
		if (me->skill[attackNum].count > 0) {

			/*確率で攻撃が外れる*/
			if (getRandom(1, 100) < 98) {
				enemy->hp -= me->skill[attackNum].value;
				if (enemy->hp < 0)enemy->hp = 0;//hpがマイナスになるのを防ぐ
				sprintf_s(s, "%sの%s! %sに%dのダメージ!%sのHP:%d", me->name, me->skill[attackNum].name, enemy->name, me->skill[attackNum].value, enemy->name, enemy->hp);
				PlaySoundMem(slap, DX_PLAYTYPE_BACK);
			}
			else {
				sprintf_s(s, "%sの攻撃は外れた!", me->name);
			}
		}
		else {
			sprintf_s(s, "%sはもう使えない!",me->skill[attackNum].name);
		}
		me->skill[attackNum].count -= 1;
		if (me->skill[attackNum].count < 0)me->skill[attackNum].count = 0;	//マイナスを防ぐ
		messageflag = true;
		setMessage(s);
		outMessage();
	}
}

/*攻撃時に敵の方を向く*/
/*のちに向かせる前に画像を敷いてダブリを無くす*/
/*もしくは向いている方向に攻撃させるのでこれは消すかも?*/
void turnToPokemon(pokemon* me, pokemon* enemy) {
	if (me->x > enemy->x)me->direction = LEFT;
	else if (me->x < enemy->x)me->direction = RIGHT;
	if (me->y > enemy->y)me->direction = UP;
	else if (me->y < enemy->y)me->direction = DOWN;
}

/*下のコンソール初期化*/
void initConsole() {

	/*メッセージフラグが立っていれば、メッセージボックス表示*/
	/*上書きしていく*/
	if (messageflag) {
		DrawRotaGraph(SCREEN_WIDTH / 2, 500, 1, 0, messageBox, true);
	}
}

/*表示するメッセージをセット*/
void setMessage(char s[]) {

	messageflag = true;

	char temp[256];
	/*初期化*/
	for (int i = 0; i < 256; i++)temp[i] = '\0';

	/*文字列コピー*/
	for (int i = 0; s[i] != '\0'; i++) { temp[i] = s[i]; }

	/*message1->上段、message2->中段、message3->下段*/
	if (message1[0] == '\0')for (int i = 0; temp[i] != '\0'; i++) { message1[i] = temp[i]; }
	else if (message2[0] == '\0')for (int i = 0; temp[i] != '\0'; i++) { message2[i] = temp[i]; }
	else if (message3[0] == '\0')for (int i = 0; temp[i] != '\0'; i++) { message3[i] = temp[i]; }
	/*全て埋まっていれば、内容を一段ずつ上げて表示する*/
	else {
		for (int i = 0; i < 256; i++) { message1[i] = '\0'; }
		for (int i = 0; i < 256; i++) { message1[i] = message2[i]; }

		for (int i = 0; i < 256; i++)message2[i] = '\0';
		for (int i = 0; i < 256; i++) { message2[i] = message3[i]; }

		for (int i = 0; i < 256; i++) { message3[i] = temp[i]; }
	}
}

/*メッセージ表示*/
void outMessage() {

	initConsole();
	/*コンソール表示*/
	DrawFormatString(100, 487, GetColor(255, 255, 255), "%s", message1);
	DrawFormatString(100, 517, GetColor(255, 255, 255), "%s", message2);
	DrawFormatString(100, 547, GetColor(255, 255, 255), "%s", message3);
}

/*ジャンプ処理*/
void moveJump(pokemon* me) {

	int f;
	/*ジャンプした瞬間*/
	if (onGround) {
		/*上への当たり判定*/
		if (me->y >= 140) {
			f = -30;
			y_temp = me->y;
			me->y += (me->y - y_prev) + f;
			y_prev = y_temp;
		}
		else {
			y_temp = 30;
			me->y = 30;
			y_prev = y_temp;
		}
	}
	/*空中の処理*/
	else {
		f = 3;
		y_temp = me->y;
		me->y += (me->y - y_prev) + f;
		y_prev = y_temp;
		/*元の位置に戻ればジャンプ処理終了*/
		if (me->y >= temp) {
			me->y = temp;
			jump = false;
		}
	}
	onGround = false;
}


/*周囲のマスに敵が居るか居ないか*/
/*攻撃時に使用する*/
bool isNearPokemon(pokemon* me, pokemon* enemy) {
	if ((me->x - CHIP_SIZE <= enemy->x) && (me->x + CHIP_SIZE >= enemy->x) && (me->y - CHIP_SIZE <= enemy->y) && (me->y + CHIP_SIZE >= enemy->y)) {
		return true;
	}
	return false;
}

/*同じマップ内にいるかどうか*/
bool findPokemon(pokemon* me, pokemon* enemy) {
	if ((me->x - SCREEN_WIDTH/2 < enemy->x) && (me->x + SCREEN_WIDTH/2 > enemy->x) && (me->y + SCREEN_HEIGHT/2 > enemy->y) && (me->y - SCREEN_HEIGHT/2 < enemy->y))return true;
	else return false;
}

/*敵の動き*/
void enemyMove(pokemon* enemy) {


	/*生きているか死んでいるか*/
	/*死亡確認*/
	if (enemy->hp == 0 && enemy->isLive) {
		PlaySoundMem(down, DX_PLAYTYPE_BACK);
		c->experience += enemy->experience;
		sprintf_s(s, 256, "%sは倒れた! 経験値%dを獲得!", enemy->name,enemy->experience);
		setMessage(s);
		outMessage();
		enemy->isLive = false;
	}

	/*敵が同じマップ内にいると、自分に向かってくる*/
	if (findPokemon(enemy, c)) {
		/*攻撃しない*/
		/*移動処理*/
		if (!isNearPokemon(enemy, c) && enemy->isLive) {
			if (c->x != enemy->x && c->x - CHIP_SIZE < enemy->x) {
				enemy->x -= CHIP_SIZE;
				enemy->direction = LEFT;
			}
			else if (c->x != enemy->x && c->x + CHIP_SIZE > enemy->x) {
				enemy->x += CHIP_SIZE;
				enemy->direction = RIGHT;
			}
			if (c->y != enemy->y && c->y - CHIP_SIZE < enemy->y) {
				enemy->y -= CHIP_SIZE;
				enemy->direction = UP;
			}
			else if (c->y != enemy->y && c->y + CHIP_SIZE > enemy->y) {
				enemy->y += CHIP_SIZE;
				enemy->direction = DOWN;
			}
		}
		/*攻撃する*/
		else if (c->hp > 0 && enemy->isLive) {
			attack(enemy, c, getRandom(0,3));		//ランダムでわざを選ぶ
		}
	}
	/*まだ自分が見つかっていない場合、うろうろする*/
	else {
		switch (getRandom(0,3)) {
		case 0:
			enemy->x -= CHIP_SIZE;
			enemy->direction = LEFT;
			break;
		case 1:
			enemy->x += CHIP_SIZE;
			enemy->direction = RIGHT;
			break;
		case 2:
			enemy->y -= CHIP_SIZE;
			enemy->direction = UP;
			break;
		case 3:
			enemy->y += CHIP_SIZE;
			enemy->direction = DOWN;
			break;
		}
	}

}

/*0～3の乱数を得る*/
int getRandom(int min,int max) {
	
	std::random_device rd;

	std::mt19937 mt(rd());

	std::uniform_int_distribution<int> dice(min, max);

	return dice(mt);
}
