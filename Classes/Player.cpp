#include "Player.h"
#include "RigidBody.h"

USING_NS_CC;

//初期化
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
            image = CCSpriteBatchNode::create("player001.png", 100);
            break;
        case DOWN:
            image = CCSpriteBatchNode::create("player002.png", 100);
            break;
        case LEFT:
            image = CCSpriteBatchNode::create("player003.png", 100);
            break;
        case RIGHT:
            image = CCSpriteBatchNode::create("player004.png", 100);
            break;
    }
    
    //画像の差し替え
    cocos2d::CCTexture2D* m_pSpriteTexture = image->getTexture();
    pSprite->setTexture(m_pSpriteTexture);
}

/*
//受け取ったIDのPlayerの位置を返す
CCPoint Player::getPlayerPosition() {
    CCPoint playerPoint = CCPointMake(_body->GetPosition().x * PTM_RATIO, _body->GetPosition().y * PTM_RATIO);
    return playerPoint;
}
*/