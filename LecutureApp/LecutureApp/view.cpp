#include "Map.h"
#include "Character.h"
#include "Enemy.h"
#include "Generic.h"
#include "allVariableName.h"

void allView(int);
void drawCharacter(pokemon*, int);
void drawHpBar();

void allView(int d_num) {

	//マップ描画
	drawMap();
	//ミニマップ描画
	drawMiniMap();

	/*階層、level、HP表示(固定)*/
	DrawFormatString(0, 0, WHITE, "B%dF", m->floor + 1);
	DrawFormatString(50, 0, WHITE, "Lv: %d", c->level);
	DrawFormatString(120, 0, WHITE, "HP: %d/ %d", c->hp, c->maxHp);
	DrawFormatString(360, 0, WHITE, "セットわざ名 : %s", c->skill[c->attackNum].name);

	//hpバー表示
	drawHpBar();

	//キャラ表示
	drawCharacter(c, d_num);
	
	if (m->floor != 2) {
		for (int i = 0; i < ENEMYNUM; i++) {
			drawCharacter(enemy[m->floor][i], d_num);
		}
	}
	else {
		drawCharacter(lastboss, d_num);
	}


}



void drawCharacter(pokemon* me, int d_num) {
	//生きていれば表示
	if (me->isLive) {
		/*座標は画像の真ん中に持つ*/
		/*DrawRotaGraph(x座標,y座標,縮尺度,角度,描画する画像ハンドル,背景透過処理のON,OFF)*/
		DrawRotaGraph(me->x + 20, me->y + 20, 1.5, 0, me->moveTexture[me->direction][d_num], true);
	}
}

void drawHpBar() {
	int hpColor = 0;
	if (c->hp <= c->maxHp / 3) { hpColor = RED; }
	else if (c->hp <= c->maxHp / 2) { hpColor = YELLOW; }
	else { hpColor = GREEN; }
	DrawBox(240, 1, c->maxHp + 240, 15, BLACK, true);
	DrawBox(240, 1, c->hp + 240, 15, hpColor, true);
	DrawBox(239, 0, c->maxHp + 241, 16, WHITE, false);
}