#include "Enemy.h"
#include "RigidBody.h"

USING_NS_CC;

// 攻撃を受ける
void Enemy::damaged() {
    hp--;

    if(hp > 0) {
        isInvincible = true;
        
        // 回転アクション（回転せず、0.7秒の間を持たせる＝無敵時間）
        cocos2d::CCActionInterval* action1 = cocos2d::CCRotateBy::create(0.7f, 0);
        // 無敵解除
        cocos2d::CCCallFunc* action2 = cocos2d::CCCallFunc::create(this, callfunc_selector(Enemy::setNoInvincible));
        // 順番に実行
        cocos2d::CCSequence* action = CCSequence::create(action1, action2);
        
        PhysicsSprite* sprite = (PhysicsSprite*)this->getBody()->GetUserData();
        sprite->runAction(action);
    }
    
    if(hp <= 1 && hp > 0) {
        hpSprite->setTexture(cocos2d::CCTextureCache::sharedTextureCache()->addImage("hp1-2.png"));
    } else if (hp <= 0) {
        hpSprite->setVisible(false);
    }
}


// 無敵状態を解除する
void Enemy::setNoInvincible()
{
    isInvincible = false;
}

// プレイヤーのバウンド率の調整
int Enemy::getPlayerBoundRatio() {
    switch(_kind) {
        case KIND_DRAGON:
            return 4.0f;
            break;
        case KIND_GOBLIN:
            return 6.0f;
            break;
        case KIND_ONION:
            return 8.0f;
            break;
        default:
            return -1;
            break;
    }
}