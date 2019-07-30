#pragma once
#include "allVariableName.h"
#include "Generic.h"
#include "Enemy.h"
#include "Character.h"
#include "Init.h"

/*マップ構造体*/
maps mp;
maps* m = &mp;

bool story;

void stairsMove(int stairs) {
	m->floor += stairs;
	if (m->floor != 2) {
		sprintf_s(s, "はじまりのダンジョン\n　　　　B%dF", m->floor + 1);
		wait(2000, s);
		
		while (c->x != SCREEN_WIDTH / 2 || c->y != MAP_HEIGHT/2) {

			if (c->x < SCREEN_WIDTH / 2) { c->x += CHIP_SIZE; m->x++; }
			if (c->x > SCREEN_WIDTH / 2) { c->x -= CHIP_SIZE; m->x--; }
			if (c->y < MAP_HEIGHT / 2) { c->y += CHIP_SIZE; m->y++; }
			if (c->y > MAP_HEIGHT / 2) { c->y -= CHIP_SIZE; m->y--; }

		}
		randomEnemyPut(enemy[m->floor]);
		
	}
	else {
		isReturn = false;
		//BGM止める
		StopSoundMem(bgm);
		//ボス戦BGM
		PlaySoundMem(boss_bgm,DX_PLAYTYPE_LOOP);
		sprintf_s(s, "はじまりのダンジョン\n       最終階");
		wait(4000, s);

		while (c->x != SCREEN_WIDTH / 2 || c->y != MAP_HEIGHT / 2) {

			if (c->x < SCREEN_WIDTH / 2) { c->x += CHIP_SIZE; m->x++; }
			if (c->x > SCREEN_WIDTH / 2) { c->x -= CHIP_SIZE; m->x--; }
			if (c->y < MAP_HEIGHT / 2 ) { c->y += CHIP_SIZE; m->y++; }
			if (c->y > MAP_HEIGHT / 2 ) { c->y -= CHIP_SIZE; m->y--; }

		}
		setDirection(c, DOWN);
		c->y += CHIP_SIZE * 3;
		lastboss->x = c->x;
		lastboss->y = c->y - CHIP_SIZE*3;
		
	}
}

void mapMove(maps* m, pokemon* me, int x, int y) {

	int nextCell = mapping[m->floor][SY(me) - y][SX(me) - x];

	//移動処理と引っ掛かり処理
	if (nextCell > 0 && !keyState[KEY_INPUT_Y]) {
		if (m->floor != 2) {
			for (int i = 0; i < ENEMYNUM; i++) {
				int nextEnemyCell = mapping[m->floor][SY(enemy[m->floor][i]) - y][SX(enemy[m->floor][i]) - x];
				if (nextEnemyCell <= 0)charaMoveEnemy(enemy[m->floor][i], x, y);
			}
		}
		else {
			//マップ移動後の自分のセルが障害物なら
			int nextEnemyCell = mapping[m->floor][SY(lastboss) - y][SX(lastboss) - x];
			if (nextEnemyCell <= 0)charaMoveEnemy(lastboss, x, y);
		}
		m->x += x;
		m->y += y;
	}
}


void drawMap() {
	/*マップ全範囲走査*/
	for (int x = m->x; x < SCREEN_WIDTH / CHIP_SIZE - m->x; x++) {
		for (int y = m->y; y < SCREEN_HEIGHT / CHIP_SIZE - m->y; y++) {
			if (mapping[m->floor][y][x] == 0) { DrawFormatString((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, WHITE, " "); }
			else if (mapping[m->floor][y][x] == 1) { DrawRotaGraph((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, 1, 0, load, true); }
			else if (mapping[m->floor][y][x] == 2) { DrawRotaGraph((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, 1, 0, load2, true); }
			else if (mapping[m->floor][y][x] == 3) { DrawRotaGraph((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, 1, 0, load3, true); }
			else if (mapping[m->floor][y][x] == 100) { DrawRotaGraph((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, 1, 0, stairs_down, true); }
			else if (mapping[m->floor][y][x] == 101) { DrawRotaGraph((m->x + x) * CHIP_SIZE + 20, (m->y + y) * CHIP_SIZE + 20, 1, 0, stairs_up, true); }
		}
	}
}


void drawMiniMap() {

	for (int i = GX - 1; i <= GX + 1; i++) {
		for (int j = GY - 1; j <= GY + 1; j++) {
			miniMapFlag[m->floor][j][i] = 1;
		}
	}

	for (int i = 0; i < MAP_XNUM; i++) {
		for (int j = 0; j < MAP_YNUM; j++) {
			if ((mapping[m->floor][j][i] == 1 || mapping[m->floor][j][i] == 2) && miniMapFlag[m->floor][j][i] == 1)DrawFormatString(i * 3, j * 3, WHITE, ".");
			if (i == GX && j == GY)DrawFormatString(i * 3, j * 3, RED, ".");
			if (m->floor != 2) {
				for (int k = 0; k < ENEMYNUM; k++) {
					if (i == SX(enemy[m->floor][k]) && j == SY(enemy[m->floor][k]) && enemy[m->floor][k]->isLive)DrawFormatString(i * 3, j * 3, BLUE, ".");
				}
			}
			else {
				if (i == SX(lastboss) && j == SY(lastboss) && lastboss->isLive)DrawFormatString(i * 3, j * 3, BLUE, ".");
			}
			if (mapping[m->floor][j][i] == 100)DrawFormatString(i * 3, j * 3, GREEN, ".");
			if (mapping[m->floor][j][i] == 101)DrawFormatString(i * 3, j * 3, GREEN, ".");
		}
	}
}