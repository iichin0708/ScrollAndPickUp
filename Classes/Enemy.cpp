#include "Enemy.h"
#include "RigidBody.h"

USING_NS_CC;

//初期化
int Enemy::enemyTurnId = 0;

// 攻撃を受ける
void Enemy::damaged(int offence) {
    hp -= offence;

    if(hp > 0) {
        isInvincible = true;
        
        barSprite->setScaleX((float)hp/maxHp);
        
        // 回転アクション（回転せず、0.7秒の間を持たせる＝無敵時間）
        cocos2d::CCActionInterval* action1 = cocos2d::CCRotateBy::create(0.7f, 0);
        // 無敵解除
        cocos2d::CCCallFunc* action2 = cocos2d::CCCallFunc::create(this, callfunc_selector(Enemy::setNoInvincible));
        // 順番に実行
        cocos2d::CCSequence* action = CCSequence::create(action1, action2);
        
        PhysicsSprite* sprite = (PhysicsSprite*)this->getBody()->GetUserData();
        sprite->runAction(action);
    } else {
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
            return 7.0f;
            break;
        case KIND_GOBLIN:
            return 8.0f;
            break;
        case KIND_ONION:
            return 9.0f;
            break;
        default:
            return -1;
            break;
    }
}

//現在のターンIDを返す
int Enemy::getEnemyTurnId() {
    //プレイヤーの合計人数の余りが次の人のターン
    Enemy::enemyTurnId %= ENEMY_NUM;
    
    return Enemy::enemyTurnId;
}

//Enemyの位置を返す
CCPoint Enemy::getEnemyPosition() {
    CCPoint enemyPoint = CCPointMake(_body->GetPosition().x * PTM_RATIO, _body->GetPosition().y * PTM_RATIO);
    return enemyPoint;
}

