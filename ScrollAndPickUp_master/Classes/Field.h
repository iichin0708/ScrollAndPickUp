#ifndef __ScrollAndPickUp__Field__
#define __ScrollAndPickUp__Field__

#include "cocos2d.h"

class Field
{
private:
    cocos2d::CCSprite* sprite;
    
public:
    Field();
    
    int width;
    int height;
    
    cocos2d::CCSprite* getSprite();

    // スプライトの座標を取得
    cocos2d::CCPoint getPosition();
    
    // 表示範囲の矩形を取得する
    cocos2d::CCRect getRect();

    // スプライトの座標をセット
    void setPosition(float, float);
};

#endif /* defined(__ScrollAndPickUp__Field__) */
