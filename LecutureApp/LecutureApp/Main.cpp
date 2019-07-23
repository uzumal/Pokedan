#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <stdio.h>
#include "allVariableName.h"
#include "allMethod.h"

/***********************      Mainの処理      *********************************/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	/*初期化処理*/
	if (init() == -1) { return -1; }

	int d_num = 0;
	int tempTime = 0;

	m->floor = 0;
	int tmp_mx = m->x;
	int tmp_my = m->y;
	
	prologue();

	SetFontSize(16);
	SetFontThickness(4);

	/*bgm再生開始*/
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);

	pokemon* tmp = new pokemon;

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

		DrawFormatString(500, 0, WHITE, "1(%d,%d)", SX(enemy[m->floor][0]),SY(enemy[m->floor][0]));
		DrawFormatString(500, 20, WHITE, "2(%d,%d)", SX(enemy[m->floor][1]),SY(enemy[m->floor][1]));
		DrawFormatString(500, 40, WHITE, "3(%d,%d)", SX(enemy[m->floor][2]),SY(enemy[m->floor][2]));

		drawCharacter(c, d_num);
		
		for (int i = 0; i < ENEMYNUM; i++) {
			drawCharacter(enemy[m->floor][i], d_num);
		}

		/*メッセージ出力*/
		outMessage();

		/*iを押すとメニュー表示、再度押すか技を選ぶと消去*/
		showMenu();
		
		/*主人公の動き、移動や攻撃、階段移動*/
		mainCharaMove();

		/*敵の動き、移動や攻撃*/
		mainEnemyMove(tmp_mx,tmp_my);

		if (c->hp == 0)gameover();

		/*spaceキーで終わり*/
		if (keyState[KEY_INPUT_SPACE] == 1) { endflag = true; }

		if (endflag)break;

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







