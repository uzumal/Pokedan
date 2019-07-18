#include "DxLib.h"
#include "all.h"
#include <iostream>
#include <random>
#include <stdio.h>
#include <tchar.h>
#include <map>
#include <stack>
#include <stdio.h>

#define SCREEN_WIDTH     (800)                          // 画面の横幅
#define SCREEN_HEIGHT    (600)                          // 画面の縦幅
#define CHIP_SIZE        (40)                           // 一つのチップのサイズ
#define MAP_HEIGHT		 (400)
#define MAP_WIDTH		 (680)

#define GX (c->x/CHIP_SIZE - m->x)
#define GY (c->y/CHIP_SIZE - m->y)
#define SX (d->x/CHIP_SIZE - m->x)
#define SY (d->y/CHIP_SIZE - m->y)

#define WHITE	GetColor(255,255,255)
#define RED		GetColor(255,0,0)
#define GREEN	GetColor(0,255,0)
#define BLUE	GetColor(0,0,255)
#define BLACK	GetColor(0,0,0)

#define KEY(X,Y) ((X) * 100 + (Y))
#define KEYDATA(X, Y, N) std::pair<int, NODE>(KEY(X,Y), N)		//座標とノードをペアにする

#define ENEMYNUM 3

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
pokemon dark2;
pokemon dark3;
pokemon* c = &poke;
//pokemon* d = &dark;
pokemon* enemy[ENEMYNUM] = {&dark,&dark2,&dark3};

/*マップ構造体*/
maps mp;
maps* m = &mp;


/*音声ファイルメモリ用配列*/
int slap;
int bgm;
int down;

/*画像ファイルメモリ用配列*/
int messageBox;
int skillBox;
int stairs_down;
int stairs_up;
int load;
int load2;



/*関数プロトタイプ宣言*/
int init();										//初期化
int getRandom(int,int);							//int max～int minの範囲で乱数を取得

void wait(int);									//int時間待つ
void wait(int,char* s);							//int時間待つ、中心に文字表示
void w_press();									//Kボタン押したら進


//主に自分用
void attack(pokemon*, pokemon*);			//攻撃する
void attack_for(pokemon* me);	//プレイヤー用攻撃
void skillfull(int experience);					//技威力設定
void turnToPokemon(pokemon*, pokemon*);			//ポケモンの方を向く
void moveJump(pokemon*);						//Jumpする


//主に敵用
void enemyMove(pokemon*);					//敵の動き
bool isNearPokemon(pokemon*, pokemon*);			//敵が近くにいたら(攻撃圏内にいたら)true
bool isNearPokemon2(pokemon*, pokemon*);			//敵が近くにいたら(攻撃圏内にいたら)true
bool findPokemon(pokemon*, pokemon*);			//敵が発見できる近さにいたら
bool life(pokemon* enemy, pokemon* me);			//敵死んでいるかどうか
//A*アルゴリズム敵追尾
int h(NODE*, NODE*);		//ヒューリスティック関数値を返す、マンハッタン距離
void setNode(NODE* child, int x, int y, NODE* parent, int cost, int f);		//ノードの情報を一気に設定する
int getCost();				//移動コストを返す、この場合は1を返す
NODE* Astar(pokemon*);			//A*アルゴリズム適用後、次に進むセルのノードを返す


//メッセージ系
void initConsole();								//メッセージボックスを初期化する
void setMessage(char[]);						//表示したいメッセージをセットする
void outMessage();								//メッセージを表示する
void initMessage();

/*移動*/
void charaMove(pokemon*, int, int);				//キャラ移動、敵用
void charaMove(pokemon*,pokemon*[ENEMYNUM], int, int);		//キャラ移動、自分用　入れ替わり考慮
void mapMove(maps*,pokemon*,pokemon*[ENEMYNUM], int, int);	//マップ移動、引っ掛かり考慮
void setDirection(pokemon*, int);						//キャラの向いている方向をセットする

void drawMap();
void drawMiniMap();
void drawCharacter(pokemon*,int d_num);
void drawMenu();

pokemon* getAttackEnemy();

bool isPutMoveKey();

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

/******************************************************************************/

/*
void Screen() {
	while () {
		sprintf_s(s, "ようこそポケモンの世界へ");

		messageflag = true;
		setMessage(s);
		outMessage();
	}
}*/

void w_press() {
	while (getCountFrame() == 0) {
		if (keyState[KEY_INPUT_K] == 1) {
			break;
		}
	}
}


/***********************      Mainの処理      *********************************/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	/*初期化処理*/
	if (init() == -1) { return -1; }

	int d_num = 0;
	int tempTime = 0;

	m->floor = 0;
	int tmp_mx = m->x;
	int tmp_my = m->y;
	
	/*bgm再生開始*/
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);

	pokemon* tmp = new pokemon;

	/*描画する*/
	// while(裏画面を表画面に反映, メッセージ処理, 画面クリア,フレームカウント)
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && getCountFrame() == 0) {

		tmp_mx = m->x;
		tmp_my = m->y;

		drawMap();
		drawMiniMap();

		/*500ms(0.5秒)に一度画像更新(歩いているように見える)*/
		if (GetNowCount() - tempTime> 500) 
		{	
			if (d_num == 1)d_num = 0;
			else d_num = 1;
			tempTime = GetNowCount(); 
		}

		/*階層、level、HP表示(固定)*/
		DrawFormatString(0	, 0, WHITE, "B%dF",m->floor + 1);
		DrawFormatString(50	, 0, WHITE, "Lv: %d",c->level);
		DrawFormatString(120, 0, WHITE, "HP: %d/ %d",c->hp,c->maxHp);
		DrawFormatString(220, 0, WHITE, "セットわざ名 : %s",c->skill[c->attackNum].name);

		/*確認用座標(あとで消す)*/
		DrawFormatString(500, 0,  WHITE, "ピカ座標(%d,%d)", c->x + 20 - m->x * CHIP_SIZE, c->y + 20 - m->y * CHIP_SIZE);
		DrawFormatString(500, 20, WHITE, "ダー座標(%d,%d)", enemy[0]->x + 20 - m->x * CHIP_SIZE, enemy[0]->y + 20 - m->y * CHIP_SIZE);
		DrawFormatString(500, 40, WHITE, "マップ座標(%d,%d)", m->x, m->y);


		/*階段移動処理*/
		if (mapping[m->floor][GY][GX] == 100) {
			m->floor += 1;
			wait(2000, (char*)(m->floor + 1));
		}

		if (mapping[m->floor][GY][GX] == 101) {
			m->floor -= 1;
			wait(2000, (char*)(m->floor + 1));
		}


		drawCharacter(c, d_num);
		
		for (int i = 0; i < ENEMYNUM; i++) {
			drawCharacter(enemy[i], d_num);
		}

		/*メッセージ出力*/
		outMessage();

		/*spaceキーで終わり*/
		if (keyState[KEY_INPUT_SPACE] == 1) { endflag = true; }
		
		/*わざ表を消去*/
		if (menuflag && keyState[KEY_INPUT_I] == 1) {
			menuflag = false;
		}

		/*わざ表を表示*/
		else if (!menuflag && keyState[KEY_INPUT_I] == 1) {
			menuflag = true;
		}

		if (menuflag) {
			drawMenu();
		}
		

		/*Attack*/
		if (!menuflag && keyState[KEY_INPUT_J] == 1) {

			tmp = getAttackEnemy();
			if (tmp != NULL)attack(c, tmp);
			else attack_for(c);

			for (int i = 0; i < ENEMYNUM; i++) {
				if (life(enemy[i], c) == FALSE) {
					enemyMove(enemy[i]);
				}
			}
		}
		/*Right*/
		if (!menuflag && keyState[KEY_INPUT_D]==1) {
			charaMove(c,enemy,1,0);
		}
		/*Left*/
		else if (!menuflag && keyState[KEY_INPUT_A]==1) { 
			charaMove(c,enemy, -1, 0);
		}
		/*Up*/
		else if (!menuflag && keyState[KEY_INPUT_W]==1) {
			charaMove(c,enemy, 0, -1);
		}
		/*Down*/
		else if (!menuflag && keyState[KEY_INPUT_X]==1) {
			charaMove(c,enemy, 0, 1);
		}
		/*RightUp*/
		else if (!menuflag && keyState[KEY_INPUT_E] == 1) {
			charaMove(c, enemy, 1, -1);
		}
		/*RightDown*/
		else if (!menuflag && keyState[KEY_INPUT_C] == 1) {
			charaMove(c, enemy, 1, 1);
		}
		/*LeftUp*/
		else if (!menuflag && keyState[KEY_INPUT_Q] == 1) {
			charaMove(c, enemy, -1, -1);
		}
		/*LeftDown*/
		else if (!menuflag && keyState[KEY_INPUT_Z] == 1) {
			charaMove(c, enemy, -1, 1);
		}

		
		if (!menuflag && isPutMoveKey() && tmp_mx == m->x && tmp_my == m->y) {
			if (keyState[KEY_INPUT_Y] == 0 && keyState[KEY_INPUT_I] == 0) {
				for (int i = 0; i < ENEMYNUM; i++) {
					enemyMove(enemy[i]);
				}
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


		if (endflag) {
			break;
		}
	}

	DxLib::DxLib_End();    // DXライブラリ終了処理
	
	return 0;
}


void wait(int ms,char* s) {
	int tmp = GetNowCount();
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && GetNowCount() - tmp < ms) {
		DrawFormatString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, GetColor(255,255,255), "B%dF", s);
	}
}

void wait(int ms) {
	int tmp = GetNowCount();
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && GetNowCount() - tmp < ms) {
		
	}
}

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

	/*後々自分の名前を入力させる*/
	c->name = "ピカチュウ";
	enemy[0]->name = "ダークライ";
	enemy[1]->name = "ダークライ";
	enemy[2]->name = "ダークライ";

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

	skillfull(0);

	for (int i = 0; i < ENEMYNUM; i++) {
		enemy[i]->moveTexture[DOWN][0] = LoadGraph("画像/ダークライd_1.png");
		enemy[i]->moveTexture[DOWN][1] = LoadGraph("画像/ダークライd_2.png");
		enemy[i]->moveTexture[UP][0] = LoadGraph("画像/ダークライu_1.png");
		enemy[i]->moveTexture[UP][1] = LoadGraph("画像/ダークライu_2.png");
		enemy[i]->moveTexture[LEFT][0] = LoadGraph("画像/ダークライl_1.png");
		enemy[i]->moveTexture[LEFT][1] = LoadGraph("画像/ダークライl_2.png");
		enemy[i]->moveTexture[RIGHT][0] = LoadGraph("画像/ダークライr_1.png");
		enemy[i]->moveTexture[RIGHT][1] = LoadGraph("画像/ダークライr_2.png");

		enemy[i]->skill[0].name = "でんこうせっか";
		enemy[i]->skill[1].name = "だましうち";
		enemy[i]->skill[2].name = "あくのはどう";
		enemy[i]->skill[3].name = "れいとうビーム";

		enemy[i]->skill[0].value = 2;
		enemy[i]->skill[1].value = 7;
		enemy[i]->skill[2].value = 10;
		enemy[i]->skill[3].value = 3;


		enemy[i]->skill[0].maxCount = 20;
		enemy[i]->skill[1].maxCount = 10;
		enemy[i]->skill[2].maxCount = 20;
		enemy[i]->skill[3].maxCount = 10;

		enemy[i]->skill[0].count = c->skill[0].maxCount;
		enemy[i]->skill[1].count = c->skill[1].maxCount;
		enemy[i]->skill[2].count = c->skill[2].maxCount;
		enemy[i]->skill[3].count = c->skill[3].maxCount;

		enemy[i]->voice = LoadSoundMem("音楽/ダークライvoice.mp3");

		enemy[i]->experience = 10000;

		enemy[i]->maxHp = 70;
		enemy[i]->hp = 70;
	}
	/*階段の画像*/
	stairs_down = LoadGraph("画像/stairs.png");
	stairs_up = LoadGraph("画像/stairs2.png");

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
	
	bool flag = 0;
	for (int i = 0; i < ENEMYNUM; i++) {
		while (true) {
			flag = true;
			enemy[i]->x = getRandom(3, 27) * CHIP_SIZE;
			enemy[i]->y = getRandom(3, 27) * CHIP_SIZE;
			if (mapping[0][enemy[i]->y/CHIP_SIZE][enemy[i]->x/CHIP_SIZE] > 0) {
				//被りを無くす
				for (int j = 0; j < i; j++) {
					if (enemy[i]->x == enemy[j]->x && enemy[i]->y == enemy[j]->y)flag = false;
				}
				if (flag)break;
			}
		}
	}



	/*音声をロード*/
	slap = LoadSoundMem("音楽/slap1.mp3");
	bgm = LoadSoundMem("音楽/bgm.mp3");
	down = LoadSoundMem("音楽/down1.mp3");

	load = LoadGraph("画像/load.png");
	load2 = LoadGraph("画像/load2.png");

	return 0;
}

//技威力設定
void skillfull(int level) {
	c->skill[0].min = 2 + level * 2;
	c->skill[0].max = 4 + level * 2;
	c->skill[1].min = 5 + level * 2;
	c->skill[1].max = 8 + level * 2;
	c->skill[2].min = 8 + level * 2;
	c->skill[2].max = 14 + level * 2;
	c->skill[3].min = 1 + level * 2;
	c->skill[3].max = 3 + level * 2;

	for (int i = 0; i < ENEMYNUM; i++) {

		enemy[i]->skill[0].min = 2;
		enemy[i]->skill[0].max = 4;
		enemy[i]->skill[1].min = 3;
		enemy[i]->skill[1].max = 6;
		enemy[i]->skill[2].min = 8;
		enemy[i]->skill[2].max = 14;
		enemy[i]->skill[3].min = 4;
		enemy[i]->skill[3].max = 8;
	}
}


/*attack(自分,敵,攻撃の種類)*/
void attack(pokemon* me, pokemon* enemy) {

	//turnToPokemon(me, enemy);//敵の方を向く
	int value = getRandom(me->skill[me->attackNum].min, me->skill[me->attackNum].max);	//わざを出すたびに乱数生成
	if (me->skill[me->attackNum].count > 0) {
		//敵の方を向いている場合
		if (enemy->isLive && isNearPokemon(me,enemy)&&((me->y == enemy->y &&  ((me->x > enemy->x && me->direction == LEFT)||(me->x < enemy->x && me->direction == RIGHT)))||(me->x == enemy->x && ((me->y > enemy->y && me->direction==UP|| me->y<enemy->y && me->direction==DOWN))))) {
			/*2%の確率で攻撃が外れる*/
			if (getRandom(1, 100) < 98) {
				enemy->hp -= value;
				if (enemy->hp < 0)enemy->hp = 0;//hpがマイナスになるのを防ぐ
				sprintf_s(s, "%sの%s! %sに%dのダメージ!%sのHP:%d", me->name, me->skill[me->attackNum].name, enemy->name, value, enemy->name, enemy->hp);
				PlaySoundMem(slap, DX_PLAYTYPE_BACK);
			}
			else {
				sprintf_s(s, "%sの%s!しかし攻撃は外れた", me->name, me->skill[me->attackNum].name);
				PlaySoundMem(slap, DX_PLAYTYPE_BACK);
			}
		}//敵の方を向いていない場合
		else {
			/*攻撃が外れる*/
			sprintf_s(s, "%sの%s!しかし攻撃は外れた", me->name, me->skill[me->attackNum].name);
			PlaySoundMem(slap, DX_PLAYTYPE_BACK);
		}
	}
	else {
		sprintf_s(s, "%sはもう使えない!", me->skill[me->attackNum].name);
	}
	me->skill[me->attackNum].count -= 1;
	if (me->skill[me->attackNum].count < 0)me->skill[me->attackNum].count = 0;	//マイナスを防ぐ
	messageflag = true;
	setMessage(s);
	outMessage();
	
}

//プレイヤー攻撃用,敵が周りにおらず攻撃が必ず外れる場合
void attack_for(pokemon* me) {

	if (me->skill[me->attackNum].count > 0) {

		/*攻撃が外れる*/
		sprintf_s(s, "%sの%s!しかし攻撃は外れた", me->name, me->skill[me->attackNum].name);
		PlaySoundMem(slap, DX_PLAYTYPE_BACK);
	}
	else {
		sprintf_s(s, "%sはもう使えない!", me->skill[me->attackNum].name);
	}
	me->skill[me->attackNum].count -= 1;
	if (me->skill[me->attackNum].count < 0)me->skill[me->attackNum].count = 0;	//マイナスを防ぐ
	messageflag = true;
	setMessage(s);
	outMessage();
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
/*ななめを抜く*/
/*4方ver*/
bool isNearPokemon(pokemon* me, pokemon* enemy) {
	if ((me->y == enemy->y && (me->x - CHIP_SIZE == enemy->x || me->x + CHIP_SIZE == enemy->x)) || (me->x == enemy->x && (me->y - CHIP_SIZE == enemy->y || me->y + CHIP_SIZE == enemy->y))) {
		return true;
	}
	return false;
}

/*8方ver*/
bool isNearPokemon2(pokemon* me, pokemon* enemy) {
	if (me->x - CHIP_SIZE <= enemy->x && me->x + CHIP_SIZE >= enemy->x && me->y - CHIP_SIZE <= enemy->y && me->y + CHIP_SIZE >= enemy->y) {
		return true;
	}
	return false;
}

/*同じマップ内にいるかどうか*/
bool findPokemon(pokemon* me, pokemon* enemy) {
	if ((me->x - SCREEN_WIDTH/2 < enemy->x) && (me->x + SCREEN_WIDTH/2 > enemy->x) && (me->y + SCREEN_HEIGHT/2 > enemy->y) && (me->y - SCREEN_HEIGHT/2 < enemy->y))return true;
	else return false;
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
		while (me->Max_ex < enemy->experience) {
			enemy->experience = enemy->experience - me->Max_ex;
			me->level++;
			me->Max_ex += 30;
			sprintf_s(s, "%sは%dレベルに上がった！", me->name, me->level);
			setMessage(s);
			outMessage();
			wait(400);
			//w_press();
		}
		enemy->isLive = false;
		initMessage();
		return TRUE;
	}
	return FALSE;
}


/*敵の動き*/
void enemyMove(pokemon* enemy) {

	//死んでいたら何もしない
	if (!enemy->isLive)return;

	//自分の座標取得
	int sx = enemy->x / CHIP_SIZE - m->x;
	int sy = enemy->y / CHIP_SIZE - m->y;

	/*敵が同じマップ内にいると、自分に向かってくる*/
	if (findPokemon(enemy, c)) {

		/*4方にいるとき、攻撃する*/
		if (isNearPokemon(enemy, c)) {
			turnToPokemon(enemy, c);
			enemy->attackNum = getRandom(0, 3);
			attack(enemy, c);		//ランダムでわざを選ぶ
		}
		/*8方(ここではななめ)にいるとき、攻撃できる範囲に移動する*/
		else if (isNearPokemon2(enemy, c)) {
			charaMove(enemy, (c->x - enemy->x)/CHIP_SIZE, (c->y - enemy->y)/CHIP_SIZE);	//攻撃できるところに移動する
		}
		/*8方にいないときはA*アルゴリズム*/
		else if (!isNearPokemon2(enemy, c)) {
			NODE* nextEnemy = Astar(enemy);
			charaMove(enemy,nextEnemy->x - sx,nextEnemy->y - sy);
		}
	}
	/*まだ対象が見つかっていない場合、うろうろする*/
	else {
		switch (getRandom(0,8)) {
		case LEFT:
			charaMove(enemy, -1, 0);
			break;
		case RIGHT:
			charaMove(enemy, 1, 0);
			break;
		case UP:
			charaMove(enemy, 0, -1);
			break;
		case DOWN:
			charaMove(enemy, 0, 1);
			break;
		case LEFT_UP:
			charaMove(enemy, -1, -1);
			break;
		case LEFT_DOWN:
			charaMove(enemy, -1, 1);
			break;
		case RIGHT_UP:
			charaMove(enemy, 1, -1);
			break;
		case RIGHT_DOWN:
			charaMove(enemy, 1, 1);
			break;
		default:
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

/*自分用の処理*/
void charaMove(pokemon* me,pokemon* enemy[ENEMYNUM], int x, int y) {

	//向いている方向をセット
	if (x == 1)setDirection(me,RIGHT);
	else if(x == -1)setDirection(me,LEFT);

	if (y == 1)setDirection(me,DOWN);
	else if(y == -1)setDirection(me,UP);

	//マップ移動用
	int mx = 0;
	int my = 0;

	/*移動先のセル確認*/
	int nextCell = mapping[m->floor][me->y / CHIP_SIZE - m->y + y][me->x  / CHIP_SIZE - m->x + x];

	/*ななめ移動のとき*/
	if ((x == 1 || x == -1) && (y == 1 || y == -1)) {

		int nextCell_x = mapping[m->floor][me->y / CHIP_SIZE - m->y][me->x / CHIP_SIZE - m->x + x];	//自分の左右どちらかのセルの状態
		int nextCell_y = mapping[m->floor][me->y / CHIP_SIZE - m->y + y][me->x / CHIP_SIZE - m->x];	//自分の上下どちらかのセルの状態

		//進む先が障害物で、左右どちらか、上下どちらかが道であれば滑る
		if (nextCell <= 0 && !keyState[KEY_INPUT_Y]) {
			if (nextCell_x > 0 && nextCell_y <=0)y = 0;
			if (nextCell_y > 0 && nextCell_x <=0)x = 0;
		}
	}

	//マップの端であればマップ移動
	if (me->x == CHIP_SIZE * 2 && x == -1 || me->x == MAP_WIDTH && x == 1) {
		mx = x * -1;
		x = 0;
	}
	if (me->y == CHIP_SIZE * 2 && y == -1 || me->y == MAP_HEIGHT && y == 1) {
		my = y * -1;
		y = 0;
	}

	//nextCellの更新
	nextCell = mapping[m->floor][me->y / CHIP_SIZE - m->y + y][me->x / CHIP_SIZE - m->x + x];

	if (nextCell > 0 && !keyState[KEY_INPUT_Y]) {
		/*移動先に敵がいたら入れ替わる*/
		for (int i = 0; i < ENEMYNUM; i++) {
			if (me->x + x * CHIP_SIZE == enemy[i]->x && me->y + y * CHIP_SIZE == enemy[i]->y) { enemy[i]->x = me->x; enemy[i]->y = me->y; }
		}
			me->x += x * CHIP_SIZE;
			me->y += y * CHIP_SIZE;
	}

	//マップ移動があれば
	if (!(mx == 0 && my == 0)) {
		mapMove(m, me, enemy, mx, my);
	}
}

/*敵用の処理,入れ替わり処理を無くした*/
void charaMove(pokemon* me, int x, int y) {

	if (x == 1)setDirection(me, RIGHT);
	else if (x == -1)setDirection(me, LEFT);

	if (y == 1)setDirection(me, DOWN);
	else if (y == -1)setDirection(me, UP);

	int tmpX = (me->x - c->x)/CHIP_SIZE;
	int tmpY = (me->y - c->y)/CHIP_SIZE;
	int nextCell_x = mapping[m->floor][me->y / CHIP_SIZE - m->y][me->x / CHIP_SIZE - m->x + x];
	int nextCell_y = mapping[m->floor][me->y / CHIP_SIZE - m->y + y][me->x / CHIP_SIZE - m->x];

	//ななめ移動時に、ななめに主人公がいれば
	if ((x == 1 || x == -1) && (y == 1 || y == -1)) {
		if ((tmpX == 1 || tmpX == -1) && (tmpY == 1 || tmpY == -1)) {
			//攻撃できる範囲に移動する
			if (nextCell_x > 0)y = 0;
			else if (nextCell_y > 0)x = 0;
		}
	}
	/*移動先のセル確認*/
	int nextCell = mapping[m->floor][me->y / CHIP_SIZE - m->y + y][me->x / CHIP_SIZE - m->x + x];

	if (nextCell > 0) {
		me->x += x * CHIP_SIZE;
		me->y += y * CHIP_SIZE;
	}
}

void mapMove(maps* m,pokemon* me,pokemon* enemy[ENEMYNUM], int x, int y) {

	int nextCell = mapping[m->floor][me->y / CHIP_SIZE - m->y - y][me->x  / CHIP_SIZE - m->x - x];

	//移動処理と引っ掛かり処理
	if (nextCell > 0 && !keyState[KEY_INPUT_Y]) {
		m->x += x;
		m->y += y;
		for (int i = 0; i < ENEMYNUM; i++) {
			int enemyCell = mapping[m->floor][enemy[i]->y / CHIP_SIZE - m->y][enemy[i]->x / CHIP_SIZE - m->x];
			if (enemyCell <= 0)charaMove(enemy[i], x, y);
		}
	}

}

void setDirection(pokemon* me, int direction) {
	me->direction = direction;
}


void initMessage() {
	messageflag = false;
	for (int i = 0; i < 256; i++) {
		message1[i] = '\0';
		message2[i] = '\0';
		message3[i] = '\0';
	}
}


void setNode(NODE* child, int x, int y, NODE* parent, int cost,int f) {
	child->x = x;
	child->y = y;
	child->parent = parent;
	child->cost = cost;
	child-> f = f;
}

int getCost() {
	return 1;
}


int g(NODE* s, NODE* n) {
	return n->cost;
}

//ヒューリスティック関数
//マンハッタン距離
int h(NODE* e, NODE* n) {
	//終点から見た現在のノードまでの推定コストを計算
	int cx = e->x - n->x;
	int cy = e->y - n->y;
	if (cx < 0)cx *= -1;
	if (cy < 0)cy *= -1;

	if (cx < cy) {
		return (cx + cy);
	}
	else {
		return (cy + cx);
	}
}

NODE* Astar(pokemon* enemy) {

	//結果保持用スタック
	std::stack<NODE*> st;
	std::map<int, NODE> openList;
	std::map<int, NODE>closedList;
	int loop = 0;
	NODE s = { 0,0,0 };
	NODE e = { 0,0,0 };

	
	int key[8][2] = {
		{1,0},
		{-1,0},
		{0,-1},
		{0,1},
		{1,1},
		{-1,1},
		{1,-1},
		{-1,-1},
	};

	//open.index = 0;
	//close.index = 0;

	//スタート
	//敵の座標
	s.x = (enemy->x / CHIP_SIZE - m->x);
	s.y = (enemy->y / CHIP_SIZE - m->y);

	//親ノード、コスト、評価値なし
	setNode(&s, s.x, s.y, NULL, 0, 0);
	//open.node[open.index++] = &s;
	//オープンリストに代入
	openList.insert(KEYDATA(s.x,s.y,s));

	//ゴール
	//自分の座標
	e.x = (c->x / CHIP_SIZE - m->x);
	e.y = (c->y / CHIP_SIZE - m->y);

	while (true) {

		

		// openリストがなくなったので終了する
		if (openList.empty()) { break; }
		
		int f_min = 999;

		//展開用
		NODE *p = new NODE;

		auto it = openList.begin();		//先頭イテレータ取得
		std::map<int, NODE>::iterator it_min;
		while (it != openList.end()) {
			if (f_min > it->second.f) {
				f_min = it->second.f;
				it_min = it;
			}
			it++;
		}
		
		setNode(
			p,
			it_min->second.x, it_min->second.y,
			it_min->second.parent,
			it_min->second.cost, it_min->second.f
		);
		

		//親ノードをオープンリストから削除
		openList.erase(KEY(p->x, p->y));
		//親ノードをクローズドリストに挿入
		closedList.insert(KEYDATA(p->x, p->y, *p));

		// もしゴールならただちに終了する
		if (p->y == e.y && p->x == e.x) {

			st.push(p);
			p = p->parent;
			while (p->parent != NULL) {
				st.push(p);
				p = p->parent;
			}
			
			break;
		}

		//子ノード展開
		for (int i = 0; i < 8; i++) {
			
			//子ノード評価用
			NODE* child = new NODE;
			
			
			//周囲8マスを計算
			int cx = p->x + key[i][0];
			int cy = p->y + key[i][1];
			
			//0以下は通れないので飛ばす
			if (mapping[m->floor][cy][cx] <= 0)continue;

			//ノードの実体はほぼ座標
			child->x = cx;
			child->y = cy;
			
			//仮のコスト、評価値計算
			int cost = p->cost + getCost();
			int f = cost + h(&e, child);
			
			//最小コストの親のはずなので、先に親を入れてしまう
			setNode(child, cx, cy, p, cost, f);

			//オープンリスト
			if (openList.count(KEY(cx, cy))) {
				auto open_child = openList.find(KEY(cx, cy));
				//評価値が小さい方の情報を使用
				if (child->f < open_child->second.f) {
					openList[KEY(cx,cy)].cost = child->cost;
					openList[KEY(cx,cy)].f	  = child->f;
					openList[KEY(cx,cy)].parent = child->parent;
				}
			}
			//クローズドリスト
			else if (closedList.count(KEY(cx, cy))) {
				auto closed_child = closedList.find(KEY(cx, cy));
				//評価値が小さい方の情報を使用
				if (child->f < closed_child->second.f) {
					closed_child->second.cost = child->cost;
					closed_child->second.f = child->f;
					closed_child->second.parent = child->parent;
				
					//オープンリストに戻す
					openList.insert(KEYDATA(cx,cy,closed_child->second));
					closedList.erase(KEY(cx, cy));
				}

			}
			else {//オープンリストにもクローズドリストにも含まれていない
				openList.insert(KEYDATA(cx, cy, *child));
			}


			
		}

		if (loop++ > 1000) { break; }
		
	}
	
	NODE* node = st.top();

	
	return node;
}

void drawMap() {
	/*マップ全範囲走査*/
	for (int x = m->x; x < SCREEN_WIDTH / CHIP_SIZE - m->x; x++) {
		for (int y = m->y; y < SCREEN_HEIGHT / CHIP_SIZE - m->y; y++) {
			if (mapping[m->floor][y][x] == 0) { DrawFormatString((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, WHITE, " "); }
			else if (mapping[m->floor][y][x] == 1) { DrawRotaGraph((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, 1, 0, load, true); }
			else if (mapping[m->floor][y][x] == 2) { DrawRotaGraph((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, 1, 0, load2, true); }
			else if (mapping[m->floor][y][x] == 3) { DrawFormatString((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, GetColor(0, 0, 255), "B"); }
			else if (mapping[m->floor][y][x] == 5) { DrawFormatString((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, GetColor(0, 0, 255), "*"); }
			else if (mapping[m->floor][y][x] == 100) { DrawRotaGraph((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, 1, 0, stairs_down, true); }
			else if (mapping[m->floor][y][x] == 101) { DrawRotaGraph((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, 1, 0, stairs_up, true); }
		}
	}
}

void drawCharacter(pokemon* me, int d_num) {
	//生きていれば表示
	if (me->isLive) {
	/*座標は画像の真ん中に持つ*/
	/*DrawRotaGraph(x座標,y座標,縮尺度,角度,描画する画像ハンドル,背景透過処理のON,OFF)*/
		DrawRotaGraph(me->x + 20, me->y + 20, 1.5, 0, me->moveTexture[me->direction][d_num], true);
	}
}

void drawMiniMap() {
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			if (mapping[m->floor][j][i] == 1 || mapping[m->floor][j][i] == 2)DrawFormatString(i * 3, j * 3, WHITE, ".");
			if (i == GX && j == GY)DrawFormatString(i * 3, j * 3, RED, ".");
			for (int k = 0; k < ENEMYNUM; k++) {
				if (i == enemy[k]->x/CHIP_SIZE - m->x && j == enemy[k]->y/CHIP_SIZE - m->y && enemy[k]->isLive)DrawFormatString(i * 3, j * 3, BLUE, ".");
			}
			if (mapping[m->floor][j][i] == 100)DrawFormatString(i * 3, j * 3, GREEN, ".");
			if (mapping[m->floor][j][i] == 101)DrawFormatString(i * 3, j * 3, GREEN, ".");
		}
	}
}

void drawMenu() {
	DrawRotaGraph(550, 160, 1, 0, skillBox, true);
	DrawFormatString(530, 98, WHITE, "%s (%d/%d)", c->skill[0].name, c->skill[0].count, c->skill[0].maxCount);
	DrawFormatString(530, 149, WHITE, "%s (%d/%d)", c->skill[1].name, c->skill[1].count, c->skill[1].maxCount);
	DrawFormatString(530, 199, WHITE, "%s (%d/%d)", c->skill[2].name, c->skill[2].count, c->skill[2].maxCount);
	DrawFormatString(530, 250, WHITE, "%s (%d/%d)", c->skill[3].name, c->skill[3].count, c->skill[3].maxCount);
	if (keyState[KEY_INPUT_1]) { c->attackNum = 0; menuflag = false; }
	else if (keyState[KEY_INPUT_2]) { c->attackNum = 1; menuflag = false; }
	else if (keyState[KEY_INPUT_3]) { c->attackNum = 2; menuflag = false; }
	else if (keyState[KEY_INPUT_4]) { c->attackNum = 3; menuflag = false; }
}

bool isPutMoveKey() {
	
	if (keyState[KEY_INPUT_Q] == 1 || keyState[KEY_INPUT_W] == 1 || keyState[KEY_INPUT_E] == 1 || keyState[KEY_INPUT_D] == 1 || keyState[KEY_INPUT_C] == 1 || keyState[KEY_INPUT_X] == 1 || keyState[KEY_INPUT_Z] == 1 || keyState[KEY_INPUT_A] == 1) {
		return true;
	}
	else {
		return false;
	}

}

pokemon* getAttackEnemy() {
	
	for (int i = 0; i < ENEMYNUM; i++) {

		if (c->direction == UP) {
			if (c->x == enemy[i]->x && c->y - CHIP_SIZE == enemy[i]->y) {
				if (enemy[i]->isLive) {
					return enemy[i];
				}
			}
		}
		else if (c->direction == RIGHT) {
			if (c->y == enemy[i]->y && c->x + CHIP_SIZE == enemy[i]->x) {
				if (enemy[i]->isLive) {
					return enemy[i];
				}
			}
		}
		else if (c->direction == DOWN) {
			if (c->x == enemy[i]->x && c->y + CHIP_SIZE == enemy[i]->y) {
				if (enemy[i]->isLive) {
					return enemy[i];
				}
			}
		}
		else if (c->direction == LEFT) {
			if (c->y == enemy[i]->y && c->x - CHIP_SIZE == enemy[i]->x) {
				if (enemy[i]->isLive) {
					return enemy[i];
				}
			}
		}
	}
	return NULL;
}