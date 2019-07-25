#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <stdio.h>
#include "allVariableName.h"
#include "allMethod.h"

/***********************      Mainの処理      *********************************/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	bool over;
	bool clear;
	bool story;
	int tmp_mx = m->x;
	int tmp_my = m->y;
	int d_num = 0;
	int tempTime = 0;
	int hpColor = 0;
	/*初期化処理*/
	if (init() == -1) { return -1; }

	while (true) {


		prologue();

		charaInit();

		SetFontSize(16);
		SetFontThickness(4);

		over = false;
		clear = false;
		story = false;
		/*bgm再生開始*/
		PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);

		/*描画する*/
		// while(裏画面を表画面に反映, メッセージ処理, 画面クリア,フレームカウント)
		while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && getCountFrame() == 0) {

			tmp_mx = m->x;
			tmp_my = m->y;

			//マップ描画
			drawMap();
			//ミニマップ描画
			drawMiniMap();

			/*500ms(0.5秒)に一度画像更新(歩いているように見える)*/
			if (GetNowCount() - tempTime > 500)
			{
				if (d_num == 1)d_num = 0;
				else d_num = 1;
				tempTime = GetNowCount();
			}

			/*階層、level、HP表示(固定)*/
			DrawFormatString(0, 0, WHITE, "B%dF", m->floor + 1);
			DrawFormatString(50, 0, WHITE, "Lv: %d", c->level);
			DrawFormatString(120, 0, WHITE, "HP: %d/ %d", c->hp, c->maxHp);
			DrawFormatString(360, 0, WHITE, "セットわざ名 : %s", c->skill[c->attackNum].name);
			DrawFormatString(600, 0, WHITE, "マップ(%d,%d)", m->x, m->y);
			DrawFormatString(600, 20, WHITE, " ピカチュウ(%d,%d)", GX * CHIP_SIZE ,GY*CHIP_SIZE);
			for (int i = 0; i < ENEMYNUM; i++) {
				if (enemy[m->floor][i]->name == "ミュウツー") { DrawFormatString(600, 40, WHITE, " ミュウツー(%d,%d)", SX(enemy[m->floor][i])*CHIP_SIZE, SY(enemy[m->floor][i]) * CHIP_SIZE); }
			}
			
			if (c->hp <= c->maxHp / 3) { hpColor = RED; }
			else if (c->hp <= c->maxHp / 2) { hpColor = YELLOW; }
			else { hpColor = GREEN; }
			DrawBox(230, 1, c->maxHp + 230, 15, BLACK, true);
			DrawBox(230, 1, c->hp + 230, 15, hpColor, true);
			DrawBox(230, 0, c->maxHp + 231, 16, WHITE, false);
			
			

			drawCharacter(c, d_num);

			for (int i = 0; i < ENEMYNUM; i++) {
				drawCharacter(enemy[m->floor][i], d_num);
			}

			/*メッセージ出力*/
			outMessage();

			if (m->floor == 2) {
				pokemon* tmp = new pokemon;
				for (int i = 0; i < ENEMYNUM; i++) {
					if (enemy[m->floor][i]->name=="ミュウツー") { tmp = enemy[m->floor][i]; break; }
				}
				//ボスが死んだら
				if (!tmp->isLive) { clear = true; break; }
			}

			//自分が死んだら
			if (c->hp == 0) { over = true; break; }

			/*iを押すとメニュー表示、再度押すか技を選ぶと消去*/
			showMenu();

			/*主人公の動き、移動や攻撃、階段移動*/
			mainCharaMove();

			/*敵の動き、移動や攻撃*/
			mainEnemyMove(tmp_mx, tmp_my);

			/*spaceキーで終わり*/
			if (keyState[KEY_INPUT_SPACE] == 1) { endflag = true; break; }

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

		if (endflag)break;
		else if (over)gameover();
		else if(clear)gameClear();
		if (endflag)break;
	}
	DxLib::DxLib_End();    // DXライブラリ終了処理
	
	return 0;
}







