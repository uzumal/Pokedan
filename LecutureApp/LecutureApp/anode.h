#pragma once

#define ARRAY_NUM(a) (sizeof(a)/sizeof(a[0]))
#define NODE_MAX 1000

typedef struct {
	int x;
	int y;
	int px;
	int py;
	int cost;
} anode;

struct NODE{
	int i;
	int j;
	int cost;
	struct NODE *parent;
};
typedef struct NODE NODE;

struct LIST {
	NODE* node[NODE_MAX];
	int index;
};
typedef struct LIST LIST;