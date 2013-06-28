//
//  RigidBody.h
//  box2d
//
//  Created by TakuyaIida on 13/06/20.
//
//

#ifndef __box2d__RigidBody__
#define __box2d__RigidBody__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "PhysicsSprite.h"

#define CIRCLE_SHAPE 0
#define BOX_SHAPE 1

#define UP 2
#define DOWN 3
#define RIGHT 4
#define LEFT 5

class RigidBody : public cocos2d::CCLayer {
public:
    RigidBody(){};
    
    RigidBody(cocos2d::CCPoint location, PhysicsSprite *pSprite, float density, float friction, float restitution);

    ~RigidBody();

    void createCircleBody(cocos2d::CCPoint location, PhysicsSprite *pSprite, float radius, float density, float friction, float restitution);

    void createBoxBody(cocos2d::CCPoint location, PhysicsSprite *pSprite, cocos2d::CCSize size, float density, float friction, float restitution);

    virtual b2Body* getBody();

    b2Body *_body;
    
    //オブジェクトの向いている方角
    int _direction;
    
    //オブジェクトのベクトルから角度を取得
    float getAngle(b2Vec2 vec);
    
    //角度から向いている方角を取得
    int getDirectionFromAngle(float angle);
    
    //ベクトルから向いている方角を取得
    int getDirectionFromVec(b2Vec2 vec);

    void setDecreaseSpeedRatio(float ratio);
    
    //位置を返す
    cocos2d::CCPoint getRigidPosition();
protected:


};

#endif /* defined(__box2d__RigidBody__) */
