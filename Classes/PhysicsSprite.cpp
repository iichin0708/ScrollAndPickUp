//
//  PhysicsSprite.cpp
//  box2d
//
//  Created by TakuyaIida on 13/06/19.
//
//

#include "PhysicsSprite.h"

using namespace cocos2d;

// タグの種類でどの子クラスのオブジェクトかを管理する
// ヘッダーに設定しているTAGを使う
PhysicsSprite::PhysicsSprite(int tag, int index): m_pBody(NULL)
{
    _tag = tag;
    _index = index;
}

void PhysicsSprite::setPhysicsBody(b2Body * body)
{
    m_pBody = body;
}

// this method will only get called if the sprite is batched.
// return YES if the physics values (angles, position ) changed
// If you return NO, then nodeToParentTransform won't be called.
bool PhysicsSprite::isDirty(void)
{
    return true;
}

// returns the transform matrix according the Chipmunk Body values
CCAffineTransform PhysicsSprite::nodeToParentTransform(void)
{
    b2Vec2 pos  = m_pBody->GetPosition();
    
    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;
    
    if ( isIgnoreAnchorPointForPosition() ) {
        x += m_obAnchorPointInPoints.x;
        y += m_obAnchorPointInPoints.y;
    }
    
    // Make matrix
    float radians = m_pBody->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);
    
    if( ! m_obAnchorPointInPoints.equals(CCPointZero) ){
        x += c*-m_obAnchorPointInPoints.x * m_fScaleX + -s*-m_obAnchorPointInPoints.y * m_fScaleY;
        y += s*-m_obAnchorPointInPoints.x * m_fScaleX + c*-m_obAnchorPointInPoints.y * m_fScaleY;
    }
    
    // Rot, Translate Matrix
    m_sTransform = CCAffineTransformMake( c * m_fScaleX,  s * m_fScaleX,
                                        -s * m_fScaleY,    c * m_fScaleY,
                                         x,    y );
    
    return m_sTransform;
}
