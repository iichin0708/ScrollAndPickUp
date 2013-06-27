#ifndef __ScrollAndPickUp__Player__
#define __ScrollAndPickUp__Player__

#include "cocos2d.h"
#include "World.h"
#include "PhysicsSprite.h"
#include "RigidBody.h"

#define PLAYER_NUM 3
#define OFFENSE 1;

class Player : public RigidBody
{
private:

    
protected:
    
public:
    
    static int playerTurnId;
    
    Player(cocos2d::CCNode* parent, cocos2d::CCPoint location, float density, float friction, float restitution) : RigidBody() {
        isFalled = false;
        offense = OFFENSE;
        
        cocos2d::CCSpriteBatchNode *image = cocos2d::CCSpriteBatchNode::create("001.png", 100);
        cocos2d::CCTexture2D* m_pSpriteTexture = image->getTexture();
        
        PhysicsSprite *sprite = new PhysicsSprite(TAG_PLAYER);
        width = 110;
        height = 110;
        sprite->initWithTexture(m_pSpriteTexture, cocos2d::CCRectMake(0,0,width,height));
        sprite->autorelease();
        //sprite->setTag(sprite_id++);
        sprite->setPosition( cocos2d::CCPointMake( location.x, location.y) );
        sprite->setScale(0.8f);
        
        parent->addChild(sprite);
        
        createCircleBody(location, sprite, width / 2, density, friction, restitution);
        
        //画面に表示する.
        sprite->setPhysicsBody(_body);
        //sprite->setScale(2);
    }
    
    // 画像のサイズ
    float width , height;

    cocos2d::CCPoint getPlayerPosition();
    
    //現在のターンのユーザIDを取得
    static int getPlayerTurnId();
    
    // 水に落ちているかどうかのフラグ
    bool isFalled;
    
    // 攻撃力
    int offense;
};

#endif /* defined(__ScrollAndPickUp__Player__) */
