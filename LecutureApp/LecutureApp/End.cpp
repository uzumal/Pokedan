#include "all.h"
#include "allVariableName.h"
#include "Generic.h"
#include "Message.h"

bool endflag = false;
bool startflag = false;

void gameover() {
	StopSoundMem(bgm);
	StopSoundMem(boss_bgm);
	int overSound = LoadSoundMem("音楽/over.mp3");
	char h[256];
	sprintf_s(s, "プレイヤーは死んでしまった!");
	initMessage();
	messageflag = true;
	setMessage(s);
	outMessage();
	ScreenFlip();
	PlaySoundMem(overSound, DX_PLAYTYPE_NORMAL);
	wait_key(KEY_INPUT_A);
	sprintf_s(h, "ゲームを終了する:A");
	sprintf_s(s, "リトライする:B");
	while (ScreenFlip() == 0 && ClearDrawScreen() == 0 && getCountFrame() == 0) {
		SetFontSize(26);                             //サイズを26に変更
		SetFontThickness(8);                         //太さを8に変更
		ChangeFont("HGS創英角ﾎﾟｯﾌﾟ体");              //HGS創英角ﾎﾟｯﾌﾟ体に変更
		ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);//アンチエイリアス＆エッジ付きフォントに変更
		DrawRotaGraph(400, 300, 1.5, 0, over[0], false);
		DrawFormatString(270, 470, GetColor(255, 0, 0), "%s", h);
		DrawFormatString(300, 520, GetColor(0, 255, 0), "%s", s);
		if (keyState[KEY_INPUT_A] == 1) { endflag = true; break; }
		if (keyState[KEY_INPUT_B] == 1) { break; }
	}
}

void gameClear() {
	StopSoundMem(boss_bgm);
	int clearSound = LoadSoundMem("音楽/clear.mp3");
	char h[256];
	char clear[256];
	sprintf_s(s, "ダンジョンを制覇した!!!");
	initMessage();
	messageflag = true;
	setMessage(s);
	outMessage();
	ScreenFlip();
	wait_key(KEY_INPUT_A);
	sprintf_s(h, "ゲームを終了する:A");
	sprintf_s(s, "リトライする:B");
	sprintf_s(clear, "GameClear!!!");
	PlaySoundMem(clearSound, DX_PLAYTYPE_NORMAL);
	while (ScreenFlip() == 0 && ClearDrawScreen() == 0 && getCountFrame() == 0) {
		SetFontSize(26);                             //サイズを26に変更
		SetFontThickness(8);                         //太さを8に変更
		ChangeFont("HGS創英角ﾎﾟｯﾌﾟ体");              //HGS創英角ﾎﾟｯﾌﾟ体に変更
		ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);//アンチエイリアス＆エッジ付きフォントに変更
		DrawFormatString(SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2, WHITE, "%s", clear);
		DrawFormatString(270, 470, RED, "%s", h);
		DrawFormatString(300, 520, BLUE, "%s", s);
		if (keyState[KEY_INPUT_A] == 1) { endflag = true; break; }
		if (keyState[KEY_INPUT_B] == 1) { break; }
	}
}