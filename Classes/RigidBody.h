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

class RigidBody : public cocos2d::CCLayer {
public:
    RigidBody(){};
    
    RigidBody(cocos2d::CCPoint location, PhysicsSprite *pSprite, float density, float friction, float restitution);

    ~RigidBody();

    void createCircleBody(cocos2d::CCPoint location, PhysicsSprite *pSprite, float radius, float density, float friction, float restitution);

    void createBoxBody(cocos2d::CCPoint location, PhysicsSprite *pSprite, cocos2d::CCSize size, float density, float friction, float restitution);

    virtual b2Body* getBody();

    b2Body *_body;

    void setDecreaseSpeedRatio(float ratio);
protected:

};

#endif /* defined(__box2d__RigidBody__) */
