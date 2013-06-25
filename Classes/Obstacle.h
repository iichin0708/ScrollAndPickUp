//
//  Obstacle.h
//  ScrollAndPickUp
//
//  Created by GRENGE-MAC0015 on 13/06/25.
//
//

#ifndef __ScrollAndPickUp__Obstacle__
#define __ScrollAndPickUp__Obstacle__

#include "cocos2d.h"
#include "World.h"
#include "PhysicsSprite.h"
#include "RigidBody.h"

class Obstacle : RigidBody {
public:
    Obstacle(cocos2d::CCNode* parent, const char *imgName, cocos2d::CCSize imgSize, cocos2d::CCPoint location, float density, float friction, float restitution) : RigidBody() {
        cocos2d::CCSpriteBatchNode *image = cocos2d::CCSpriteBatchNode::create(imgName, 100);
        cocos2d::CCTexture2D* m_pSpriteTexture = image->getTexture();
        
        PhysicsSprite *sprite = new PhysicsSprite(TAG_OBSTACLE);
        
        width = imgSize.width;
        height = imgSize.height;
        
        sprite->initWithTexture(m_pSpriteTexture, cocos2d::CCRectMake(0,0,width,height));
        sprite->autorelease();
        //sprite->setTag(sprite_id++);
        sprite->setPosition( cocos2d::CCPointMake( location.x, location.y) );
        
        parent->addChild(sprite);
        
        createBody(location, sprite, density, friction, restitution);
        
        //画面に表示する.
        sprite->setPhysicsBody(_body);
    }
    
private:
    // 画像のサイズ
    float width, height;
    
    cocos2d::CCSize getImgSize();
};

#endif /* defined(__ScrollAndPickUp__Obstacle__) */
