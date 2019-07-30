#include "Map.h"
#include "Character.h"
#include "Enemy.h"
#include "Generic.h"
#include "allVariableName.h"
#include "Message.h"

void allView(int);
void drawCharacter(pokemon*, int);
void drawHpBar();
void drawExplain();

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
	DrawBox(250, 1, c->maxHp + 250, 15, BLACK, true);
	DrawBox(250, 1, c->hp + 250, 15, hpColor, true);
	DrawBox(249, 0, c->maxHp + 251, 16, WHITE, false);
}


void drawExplain() {
	bool explain = false;
	setMessage("操作説明は必要ですか?");
	setMessage("YES→Y");
	setMessage("NO→N");
	outMessage();
	while (getCountFrame() == 0 && ScreenFlip() == 0 && ProcessMessage() == 0) {
		if (keyState[KEY_INPUT_Y]) { explain = true; break; }
		if (keyState[KEY_INPUT_N]) { explain = false; break; }
	}
	if (explain) {
		showMessage("操作説明");
		showMessage("Q,W,E,A,D,Z,X,Cでそれぞれの方向に動きます");
		showMessage("Yを押しながら先程の移動キーを押すと方向だけ変えます");
		showMessage("Iでわざメニューを開け、数字でわざを選べます");
		showMessage("Jでセットしたわざで敵に攻撃できます");
		showMessage("敵の方を向いていないと攻撃が当たらないので注意してください");
		showMessage("spaceキーを押すと強制的にゲームを終了できます");
		showMessage("Kでセーブができます");
		showMessage("敵はある一定の近さになると追ってきます");
		showMessage("それ以外はランダムに動いています");
		showMessage("最下層に居るボスモンスターを倒すとゲームクリアとなります");
		showMessage("それではポケダンをお楽しみください");
	}
	initMessage();
}