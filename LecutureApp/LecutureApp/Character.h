#pragma once
#include "all.h"

extern void mainCharaMove();
extern void setDirection(pokemon* me, int direction);
extern void charaMove(pokemon* me, int x, int y);
extern void attack(pokemon* me, pokemon* enemy);
extern void attack_for(pokemon* me);
extern void moveJump(pokemon* me);
extern pokemon* getFrontEnemy();