#include "all.h"
#include "allVariableName.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include "Init.h"
#include "Message.h"

#define CHARBUFF 256
#define BUFFSIZE 1024

void getCurrentDirectory(char*);
void setPoke(pokemon*);
int saveData();
int readData();		//txtファイル読み込み、構造体へ格納


void getCurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(CHARBUFF, currentDirectory);
}

void setPoke(pokemon* me) {

	char keyValue[CHARBUFF];
	char currentDirectory[CHARBUFF];
	getCurrentDirectory(currentDirectory);

	char section[CHARBUFF];
	sprintf_s(section, me->name);
	char keyWord[CHARBUFF];
	char pokeFile[CHARBUFF];
	char skillFile[CHARBUFF];
	sprintf_s(pokeFile,CHARBUFF, "%s\\poke.ini", currentDirectory);
	sprintf_s(skillFile,CHARBUFF, "%s\\skill.ini", currentDirectory);
	sprintf_s(keyWord, CHARBUFF, "hp");
	//hpと経験値設定
	
	if (GetPrivateProfileString(section, keyWord, "none",keyValue, CHARBUFF, pokeFile) != 0) {
		me->maxHp = atoi(keyValue);
	}
	else {
		fprintf(stdout, "%s doesn't contain [%s] %s\n", pokeFile, section, keyWord);
	}
	me->hp = me->maxHp;
	
	sprintf_s(keyWord, CHARBUFF, "experience");
	if (GetPrivateProfileString(section, keyWord, "none", keyValue, CHARBUFF, pokeFile) != 0) {
		me->experience = atoi(keyValue);
	}
	else {
		fprintf(stdout, "%s doesn't contain [%s] %s\n", pokeFile, section, keyWord);
	}

	//わざの名前設定
	for (int i = 1; i <= 4; i++) {
		sprintf_s(keyWord,CHARBUFF, "skill%d",i);
		if (GetPrivateProfileString(section, keyWord, "none", keyValue, CHARBUFF, pokeFile) != 0) {
			strcpy_s(me->skill[i-1].name,CHARBUFF,keyValue);
		}
		else {
			fprintf(stdout, "%s doesn't contain [%s] %s\n", pokeFile, section, keyWord);
		}
	}

	//わざの値設定
	for (int i = 0; i < 4; i++) {
		//sprintf_s(section,CHARBUFF, "%s", me->skill[i].name);
		strcpy_s(section, CHARBUFF, me->skill[i].name);
		sprintf_s(keyWord,CHARBUFF, "max");
		if (GetPrivateProfileString(section, keyWord, "none", keyValue, CHARBUFF, skillFile) != 0) {
			me->skill[i].max = atoi(keyValue);
		}
		else {
			fprintf(stdout, "%s doesn't contain [%s] %s\n", pokeFile, section, keyWord);
		}
		sprintf_s(keyWord, CHARBUFF, "min");
		if (GetPrivateProfileString(section, keyWord, "none", keyValue, CHARBUFF, skillFile) != 0) {
			me->skill[i].min = atoi(keyValue);
		}
		else {
			fprintf(stdout, "%s doesn't contain [%s] %s\n", pokeFile, section, keyWord);
		}
		sprintf_s(keyWord, CHARBUFF, "count");
		if (GetPrivateProfileString(section, keyWord, "none", keyValue, CHARBUFF, skillFile) != 0) {
			me->skill[i].maxCount = atoi(keyValue);
		}
		else {
			fprintf(stdout, "%s doesn't contain [%s] %s\n", pokeFile, section, keyWord);
		}
		me->skill[i].count = me->skill[i].maxCount;
	}
}

int saveData() {

	FILE* fp;

	int maxMonth = 0;
	int minMonth = 0;
	int maxDay = 0;
	int minDay = 0;

	errno_t error;
	error = fopen_s(&fp, "saveData.txt", "w");

	if (error != 0)
		return -1;
	else {
		char ss[BUFFSIZE];
		sprintf_s(ss, "名前,最大体力,体力,わざカウント1,2,3,4,セットわざ,レベル,経験値,MAX経験値,x,y,direction,階数,m->x,m->y\n");
		fputs(ss, fp);
		int n = 0;
		if (c->name == "ヒノアラシ")n = 1;
		else if (c->name == "ゼニガメ")n = 2;
		sprintf_s(ss, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",n,c->maxHp,c->hp,c->skill[0].count,c->skill[1].count, c->skill[2].count, c->skill[3].count,c->attackNum,c->level,c->experience,c->Max_ex,c->x,c->y,c->direction,m->floor,m->x,m->y);
		fputs(ss, fp);

		fclose(fp);
		return 0;
	}
}


int readData() {

	FILE* fp;			//ファイル用変数
	char ss[BUFFSIZE];

	//コンマ区切り
	char delim[] = ", ";
	char* ctx;

	char* p1;

	char* fileName = "saveData.txt";

	errno_t error;
	error = fopen_s(&fp, fileName, "r");

	int count = 0;

	if (error != 0)
		return -1;
	else {

		//一行ずつ読み込む
		while (fgets(ss, BUFFSIZE, fp) != NULL) {

			//,で区切った1個目
			p1 = strtok_s(ss, delim, &ctx);

			//1行飛ばして代入
			if (count >= 1) {
				switch (atoi(p1)) {
				case 0:
					c->name = "ピカチュウ";
					break;
				case 1:
					c->name = "ヒノアラシ";
					break;
				case 2:
					c->name = "ゼニガメ";
					break;
				default:
					break;
				}
				charaSet(c);
				c->maxHp = atoi(strtok_s(NULL, delim, &ctx));
				c->hp = atoi(strtok_s(NULL, delim, &ctx));
				c->skill[0].count = atoi(strtok_s(NULL, delim, &ctx));
				c->skill[1].count = atoi(strtok_s(NULL, delim, &ctx));
				c->skill[2].count = atoi(strtok_s(NULL, delim, &ctx));
				c->skill[3].count = atoi(strtok_s(NULL, delim, &ctx));

				c->attackNum = atoi(strtok_s(NULL, delim, &ctx));
				c->level = atoi(strtok_s(NULL, delim, &ctx));
				c->experience = atoi(strtok_s(NULL, delim, &ctx));
				c->Max_ex = atoi(strtok_s(NULL, delim, &ctx));
				c->x = atoi(strtok_s(NULL, delim, &ctx));
				c->y = atoi(strtok_s(NULL, delim, &ctx));
				c->direction = atoi(strtok_s(NULL, delim, &ctx));

				m->floor = atoi(strtok_s(NULL, delim, &ctx));
				m->x = atoi(strtok_s(NULL, delim, &ctx));
				m->y = atoi(strtok_s(NULL, delim, &ctx));
			}

			count++;
		}

		fclose(fp);
		return 0;
	}
}
