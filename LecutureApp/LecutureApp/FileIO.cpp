#include "all.h"
#include "allVariableName.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <string.h>

#define CHARBUFF 256
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
