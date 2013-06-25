//
//  HitEffect.h
//  ScrollAndPickUp
//
//  Created by Kenji Kawanabe on 2013/06/23.
//
//

#ifndef __ScrollAndPickUp__HitEffect__
#define __ScrollAndPickUp__HitEffect__

#include "cocos2d.h"

class HitEffect : public cocos2d::CCObject
{
private:
    cocos2d::CCSprite* sprite;
    void setNoVisible();
    
public:
    HitEffect();
    
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

#endif /* defined(__ScrollAndPickUp__HitEffect__) */
