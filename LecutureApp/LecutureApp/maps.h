#pragma once
#include "define.h"
#define FLOORNUM 3
#define MAP_XNUM 50
#define MAP_YNUM 50

struct maps {

	int x = 0;
	int y = 0;
	int floor = 0;

};

extern int mapping[FLOORNUM][MAP_YNUM][MAP_XNUM];