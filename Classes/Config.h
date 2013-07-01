//
//  Config.h
//  ScrollAndPickUp
//
//  Created by GRENGE-MAC0015 on 13/06/30.
//
//

#ifndef __ScrollAndPickUp__Config__
#define __ScrollAndPickUp__Config__

#include <iostream>


/*** Game Configの設定 ***/
/*** 現在はPhysicsSpriteのみがincludeしている ****/

//プレイヤーの向き
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

//プレイヤーの衝突された方向
#define FRONT 0
#define BACK 1
#define SIDE 2

//物理エンジンの判定の形
#define CIRCLE_SHAPE 0
#define BOX_SHAPE 1

//物理エンジンの1mあたりのピクセル数
#define PTM_RATIO 32

//配置するオブジェクトのタグ
#define TAG_NULL 100
#define TAG_PLAYER 101
#define TAG_ENEMY 102
#define TAG_OBSTACLE 103

//各オブジェクトの出現数
#define PLAYER_NUM 3
#define ENEMY_NUM 8
#define COIN_NUM 50
#define HIT_EF_NUM 10
#define OBSTACLE_NUM 12
#define WATER_NUM 30

//敵オブジェクトの種類
#define KIND_ONION 0
#define KIND_GOBLIN 1
#define KIND_DRAGON 2



#endif /* defined(__ScrollAndPickUp__Config__) */
