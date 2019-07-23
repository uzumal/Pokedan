#include "all.h"
#include "allVariableName.h"
#include "Message.h"
#include "Generic.h"

void prologue();
void setMainChara(int);


void prologue() {
	int f_num = 0;
	PlaySoundMem(main, DX_PLAYTYPE_LOOP);
	char d[500] = "<Press A Botton>";
	int e = 0;
	while (keyState[KEY_INPUT_A] == 0 && ScreenFlip() == 0 && getCountFrame() == 0 && ClearDrawScreen() == 0) {
		DrawRotaGraph(400, 250, 1.0, 0, title[0], false);
		DrawRotaGraph(400, 250, 1.5, 0, title[1], true);
		SetFontSize(32);                             //サイズを32に変更
		SetFontThickness(8);                         //太さを8に変更
		ChangeFont("HGS創英角ﾎﾟｯﾌﾟ体");              //HGS創英角ﾎﾟｯﾌﾟ体に変更
		ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);//アンチエイリアス＆エッジ付きフォントに変更
		DrawFormatString(230, 420, GetColor(255, 0, 0), "%s", d);
		/*500ms(0.5秒)に一度画像更新(歩いているように見える)*/
		if (GetNowCount() - tempTime > 1500)
		{
			if (f_num == 1) {
				f_num = 0;
				DrawRotaGraph(400, 250, 1.0, 0, title[0], false);
				DrawRotaGraph(400, 250, 1.5, 0, title[1], true);
				wait(100);
			}
			else {
				f_num = 1;
			}
			tempTime = GetNowCount();
		}

	}
	StopSoundMem(main);
	PlaySoundMem(start, DX_PLAYTYPE_LOOP);
	ChangeFont("游ゴシック　Light");              //変更
	SetFontSize(20);                             //サイズを64に変更
	SetFontThickness(4);                         //太さを8に変更
	DrawRotaGraph(400, 300, 3.5, 0, bl, true);
	sprintf_s(s, "ようこそポケモンの世界へ");
	showDisplay(s);
	initMessage();
	sprintf_s(s, "今からあなたにはダンジョンをクリアしてもらう");
	showDisplay(s);
	initMessage();
	sprintf_s(s, "では初めにプレイするポケモンを選んでもらおう");
	showDisplay(s);
	initMessage();
	double c_num = 0;
	int e_num = 0;
	int count = 0;
	DrawRotaGraph(400, 100, 3.0, 0, ball[0], true);
	DrawRotaGraph(600, 350, 3.0, 0, ball[0], true);
	DrawRotaGraph(200, 350, 3.0, 0, ball[0], true);
	ScreenFlip();
	bool flag[3] = { false,false,false };
	bool opFlag = true;
	while (opFlag && getCountFrame() == 0 && ScreenFlip() == 0) {

		if (keyState[KEY_INPUT_UP] == 1) {
			flag[0] = true;
			flag[1] = false;
			flag[2] = false;
		}
		if (keyState[KEY_INPUT_LEFT] == 1) {
			flag[0] = false;
			flag[1] = true;
			flag[2] = false;
			DrawString(390, 150, "◇", BLACK);
		}
		if (keyState[KEY_INPUT_RIGHT] == 1) {
			flag[0] = false;
			flag[1] = false;
			flag[2] = true;
			DrawString(390, 150, "◇", BLACK);
		}

		if (flag[0]) {
			DrawString(390, 150, "◇", RED);
			DrawRotaGraph(400, 300, 0.5, 0, bl, true);
			while (ScreenFlip() == 0 && getCountFrame() == 0 && !keyState[KEY_INPUT_LEFT] && !keyState[KEY_INPUT_RIGHT] && !keyState[KEY_INPUT_A]) {

				if (GetNowCount() - tempTime > 500)
				{
					switch (count % 4) {
					case 0:c_num = 0; break;
					case 1:c_num = PI / 4; break;
					case 2:c_num = 0; break;
					case 3:c_num = -PI / 4; break;
					}
					count++;
					if (count == 4)count = 0;
				}
				DrawRotaGraph(400, 100, 3.0, c_num, ball[0], true);
				/*500ms(0.5秒)に一度画像更新(歩いているように見える)*/
				if (GetNowCount() - tempTime > 500)
				{
					if (e_num == 1)e_num = 0;
					else e_num = 1;
					tempTime = GetNowCount();
				}
				e = 1;
				DrawRotaGraph(400, 300, 2.5, 0, z[0]->moveTexture[DOWN][e_num], true);
			}
			c_num = 0;
			count = 0;
			DrawRotaGraph(400, 100, 3.0, 0, ball[0], true);
			DrawString(390, 150, "◇", BG);
			if (keyState[KEY_INPUT_LEFT] == 1) {
				flag[0] = false;
				flag[1] = true;
				flag[2] = false;
			}
			if (keyState[KEY_INPUT_RIGHT] == 1) {
				flag[0] = false;
				flag[1] = false;
				flag[2] = true;
			}
			if (keyState[KEY_INPUT_A] == 1) {
				flag[0] = false;
				flag[1] = false;
				flag[2] = false;
				opFlag = false;
			}
		}

		if (flag[1]) {
			DrawString(190, 400, "◇", RED);
			DrawRotaGraph(400, 300, 0.5, 0, bl, true);
			while (ScreenFlip() == 0 && getCountFrame() == 0 && !keyState[KEY_INPUT_UP] && !keyState[KEY_INPUT_RIGHT] && !keyState[KEY_INPUT_A]) {

				if (GetNowCount() - tempTime > 500)
				{	
					switch (count % 4) {
					case 0:c_num = 0; break;
					case 1:c_num = PI / 4; break;
					case 2:c_num = 0; break;
					case 3:c_num = -PI/4; break;
					}
					count++;
					if (count == 4)count = 0;
				}
				DrawRotaGraph(200, 350, 3.0, c_num, ball[0], true);
				/*500ms(0.5秒)に一度画像更新(歩いているように見える)*/
				if (GetNowCount() - tempTime > 500)
				{
					if (e_num == 1)e_num = 0;
					else e_num = 1;
					tempTime = GetNowCount();
				}
				e = 2;
				DrawRotaGraph(400, 300, 2.5, 0, z[1]->moveTexture[DOWN][e_num], true);
				if (keyState[KEY_INPUT_A] == 1)opFlag = false;
			}
			c_num = 0;
			e_num = 0;
			count = 0;
			DrawRotaGraph(200, 350, 3.0, 0, ball[0], true);
			DrawString(190, 400, "◇", BG);
			if (keyState[KEY_INPUT_UP] == 1) {
				flag[0] = true;
				flag[1] = false;
				flag[2] = false;
			}
			if (keyState[KEY_INPUT_RIGHT] == 1) {
				flag[0] = false;
				flag[1] = false;
				flag[2] = true;
			}
			if (keyState[KEY_INPUT_A] == 1) {
				flag[0] = false;
				flag[1] = false;
				flag[2] = false;
				opFlag = false;
			}
		}
		if (flag[2]) {
			DrawString(590, 400, "◇", RED);
			DrawRotaGraph(400, 300, 0.5, 0, bl, true);
			while (ScreenFlip() == 0 && getCountFrame() == 0 && !keyState[KEY_INPUT_UP] && !keyState[KEY_INPUT_LEFT] && !keyState[KEY_INPUT_A]) {

				if (GetNowCount() - tempTime > 500)
				{
					switch (count % 4) {
					case 0:c_num = 0; break;
					case 1:c_num = PI / 4; break;
					case 2:c_num = 0; break;
					case 3:c_num = -PI / 4; break;
					}
					count++;
					if (count == 4)count = 0;
				}
				DrawRotaGraph(600, 350, 3.0, c_num, ball[0], true);
				/*500ms(0.5秒)に一度画像更新(歩いているように見える)*/
				if (GetNowCount() - tempTime > 500)
				{
					if (e_num == 1)e_num = 0;
					else e_num = 1;
					tempTime = GetNowCount();
				}
				e = 3;
				DrawRotaGraph(400, 300, 2.5, 0, z[2]->moveTexture[DOWN][e_num], true);
				if (keyState[KEY_INPUT_A] == 1)opFlag = false;
			}
			c_num = 0;
			e_num = 0;
			count = 0;
			DrawRotaGraph(600, 350, 3.0, 0, ball[0], true);
			DrawString(590, 400, "◇", BG);
			if (keyState[KEY_INPUT_UP] == 1) {
				flag[0] = true;
				flag[1] = false;
				flag[2] = false;
			}
			if (keyState[KEY_INPUT_LEFT] == 1) {
				flag[0] = false;
				flag[1] = true;
				flag[2] = false;
			}
			if (keyState[KEY_INPUT_A] == 1) {
				flag[0] = false;
				flag[1] = false;
				flag[2] = false;
				opFlag = false;
			}
		}
	}
	setMainChara(e);
	messageflag = false;
	initMessage();
	SetFontSize(18);
	StopSoundMem(start);

}


void setMainChara(int e) {
	if (e == 1) {
		c->moveTexture[DOWN][0] = LoadGraph("画像/ピカチュウd_1.png");
		c->moveTexture[DOWN][1] = LoadGraph("画像/ピカチュウd_2.png");
		c->moveTexture[UP][0] = LoadGraph("画像/ピカチュウu_1.png");
		c->moveTexture[UP][1] = LoadGraph("画像/ピカチュウu_2.png");
		c->moveTexture[LEFT][0] = LoadGraph("画像/ピカチュウl_1.png");
		c->moveTexture[LEFT][1] = LoadGraph("画像/ピカチュウl_2.png");
		c->moveTexture[RIGHT][0] = LoadGraph("画像/ピカチュウr_1.png");
		c->moveTexture[RIGHT][1] = LoadGraph("画像/ピカチュウr_2.png");
	}
	else if (e == 2) {
		c->moveTexture[DOWN][0] = LoadGraph("画像/ヒノアラシd_1.png");
		c->moveTexture[DOWN][1] = LoadGraph("画像/ヒノアラシd_2.png");
		c->moveTexture[UP][0] = LoadGraph("画像/ヒノアラシu_1.png");
		c->moveTexture[UP][1] = LoadGraph("画像/ヒノアラシu_2.png");
		c->moveTexture[LEFT][0] = LoadGraph("画像/ヒノアラシl_1.png");
		c->moveTexture[LEFT][1] = LoadGraph("画像/ヒノアラシl_2.png");
		c->moveTexture[RIGHT][0] = LoadGraph("画像/ヒノアラシr_1.png");
		c->moveTexture[RIGHT][1] = LoadGraph("画像/ヒノアラシr_2.png");
	}
	else if (e == 3) {
		c->moveTexture[DOWN][0] = LoadGraph("画像/ゼニガメd_1.png");
		c->moveTexture[DOWN][1] = LoadGraph("画像/ゼニガメd_2.png");
		c->moveTexture[UP][0] = LoadGraph("画像/ゼニガメu_1.png");
		c->moveTexture[UP][1] = LoadGraph("画像/ゼニガメu_2.png");
		c->moveTexture[LEFT][0] = LoadGraph("画像/ゼニガメl_1.png");
		c->moveTexture[LEFT][1] = LoadGraph("画像/ゼニガメl_2.png");
		c->moveTexture[RIGHT][0] = LoadGraph("画像/ゼニガメr_1.png");
		c->moveTexture[RIGHT][1] = LoadGraph("画像/ゼニガメr_2.png");
	}

}