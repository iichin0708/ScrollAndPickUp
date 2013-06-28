#include "Player.h"
#include "RigidBody.h"

USING_NS_CC;

//初期化
int Player::prePlayerTurnId = 0;
int Player::playerTurnId = 0;


//現在のターンIDを返す
int Player::getPlayerTurnId() {
    //プレイヤーの合計人数の余りが次の人のターン
    Player::playerTurnId %= PLAYER_NUM;
    
    return Player::playerTurnId;
}

void Player::setImage(int direction) {
    PhysicsSprite *pSprite = (PhysicsSprite *)_body->GetUserData();
    CCSpriteBatchNode *image;
    
    //与えられた方向によって画像を変更
    switch (direction) {
        case UP:
            _direction = UP;
            image = CCSpriteBatchNode::create("player001.png", 100);
            break;
        case DOWN:
            _direction = DOWN;
            image = CCSpriteBatchNode::create("player002.png", 100);
            break;
        case LEFT:
            _direction = LEFT;
            image = CCSpriteBatchNode::create("player003.png", 100);
            break;
        case RIGHT:
            _direction = RIGHT;
            image = CCSpriteBatchNode::create("player004.png", 100);
            break;
    }
    
    //画像の差し替え
    cocos2d::CCTexture2D* m_pSpriteTexture = image->getTexture();
    pSprite->setTexture(m_pSpriteTexture);
}

// 攻撃を受ける
void Player::damaged(int offence) {
    hp -= offence;
    
    if(hp > 0) {
        isInvincible = true;
        
        barSprite->setScaleX((float)hp/maxHp);
        
        // 回転アクション（回転せず、0.7秒の間を持たせる＝無敵時間）
        cocos2d::CCActionInterval* action1 = cocos2d::CCRotateBy::create(0.7f, 0);
        // 無敵解除
        cocos2d::CCCallFunc* action2 = cocos2d::CCCallFunc::create(this, callfunc_selector(Player::setNoInvincible));
        // 順番に実行
        cocos2d::CCSequence* action = CCSequence::create(action1, action2);
        
        PhysicsSprite* sprite = (PhysicsSprite*)this->getBody()->GetUserData();
        sprite->runAction(action);
    } else {
        hpSprite->setVisible(false);
    }
}

// 無敵状態を解除する
void Player::setNoInvincible()
{
    isInvincible = false;
}