#pragma once
#include "all.h"

extern void showDisplay(char[]);
extern void wait_key(int);
extern int getCountFrame();
extern void wait(int ms, char* s);
extern void wait(int ms);
extern int getRandom(int min, int max);
extern bool isPutMoveKey();
extern void levelUp(int level);
extern bool life(pokemon*, pokemon*);
extern double skillCalc(int, int);