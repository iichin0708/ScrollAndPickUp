//
//  Cursor.h
//  ScrollAndPickUp
//
//  Created by Kenji Kawanabe on 2013/06/23.
//
//

#ifndef __ScrollAndPickUp__Cursor__
#define __ScrollAndPickUp__Cursor__

#include "cocos2d.h"

class Cursor
{
private:
    cocos2d::CCSprite* sprite;
    
public:
    Cursor();
    
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

#endif /* defined(__ScrollAndPickUp__Cursor__) */
