#include "all.h"
#include "allVariableName.h"
#include "Generic.h"

bool endflag = false;

void gameover() {
	sprintf_s(s, "プレイヤーは死んでしまった");
	showDisplay(s);
	int tempTime = 0;
	char h[256];
	sprintf_s(h, "ゲームを終了する:A");
	sprintf_s(s, "リトライする:B");
	tempTime = GetNowCount();
	while (ScreenFlip() == 0 && ClearDrawScreen() == 0 && getCountFrame() == 0) {
		/*if (GetNowCount() - tempTime > 500 && g_num!=3)
		{
		DrawRotaGraph(400, 300, 0.5, 0, over[0], false);
			if (g_num == 1) {
				DrawRotaGraph(400, 300, 0.5, 0, over[1], false);
				g_num = 2;
			}
			else if (g_num == 2) {
				DrawRotaGraph(400, 300, 0.5, 0, over[2], false);
				g_num = 3;
			}
			tempTime = GetNowCount();
		}
		else if (g_num == 3) {
			DrawRotaGraph(400, 300, 0.5, 0, over[2], false);
		}*/
		SetFontSize(26);                             //サイズを26に変更
		SetFontThickness(8);                         //太さを8に変更
		ChangeFont("HGS創英角ﾎﾟｯﾌﾟ体");              //HGS創英角ﾎﾟｯﾌﾟ体に変更
		ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);//アンチエイリアス＆エッジ付きフォントに変更
		DrawRotaGraph(400, 300, 1.5, 0, over[0], false);
		DrawFormatString(270, 470, GetColor(255, 0, 0), "%s", h);
		DrawFormatString(300, 520, GetColor(0, 255, 0), "%s", s);
		if (keyState[KEY_INPUT_A] == 1) { endflag = true; break; }
	}
}