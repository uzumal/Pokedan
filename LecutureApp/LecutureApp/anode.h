#pragma once
#include "define.h"

struct NODE{
	int y;
	int x;
	int cost;
	int f;
	struct NODE *parent;
};
typedef struct NODE NODE;