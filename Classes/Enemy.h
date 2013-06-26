#ifndef __ScrollAndPickUp__Enemy__
#define __ScrollAndPickUp__Enemy__

#include "cocos2d.h"
#include "World.h"
#include "PhysicsSprite.h"
#include "RigidBody.h"

#define KIND_ONION 0
#define KIND_GOBLIN 1
#define KIND_DRAGON 2

class Enemy : public RigidBody
{
private:
    
protected:
    
public:
    // コンストラクタ
    Enemy(cocos2d::CCNode* parent, int kind, cocos2d::CCPoint location, float density, float friction, float restitution) : RigidBody()
    {
        const char* imageName;
        isInvincible = false;
        switch (kind) {
            case KIND_ONION:
                imageName = "004.png";//"004.png";
                width = 80;
                height = 80;
                density = 2.5f;
                maxHp = 1;
                //プレイヤーをどの程度跳ね返すか
//                restitution = 0.8f;
                //自身の吹っ飛び率
                decreaseRatio = 10;
                break;
                
            case KIND_GOBLIN:
                imageName = "003.png";
                width = 115;
                height = 121;
                density = 3.0f;
                maxHp = 2;
                //プレイヤーをどの程度跳ね返すか
//                restitution = 1.0f;
                decreaseRatio =10;
                break;
                
            case KIND_DRAGON:
                imageName = "002.png";
                width = 150;
                height = 150;
                density = 3.5f;
                maxHp = 4;
                //プレイヤーをどの程度跳ね返すか
//                restitution = 1.5f;
                decreaseRatio = 14;
                break;
        }
        hp = maxHp;
        _kind = kind;
        cocos2d::CCSpriteBatchNode *image = cocos2d::CCSpriteBatchNode::create(imageName, 100);
        cocos2d::CCTexture2D* m_pSpriteTexture = image->getTexture();
        
        PhysicsSprite *sprite = new PhysicsSprite(TAG_PLAYER);
        sprite->initWithTexture(m_pSpriteTexture, cocos2d::CCRectMake(0, 0, width, height));
        sprite->autorelease();
        //sprite->setTag(sprite_id++);
        sprite->setPosition( cocos2d::CCPointMake( location.x, location.y) );
        
        parent->addChild(sprite);
        
        createCircleBody(location, sprite, width / 2, density, friction, restitution);
        setDecreaseSpeedRatio(decreaseRatio);
        //画面に表示する.
        sprite->setPhysicsBody(_body);
        
        // 体力画像のセット
        hpSprite = cocos2d::CCSprite::create("hp_frame.png", cocos2d::CCRectMake(0, 0, 98, 11) );
        hpSprite->setPosition(ccp(this->getBody()->GetPosition().x * PTM_RATIO,
                                  this->getBody()->GetPosition().y * PTM_RATIO - this->height / 2));
        
        barSprite = cocos2d::CCSprite::create("red_bar.png", cocos2d::CCRectMake(0, 0, 80, 7) );
        barSprite->setPosition(ccp(hpSprite->getContentSize().width / 2 - barSprite->getContentSize().width / 2,
                                   hpSprite->getContentSize().height / 2));
        barSprite->setAnchorPoint(ccp(0.0, 0.5));
        barSprite->setScaleX(1);
        hpSprite->addChild(barSprite);

        parent->addChild(hpSprite);
    }

    // 画像のサイズ
    float width , height;

    // 攻撃を受けたかどうかのフラグ
    bool isDamaged;
    
    // 体力
    int hp;
    int maxHp;
    
    // インスタンスの種類
    int _kind;
    
    // プレイヤーの反発係数 (プレイヤーの攻撃からどの程度自分が跳ね返るか)
    int decreaseRatio;
    
    // 体力表示用のスプライト
    cocos2d::CCSprite* hpSprite;
    cocos2d::CCSprite* barSprite;
    
    // 攻撃を受ける
    void damaged(int);
    
    // 無敵状態のフラグ
    bool isInvincible;
    
    // 無敵状態を解除する
    void setNoInvincible();

    //プレイヤーのバウンド率を取得する
    int getPlayerBoundRatio();
};

#endif /* defined(__ScrollAndPickUp__Enemy__) */
