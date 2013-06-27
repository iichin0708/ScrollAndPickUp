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

/*
//受け取ったIDのPlayerの位置を返す
CCPoint Player::getPlayerPosition() {
    CCPoint playerPoint = CCPointMake(_body->GetPosition().x * PTM_RATIO, _body->GetPosition().y * PTM_RATIO);
    return playerPoint;
}
*/