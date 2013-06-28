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
    
    static int prePlayerTurnId;
    
    static int playerTurnId;
    
    Player(cocos2d::CCNode* parent, cocos2d::CCPoint location, float density, float friction, float restitution) : RigidBody() {
        isFalled = false;
        isInvincible = false;
        offense = OFFENSE;
        maxHp = 10;
        hp=maxHp;
        _direction = UP;
        cocos2d::CCSpriteBatchNode *image = cocos2d::CCSpriteBatchNode::create("player001.png", 100);
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

        // 体力画像のセット
        hpSprite = cocos2d::CCSprite::create("hp_frame.png", cocos2d::CCRectMake(0, 0, 98, 11) );
        hpSprite->setPosition(ccp(sprite->getContentSize().width / 2,
                                  -hpSprite->getContentSize().height / 2));
        barSprite = cocos2d::CCSprite::create("blue_bar.png", cocos2d::CCRectMake(0, 0, 81, 7) );
        barSprite->setPosition(ccp(hpSprite->getContentSize().width / 2 - barSprite->getContentSize().width / 2,
                                   hpSprite->getContentSize().height / 2));
        barSprite->setAnchorPoint(ccp(0.0, 0.5));
        barSprite->setScaleX(1);
        hpSprite->addChild(barSprite);
        hpSprite->setScale(10.0/8.0);
        
        isFalled = false;
        
        sprite->addChild(hpSprite);
    }
    
    // 画像のサイズ
    float width , height;

    //現在のターンのユーザIDを取得
    static int getPlayerTurnId();

    // 方角によって画像を変える.
    void setImage(int direction);
    
    // 水に落ちているかどうかのフラグ
    bool isFalled;
    
    // 体力表示用のスプライト
    cocos2d::CCSprite* hpSprite;
    cocos2d::CCSprite* barSprite;
    
    // 体力
    int hp;
    int maxHp;
    
    // 攻撃力
    int offense;
    
    // 攻撃を受ける
    void damaged(int);
    
    // 無敵状態のフラグ
    bool isInvincible;
    
    // 無敵状態を解除する
    void setNoInvincible();
};

#endif /* defined(__ScrollAndPickUp__Player__) */
