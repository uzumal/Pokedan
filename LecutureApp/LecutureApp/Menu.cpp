#pragma once
#include "all.h"
#include "allVariableName.h"

void drawMenu() {
	DrawRotaGraph(550, 160, 1, 0, skillBox, true);
	DrawFormatString(530, 98, WHITE, "%s (%d/%d)", c->skill[0].name, c->skill[0].count, c->skill[0].maxCount);
	DrawFormatString(530, 149, WHITE, "%s (%d/%d)", c->skill[1].name, c->skill[1].count, c->skill[1].maxCount);
	DrawFormatString(530, 199, WHITE, "%s (%d/%d)", c->skill[2].name, c->skill[2].count, c->skill[2].maxCount);
	DrawFormatString(530, 250, WHITE, "%s (%d/%d)", c->skill[3].name, c->skill[3].count, c->skill[3].maxCount);
	if (keyState[KEY_INPUT_1]) { c->attackNum = 0; menuflag = false; }
	else if (keyState[KEY_INPUT_2]) { c->attackNum = 1; menuflag = false; }
	else if (keyState[KEY_INPUT_3]) { c->attackNum = 2; menuflag = false; }
	else if (keyState[KEY_INPUT_4]) { c->attackNum = 3; menuflag = false; }
}

void showMenu() {
	/*‚í‚´•\‚ğÁ‹*/
	if (menuflag && keyState[KEY_INPUT_I] == 1) {
		menuflag = false;
	}

	/*‚í‚´•\‚ğ•\¦*/
	else if (!menuflag && keyState[KEY_INPUT_I] == 1) {
		menuflag = true;
	}

	if (menuflag) {
		drawMenu();
	}
}