#include "DxLib.h"
#include "pokemon.h"
#include <iostream>
#include <random>

#define SCREEN_WIDTH     (800)                          // 画面の横幅
#define SCREEN_HEIGHT    (600)                          // 画面の縦幅
#define CHIP_SIZE        (40)                           // 一つのチップのサイズ
#define MAP_HEIGHT		 (400)
#define MAP_WIDTH		 (680)
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

map mp;
map* m = &mp;

/*音声ファイルメモリ用配列*/
int slap;
int bgm;
int down;

/*画像ファイルメモリ用配列*/
int messageBox;
int skillBox;
int stairs_down;
int stairs_up;

/*プロトタイプ宣言*/
int init();
void w_press();										//Kボタン押したら進
void attack(pokemon*, pokemon*, int);			//攻撃
void attack_for(pokemon* me, int attackNum);	//プレイヤー用攻撃
void skillfull(int experience);					//技威力
void turnToPokemon(pokemon*, pokemon*);			//ポケモンの方を向く
void moveJump(pokemon*);						//Jumpする
void initConsole();								//メッセージボックスを初期化する
void setMessage(char[]);						//表示したいメッセージをセットする
void outMessage();								//メッセージを表示する
bool isNearPokemon(pokemon*, pokemon*);			//敵が近くにいたら(攻撃圏内にいたら)true
bool findPokemon(pokemon*, pokemon*);
bool life(pokemon* enemy, pokemon* me);						//敵死んでいるかどうか
void enemyMove(pokemon*,int);						//敵の動き
int getRandom(int,int);
void wait(int,char* s);
void wait(int);

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
	int floor = 0;
	int nearCell[9];
	int enemyCell = 0;

	/*白色を格納*/
	const int white = GetColor(255, 255, 255);

	/*bgm再生開始*/
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);

	/*描画する*/
	// while(裏画面を表画面に反映, メッセージ処理, 画面クリア,フレームカウント)
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && getCountFrame() == 0) {

		for (int x = m->x; x < SCREEN_WIDTH/CHIP_SIZE - m->x; x++) {
			for (int y = m->y; y < SCREEN_HEIGHT/CHIP_SIZE - m->y; y++) {
				if (mapping[floor][y][x] == 0)		 { DrawFormatString((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, white, "0"); }//DrawRotaGraph(x*CHIP_SIZE + 20, y*CHIP_SIZE + 20, 1.5,0,c->moveTexture[0][0],true); }
				else if (mapping[floor][y][x] == 1)  { DrawFormatString((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, GetColor(255,0,0), "1"); }
				else if (mapping[floor][y][x] == 2)  { DrawRotaGraph((m->x + x) * CHIP_SIZE + 20   , (m->y + y) * CHIP_SIZE + 20, 1.5,0, d->moveTexture[0][0],true); }
				else if (mapping[floor][y][x] == 3)  { DrawFormatString((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, GetColor(0, 0, 255), "B"); }
				else if (mapping[floor][y][x] == 5)  { DrawFormatString((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, GetColor(0,0,255), "*"); }
				else if (mapping[floor][y][x] == 100){ DrawRotaGraph((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, 1, 0, stairs_down, true);}
				else if (mapping[floor][y][x] == 101){ DrawRotaGraph((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, 1, 0, stairs_up, true);}
			}
		}

		/*500ms(0.5秒)に一度画像更新(歩いているように見える)*/
		if (GetNowCount() - tempTime> 500) 
		{	
			if (d_num == 1)d_num = 0;
			else d_num = 1;
			tempTime = GetNowCount(); 
		}

		/*階層、level、HP表示(固定)*/
		DrawFormatString(0	, 0, white, "B%dF",floor + 1);
		DrawFormatString(50	, 0, white, "Lv: %d",c->level);
		DrawFormatString(120, 0, white, "HP: %d/ %d",c->hp,c->maxHp);
		DrawFormatString(220, 0, white, "セットわざ名 : %s",c->skill[c->attackNum].name);
		/*確認用座標(あとで消す)*/
		DrawFormatString(500, 0, white, "ピカ座標(%d,%d)", c->x + 20 - m->x * CHIP_SIZE, c->y + 20 - m->y * CHIP_SIZE);
		DrawFormatString(500, 20, white, "ダー座標(%d,%d)", d->x + 20 - m->x * CHIP_SIZE, d->y + 20 - m->y*CHIP_SIZE);
		DrawFormatString(500, 40, white, "マップ座標(%d,%d)", m->x, m->y);

		/*DrawRotaGraph(x座標,y座標,縮尺度,角度,描画する画像ハンドル,背景透過処理のON,OFF)*/
		/*座標は画像の真ん中に持つ*/
		DrawRotaGraph(c->x + 20, c->y + 20, 1.5, 0, c->moveTexture[c->direction][d_num], true);
		
		/*生きてたら敵表示*/
		if (d->isLive) { DrawRotaGraph(d->x + 20, d->y + 20, 1.5, 0, d->moveTexture[d->direction][d_num], true); }

		/*メッセージ出力*/
		outMessage();

		/*qキーで終わり*/
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
			DrawRotaGraph(550, 160, 1, 0, skillBox, true);
			DrawFormatString(530, 98,  white, "%s (%d/%d)", c->skill[0].name, c->skill[0].count, c->skill[0].maxCount);
			DrawFormatString(530, 149, white, "%s (%d/%d)", c->skill[1].name, c->skill[1].count, c->skill[1].maxCount);
			DrawFormatString(530, 199, white, "%s (%d/%d)", c->skill[2].name, c->skill[2].count, c->skill[2].maxCount);
			DrawFormatString(530, 250, white, "%s (%d/%d)", c->skill[3].name, c->skill[3].count, c->skill[3].maxCount);
			if (keyState[KEY_INPUT_1])		{ c->attackNum = 0; menuflag = false; }
			else if (keyState[KEY_INPUT_2]) { c->attackNum = 1; menuflag = false; }
			else if (keyState[KEY_INPUT_3]) { c->attackNum = 2; menuflag = false; }
			else if (keyState[KEY_INPUT_4]) { c->attackNum = 3; menuflag = false; }
		}
		
		/*後にsetNearMap関数化してもいいかも*/
		nearCell[RIGHT]		= mapping[floor][c->y / CHIP_SIZE - m->y][(c->x + CHIP_SIZE) / CHIP_SIZE - m->x];
		nearCell[LEFT]		= mapping[floor][c->y / CHIP_SIZE - m->y][(c->x - CHIP_SIZE) / CHIP_SIZE - m->x];
		nearCell[UP]		= mapping[floor][(c->y - CHIP_SIZE) / CHIP_SIZE - m->y][c->x / CHIP_SIZE - m->x];
		nearCell[DOWN]		= mapping[floor][(c->y + CHIP_SIZE) / CHIP_SIZE - m->y][c->x / CHIP_SIZE - m->x];

		nearCell[RIGHT_UP]	= mapping[floor][(c->y - CHIP_SIZE) / CHIP_SIZE - m->y][(c->x + CHIP_SIZE) / CHIP_SIZE - m->x];
		nearCell[RIGHT_DOWN]= mapping[floor][(c->y + CHIP_SIZE) / CHIP_SIZE - m->y][(c->x + CHIP_SIZE) / CHIP_SIZE - m->x];
		nearCell[LEFT_UP]	= mapping[floor][(c->y - CHIP_SIZE) / CHIP_SIZE - m->y][(c->x - CHIP_SIZE) / CHIP_SIZE - m->x];
		nearCell[LEFT_DOWN]	= mapping[floor][(c->y + CHIP_SIZE) / CHIP_SIZE - m->y][(c->x - CHIP_SIZE) / CHIP_SIZE - m->x];

		nearCell[CENTER]	= mapping[floor][c->y / CHIP_SIZE - m->y][c->x / CHIP_SIZE - m->x];

		enemyCell = mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x];

		/*Attack*/
		if (!menuflag && keyState[KEY_INPUT_J] == 1) {
			if (isNearPokemon(c, d) && d->isLive) {
				attack(c, d, c->attackNum);
			}
			else if (d->isLive) {
				attack_for(c, c->attackNum);
			}
			if (life(d, c) == FALSE) {
				enemyMove(d, floor);
			}
		}


		/*Right*/
		if (!menuflag && keyState[KEY_INPUT_D]==1) {
			c->direction = RIGHT;
			if(nearCell[RIGHT]>0){
				if (c->x == MAP_WIDTH) { 
					m->x--;
					if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0)d->x-=CHIP_SIZE;
				}
				else if (!(keyState[KEY_INPUT_Y]) || !d->isLive) {
					c->x += CHIP_SIZE;
					if (c->x == d->x && c->y == d->y) { d->x = c->x- CHIP_SIZE; d->direction = RIGHT;}
					enemyMove(d, floor);
				}
			}
		}

		/*Left*/
		else if (!menuflag && keyState[KEY_INPUT_A]==1) { 
			c->direction = LEFT;
			if(nearCell[LEFT]>0){
				if (c->x == CHIP_SIZE * 2 && m->x != 0) {
					m->x++;
					if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0)d->x+=CHIP_SIZE;
				}
				else if (!(keyState[KEY_INPUT_Y])  || !d->isLive) {
					c->x -= CHIP_SIZE;
					if (c->x == d->x && c->y == d->y) { d->x = c->x + CHIP_SIZE; d->direction = LEFT;}
					enemyMove(d, floor);
				}
			}
		}

		/*Up*/
		else if (!menuflag && keyState[KEY_INPUT_W]==1) {
			c->direction = UP;
			if (nearCell[UP]>0) {
				if (c->y == CHIP_SIZE * 2 && m->y != 0) { 
					m->y++;
					if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0)d->y+=CHIP_SIZE;
				}
				else if (!(keyState[KEY_INPUT_Y]) || !d->isLive) {
					c->y -= CHIP_SIZE;
					if (c->x == d->x && c->y == d->y) { d->y = c->y + CHIP_SIZE; d->direction = UP;}
					enemyMove(d, floor);
				}
			}
		}

		/*Down*/
		else if (!menuflag && keyState[KEY_INPUT_X]==1) {
			c->direction = DOWN;
			if (nearCell[DOWN] > 0) {
				if (c->y == MAP_HEIGHT){
					m->y--;
					if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0)d->y-=CHIP_SIZE;
				}
				else if (!(keyState[KEY_INPUT_Y]) || !d->isLive) {
					c->y += CHIP_SIZE;
					if (c->x == d->x && c->y == d->y) { d->y = c->y - CHIP_SIZE; d->direction = DOWN;}
					enemyMove(d, floor);
				}
			}
		}

		/*RightUp*/
		else if (!menuflag && keyState[KEY_INPUT_E] == 1) {
			c->direction = UP;
			if (nearCell[RIGHT_UP] > 0) {
				if (c->y == CHIP_SIZE * 2 && c->x == MAP_WIDTH) { //右上端の場合
					if (m->y == 0) {							  //マップがこれ以上上に行けない場合
						m->x--;
						c->y-=CHIP_SIZE;
						if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) {d->x-=CHIP_SIZE;}
					}
					else {
						m->y++; m->x--;
						if (mapping[floor][d->y / CHIP_SIZE - m->y][c->x / CHIP_SIZE - m->x] <= 0) { d->x-=CHIP_SIZE; d->y+=CHIP_SIZE;}
					}
				}
				else if (c->y == CHIP_SIZE * 2) {			//右端ではないが上端の場合
					if (m->y != 0) {
						m->y++;
						c->x+=CHIP_SIZE;
						if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) { d->y += CHIP_SIZE; }
					}
				}
				else if (c->x == MAP_WIDTH){				//上端ではないが右端の場合
					m->x--;
					c->y-=CHIP_SIZE;
					if (mapping[floor][d->y / CHIP_SIZE - m->y][c->x / CHIP_SIZE - m->x] <= 0) { d->x -= CHIP_SIZE; }
				}
				else if (!(keyState[KEY_INPUT_Y]) || !d->isLive) {	//マップの端でない場合、yを押していないと普通に移動
					c->y-=CHIP_SIZE; c->x+=CHIP_SIZE;
					if (c->x == d->x && c->y == d->y) { d->x = c->x - CHIP_SIZE; d->y = c->y + CHIP_SIZE; d->direction = UP; }
					enemyMove(d, floor);
				}
			}
		}
		/*RightDown*/
		else if (!menuflag && keyState[KEY_INPUT_C] == 1) {
			c->direction = DOWN;
			if (nearCell[RIGHT_DOWN] > 0) {						//自分の右下が障害物でない場合
				if (c->y == MAP_HEIGHT && c->x == MAP_WIDTH) {	//マップの右下端であれば
					m->x--; m->y--; 
					if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) { d->x-=CHIP_SIZE; d->y-=CHIP_SIZE; }
				}
				else if (c->y == MAP_HEIGHT) {					//右端ではないが下端である場合
					m->y--;
					c->x+=CHIP_SIZE;
					if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) {d->y -= CHIP_SIZE; }
				}
				else if (c->x == MAP_WIDTH) {					//下端ではないが右端である場合
					m->x--;
					c->y+=CHIP_SIZE;
					if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) {d->x -= CHIP_SIZE;}
				}
				else if (!(keyState[KEY_INPUT_Y]) || !d->isLive) {
					c->y+=CHIP_SIZE; c->x+=CHIP_SIZE;
					if (c->x == d->x && c->y == d->y) { d->x = c->x - CHIP_SIZE; d->y = c->y - CHIP_SIZE; d->direction = DOWN;}
					enemyMove(d, floor);
				}
			}
		}

		/*LeftUp*/
		else if (!menuflag && keyState[KEY_INPUT_Q] == 1) {		//メニューを開いておらず、Qキーを押すと
			c->direction = UP;
			if (nearCell[LEFT_UP] > 0) {
				if (c->y == CHIP_SIZE * 2 && c->x == CHIP_SIZE * 2) {		//左上端にいる場合
					if (m->y == 0) {							//マップがこれ以上上に行けない場合
						m->x++;
						m->y--;
						if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) { d->x+=CHIP_SIZE; }
					}
					else if (m->x == 0) {						//マップがこれ以上左に行けない場合 
						m->y++;
						c->x-=CHIP_SIZE;
						if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) { d->y+=CHIP_SIZE; }
					}
					else {										//マップがこれ以上上にも左にも行けない場合
						if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) { d->x+=CHIP_SIZE; d->y+=CHIP_SIZE; }
					}
				}
				else if (c->y == CHIP_SIZE * 2) {							//左端ではないが上端にいる場合
					if (m->y != 0) {							//マップがまだ上に行ける場合
						m->y++;
						c->x-=CHIP_SIZE;
						if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) { d->y += CHIP_SIZE; }
					}
				}
				else if (c->x == CHIP_SIZE * 2) {							//上端ではないが左端にいる場合
					if (m->x != 0) {							//マップがまだ左に行ける場合
						m->x++;
						c->y-=CHIP_SIZE;
						if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) { d->x += CHIP_SIZE; }
					}
				}
				else if (!(keyState[KEY_INPUT_Y]) || !d->isLive) {
					c->y-=CHIP_SIZE; c->x-=CHIP_SIZE;
					if (c->x == d->x && c->y == d->y) { d->x = c->x + CHIP_SIZE; d->y = c->y + CHIP_SIZE; d->direction = UP;}
					enemyMove(d, floor);
				}
			}
		}

		/*LeftDown*/
		else if (!menuflag && keyState[KEY_INPUT_Z] == 1) {
			c->direction = DOWN;
			if (nearCell[LEFT_DOWN] > 0) {
				if (c->y ==  MAP_HEIGHT && c->x == CHIP_SIZE * 2) {				//マップの左下端にいる場合
					if (m->x == 0) {											//マップがこれ以上左に行けない場合
						m->y--;
						c->x-=CHIP_SIZE;
						if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) { d->y-=CHIP_SIZE; }
					}
					else {
						m->x++; m->y--;
						if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) { d->x+=CHIP_SIZE; d->y-=CHIP_SIZE; }
					}
				}
				else if (c->y == MAP_HEIGHT) {									//マップの左端でないが下端にいる場合
					m->y--;
					c->x-=CHIP_SIZE;
					if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) { d->y -= CHIP_SIZE; }
				}
				else if (c->x == CHIP_SIZE * 2) {								//マップの下端でないが左端にいる場合
					if (m->x != 0) {											//マップがまだ左に行ける場合
						m->x++;
						c->y+=CHIP_SIZE;
						if (mapping[floor][d->y / CHIP_SIZE - m->y][d->x / CHIP_SIZE - m->x] <= 0) { d->x += CHIP_SIZE; }
					}
				}
				else if (!(keyState[KEY_INPUT_Y]) || !d->isLive) {
					c->y+=CHIP_SIZE; c->x-=CHIP_SIZE;
					if (c->x == d->x && c->y == d->y) { d->x = c->x + CHIP_SIZE; d->y = c->y - CHIP_SIZE; d->direction = DOWN; }
					enemyMove(d, floor);
				}
			}
		}


		/*
		if (!menuflag && !keyState[KEY_INPUT_Y]) {
			if (keyState[KEY_INPUT_Q] == 1 || keyState[KEY_INPUT_W] == 1 || keyState[KEY_INPUT_] == 1 || keyState[KEY_INPUT_DOWN] == 1){
				if(!(c->x == CHIP_SIZE * 2||c->x == MAP_WIDTH||c->y == CHIP_SIZE * 2 || c->y == MAP_HEIGHT) || !isNearPokemon(c,d))
					enemyMove(d,floor);
			}
		}
		*/


		if (nearCell[CENTER] == 100) {
			floor += 1;
			wait(2000,(char*)(floor+1));
		}

		if (nearCell[CENTER] == 101) {
			floor -= 1;
			wait(2000,(char*)(floor+1));
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

	skillfull(0);

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

	d->voice = LoadSoundMem("音楽/ダークライvoice.mp3");

	d->experience = 10000;

	d->maxHp = 70;
	d->hp = 70;

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

	d->x = CHIP_SIZE * 10;
	d->y = CHIP_SIZE * 2;


	/*音声をロード*/
	slap = LoadSoundMem("音楽/slap1.mp3");
	bgm = LoadSoundMem("音楽/bgm.mp3");
	down = LoadSoundMem("音楽/down1.mp3");

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

	d->skill[0].min = 2;
	d->skill[0].max = 4;
	d->skill[1].min = 3;
	d->skill[1].max = 6;
	d->skill[2].min = 8;
	d->skill[2].max = 14;
	d->skill[3].min = 4;
	d->skill[3].max = 8;
}


/*attack(自分,敵,攻撃の種類)*/
void attack(pokemon* me, pokemon* enemy, int attackNum) {

	//turnToPokemon(me, enemy);//敵の方を向く
	int value = getRandom(me->skill[attackNum].min, me->skill[attackNum].max);
	//ななめに居る時は攻撃しない
	if (!(((me->x - CHIP_SIZE == enemy->x) && (me->y + CHIP_SIZE == enemy->y || me->y - CHIP_SIZE == enemy->y)) || (me->x + CHIP_SIZE == enemy->x) && (me->y + CHIP_SIZE == enemy->y || me->y - CHIP_SIZE == enemy->y))) {
		if ((me->x > enemy->x && me->direction == LEFT) || (me->x < enemy->x && me->direction == RIGHT) || (me->y > enemy->y && me->direction == UP) || (me->y < enemy->y && me->direction == DOWN)) {
			if (me->skill[attackNum].count > 0) {

				/*確率で攻撃が外れる*/
				if (getRandom(1, 100) < 98) {
					enemy->hp -= value;
					if (enemy->hp < 0)enemy->hp = 0;//hpがマイナスになるのを防ぐ
					sprintf_s(s, "%sの%s! %sに%dのダメージ!%sのHP:%d", me->name, me->skill[attackNum].name, enemy->name, value, enemy->name, enemy->hp);
					PlaySoundMem(slap, DX_PLAYTYPE_BACK);
				}
				else {
					sprintf_s(s, "%sの攻撃は外れた!", me->name);
				}
			}
			else {
				sprintf_s(s, "%sはもう使えない!", me->skill[attackNum].name);
			}
			me->skill[attackNum].count -= 1;
			if (me->skill[attackNum].count < 0)me->skill[attackNum].count = 0;	//マイナスを防ぐ
			messageflag = true;
			setMessage(s);
			outMessage();
		}
		else {
			attack_for(me, attackNum);
		}
	}


}
//プレイヤー攻撃用
void attack_for(pokemon* me, int attackNum) {

	//斜め以外
	if (me->direction == LEFT || me->direction == RIGHT || me->direction == UP || me->direction == DOWN) {
		if (me->skill[attackNum].count > 0) {

			/*攻撃が外れる*/
			sprintf_s(s, "%sの%s!しかし攻撃は外れた", me->name, me->skill[attackNum].name);
			PlaySoundMem(slap, DX_PLAYTYPE_BACK);
		}
		else {
			sprintf_s(s, "%sはもう使えない!", me->skill[attackNum].name);
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
		return TRUE;
	}
}


/*敵の動き*/
void enemyMove(pokemon* enemy,int floor) {

	/*敵が同じマップ内にいると、自分に向かってくる*/
	if (findPokemon(enemy, c)) {
		/*攻撃しない*/
		/*移動処理*/
		int nearCell[9];

		nearCell[RIGHT]		= mapping[floor][enemy->y / CHIP_SIZE - m->y][(enemy->x + CHIP_SIZE) / CHIP_SIZE - m->x];
		nearCell[LEFT]		= mapping[floor][enemy->y / CHIP_SIZE - m->y][(enemy->x - CHIP_SIZE) / CHIP_SIZE - m->x];
		nearCell[UP]		= mapping[floor][(enemy->y - CHIP_SIZE) / CHIP_SIZE - m->y][enemy->x / CHIP_SIZE - m->x];
		nearCell[DOWN]		= mapping[floor][(enemy->y + CHIP_SIZE) / CHIP_SIZE - m->y][enemy->x / CHIP_SIZE - m->x];

		nearCell[RIGHT_UP]	= mapping[floor][(enemy->y - CHIP_SIZE) / CHIP_SIZE - m->y][(enemy->x + CHIP_SIZE) / CHIP_SIZE - m->x];
		nearCell[RIGHT_DOWN]= mapping[floor][(enemy->y + CHIP_SIZE) / CHIP_SIZE - m->y][(enemy->x + CHIP_SIZE) / CHIP_SIZE - m->x];
		nearCell[LEFT_UP]	= mapping[floor][(enemy->y - CHIP_SIZE) / CHIP_SIZE - m->y][(enemy->x - CHIP_SIZE) / CHIP_SIZE - m->x];
		nearCell[LEFT_DOWN]	= mapping[floor][(enemy->y + CHIP_SIZE) / CHIP_SIZE - m->y][(enemy->x - CHIP_SIZE) / CHIP_SIZE - m->x];

		nearCell[CENTER]	= mapping[floor][enemy->y / CHIP_SIZE - m->y][enemy->x / CHIP_SIZE - m->x];
		

		if (!isNearPokemon(enemy, c) && enemy->isLive) {
			if (c->x != enemy->x && c->x + CHIP_SIZE < enemy->x) {
				enemy->direction = LEFT;
				if (!(c->x == MAP_WIDTH)) {
					if (nearCell[LEFT] > 0) enemy->x -= CHIP_SIZE;
				}
			}
			else if (c->x != enemy->x && c->x + CHIP_SIZE > enemy->x) {
				enemy->direction = RIGHT;
				if (!(c->x != enemy->x && c->x - CHIP_SIZE < enemy->x)) {
					if (nearCell[RIGHT] > 0 ) enemy->x += CHIP_SIZE;
				}
			}
			else if (c->y != enemy->y && c->y - CHIP_SIZE < enemy->y) {
				enemy->direction = UP;
				if (!(c->y == CHIP_SIZE * 2 && m->y != 0)) {
					if (nearCell[UP] > 0) enemy->y -= CHIP_SIZE;
				}
			}
			else if (c->y != enemy->y && c->y + CHIP_SIZE > enemy->y) {
				enemy->direction = DOWN;
				if (!(c->y == MAP_HEIGHT)) {					//マップ動いた時用
					if (nearCell[DOWN] > 0 ) enemy->y += CHIP_SIZE;
				}
			}
			else if (c->x != enemy->x && c->y != enemy->y &&c->x < enemy->x && c->y < enemy->y) {
				enemy->direction = UP;
				if (nearCell[LEFT_UP] > 0) { enemy->y -= CHIP_SIZE; enemy->x -= CHIP_SIZE; }
			}
			else if (c->x != enemy->x && c->y != enemy->y && c->x < enemy->x && c->y > enemy->y) {
				enemy->direction = DOWN;
				if (nearCell[LEFT_DOWN] > 0) { enemy->y += CHIP_SIZE; enemy->x -= CHIP_SIZE; }
			}
			else if (c->x != enemy->x && c->y != enemy->y && c->x > enemy->x && c->y < enemy->y) {
				enemy->direction = UP;
				if (nearCell[RIGHT_UP] > 0) { enemy->y -= CHIP_SIZE; enemy->x += CHIP_SIZE; }
			}
			else if (c->x != enemy->x && c->y != enemy->y && c->x > enemy->x && c->y > enemy->y) {
				enemy->direction = DOWN;
				if (nearCell[RIGHT_DOWN] > 0) { enemy->y += CHIP_SIZE; enemy->x += CHIP_SIZE; }
			}
		}
		/*攻撃する*/
		else if (isNearPokemon(enemy, c) && c->hp > 0 && enemy->isLive) {
			turnToPokemon(enemy, c);
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
