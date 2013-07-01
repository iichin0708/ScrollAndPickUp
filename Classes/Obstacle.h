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
    Obstacle(cocos2d::CCNode* parent, const char *imgName, cocos2d::CCSize collisionSize, cocos2d::CCPoint location, int shapeTag, float density, float friction, float restitution, int index) : RigidBody() {
        //一度だけ初期化
        if(imgDict->count() == 0) {
            initImgDictionary();
        }
        
        cocos2d::CCSize imgSize = getImgSize(imgName);
        
        cocos2d::CCSpriteBatchNode *image = cocos2d::CCSpriteBatchNode::create(imgName, 100);
        cocos2d::CCTexture2D* m_pSpriteTexture = image->getTexture();
        
        PhysicsSprite *sprite = new PhysicsSprite(TAG_OBSTACLE, index);
        
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
    
    static cocos2d::CCDictionary *imgDict;
    
    cocos2d::CCSize getImgSize();
    
    cocos2d::CCPoint getObstaclePosition();

    int getKind();
    
    cocos2d::CCSize getImgSize(const char *imgName);
    
    //プレイヤーのバウンド率を取得する
    int getPlayerBoundRatio();
private:
    void setKind(const char *imgName);
    
    void initImgDictionary();
    
    void setImgDictionary(const char *imgName, int width, int height);
};

#endif /* defined(__ScrollAndPickUp__Obstacle__) */
