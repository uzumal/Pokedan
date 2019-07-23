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

#define WHITE	GetColor(255,255,255)
#define RED		GetColor(255,0,0)
#define GREEN	GetColor(0,255,0)
#define BLUE	GetColor(0,0,255)
#define BLACK	GetColor(0,0,0)


#define SCREEN_WIDTH     (800)                          // 画面の横幅
#define SCREEN_HEIGHT    (600)                          // 画面の縦幅
#define CHIP_SIZE        (40)                           // 一つのチップのサイズ
#define MAP_HEIGHT		 (400)
#define MAP_WIDTH		 (680)


#define WHITE	GetColor(255,255,255)
#define RED		GetColor(255,0,0)
#define GREEN	GetColor(0,255,0)
#define BLUE	GetColor(0,0,255)
#define BLACK	GetColor(0,0,0)

#define ENEMYNUM 3

#define KEY(X,Y) ((X) * 100 + (Y))
#define KEYDATA(X, Y, N) std::pair<int, NODE>(KEY(X,Y), N)		//座標とノードをペアにする


struct pokemon;
struct skill;
struct maps;
struct NODE;
