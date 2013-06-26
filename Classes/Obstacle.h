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

class Obstacle : public RigidBody {
public:
    // コンストラクタ
    Obstacle(cocos2d::CCNode* parent, const char *imgName, cocos2d::CCSize imgSize, cocos2d::CCSize collisionSize, cocos2d::CCPoint location, int shapeTag, float density, float friction, float restitution) : RigidBody() {
        cocos2d::CCSpriteBatchNode *image = cocos2d::CCSpriteBatchNode::create(imgName, 100);
        cocos2d::CCTexture2D* m_pSpriteTexture = image->getTexture();
        
        PhysicsSprite *sprite = new PhysicsSprite(TAG_OBSTACLE);
        
        width = imgSize.width;
        height = imgSize.height;
        
        setKind(imgName);
        
        sprite->initWithTexture(m_pSpriteTexture, cocos2d::CCRectMake(0,0,width,height));
        sprite->autorelease();
        //sprite->setTag(sprite_id++);
        sprite->setPosition( cocos2d::CCPointMake( location.x, location.y) );
        
        parent->addChild(sprite);
        
        if (shapeTag == CIRCLE_SHAPE) {
            createCircleBody(location, sprite, collisionSize.width, density, friction, restitution);
        } else if (shapeTag == BOX_SHAPE) {
            createBoxBody(location, sprite, collisionSize, density, friction, restitution);
        }
        
        //画面に表示する.
        sprite->setPhysicsBody(_body);
    }
    // 画像のサイズ
    float width, height;
    
    // 障害物の種類
    int _kind;
    
    cocos2d::CCSize getImgSize();
    
    cocos2d::CCPoint getObstaclePosition();

    int getKind();
    
    //プレイヤーのバウンド率を取得する
    int getPlayerBoundRatio();
private:
    void setKind(const char *imgName);
};

#endif /* defined(__ScrollAndPickUp__Obstacle__) */
