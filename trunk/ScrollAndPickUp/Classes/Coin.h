#ifndef __ScrollAndPickUp__Coin__
#define __ScrollAndPickUp__Coin__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#define TAG_COIN 6
#define GRAVITY -6
#define FIRST_SPEED 60
#define SIDE_SPEED 5
#define BOUND_HEIGHT 50

class Coin
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
    Coin();
    
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

#endif /* defined(__ScrollAndPickUp__Coin__) */
