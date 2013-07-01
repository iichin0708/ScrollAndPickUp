//
//  CContactListener.cpp
//  box2d
//
//  Created by TakuyaIida on 13/06/19.
//
//



#include "CContactListener.h"
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "PhysicsSprite.h"

using namespace cocos2d;

//using namespace CocosDenshion;

CContactListener::CContactListener() {
    isContactedToEnemy = false;
    contactPlayerNum = 0;
}

CContactListener::~CContactListener() {
    
}

//衝突検知後、最初に呼ばれる
void CContactListener::BeginContact(b2Contact *contact) {
    
    //CCLog("BeginContact");
    if (contact->IsTouching()) {
        b2Fixture *a = contact->GetFixtureA();
        b2Fixture *b = contact->GetFixtureB();
        b2Body *aBody = a->GetBody();
        b2Body *bBody = b->GetBody();
        
        if(aBody != NULL && bBody != NULL) {
            /*
             GameObject *obj1 = static_cast<GameObject*>(aBody->GetUserData());
             GameObject *obj2 = static_cast<GameObject*>(bBody->GetUserData());
             
             if (obj1 != NULL && obj2 != NLLL) {
             obj1->collisionCallback(obj2);
             obj2->collisionCallback(obj);
             }
             */
        }
    }

}

//衝突検知後、最後に呼ばれる
void CContactListener::EndContact(b2Contact *contact) {
    //敵とぶつかったなら
    if(isContactedToEnemy) {
     isContactedToEnemy = false;
    }
}

//衝突検知後、2番目に呼ばれる
void CContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold) {
}

//衝突検知後、3番目に呼ばれる
void CContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
    // ハローワールドインスタンスへの参照
    HelloWorld* hw = HelloWorld::Instance;    
    if(hw->isContacted) return;

    //CCLog("衝突検知");
    b2Fixture *a = contact->GetFixtureA();
    b2Fixture *b = contact->GetFixtureB();
    b2Body *aBody = a->GetBody();
    b2Body *bBody = b->GetBody();
    
    //bodyからスプライトの位置を算出
    CCPoint aPoint = CCPointMake(aBody->GetPosition().x * PTM_RATIO, aBody->GetPosition().y * PTM_RATIO);
    CCPoint bPoint = CCPointMake(bBody->GetPosition().x * PTM_RATIO, bBody->GetPosition().y * PTM_RATIO);
    
    //中点を衝突点とする
    CCPoint contactPoint = ccpMidpoint(aPoint, bPoint);
    
    //Spriteを取得
    PhysicsSprite *spriteA = (PhysicsSprite *)aBody->GetUserData();
    PhysicsSprite *spriteB = (PhysicsSprite *)bBody->GetUserData();
    
    //衝突の強さ
    float impactForce = impulse->normalImpulses[0];
    if(impactForce > 0.000001) {
        //CCLog("衝突の力:%f", impactForce);
    }
    
    //モノ同士の衝突で、強さが指定より大きいときにイベント
    if (spriteA && spriteB && impactForce < 20) return;

    //それぞれのタグとインデックスを取得
    int aTag = spriteA->_tag;
    int aIndex = spriteA->_index;
    int bTag = spriteB->_tag;
    int bIndex = spriteB->_index;
    
    //プレイヤーとプレイヤーの接触の場合
    if (aTag == TAG_PLAYER && bTag == TAG_PLAYER) {
        //減衰率の調整
        setPlayerBoundRatio(aBody, bTag, bBody, bIndex);
        
        //衝突された方の画像と速度を変える
        if(aIndex != Player::prePlayerTurnId) {
            changeImageAndVelocity(aTag, aIndex, aBody->GetLinearVelocity());
        } else if(bIndex != Player::prePlayerTurnId) {
            changeImageAndVelocity(bTag, bIndex, bBody->GetLinearVelocity());
        }
    }
    
    //敵プレイヤーと敵プレイヤーの接触の場合
    if (aTag == TAG_ENEMY && bTag == TAG_ENEMY) {
        
        //衝突された方の画像と速度を変える
        if(aIndex != Enemy::enemyTurnId) {
            changeImageAndVelocity(aTag, aIndex, aBody->GetLinearVelocity());
        } else if(bIndex != Enemy::enemyTurnId) {
            changeImageAndVelocity(bTag, bIndex, bBody->GetLinearVelocity());
        }
    }
    
    // プレイヤーと敵の接触の場合
    if(aTag == TAG_PLAYER && bTag == TAG_ENEMY) {
        //プレイヤーのターンの処理
        if(hw->isPlayerTurn) {
            if( ! hw->enemys[bIndex]->isInvincible ) {
                hw->isContacted = true;
                isContactedToEnemy = true;
                contactPlayerNum = aIndex;
                hw->contactPlayerOffence = hw->monkeys[aIndex]->offense;
                hw->contactEnemyindex = bIndex;
                hw->pIndex = aIndex;
                
                setPlayerBoundRatio(aBody, bTag, bBody, bIndex);
                
                for(int i = 0; i < HIT_EF_NUM; i++) {
                    if(hw->hitEfs[i]->getVisible()) continue;
                    hw->hitEfs[i]->setPosition(contactPoint.x, contactPoint.y);
                    hw->hitEfs[i]->setVisible(true);
                    break;
                }
                //衝突された方の画像と速度を変える
                changeImageAndVelocity(bTag, bIndex, bBody->GetLinearVelocity());

            }
        }
        //敵プレイヤーのターン
        else
        {
            //if( ! hw->monkeys[aIndex]->isInvincible ) {
            hw->isContacted = true;
            isContactedToEnemy = true;
            contactPlayerNum = aIndex;
            hw->contactPlayerOffence = hw->monkeys[aIndex]->offense;
            hw->contactEnemyindex = bIndex;
            hw->contactEnemyOffence = hw->enemys[bIndex]->offense;
            hw->contactPlayerindex = aIndex;
            hw->pIndex = aIndex;
            
            //setPlayerBoundRatio(bBody, aTag, aBody, aIndex);
            
            for(int i = 0; i < HIT_EF_NUM; i++) {
                if(hw->hitEfs[i]->getVisible()) continue;
                hw->hitEfs[i]->setPosition(contactPoint.x, contactPoint.y);
                hw->hitEfs[i]->setVisible(true);
                break;
            }
            //衝突された方の画像と速度を変える
            changeImageAndVelocity(aTag, aIndex, aBody->GetLinearVelocity());
            
        }
    } else if(aTag == TAG_ENEMY && bTag == TAG_PLAYER) {
        //プレイヤーのターン
        if(hw->isPlayerTurn) {
            if( ! hw->enemys[aIndex]->isInvincible ) {
                hw->isContacted = true;
                isContactedToEnemy = true;
                contactPlayerNum = bIndex;
                hw->contactPlayerOffence = hw->monkeys[bIndex]->offense;
                hw->contactEnemyindex = aIndex;
                hw->pIndex = bIndex;
                
                setPlayerBoundRatio(bBody, aTag, aBody, aIndex);
                
                for(int i = 0; i < HIT_EF_NUM; i++) {
                    if(hw->hitEfs[i]->getVisible()) continue;
                    hw->hitEfs[i]->setPosition(contactPoint.x, contactPoint.y);
                    hw->hitEfs[i]->setVisible(true);
                    break;
                }
                //衝突された方の画像と速度を変える
                changeImageAndVelocity(aTag, aIndex, aBody->GetLinearVelocity());
                
            }
        }
        //敵プレイヤーのターン
        else
        {
            if( ! hw->monkeys[bIndex]->isInvincible ) {
                hw->isContacted = true;
                isContactedToEnemy = true;
                contactPlayerNum = bIndex;
                hw->contactPlayerOffence = hw->monkeys[bIndex]->offense;
                hw->contactEnemyindex = aIndex;
                hw->contactEnemyOffence = hw->enemys[aIndex]->offense;
                hw->contactPlayerindex = bIndex;
                hw->pIndex = bIndex;
                
                //setPlayerBoundRatio(bBody, aTag, aBody, aIndex);
                
                for(int i = 0; i < HIT_EF_NUM; i++) {
                    if(hw->hitEfs[i]->getVisible()) continue;
                    hw->hitEfs[i]->setPosition(contactPoint.x, contactPoint.y);
                    hw->hitEfs[i]->setVisible(true);
                    break;
                }
                //衝突された方の画像と速度を変える
                changeImageAndVelocity(bTag, bIndex, bBody->GetLinearVelocity());
                
            }
        }
    }
    
    //プレイヤーと障害物との接触の場合
    if (aTag == TAG_PLAYER && bTag == TAG_OBSTACLE) {
        setPlayerBoundRatio(aBody, bTag, bBody, bIndex);
        
        //衝突によってプレイヤー画像の差し替えを行う
        float angle = hw->monkeys[aIndex]->getAngle(aBody->GetLinearVelocity());
        hw->monkeys[aIndex]->setImage(hw->monkeys[aIndex]->getDirectionFromAngle(angle));
    } else if(aTag == TAG_OBSTACLE && bTag == TAG_PLAYER) {
        setPlayerBoundRatio(bBody, aTag, aBody, aIndex);
        //衝突によってプレイヤー画像の差し替えを行う
        float angle = hw->monkeys[bIndex]->getAngle(bBody->GetLinearVelocity());
        hw->monkeys[bIndex]->setImage(hw->monkeys[bIndex]->getDirectionFromAngle(angle));
    }
    
    //敵プレイヤーと障害物との接触の場合
    if (aTag == TAG_ENEMY && bTag == TAG_OBSTACLE) {
        //衝突によって敵画像の差し替えを行う
        float angle = hw->enemys[aIndex]->getAngle(aBody->GetLinearVelocity());
        hw->enemys[aIndex]->setImage(hw->enemys[aIndex]->getDirectionFromAngle(angle));
    } else if(aTag == TAG_OBSTACLE && bTag == TAG_ENEMY) {
        //衝突によって敵画像の差し替えを行う
        float angle = hw->enemys[bIndex]->getAngle(bBody->GetLinearVelocity());
        hw->enemys[bIndex]->setImage(hw->enemys[bIndex]->getDirectionFromAngle(angle));
    }
    
}

//衝突後の画像と速度の変更に関わるメソッド
void CContactListener::changeImageAndVelocity(int tag, int index, b2Vec2 vec) {
    // ハローワールドインスタンスへの参照
    HelloWorld* hw = HelloWorld::Instance;
    float angle = 0.0f;
    switch (tag) {
        case TAG_PLAYER:
            //衝突によってプレイヤー画像の差し替えを行う
            angle = hw->monkeys[index]->getAngle(vec);
            hw->monkeys[index]->setImage(hw->monkeys[index]->getDirectionFromAngle(angle));
            //衝突された時の方向によって速度を変化
            hw->monkeys[index]->setContactedDirection();
            hw->monkeys[index]->setReviceVelocity(hw->monkeys[index]->_contactedDirection);
            break;
        case TAG_ENEMY:
            //衝突によって敵画像の差し替えを行う
            angle = hw->enemys[index]->getAngle(vec);
            hw->enemys[index]->setImage(hw->enemys[index]->getDirectionFromAngle(angle));
            //衝突された時の方向によって速度を変化
            hw->enemys[index]->setContactedDirection();
            hw->enemys[index]->setReviceVelocity(hw->enemys[index]->_contactedDirection);
            break;
    }
}

//どのオブジェクトに衝突したかによって、プレイヤーの速度の減衰率を調整
void CContactListener::setPlayerBoundRatio(b2Body *playerBody, int npTag, b2Body *npBody, int npIndex) {
    //プレイヤーの速度を取得(当たる速度が小さい時は跳ね返らない).
    b2Vec2 playerVec = playerBody->GetLinearVelocity();
    if ((-30 < playerVec.x && playerVec.x < 30) && (-30 < playerVec.y && playerVec.y < 30)) {
        return;
    }
    
    HelloWorld* hw = HelloWorld::Instance;
    switch (npTag) {
        case TAG_PLAYER:
        {
            playerBody->SetLinearDamping(8.0f);
            npBody->SetLinearDamping(8.0f);
            break;
        }
        case TAG_ENEMY:
        {
            playerBody->SetLinearDamping(hw->enemys[npIndex]->getPlayerBoundRatio());
            break;
        }
        case TAG_OBSTACLE:
        {
            playerBody->SetLinearDamping(hw->obstacles[npIndex]->getPlayerBoundRatio());
            break;
        }
        default:
            break;
    }
    
}

void setEnemyBoundRatio(b2Body *enemyBody, int contactedType, b2Body *contactedBody, int contactedNum) {
    
}
