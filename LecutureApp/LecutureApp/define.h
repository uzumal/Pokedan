#pragma once

#define DOWN		0
#define UP			1
#define LEFT		2
#define RIGHT		3
#define RIGHT_DOWN	4
#define RIGHT_UP	5
#define LEFT_DOWN	6
#define LEFT_UP		7
#define CENTER		8

#define SCREEN_WIDTH     (800)                          // 画面の横幅
#define SCREEN_HEIGHT    (600)                          // 画面の縦幅
#define CHIP_SIZE        (40)                           // 一つのチップのサイズ
#define MAP_HEIGHT		 (400)
#define MAP_WIDTH		 (680)

#define SCREEN_WIDTH     (800)                          // 画面の横幅
#define SCREEN_HEIGHT    (600)                          // 画面の縦幅
#define CHIP_SIZE        (40)                           // 一つのチップのサイズ
#define MAP_HEIGHT		 (400)
#define MAP_WIDTH		 (680)


//色の定義
#define WHITE	GetColor(255,255,255)
#define RED		GetColor(255,0,0)
#define GREEN	GetColor(0,255,0)
#define BLUE	GetColor(0,0,255)
#define YELLOW	GetColor(255,242,0)
#define BLACK	GetColor(0,0,0)
#define BG		GetColor(20,20,20)


//敵キャラ
#define ENEMYNAME1	"フシギダネ"
#define ENEMYNAME2	"ディグダ"
#define ENEMYNAME3	"グラードン"
#define ENEMYNAME4	"ダークライ"
#define ENEMYNAME5	"ボスゴドラ"
#define ENEMYNAME6	"スイクン"
#define BOSSNAME	"ミュウツー"

//メインキャラ、ここを書き換えると使用できるキャラが変わる
#define MAINCHARANAME1 "ピカチュウ"
#define MAINCHARANAME2 "ヒノアラシ"
#define MAINCHARANAME3 "ゼニガメ"

//タイプ
#define NORMAL 0
#define FIRE 1
#define WATER 2
#define ELECTRIC 3
#define GLASS 4
#define ICE 5
#define FIGHT 6
#define POISON 7
#define GROUND 8
#define FLIGHT 9
#define ESPER 10
#define INSECT 11
#define ROCK 12
#define GHOST 13
#define DRAGON 14
#define EVIL 15
#define STEEL 16

//歩いて回復するまでにかかるターン数
#define RECOVERYTURN 5

#define ENEMYNUM 3
#define PI 3.141592654

#define KEY(X,Y) ((X) * 100 + (Y))
#define KEYDATA(X, Y, N) std::pair<int, NODE>(KEY(X,Y), N)		//座標とノードをペアにする


struct pokemon;
struct skill;
struct maps;
struct NODE;
