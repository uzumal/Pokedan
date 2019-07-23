#pragma once
#include "all.h"

extern int getCountFrame();
extern void wait(int ms, char* s);
extern void wait(int ms);
extern int getRandom(int min, int max);
extern bool isPutMoveKey();
extern int init();
extern void skillfull(int level);
extern void drawCharacter(pokemon* me, int d_num);