//
//  Water.h
//  ScrollAndPickUp
//
//  Created by Kenji Kawanabe on 2013/06/25.
//
//

#ifndef __ScrollAndPickUp__Warter__
#define __ScrollAndPickUp__Warter__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#define TAG_COIN 6
#define W_GRAVITY -1
#define W_FIRST_SPEED 20
#define SIDE_SPEED 5
#define W_BOUND_HEIGHT 100

class Water
{
private:
    // 移動量
    int speedX;
    int speedY;
    
    // 今見えるかどうか
    bool isVisible;
    
    // スプライト
    cocos2d::CCSprite *sprite;
    
    // 経過フレーム数
    int frameCount;
    
public:
    Water();
    
    // プレイヤーに集まる状態
    bool isCorrect;
    
    // 追いかけるプレイヤーのインデックス
    int tarIndex;
    
    cocos2d::CCSprite* getSprite();
    
    // バウンドする地面の高さ
    int groundHeight;
    
    // 表示状態の取得
    bool getVisible();
    
    // スプライトの座標を取得
    cocos2d::CCPoint getPosition();
    
    // バウンド線の高さを取得
    int getGroundHeight();
    
    // 表示状態をセット
    void setVisible(bool, int);
    
    // スプライトの座標をセット
    void setPosition(float, float);
    
    // バウンド線の高さをセット
    void setGroundHeight(int);
    
    // 更新処理
    void update();
};

/*
class Water : public cocos2d::CCObject
{
private:
    cocos2d::CCSprite* sprite;
    void setNoVisible();
    
public:
    Water();
    
    int width;
    int height;
    int frameCount;
    bool isVisible;
    int alpha;
    
    cocos2d::CCSprite* getSprite();
    
    // スプライトの座標を取得
    cocos2d::CCPoint getPosition();
    
    // 表示状態の取得
    bool getVisible();
    
    // 表示範囲の矩形を取得する
    cocos2d::CCRect getRect();
    
    // スプライトの座標をセット
    void setPosition(float, float);
    
    void setVisible(bool);
};
*/

#endif /* defined(__ScrollAndPickUp__Warter__) */
