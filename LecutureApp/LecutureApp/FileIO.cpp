#include "all.h"
#include "allVariableName.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <string.h>

#define CHARBUFF 256
#define BUFFSIZE 1024

void getCurrentDirectory(char*);
void setPoke(pokemon*);
int readData(pokemon* me, char* fileName, char* outputName, char dataName[BUFFSIZE]);		//CSVファイル読み込み、構造体へ格納


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
/*
void readData(pokemon* me, char* fileName, char* outputName, char dataName[BUFFSIZE]) {

	FILE* fp;			//ファイル用変数
	char s[BUFFSIZE];

	//ポインタを使用して実態を操作

	me->name = dataName;

	//コンマ区切り
	char delim[] = ", ";
	char* ctx;
	char* ctx2;

	char* p1;
	char* p2;


	errno_t error;
	error = fopen_s(&fp, fileName, "r");

	if (error != 0)
		fprintf_s(stderr, "failed to open\n");
	else {
		//ここまでsuccess

		//一行ずつ読み込む
		while (fgets(s, BUFFSIZE, fp) != NULL) {

			//,で区切った1個目
			p1 = strtok_s(s, delim, &ctx);


			//5行飛ばして代入
			if (count >= 5) {
				int c = count - 5;
				p2 = strtok_s(p1, "/", &ctx2);

				csv->year[c] = atoi(p2);			//年を格納、2018

				p2 = strtok_s(NULL, "/", &ctx2);
				M = atoi(p2) - 1;					//M=月-1、配列用の変数

				csv->month[c] = atoi(p2);			//月を格納


				p2 = strtok_s(NULL, "/", &ctx2);
				D = atoi(p2) - 1;					//D=日-1、配列用の変数

				csv->day[M][D] = atoi(p2);			//日を格納


				p1 = strtok_s(NULL, delim, &ctx);

				csv->water[M][D] = atof(p1);

				//2個飛ばしている
				for (int i = 0; i < 3; i++) { p1 = strtok_s(NULL, delim, &ctx); }

				p1 = strtok_s(NULL, delim, &ctx);

				csv->averageTemp[M][D] = atof(p1);		//平均気温をatofでcharをdoubleに変換して代入
				for (int i = 0; i < 3; i++) { p1 = strtok_s(NULL, delim, &ctx); }


			}

			count++;
		}

		fclose(fp);

	}


	//ここまでsuccess

	FILE* fp2;

	int maxMonth = 0;
	int minMonth = 0;
	int maxDay = 0;
	int minDay = 0;

	error = fopen_s(&fp2, outputName, "w");
	if (error != 0)
		fprintf_s(stderr, "failed to open\n");
	else {
		char kk[BUFFSIZE];
		sprintf_s(kk, "%s\n", csv->dataName);
		fputs(kk, fp2);

		int l = 0;
		for (int i = 0; i < ALLMONTH; i++) {			//月を表示
			for (int j = 0; j < allMonth[i]; j++) {		//その月の最大の日まで回す

				double t = csv->averageTemp[i][j];	//そのインスタンスの平均気温を格納
				double w = csv->water[i][j];		//そのインスタンスの降水量を格納

				csv->sumWater += w;

				if (csv->maxTemp < t) {
					csv->maxTemp = t;
					maxMonth = i + 1;
					maxDay = j + 1;
				}
				if (csv->minTemp > t) {
					csv->minTemp = t;
					minMonth = i + 1;
					minDay = j + 1;
				}

				char charYear[BUFFSIZE];
				char charMonth[BUFFSIZE];
				char charDay[BUFFSIZE];
				char charWater[BUFFSIZE];
				char charAverageTemp[BUFFSIZE];

				sprintf_s(charYear, BUFFSIZE, "%d年", csv->year[l++]);
				sprintf_s(charMonth, BUFFSIZE, "%d月", (i + 1));
				sprintf_s(charDay, BUFFSIZE, "%d日", (j + 1));
				sprintf_s(charAverageTemp, BUFFSIZE, "気温 => %4.1f℃ ", t);		//平均気温と改行コード挿入
				sprintf_s(charWater, BUFFSIZE, "降水量 => %4.1fmm \n", w);


				fputs(charYear, fp2);
				fputs(charMonth, fp2);
				fputs(charDay, fp2);
				fputs(charAverageTemp, fp2);
				fputs(charWater, fp2);

			}
		}

		csv->tempDif = csv->maxTemp - csv->minTemp;

		char charSumWater[BUFFSIZE];
		char charTempDif[BUFFSIZE];
		char charMaxTemp[BUFFSIZE];
		char charMinTemp[BUFFSIZE];

		sprintf_s(charMaxTemp, BUFFSIZE, "年間最高気温 => %d月%d日 %4.1f℃\n", maxMonth, maxDay, csv->maxTemp);
		sprintf_s(charMinTemp, BUFFSIZE, "年間最低気温 => %d月%d日 %4.1f℃\n", minMonth, minDay, csv->minTemp);
		sprintf_s(charSumWater, BUFFSIZE, "年間降雨量 => %4.1fmm\n", csv->sumWater);
		sprintf_s(charTempDif, BUFFSIZE, "年間最高気温と最低気温の差 => %4.1f℃\n", csv->tempDif);

		fputs(charMaxTemp, fp2);
		fputs(charMinTemp, fp2);
		fputs(charSumWater, fp2);
		fputs(charTempDif, fp2);

		fclose(fp2);
	}
}*/
