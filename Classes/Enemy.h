#ifndef __ScrollAndPickUp__Enemy__
#define __ScrollAndPickUp__Enemy__

#include "cocos2d.h"
#include "World.h"
#include "PhysicsSprite.h"
#include "RigidBody.h"

class Enemy : public RigidBody
{
private:
    
protected:
    
public:
    //敵のターンID
    static int enemyTurnId;
    static int preEnemyTurnId;
    
    // コンストラクタ
    Enemy(cocos2d::CCNode* parent, int kind, cocos2d::CCPoint location, float density, float friction, float restitution, int index) : RigidBody()
    {
        
        const char* imageName;
        isInvincible = false;
        contactDirection = 0;
        switch (kind) {
            case KIND_ONION:
//                imageName = "004.png";
                imageName = "onion002.png";
                width = 80;
                height = 80;
                density = 2.5f;
                maxHp = 1;
                speed = 1.0f;
                offense = 1;
                //プレイヤーをどの程度跳ね返すか
                //restitution = 0.8f;
                //自身の吹っ飛び率
                decreaseRatio = 10;
                break;
                
            case KIND_GOBLIN:
                //imageName = "003.png";
                imageName = "onion002.png";
//                width = 115;
//                height = 121;
                width = 80;
                height = 80;
                density = 3.0f;
                maxHp = 2;
                speed = 1.0f;
                offense = 1;
                //プレイヤーをどの程度跳ね返すか
                //restitution = 1.0f;
                decreaseRatio =10;
                break;
                
            case KIND_DRAGON:
                //imageName = "002.png";
                imageName = "onion002.png";
//                width = 150;
//                height = 150;
                width = 80;
                height = 80;
                density = 3.5f;
                maxHp = 4;
                speed = 1.0f;
                offense = 2;
                //プレイヤーをどの程度跳ね返すか
                //restitution = 1.5f;
                decreaseRatio = 14;
                break;
        }
        _preDirection = DOWN;
        _postDirection = DOWN;
        
        hp = maxHp;
        _kind = kind;
        cocos2d::CCSpriteBatchNode *image = cocos2d::CCSpriteBatchNode::create(imageName, 100);
        cocos2d::CCTexture2D* m_pSpriteTexture = image->getTexture();
        
        PhysicsSprite *sprite = new PhysicsSprite(TAG_ENEMY, index);
        sprite->initWithTexture(m_pSpriteTexture, cocos2d::CCRectMake(0, 0, width, height));
        sprite->autorelease();
        //sprite->setTag(sprite_id++);
        sprite->setPosition( cocos2d::CCPointMake( location.x, location.y) );
        
        
        if(_kind == 0) {
            
        } else if(_kind == 1) {
            sprite->setColor(cocos2d::ccc3(255, 200, 100));
        } else if(_kind == 2) {
            sprite->setColor(cocos2d::ccc3(255, 100, 100));
        }
        
        parent->addChild(sprite);
        
        createCircleBody(location, sprite, width / 2, density, friction, restitution);
        setDecreaseSpeedRatio(decreaseRatio);
        //画面に表示する.
        sprite->setPhysicsBody(_body);
        
        // 体力画像のセット
        hpSprite = cocos2d::CCSprite::create("hp_frame.png", cocos2d::CCRectMake(0, 0, 98, 11) );
        hpSprite->setPosition(ccp(sprite->getContentSize().width / 2,
                                  -hpSprite->getContentSize().height / 2));
        barSprite = cocos2d::CCSprite::create("red_bar.png", cocos2d::CCRectMake(0, 0, 81, 7) );
        barSprite->setPosition(ccp(hpSprite->getContentSize().width / 2 - barSprite->getContentSize().width / 2,
                                   hpSprite->getContentSize().height / 2));
        barSprite->setAnchorPoint(ccp(0.0, 0.5));
        barSprite->setScaleX(1);
        hpSprite->addChild(barSprite);

        isFalled = false;
        
        sprite->addChild(hpSprite);
    }

    // 画像のサイズ
    float width , height;

    // 攻撃を受けたかどうかのフラグ
    bool isDamaged;
    
    // 水に落ちているかどうかのフラグ
    bool isFalled;
    
    //衝突した場所を保持する.
    int contactDirection;

    // 体力
    int hp;
    int maxHp;
    
    // 攻撃力
    int offense;
    
    //スピード
    float speed;
    
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
    
    //現在のターンのEnemyIDを取得
    static int getEnemyTurnId();
    
    //画像の差し替え
    void setImage(int direction);
    
};

#endif /* defined(__ScrollAndPickUp__Enemy__) */
