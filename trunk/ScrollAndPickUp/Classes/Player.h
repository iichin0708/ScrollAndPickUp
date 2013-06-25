#ifndef __ScrollAndPickUp__Player__
#define __ScrollAndPickUp__Player__

#include "cocos2d.h"
#include "World.h"
#include "PhysicsSprite.h"
#include "RigidBody.h"

#define PLAYER_NUM 3

class Player : public RigidBody
{
private:

    
protected:
    
public:
    
    static int playerTurnId;
    
    Player(cocos2d::CCNode* parent, cocos2d::CCPoint location, float density, float friction, float restitution) : RigidBody() {
        cocos2d::CCSpriteBatchNode *image = cocos2d::CCSpriteBatchNode::create("001.png", 100);
        cocos2d::CCTexture2D* m_pSpriteTexture = image->getTexture();
        
        PhysicsSprite *sprite = new PhysicsSprite(TAG_PLAYER);
        width = 110;
        height = 110;
        sprite->initWithTexture(m_pSpriteTexture, cocos2d::CCRectMake(0,0,width,height));
        sprite->autorelease();
        //sprite->setTag(sprite_id++);
        sprite->setPosition( cocos2d::CCPointMake( location.x, location.y) );
        
        parent->addChild(sprite);
        
        createDynamicBody(location, sprite, density, friction, restitution);
        
        //画面に表示する.
        sprite->setPhysicsBody(_body);
        
    }
    
    // 画像のサイズ
    float width , height;

    cocos2d::CCPoint getPlayerPosition(int playerId);
    
    //現在のターンのユーザIDを取得
    static int getPlayerTurnId();
};

#endif /* defined(__ScrollAndPickUp__Player__) */
