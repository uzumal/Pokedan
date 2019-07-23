#include "all.h"
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <map>
#include <stack>
#include <stdio.h>
#include "Enemy.cpp"
#include "Character.cpp"
#include "Map.cpp"
#include "Generic.cpp"


/*関数プロトタイプ宣言*/
/********************************Generic.cpp************************************/
int init();										//初期化
int getRandom(int,int);							//int max～int minの範囲で乱数を取得
void wait(int);									//int時間待つ
void wait(int,char* s);							//int時間待つ、中心に文字表示
void w_press();									//Kボタン押したら進
void setDirection(pokemon*, int);						//キャラの向いている方向をセットする
void skillfull(int experience);					//技威力設定
void drawCharacter(pokemon*,int d_num);
bool isPutMoveKey();


/*********************************Character.cpp**********************************/
void attack(pokemon*, pokemon*);			//攻撃する
void attack_for(pokemon* me);	//プレイヤー用攻撃
void turnToPokemon(pokemon*, pokemon*);			//ポケモンの方を向く
void moveJump(pokemon*);						//Jumpする
void charaMove(pokemon*,pokemon*[ENEMYNUM],maps*,char[256], int, int);		//キャラ移動、自分用　入れ替わり考慮
pokemon* getFrontEnemy();


/*********************************Enemy.cpp**************************************/
void enemyMove(pokemon*);					//敵の動き
bool isNearPokemon(pokemon*, pokemon*);			//敵が近くにいたら(攻撃圏内にいたら)true
bool isNearPokemon2(pokemon*, pokemon*);			//敵が近くにいたら(攻撃圏内にいたら)true
bool findPokemon(pokemon*, pokemon*);			//敵が発見できる近さにいたら
bool life(pokemon* enemy, pokemon* me);			//敵死んでいるかどうか
void randomEnemyPut(pokemon*[ENEMYNUM]);
void sortEnemy();
//A*アルゴリズム敵追尾
int h(NODE*, NODE*);		//ヒューリスティック関数値を返す、マンハッタン距離
void setNode(NODE* child, int x, int y, NODE* parent, int cost, int f);		//ノードの情報を一気に設定する
int getCost();				//移動コストを返す、この場合は1を返す
NODE* Astar(pokemon*);			//A*アルゴリズム適用後、次に進むセルのノードを返す
void charaMove(pokemon*, int, int);				//キャラ移動、敵用
bool isNearEnemy(pokemon*,int ,int);


/*********************************Message.cpp************************************/
void initConsole();								//メッセージボックスを初期化する
void setMessage(char[]);						//表示したいメッセージをセットする
void outMessage();								//メッセージを表示する
void initMessage();


/**********************************Map.cpp**************************************/
void mapMove(maps*,pokemon*,pokemon*[ENEMYNUM], int, int);	//マップ移動、引っ掛かり考慮
void drawMap();
void drawMiniMap();

/**********************************Menu.cpp*************************************/
void drawMenu();

/*攻撃する相手を返す*/

/*キーが押されているフレーム数によって表示する画像を変更する*/
/*
int getDnum(int key) {
	if (keyState[key] % 60 >= 1 && keyState[key] % 60 < 30) return 0;
	else return 1;
}
*/



/*
void Screen() {
	while () {
		sprintf_s(s, "ようこそポケモンの世界へ");

		messageflag = true;
		setMessage(s);
		outMessage();
	}
}*/



/***********************      Mainの処理      *********************************/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	/*初期化処理*/
	if (init() == -1) { return -1; }

	int d_num = 0;
	int tempTime = 0;

	m->floor = 0;
	int tmp_mx = m->x;
	int tmp_my = m->y;
	
	int distance = 0;

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

		DrawFormatString(500, 0, WHITE, "1(%d,%d)", enemy[m->floor][0]->x,enemy[m->floor][0]->y);
		DrawFormatString(500, 20, WHITE, "2(%d,%d)", enemy[m->floor][1]->x,enemy[m->floor][1]->y);
		DrawFormatString(500, 40, WHITE, "3(%d,%d)",enemy[m->floor][2]->x,enemy[m->floor][2]->y);

		drawCharacter(c, d_num);
		
		for (int i = 0; i < ENEMYNUM; i++) {
			drawCharacter(enemy[m->floor][i], d_num);
		}

		/*メッセージ出力*/
		outMessage();

		/*spaceキーで終わり*/
		if (keyState[KEY_INPUT_SPACE] == 1) { break; }
		
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

			tmp = getFrontEnemy();
			if (tmp != NULL)attack(c, tmp);
			else attack_for(c);

			for (int i = 0; i < ENEMYNUM; i++) {
				if (life(enemy[m->floor][i], c) == FALSE) {
					enemyMove(enemy[m->floor][i]);
				}
			}
		}
		/*Right*/
		if (!menuflag && keyState[KEY_INPUT_D]==1) {
			charaMove(c,enemy[m->floor],m,keyState,1,0);
		}
		/*Left*/
		else if (!menuflag && keyState[KEY_INPUT_A]==1) { 
			charaMove(c,enemy[m->floor],m,keyState, -1, 0);
		}
		/*Up*/
		else if (!menuflag && keyState[KEY_INPUT_W]==1) {
			charaMove(c,enemy[m->floor],m,keyState, 0, -1);
		}
		/*Down*/
		else if (!menuflag && keyState[KEY_INPUT_X]==1) {
			charaMove(c,enemy[m->floor],m,keyState, 0, 1);
		}
		/*RightUp*/
		else if (!menuflag && keyState[KEY_INPUT_E] == 1) {
			charaMove(c, enemy[m->floor],m,keyState, 1, -1);
		}
		/*RightDown*/
		else if (!menuflag && keyState[KEY_INPUT_C] == 1) {
			charaMove(c, enemy[m->floor],m,keyState, 1, 1);
		}
		/*LeftUp*/
		else if (!menuflag && keyState[KEY_INPUT_Q] == 1) {
			charaMove(c, enemy[m->floor],m,keyState, -1, -1);
		}
		/*LeftDown*/
		else if (!menuflag && keyState[KEY_INPUT_Z] == 1) {
			charaMove(c, enemy[m->floor],m,keyState, -1, 1);
		}

		
		if (!menuflag && isPutMoveKey() && tmp_mx == m->x && tmp_my == m->y) {
			if (keyState[KEY_INPUT_Y] == 0) {
				sortEnemy();
				
				for (int i = 0; i < ENEMYNUM; i++) {
					enemyMove(enemy[m->floor][i]);
				}
			}
		}
		/*マップ移動時*/
		else if (!menuflag && isPutMoveKey() && (tmp_mx != m->x || tmp_my != m->y)) {
			
			sortEnemy();

			tmp = NULL;
			
			//入れ替わり処理を優先させる
			for (int i = 0; i < ENEMYNUM; i++) {
				if (GX == SX(enemy[m->floor][i]) + (m->x - tmp_mx) && GY == SY(enemy[m->floor][i]) + (m->y - tmp_my) ) {
					charaMove(enemy[m->floor][i], m->x - tmp_mx, m->y - tmp_my);
					enemy[m->floor][i]->x += (m->x - tmp_mx) * CHIP_SIZE; enemy[m->floor][i]->y += (m->y - tmp_my)*CHIP_SIZE;
					turnToPokemon(enemy[m->floor][i], c);
					attack(enemy[m->floor][i], c);
					tmp = enemy[m->floor][i];
				}
			}
			for (int i = 0; i < ENEMYNUM; i++) {
				if (tmp == enemy[m->floor][i])continue;
				if (isNearEnemy(enemy[m->floor][i], 0, 0)) {
					enemy[m->floor][i]->x += (m->x - tmp_mx) * CHIP_SIZE; enemy[m->floor][i]->y += (m->y - tmp_my) * CHIP_SIZE;
				}
			}
			
			for (int i = 0; i < ENEMYNUM; i++) {
				if (tmp == enemy[m->floor][i])continue;
				
				charaMove(enemy[m->floor][i], m->x - tmp_mx, m->y - tmp_my);
				//移動後、前、右、左のどちらかにいたとき
				if (isNearPokemon(enemy[m->floor][i], c) && (GX + (m->x - tmp_mx) != SX(enemy[m->floor][i]) && GY + (m->y - tmp_my) != SY(enemy[m->floor][i]))) {
					turnToPokemon(enemy[m->floor][i], c);
					attack(enemy[m->floor][i], c);
				}
				//ななめにいるとき
				else if (!isNearPokemon(enemy[m->floor][i],c) && isNearPokemon2(enemy[m->floor][i], c)) {
					charaMove(enemy[m->floor][i], (c->x - enemy[m->floor][i]->x) / CHIP_SIZE, (c->y - enemy[m->floor][i]->y) / CHIP_SIZE);
				}
				//8方にいるとき
				else if (isNearPokemon(enemy[m->floor][i], c)) {
					turnToPokemon(enemy[m->floor][i], c);
				}
				
				else {
					NODE* n = Astar(enemy[m->floor][i]);
					if (n != NULL)charaMove(enemy[m->floor][i], n->x - SX(enemy[m->floor][i]), n->y - SY(enemy[m->floor][i]));
				}
				
			}
		}


		/*階段移動処理*/
		if (mapping[m->floor][GY][GX] == 100) {
			stairsMove(1);
		}

		if (mapping[m->floor][GY][GX] == 101) {
			stairsMove(-1);
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

	}

	DxLib::DxLib_End();    // DXライブラリ終了処理
	
	return 0;
}







