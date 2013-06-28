#include "Enemy.h"
#include "RigidBody.h"

USING_NS_CC;

//初期化
int Enemy::enemyTurnId = 0;
int Enemy::preEnemyTurnId = 0;

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

//受け取った方向によって敵キャラクターの画像を差し替える
void Enemy::setImage(int direction) {
    PhysicsSprite *pSprite = (PhysicsSprite *)_body->GetUserData();
    CCSpriteBatchNode *image;
    const char *imageName;
        
    //与えられた方向によって画像を変更
    /****** imageファイルが揃ってないのでファイル名は暫定で同じものを使う ******/
    switch (direction) {
        case UP:
            _direction = UP;
            switch (_kind) {
                case KIND_ONION:
                    imageName = "onion001.png";
                    break;
                case KIND_GOBLIN:
                    imageName = "onion001.png";
                    break;
                case KIND_DRAGON:
                    imageName = "onion001.png";
                    break;
            }
            break;
        case DOWN:
            _direction = DOWN;
            switch (_kind) {
                case KIND_ONION:
                    imageName = "onion002.png";
                    break;
                case KIND_GOBLIN:
                    imageName = "onion002.png";
                    break;
                case KIND_DRAGON:
                    imageName = "onion002.png";
                    break;
            }
            break;
        case LEFT:
            _direction = LEFT;
            switch (_kind) {
                case KIND_ONION:
                    imageName = "onion003.png";
                    break;
                case KIND_GOBLIN:
                    imageName = "onion003.png";
                    break;
                case KIND_DRAGON:
                    imageName = "onion003.png";
                    break;
            }
            break;
        case RIGHT:
            _direction = RIGHT;
            switch (_kind) {
                case KIND_ONION:
                    imageName = "onion004.png";
                    break;
                case KIND_GOBLIN:
                    imageName = "onion004.png";
                    break;
                case KIND_DRAGON:
                    imageName = "onion004.png";
                    break;
            }
            break;
    }
    
    image = CCSpriteBatchNode::create(imageName, 100);
    //画像の差し替え
    cocos2d::CCTexture2D* m_pSpriteTexture = image->getTexture();
    pSprite->setTexture(m_pSpriteTexture);
}

