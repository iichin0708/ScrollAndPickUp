//
//  CContactListener.h
//  box2d
//
//  Created by TakuyaIida on 13/06/19.
//
//

#ifndef __box2d__CContactListener__
#define __box2d__CContactListener__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "HelloWorldScene.h"

class CContactListener : public b2ContactListener {
private:
    
    public :
    CContactListener();
    ~CContactListener();
    
    virtual void BeginContact(b2Contact *contact);
    virtual void EndContact(b2Contact *contact);
    virtual void PreSolve(b2Contact *contact, const b2Manifold *oldManifod);
    virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
    
    bool isContactedToEnemy;
    int contactPlayerNum;
    
    //np => Not Player
    void setPlayerBoundRatio(b2Body *playerBody, int npType, b2Body *npBody, int npNum);
    
    void setEnemyBoundRatio(b2Body *enemyBody, int contactedType, b2Body *contactedBody, int contactedNum);

    //衝突後に画像と速度を変更
    void changeImageAndVelocity(int tag, int index, b2Vec2 vec);
    
    
};

#endif /* defined(__box2d__CContactListener__) */

