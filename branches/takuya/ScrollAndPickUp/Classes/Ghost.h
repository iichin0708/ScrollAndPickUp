#ifndef __ScrollAndPickUp__Ghost__
#define __ScrollAndPickUp__Ghost__

#include "cocos2d.h"

class Ghost
{
private:
    cocos2d::CCSprite* sprite;
    
public:
    Ghost();
    
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
    
    void update();
};

#endif /* defined(__ScrollAndPickUp__Ghost__) */
