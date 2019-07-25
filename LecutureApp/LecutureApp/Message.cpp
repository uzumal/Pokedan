#pragma once
#include "allVariableName.h"
#include "Generic.h"

char message1[256];
char message2[256];
char message3[256];
char s[256];				//sprintf_s用の配列

int messageBox;
bool menuflag = false;
bool messageflag = false;

void showDisplay(char[]);

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



//メッセージ初期化
void initMessage() {
	messageflag = false;
	for (int i = 0; i < 256; i++) {
		message1[i] = '\0';
		message2[i] = '\0';
		message3[i] = '\0';
	}
}


void showDisplay(char ss[]) {
	int sound = LoadSoundMem("音楽/string.mp3");
	char tmp[256];
	//一応初期化
	for (int i = 0; i < 256; i++) { tmp[i] = '\0'; }

	messageflag = true;

	for (int i = 0; ss[i] != '\0'; i++) {
		ScreenFlip();
		wait(10);
		PlaySoundMem(sound, DX_PLAYTYPE_BACK);
		tmp[i] = ss[i];
		initMessage();
		setMessage(tmp);
		outMessage();
	}
	initMessage();
	setMessage(ss);
	outMessage();
	ScreenFlip();
	wait_key(KEY_INPUT_A);
}