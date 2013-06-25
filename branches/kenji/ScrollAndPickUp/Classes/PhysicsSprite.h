//
//  PhysicsSprite.h
//  box2d
//
//  Created by TakuyaIida on 13/06/19.
//
//

#ifndef __box2d__PhysicsSprite__
#define __box2d__PhysicsSprite__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "World.h"

#define PTM_RATIO 32
#define TAG_PLAYER 1
#define TAG_ENEMY 2


class PhysicsSprite : public cocos2d::CCSprite
{
public:
    PhysicsSprite(int _tag);
    void setPhysicsBody(b2Body * body);
    virtual bool isDirty(void);
    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
    int tag;
private:
    b2Body* m_pBody;    // strong ref
};

#endif /* defined(__box2d__PhysicsSprite__) */
