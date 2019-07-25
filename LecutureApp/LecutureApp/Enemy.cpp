#pragma once
#include "allVariableName.h"
#include "Message.h"
#include "Generic.h"
#include "Character.h"
#include <map>
#include <stack>


pokemon e1_1;
pokemon e2_1;
pokemon e3_1;
pokemon e1_2;
pokemon e2_2;
pokemon e3_2;
pokemon boss;
pokemon e_null1;
pokemon e_null2;

pokemon* enemy[FLOORNUM][ENEMYNUM] = {
	{ &e1_1 , &e2_1 , &e3_1 },
	{ &e1_2 , &e2_2 , &e3_2 },
	{ &boss , &e_null1 , &e_null2},
};

/*プロトタイプ宣言*/
void turnToPokemon(pokemon* me, pokemon* enemy);
NODE* Astar(pokemon* enemy);
bool isNearPokemon(pokemon* me, pokemon* enemy);
bool isNearPokemon2(pokemon* me, pokemon* enemy);
bool findPokemon(pokemon* me, pokemon* enemy);
void charaMoveEnemy(pokemon* me, int x, int y);
bool isNearEnemy(pokemon* e, int x, int y);
void enemyMove(pokemon* enemy);
void randomEnemyPut(pokemon* e[ENEMYNUM]);
void sortEnemy();


/*攻撃時に敵の方を向く*/
/*のちに向かせる前に画像を敷いてダブリを無くす*/
void turnToPokemon(pokemon* me, pokemon* enemy) {
	if (me->x > enemy->x)me->direction = LEFT;
	else if (me->x < enemy->x)me->direction = RIGHT;
	if (me->y > enemy->y)me->direction = UP;
	else if (me->y < enemy->y)me->direction = DOWN;
}


void setNode(NODE* child, int x, int y, NODE* parent, int cost, int f) {
	child->x = x;
	child->y = y;
	child->parent = parent;
	child->cost = cost;
	child->f = f;
}

int getCost() {
	return 1;
}


//ヒューリスティック関数
//マンハッタン距離
int h(NODE* e, NODE* n) {
	//終点から見た現在のノードまでの推定コストを計算
	int cx = e->x - n->x;
	int cy = e->y - n->y;
	if (cx < 0)cx *= -1;
	if (cy < 0)cy *= -1;

	if (cx < cy) {
		return (cx + cy);
	}
	else {
		return (cy + cx);
	}
}

NODE* Astar(pokemon* enemy) {

	//結果保持用スタック
	std::stack<NODE*> st;
	std::map<int, NODE> openList;
	std::map<int, NODE>closedList;

	NODE s = { 0,0,0 };
	NODE e = { 0,0,0 };


	int key[8][2] = {
		{ 1,0 },
		{ -1,0 },
		{ 0,-1 },
		{ 0,1 },
		{ 1,1 },
		{ -1,1 },
		{ 1,-1 },
		{ -1,-1 },
	};


	//スタート
	//敵の座標
	s.x = SX(enemy);
	s.y = SY(enemy);

	//親ノード、コスト、評価値なし
	setNode(&s, s.x, s.y, NULL, 0, 0);
	//open.node[open.index++] = &s;
	//オープンリストに代入
	openList.insert(KEYDATA(s.x, s.y, s));

	//ゴール
	//自分の座標
	e.x = GX;
	e.y = GY;

	while (true) {

		// openリストがなくなったので終了する
		if (openList.empty()) { return NULL; }

		int f_min = 9999;

		//展開用ノード
		NODE *p = new NODE;

		auto it = openList.begin();		//先頭イテレータ取得
		std::map<int, NODE>::iterator it_min;
		//最小ノード取得
		while (it != openList.end()) {
			if (f_min > it->second.f) {
				f_min = it->second.f;
				it_min = it;
			}
			it++;
		}

		setNode(
			p,
			it_min->second.x, it_min->second.y,
			it_min->second.parent,
			it_min->second.cost, it_min->second.f
		);


		//親ノードをオープンリストから削除
		openList.erase(KEY(p->x, p->y));
		//親ノードをクローズドリストに挿入
		closedList.insert(KEYDATA(p->x, p->y, *p));

		// もしゴールならただちに終了する
		//ゴールは8方に設定
		if ((p->x == e.x && (p->y == e.y + 1 || p->y == e.y - 1)) || (p->y == e.y && (p->x == e.x + 1 || p->x == e.x - 1))) {

			st.push(p);
			p = p->parent;
			while (p->parent != NULL) {
				st.push(p);
				p = p->parent;
			}

			break;
		}

		//子ノード展開
		for (int i = 0; i < 8; i++) {

			//子ノード評価用
			NODE* child = new NODE;


			//周囲8マスを計算
			int cx = p->x + key[i][0];
			int cy = p->y + key[i][1];

			//0以下は通れないので飛ばす
			if (mapping[m->floor][cy][cx] <= 0)continue;

			//ノードの実体(座標)
			child->x = cx;
			child->y = cy;

			//仮のコスト、評価値計算
			int cost = p->cost + getCost();
			int f = cost + h(&e, child);

			//最小コストの親のはずなので、先に親を入れてしまう
			setNode(child, cx, cy, p, cost, f);

			//オープンリスト
			if (openList.count(KEY(cx, cy))) {
				auto open_child = openList.find(KEY(cx, cy));
				//評価値が小さい方の情報を使用
				if (child->f < open_child->second.f) {
					openList[KEY(cx, cy)].cost = child->cost;
					openList[KEY(cx, cy)].f = child->f;
					openList[KEY(cx, cy)].parent = child->parent;
				}
			}
			//クローズドリスト
			else if (closedList.count(KEY(cx, cy))) {
				auto closed_child = closedList.find(KEY(cx, cy));
				//評価値が小さい方の情報を使用
				if (child->f < closed_child->second.f) {
					closed_child->second.cost = child->cost;
					closed_child->second.f = child->f;
					closed_child->second.parent = child->parent;

					//オープンリストに戻す
					openList.insert(KEYDATA(cx, cy, closed_child->second));
					closedList.erase(KEY(cx, cy));
				}

			}
			else {//オープンリストにもクローズドリストにも含まれていない
				openList.insert(KEYDATA(cx, cy, *child));
			}



		}
	}

	NODE* node = st.top();


	return node;
}

/*周囲のマスに敵が居るか居ないか* /
/*攻撃時に使用する*/
/*ななめを抜く*/
/*4方ver*/
bool isNearPokemon(pokemon* me, pokemon* enemy) {
	if ((me->y == enemy->y && (me->x - CHIP_SIZE == enemy->x || me->x + CHIP_SIZE == enemy->x)) || (me->x == enemy->x && (me->y - CHIP_SIZE == enemy->y || me->y + CHIP_SIZE == enemy->y))) {
		return true;
	}
	return false;
}

/*8方ver*/
bool isNearPokemon2(pokemon* me, pokemon* enemy) {
	if (me->x - CHIP_SIZE <= enemy->x && me->x + CHIP_SIZE >= enemy->x && me->y - CHIP_SIZE <= enemy->y && me->y + CHIP_SIZE >= enemy->y) {
		return true;
	}
	return false;
}

/*見つけているかどうか、見つけていればA*で追う*/
bool findPokemon(pokemon* me, pokemon* enemy) {
	if ((me->x - SCREEN_WIDTH / 2 < enemy->x) && (me->x + SCREEN_WIDTH / 2 > enemy->x) && (me->y + SCREEN_HEIGHT / 2 > enemy->y) && (me->y - SCREEN_HEIGHT / 2 < enemy->y))return true;
	else return false;
}

/*敵用の処理,入れ替わり処理を無くした*/
void charaMoveEnemy(pokemon* me, int x, int y) {

	if (x == 1)setDirection(me, RIGHT);
	else if (x == -1)setDirection(me, LEFT);

	if (y == 1)setDirection(me, DOWN);
	else if (y == -1)setDirection(me, UP);

	int tmpX = (me->x - c->x) / CHIP_SIZE;
	int tmpY = (me->y - c->y) / CHIP_SIZE;
	int nextCell_x = mapping[m->floor][SY(me)][SX(me) + x];
	int nextCell_y = mapping[m->floor][SY(me) + y][SX(me)];

	//ななめ移動時に、ななめに主人公がいれば
	if ((x == 1 || x == -1) && (y == 1 || y == -1)) {
		if ((tmpX == 1 || tmpX == -1) && (tmpY == 1 || tmpY == -1)) {
			//攻撃できる範囲に移動する
			if (nextCell_x > 0)y = 0;
			else if (nextCell_y > 0)x = 0;
		}
	}

	/*移動先のセル確認*/
	int nextCell = mapping[m->floor][SY(me) + y][SX(me) + x];

	if (nextCell > 0 && !isNearEnemy(me, x, y)) {
		me->x += x * CHIP_SIZE;
		me->y += y * CHIP_SIZE;
	}
}

/*敵(仲間)が移動先にいるか*/
/*位置被りを無くすために使用*/
bool isNearEnemy(pokemon* e, int x, int y) {

	for (int i = 0; i < ENEMYNUM; i++) {
		if (enemy[m->floor][i] != e) {
			//移動先に既に敵がいる場合
			if (enemy[m->floor][i]->isLive) {
				if (SX(e) + x == SX(enemy[m->floor][i]) && (SY(e) + y == SY(enemy[m->floor][i])))return true;
			}
		}
	}
	return false;

}
/*敵の動き*/
void enemyMove(pokemon* enemy) {

	//死んでいたら何もしない
	if (!enemy->isLive)return;

	//自分の座標取得
	int sx = enemy->x / CHIP_SIZE - m->x;
	int sy = enemy->y / CHIP_SIZE - m->y;

	/*敵が同じマップ内にいると、自分に向かってくる*/
	if (findPokemon(enemy, c)) {

		/*4方にいるとき、攻撃する*/
		if (isNearPokemon(enemy, c)) {
			turnToPokemon(enemy, c);
			enemy->attackNum = getRandom(0, 3);
			attack(enemy, c);		//ランダムでわざを選ぶ
		}
		/*8方(ここではななめ)にいるとき、攻撃できる範囲に移動する*/
		else if (isNearPokemon2(enemy, c)) {
			charaMoveEnemy(enemy, (c->x - enemy->x) / CHIP_SIZE, (c->y - enemy->y) / CHIP_SIZE);	//攻撃できるところに移動する
		}
		/*8方にいないときはA*アルゴリズム*/
		else if (!isNearPokemon2(enemy, c)) {
			NODE* nextEnemyNode = Astar(enemy);
			charaMoveEnemy(enemy, nextEnemyNode->x - SX(enemy), nextEnemyNode->y - SY(enemy));
		}
	}
	/*まだ対象が見つかっていない場合、うろうろする*/
	else {
		switch (getRandom(0, 8)) {
		case LEFT:
			charaMoveEnemy(enemy, -1, 0);
			break;
		case RIGHT:
			charaMoveEnemy(enemy, 1, 0);
			break;
		case UP:
			charaMoveEnemy(enemy, 0, -1);
			break;
		case DOWN:
			charaMoveEnemy(enemy, 0, 1);
			break;
		case LEFT_UP:
			charaMoveEnemy(enemy, -1, -1);
			break;
		case LEFT_DOWN:
			charaMoveEnemy(enemy, -1, 1);
			break;
		case RIGHT_UP:
			charaMoveEnemy(enemy, 1, -1);
			break;
		case RIGHT_DOWN:
			charaMoveEnemy(enemy, 1, 1);
			break;
		default:
			break;
		}
	}
}

void randomEnemyPut(pokemon* e[ENEMYNUM]) {
	bool flag = 0;
	for (int i = 0; i < ENEMYNUM; i++) {
		while (true) {
			flag = true;
			e[i]->x = (getRandom(5, 30)+m->x) * CHIP_SIZE;
			e[i]->y = (getRandom(5, 30)+m->y) * CHIP_SIZE;
			if (mapping[m->floor][SY(e[i])][SX(e[i])] > 0) {
				//被りを無くす
				for (int j = 0; j < i; j++) {
					if ((SX(e[i]) == SX(e[j]) && SY(e[i]) == SY(e[j])) || (SX(e[i]) == GX && SY(e[i]) == GY))flag = false;
				}
				if (flag)break;
			}
		}
	}
}

void sortEnemy() {
	pokemon* enemy_tmp[ENEMYNUM];
	bool enFlag = false;
	int sx = 0;
	int sy = 0;
	for (int i = 0; i < ENEMYNUM; i++) {
		int distance = 9999;
		for (int j = 0; j < ENEMYNUM; j++) {
			enFlag = false;
			sx = GX - SX(enemy[m->floor][j]);
			if (sx < 0)sx *= -1;
			sy = GY - SY(enemy[m->floor][j]);
			if (sy < 0)sy *= -1;
			//一番近い敵からenemy_tmpに格納していく
			if (distance > sx + sy) {
				//最初は被りがないので素直に一番近い敵を格納
				if (i == 0) {
					distance = sx + sy;
					enemy_tmp[i] = enemy[m->floor][j];
				}

				//二回目以降は被りを無視する
				else {
					//既にenemy_tmpに含まれている要素は無視する
					for (int k = 0; k < ENEMYNUM; k++) {
						if (enemy[m->floor][j] == enemy_tmp[k])enFlag = true;
					}
					if (enFlag)continue;
					distance = sx + sy;
					enemy_tmp[i] = enemy[m->floor][j];
				}
			}
		}
	}
	for (int i = 0; i < ENEMYNUM; i++) {
		enemy[m->floor][i] = enemy_tmp[i];
	}
}

void enemyMapMove(int tmp_mx,int tmp_my) {
	pokemon* tmp = NULL;

	//入れ替わり処理を優先させる
	for (int i = 0; i < ENEMYNUM; i++) {
		if (enemy[m->floor][i]->hp != 0) {
			if (GX == SX(enemy[m->floor][i]) + (m->x - tmp_mx) && GY == SY(enemy[m->floor][i]) + (m->y - tmp_my)) {
				charaMoveEnemy(enemy[m->floor][i], m->x - tmp_mx, m->y - tmp_my);
				//敵がいても無理やり移動するため、charaMoveEnemyは使わない
				enemy[m->floor][i]->x += (m->x - tmp_mx) * CHIP_SIZE;
				enemy[m->floor][i]->y += (m->y - tmp_my) * CHIP_SIZE;
				/*
				if (mapping[m->floor][SY(enemy[m->floor][i])][SX(enemy[m->floor][i])] <= 0) {
					enemy[m->floor][i]->x -= (m->x - tmp_mx) * CHIP_SIZE;
					enemy[m->floor][i]->y -= (m->y - tmp_my) * CHIP_SIZE;
				}*/

				turnToPokemon(enemy[m->floor][i], c);
				attack(enemy[m->floor][i], c);
				tmp = enemy[m->floor][i];
			}
		}
	}

	for (int i = 0; i < ENEMYNUM; i++) {
		if (tmp == enemy[m->floor][i])continue;
		if (enemy[m->floor][i]->hp != 0) {
			if (isNearEnemy(enemy[m->floor][i], 0, 0) && mapping[m->floor][SY(enemy[m->floor][i])][SX(enemy[m->floor][i])] > 0) {
				//敵がいても無理やり移動するため、charaMoveEnemyは使わない
				enemy[m->floor][i]->x += (m->x - tmp_mx) * CHIP_SIZE;
				enemy[m->floor][i]->y += (m->y - tmp_my) * CHIP_SIZE;
			}
		}
	}

	for (int i = 0; i < ENEMYNUM; i++) {
		if (tmp == enemy[m->floor][i])continue;
		if (enemy[m->floor][i]->hp != 0) {
			charaMoveEnemy(enemy[m->floor][i], m->x - tmp_mx, m->y - tmp_my);
			//移動後、前、右、左のどちらかにいたとき
			if (isNearPokemon(enemy[m->floor][i], c) && (GX + (m->x - tmp_mx) != SX(enemy[m->floor][i]) && GY + (m->y - tmp_my) != SY(enemy[m->floor][i]))) {
				turnToPokemon(enemy[m->floor][i], c);
				attack(enemy[m->floor][i], c);
			}
			//ななめにいるとき
			else if (!isNearPokemon(enemy[m->floor][i], c) && isNearPokemon2(enemy[m->floor][i], c)) {
				charaMoveEnemy(enemy[m->floor][i], (c->x - enemy[m->floor][i]->x) / CHIP_SIZE, (c->y - enemy[m->floor][i]->y) / CHIP_SIZE);
			}
			//8方にいるとき
			else if (isNearPokemon(enemy[m->floor][i], c)) {
				turnToPokemon(enemy[m->floor][i], c);
			}

			else {
				NODE* n = Astar(enemy[m->floor][i]);
				if (n != NULL)charaMoveEnemy(enemy[m->floor][i], n->x - SX(enemy[m->floor][i]), n->y - SY(enemy[m->floor][i]));
			}
		}
	}
}

void mainEnemyMove(int tmp_mx,int tmp_my) {
	if (!menuflag && isPutMoveKey() && tmp_mx == m->x && tmp_my == m->y) {
		if (keyState[KEY_INPUT_Y] == 0) {
			sortEnemy();

			for (int i = 0; i < ENEMYNUM; i++) {
				enemyMove(enemy[m->floor][i]);
			}
		}
	}
	/*マップ移動時*/
	else if (!menuflag && isPutMoveKey() && (tmp_mx != m->x || tmp_my != m->y)) {

		sortEnemy();
		enemyMapMove(tmp_mx, tmp_my);
	}
}