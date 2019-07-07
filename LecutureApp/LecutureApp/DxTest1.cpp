#include "DxLib.h"
#include "pokemon.h"
#include "thread"
#include "mutex"

#define DOWN	0
#define UP		1
#define LEFT	2
#define RIGHT	3
#define SCREEN_WIDTH     (800)                          // 画面の横幅
#define SCREEN_HEIGHT    (400)                          // 画面の縦幅
#define CHIP_SIZE        (48)                           // 一つのチップのサイズ
#define MAP_WIDTH        (SCREEN_WIDTH / CHIP_SIZE)     // マップの横幅
#define MAP_HEIGHT       (SCREEN_HEIGHT / CHIP_SIZE)    // マップの縦幅

/*押されているキーを格納*/
char keyState[256];

/*ダメージを与えたか与えていないか*/
bool damage = false;

int enemyDirection = 0;

bool messageflag = false;

/*ジャンプ用変数*/
int temp = 0;
int y_temp = 0;
int y_prev = 0;

/*ジャンプ用フラグ*/
bool onGround = false;		//ジャンプする瞬間
bool jump = false;			//ジャンプしているかどうか	

/*コンソール表示用変数*/
char message1[1024];
char message2[1024];
char message3[1024];

/*ポケモン構造体*/
pokemon poke;
pokemon dark;
pokemon* c = &poke;
pokemon* d = &dark;


void initConsole() {

	/*メッセージボックス*/
	int messageBox = LoadGraph("画像/messageBox.png");

	if (messageflag) {
		DrawRotaGraph(SCREEN_WIDTH / 2, 300, 1, 0, messageBox, true);
	}
}


void setMessage(char s[]) {

	char temp[1024];
	/*初期化*/
	for (int i = 0; i < 1024; i++)temp[i] = '\0';

	/*文字列コピー*/
	for (int i = 0; s[i] != '\0'; i++) { temp[i] = s[i]; }

	if (message1[0] == '\0')for (int i = 0; temp[i] != '\0'; i++) { message1[i] = temp[i]; }
	else if (message2[0] == '\0')for (int i = 0; temp[i] != '\0'; i++) { message2[i] = temp[i]; }
	else if (message3[0] == '\0')for (int i = 0; temp[i] != '\0'; i++) { message3[i] = temp[i]; }
	else {
		for (int i = 0; i < 1024; i++) { message1[i] = '\0'; }
		for (int i = 0; i < 1024; i++) { message1[i] = message2[i]; }

		for (int i = 0; i < 1024; i++)message2[i] = '\0';
		for (int i = 0; i < 1024; i++) { message2[i] = message3[i]; }

		for (int i = 0; i < 1024; i++) { message3[i] = temp[i]; }
	}
}


void outMessage() {

	initConsole();
	/*コンソール表示*/
	DrawFormatString(160, 287, GetColor(255, 255, 255), "%s", message1);
	DrawFormatString(160, 317, GetColor(255, 255, 255), "%s", message2);
	DrawFormatString(160, 347, GetColor(255, 255, 255), "%s", message3);
}

/*ジャンプ処理*/
void moveJump() {
	int f;
	/*ジャンプした瞬間*/
	if (onGround) {
		/*上への当たり判定*/
		if (c->y >= 140) {
			f = -30;
			y_temp = c->y;
			c->y += (c->y - y_prev) + f;
			y_prev = y_temp;
		}
		else {
			y_temp = 30;
			c->y = 30;
			y_prev = y_temp;
		}
	}
	/*空中の処理*/
	else {
		f = 3;
		y_temp = c->y;
		c->y += (c->y - y_prev) + f;
		y_prev = y_temp;
		/*元の位置に戻ればジャンプ処理終了*/
		if (c->y >= temp) {
			c->y = temp;
			jump = false;
		}
	}
	onGround = false;
}


/*キーが押されているフレーム数によって表示する画像を変更する*/
/*
int getDnum(int key) {
	if (keyState[key] % 60 >= 1 && keyState[key] % 60 < 30) return 0;
	else return 1;
}
*/


bool isNearEnemy(pokemon* pika,pokemon* dark) {
	if ((pika->x - 48 <= dark->x) && (pika->x + 48 >= dark->x) && (pika->y - 48 <= dark->y) && (pika->y + 48 >= dark->y))return true;
	else return false;
}

/*仮の敵の動き*/
void enemyMove() {

	char s[1024];

	if (!isNearEnemy(c, d)) {
		if (c->x - 48 < d->x) {
			d->x -= CHIP_SIZE;
			enemyDirection = LEFT;
		}
		if (c->x + 48 > d->x) {
			d->x += CHIP_SIZE;
			enemyDirection = RIGHT;
		}
		if (c->y - 48 < d->y) {
			d->y -= CHIP_SIZE;
			enemyDirection = UP;
		}
		if (c->y + 48 > d->y) {
			d->y += CHIP_SIZE;
			enemyDirection = DOWN;
		}
	}
	else {
		if (c->hp > 0 && d->hp>0) {
			c->hp -= 1;
			sprintf_s(s, "%sの攻撃!%sに%dのダメージ!%sのHP:%d", d->name, c->name, 1,c->name,c->hp);
			messageflag = true;
			setMessage(s);
			outMessage();
		}
	}
}

/*キーが押されているフレーム数を数える*/
int getCountFrame() {
	char tmpKey[256]; // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
	for (int i = 0; i<256; i++) {
		if (tmpKey[i] != 0) { // i番のキーコードに対応するキーが押されていたら
			keyState[i]++;     // 加算
			if (keyState[i] == 60)keyState[i] = 1;	//移動時の画像処理に使用
		}
		else {              // 押されていなければ
			keyState[i] = 0;   // 0にする
		}
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	/*ウインドウの大きさ指定*/
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16);
	/*背景色描画(後にマップ)*/
	SetBackgroundColor(100, 100, 100);
	/*ウィンドウモードに指定*/
	ChangeWindowMode(TRUE);
	if (DxLib_Init() != 0) { return -1; }	// DXライブラリ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);			//描画先を裏画面に設定
	
	/*後々自分の名前を入力させる*/
	c->name = "ピカチュウ";
	d->name = "ダークライ";

	/*画像のロード*/
	c->moveTexture[DOWN][0] = LoadGraph("画像/ピカチュウd_1.png");
	c->moveTexture[DOWN][1] = LoadGraph("画像/ピカチュウd_2.png");
	c->moveTexture[UP][0] = LoadGraph("画像/ピカチュウu_1.png");
	c->moveTexture[UP][1] = LoadGraph("画像/ピカチュウu_2.png");
	c->moveTexture[LEFT][0] = LoadGraph("画像/ピカチュウl_1.png");
	c->moveTexture[LEFT][1] = LoadGraph("画像/ピカチュウl_2.png");
	c->moveTexture[RIGHT][0] = LoadGraph("画像/ピカチュウr_1.png");
	c->moveTexture[RIGHT][1] = LoadGraph("画像/ピカチュウr_2.png");	

	d->moveTexture[DOWN][0] = LoadGraph("画像/ダークライd_1.png");
	d->moveTexture[DOWN][1] = LoadGraph("画像/ダークライd_2.png");
	d->moveTexture[UP][0] = LoadGraph("画像/ダークライu_1.png");
	d->moveTexture[UP][1] = LoadGraph("画像/ダークライu_2.png");
	d->moveTexture[LEFT][0] = LoadGraph("画像/ダークライl_1.png");
	d->moveTexture[LEFT][1] = LoadGraph("画像/ダークライl_2.png");
	d->moveTexture[RIGHT][0] = LoadGraph("画像/ダークライr_1.png");
	d->moveTexture[RIGHT][1] = LoadGraph("画像/ダークライr_2.png");
	
	/*白色の透過*/
	SetTransColor(255, 255, 255);

	/*ジャンプの速度変化に使用*/
	int f = 0;

	/*座標位置*/
	c->x = CHIP_SIZE*3;
	c->y = CHIP_SIZE*3;

	d->x = CHIP_SIZE*10;
	d->y = CHIP_SIZE*2;

	int direction = DOWN;
	int d_num = 0;

	int localeTemp = 0;

	/*フラグ*/
	bool endflag = false;		//qを押すと終わり

	int tempTime = 0;
	int currentTime = 0;
	SetWaitVSyncFlag(FALSE);

	char s[1024];

	currentTime = GetNowCount();

	/*描画する*/
	// while(裏画面を表画面に反映, メッセージ処理, 画面クリア,フレームカウント)
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && getCountFrame() == 0) {

		/*500ms(0.5秒)に一度画像更新*/
		if (currentTime - tempTime> 500) 
		{	
			if (d_num == 1)d_num = 0;
			else d_num = 1;
			tempTime = GetNowCount(); 
		}

		currentTime = GetNowCount();

		/*階層、level、HP表示(固定)*/
		DrawFormatString(0, 0, GetColor(255, 255, 255), "B1F");
		DrawFormatString(50, 0, GetColor(255, 255, 255), "Lv: %d",c->level);
		DrawFormatString(120, 0, GetColor(255, 255, 255), "HP: %d/ %d",c->hp,c->maxHp);
		

		/*DrawRotaGraph(x座標,y座標,縮尺度,角度,描画する画像ハンドル,背景透過処理のON,OFF)*/
		/*座標は画像の真ん中に持つ*/
		DrawRotaGraph(c->x, c->y, 1.5, 0, c->moveTexture[direction][d_num], true);

		outMessage();

		/*生きてたら敵表示*/
		if (d->hp > 0) { DrawRotaGraph(d->x, d->y, 1.5, 0, d->moveTexture[enemyDirection][d_num], true); }
		
		/*死亡確認*/
		if (d->hp == 0 && d->isLive) { sprintf_s(s, 1024, "%sは倒れた!", d->name); messageflag = true; setMessage(s); outMessage(); d->isLive = false; }

		/*qキーで終わり*/
		if (keyState[KEY_INPUT_Q] != 0) { endflag = true; }
		


		/*Attack*/
		if (!jump && keyState[KEY_INPUT_Z] == 1) {
			if (isNearEnemy(c, d) && d->isLive) {
				d->hp -= 1;
				sprintf_s(s, 1024, "%sの攻撃!%sに%dのダメージ!%sのHP:%d", c->name, d->name, 1, d->name, d->hp);
				messageflag = true;
				setMessage(s);
				initConsole();
				outMessage();
			}
			enemyMove();
		}
		
		/*斜め移動も入れるならelse ifじゃなくてifだけにする*/
		/*ダンジョンの移動ぽくするならx+=CHIP_SIZE*/
		/*ぬるぬる動かすならx++*/
		/*自分が動けば敵も動く*/
		/*Right*/
		else if (keyState[KEY_INPUT_D]==1) {
			direction = RIGHT;
			if (c->x < SCREEN_WIDTH-24 && (!(d->x == c->x + 48 && d->y == c->y)||!d->isLive)) {
				if (keyState[KEY_INPUT_B])c->x += CHIP_SIZE;
				else c->x += CHIP_SIZE;
			}
			enemyMove();
		}

		/*Left*/
		else if (keyState[KEY_INPUT_A]==1) { 
			direction = LEFT;
			if (c->x > 24 && (!(d->x == c->x - 48 && d->y == c->y)||!d->isLive)) {
				if (keyState[KEY_INPUT_B])c->x -= 3;
				else c->x-=CHIP_SIZE;
			}
			enemyMove();
		}

		/*Up*/
		else if (!jump && keyState[KEY_INPUT_W]==1) {
			direction = UP;
			if (c->y > 24 && (!(d->y == c->y -48 && d->x == c->x)||!d->isLive)) {
				if (keyState[KEY_INPUT_B])c->y -= 3;
				else c->y-=CHIP_SIZE;
			}
			enemyMove();
		}

		/*Down*/
		else if (!jump && keyState[KEY_INPUT_X]==1) {
			direction = DOWN;
			if (c->y < SCREEN_HEIGHT - 24 && (!(d->y == c->y + 48 && d->x == c->x)||!d->isLive)) {
				if (keyState[KEY_INPUT_B])c->y += 3;
				else c->y+=CHIP_SIZE;
			}
			enemyMove();
		}

		/*おまけのジャンプ処理*/
		if (!jump && keyState[KEY_INPUT_SPACE] == 1) { 
			y_prev = c->y; temp = c->y;
			jump = true;  onGround = true;
		}

		/*ジャンプ処理本体*/
		if (jump) {
			moveJump();
		}

		if (endflag) {
			break;
		}
	}
	DxLib_End();    // DXライブラリ終了処理
	return 0;
}