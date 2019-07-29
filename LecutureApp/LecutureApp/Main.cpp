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
		ClearDrawScreen();
		/*bgm再生開始*/
		PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
		for (int i = 0; i < 256; i++) {
			SetDrawBright(i, i, i);
			allView(d_num);
			wait(10);
		}

		talk(c, "「ここは...?」");
		initMessage();
		drawExplain();
		messageflag = false;
		/*描画する*/
		// while(裏画面を表画面に反映, メッセージ処理, 画面クリア,フレームカウント)
		while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && getCountFrame() == 0 && m->floor != 2) {

			tmp_mx = m->x;
			tmp_my = m->y;

			/*500ms(0.5秒)に一度画像更新(歩いているように見える)*/
			if (GetNowCount() - tempTime > 500)
			{
				if (d_num == 1)d_num = 0;
				else d_num = 1;
				tempTime = GetNowCount();
			}

			//地図やキャラなどの描画関数
			allView(d_num);
			
			/*メッセージは常に出力*/
			outMessage();

			//自分が死んだら
			if (c->hp == 0) { over = true; break; }

			/*iを押すとメニュー表示、再度押すか技を選ぶと消去*/
			showMenu();


			/*主人公の動き、移動や攻撃、階段移動*/
			mainCharaMove();
			/*敵の動き、移動や攻撃*/
			if (m->floor != 2) {
				mainEnemyMove(tmp_mx, tmp_my);
			}

			/*spaceキーで終わり*/
			if (keyState[KEY_INPUT_SPACE] == 1) { endflag = true; break; }
			if (keyState[KEY_INPUT_DOWN] == 1) { stairsMove(1); }

		}
		if (m->floor == 2) {
			for (int i = 0; i < 256; i++) {
				SetDrawBright(i, i, i);
				allView(d_num);
				wait(10);
			}
			
			talk(lastboss, "よくここまで来たな");
			wait(10);
			talk(lastboss, "さあ、私を倒してみろ");
			wait(10);
			talk(c, "誰やねん...");
			
		}
		//ボス用
		while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && getCountFrame() == 0 && m->floor == 2) {
			tmp_mx = m->x;
			tmp_my = m->y;

			/*500ms(0.5秒)に一度画像更新(歩いているように見える)*/
			if (GetNowCount() - tempTime > 500)
			{
				if (d_num == 1)d_num = 0;
				else d_num = 1;
				tempTime = GetNowCount();
			}

			//地図やキャラなどの描画関数
			allView(d_num);

			/*メッセージは常に出力*/
			outMessage();

			if (!lastboss->isLive) { clear = true; break; }

			//自分が死んだら
			if (c->hp == 0) { over = true; break; }

			/*iを押すとメニュー表示、再度押すか技を選ぶと消去*/
			showMenu();

			/*主人公の動き、移動や攻撃、階段移動*/
			mainCharaMove();

			/*敵の動き、移動や攻撃*/
			mainBossMove(tmp_mx, tmp_my);

			/*spaceキーで終わり*/
			if (keyState[KEY_INPUT_SPACE] == 1) { endflag = true; break; }
		}

		if (endflag)break;
		else if (over)gameover();
		else if(clear)gameClear();
		if (endflag)break;
	}
	DxLib::DxLib_End();    // DXライブラリ終了処理
	
	return 0;
}







