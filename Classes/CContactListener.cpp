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

#define PTM_RATIO 32

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
/*    HelloWorld* hw = HelloWorld::Instance;
        Player *player = hw->monkeys[contactPlayerNum];
        b2Body *pBody = player->getBody();
        b2Vec2 pVec = pBody->GetLinearVelocity();

        int boundRatio = 0;
        //止まりかけの速度でぶつかった時
        if (-1.0 < pVec.x && pVec.x < 1.0) {
            boundRatio = 0.5;
        } else {
            boundRatio = 2.5;
        }
        pVec.x *= boundRatio;
        pVec.y *= boundRatio;
        //CCLog("pVec.x = %f, pVec.y = %f", pVec.x, pVec.y);
        pBody->SetLinearVelocity(pVec);
*/      isContactedToEnemy = false;
    }
}

//衝突検知後、2番目に呼ばれる
void CContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold) {
    /*
    // ハローワールドインスタンスへの参照
    HelloWorld* hw = HelloWorld::Instance;
    if(hw->isContacted) return;
    
    b2Fixture *a = contact->GetFixtureA();
    b2Fixture *b = contact->GetFixtureB();
    b2Body *aBody = a->GetBody();
    b2Body *bBody = b->GetBody();
    
    // aBodyは何か探索
    int aNum;
    int aType = TYPE_NULL;
    for(int i = 0; i < PLAYER_NUM; i++) {
        if(hw->monkeys[i] == NULL) continue;
        Player* p = hw->monkeys[i];
        if(aBody == p->getBody()) {
            aNum = i;
            aType = TYPE_PLAYER;
        }
    }
    
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(hw->enemys[i] == NULL) continue;
        Enemy* e = hw->enemys[i];
        if(aBody == e->getBody()) {
            aNum = i;
            aType = TYPE_ENEMY;
        }
    }
    
    // bBodyは何か探索
    int bNum;
    int bType = TYPE_NULL;
    for(int i = 0; i < PLAYER_NUM; i++) {
        if(hw->monkeys[i] == NULL) continue;
        Player* p = hw->monkeys[i];
        if(bBody == p->getBody()) {
            bNum = i;
            bType = TYPE_PLAYER;
        }
    }
    
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(hw->enemys[i] == NULL) continue;
        Enemy* e = hw->enemys[i];
        if(bBody == e->getBody()) {
            bNum = i;
            bType = TYPE_ENEMY;
        }
    }
    
    // プレイヤーと敵の接触の場合
    if(aType == TYPE_PLAYER && bType == TYPE_ENEMY) {
        for (int i = 0; i < ENEMY_NUM; i++) {
            //if(hw->enemys[i] == NULL) continue;
            a->SetRestitution(hw->enemys[i]->playerRestitution);
        }
    } else if(aType == TYPE_ENEMY && bType == TYPE_PLAYER) {
        b2Fixture *playerFixture = bBody->GetFixtureList();
        //自分がどの敵にあたるのかを探索
        for (int i = 0; i < ENEMY_NUM; i++) {
            //if(hw->enemys[i] == NULL) continue;
            //if (hw->enemys[i]->getBody() == aBody) {
            b->SetRestitution(hw->enemys[i]->playerRestitution);
            //}
        }
    }
     */
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
    
    // aBodyは何か探索
    int aNum;
    int aType = TYPE_NULL;
    for(int i = 0; i < PLAYER_NUM; i++) {
        if(hw->monkeys[i] == NULL) continue;
        Player* p = hw->monkeys[i];
        if(aBody == p->getBody()) {
            aNum = i;
            aType = TYPE_PLAYER;
        }
    }
    
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(hw->enemys[i] == NULL) continue;
        Enemy* e = hw->enemys[i];
        if(aBody == e->getBody()) {
            aNum = i;
            aType = TYPE_ENEMY;
        }
    }
    
    for(int i = 0; i < OBSTACLE_NUM; i++) {
        Obstacle* obstacle = hw->obstacles[i];
        if (aBody == obstacle->getBody()) {
            aNum = i;
            aType = TYPE_OBSTACLE;
        }
    }
    
    // bBodyは何か探索
    int bNum;
    int bType = TYPE_NULL;
    for(int i = 0; i < PLAYER_NUM; i++) {
        if(hw->monkeys[i] == NULL) continue;
        Player* p = hw->monkeys[i];
        if(bBody == p->getBody()) {
            bNum = i;
            bType = TYPE_PLAYER;
        }
    }
    
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(hw->enemys[i] == NULL) continue;
        Enemy* e = hw->enemys[i];
        if(bBody == e->getBody()) {
            bNum = i;
            bType = TYPE_ENEMY;
        }
    }
    
    for (int i = 0; i < OBSTACLE_NUM; i++) {
        Obstacle *obstacle = hw->obstacles[i];
        if (bBody == obstacle->getBody()) {
            bNum = i;
            bType = TYPE_OBSTACLE;
        }
    }
    
    //プレイヤーとプレイヤーの接触の場合
    if (aType == TYPE_PLAYER && bType == TYPE_PLAYER) {
        setPlayerBoundRatio(aBody, bType, bBody, bNum);
    }
    
    // プレイヤーと敵の接触の場合
    if(aType == TYPE_PLAYER && bType == TYPE_ENEMY) {
        if( ! hw->enemys[bNum]->isInvincible ) {
            hw->isContacted = true;
            isContactedToEnemy = true;
            contactPlayerNum = aNum;
            hw->contactPlayerOffence = hw->monkeys[aNum]->offense;
            hw->contactEnemyindex = bNum;
            hw->pIndex = aNum;
            
            setPlayerBoundRatio(aBody, bType, bBody, bNum);
            
            for(int i = 0; i < HIT_EF_NUM; i++) {
                if(hw->hitEfs[i]->getVisible()) continue;
                hw->hitEfs[i]->setPosition(contactPoint.x, contactPoint.y);
                hw->hitEfs[i]->setVisible(true);
                break;
            }
        }
    } else if(aType == TYPE_ENEMY && bType == TYPE_PLAYER) {
        if( ! hw->enemys[aNum]->isInvincible ) {
            hw->isContacted = true;
            isContactedToEnemy = true;
            contactPlayerNum = bNum;
            hw->contactPlayerOffence = hw->monkeys[bNum]->offense;
            hw->contactEnemyindex = aNum;
            hw->pIndex = bNum;
            
            setPlayerBoundRatio(bBody, aType, aBody, aNum);
            
            for(int i = 0; i < HIT_EF_NUM; i++) {
                if(hw->hitEfs[i]->getVisible()) continue;
                hw->hitEfs[i]->setPosition(contactPoint.x, contactPoint.y);
                hw->hitEfs[i]->setVisible(true);
                break;
            }
        }
    }
    
    //プレイヤーと障害物との接触の場合
    if (aType == TYPE_PLAYER && bType == TYPE_OBSTACLE) {
        setPlayerBoundRatio(aBody, bType, bBody, bNum);
    } else if(aType == TYPE_OBSTACLE && bType == TYPE_PLAYER) {
        setPlayerBoundRatio(bBody, aType, aBody, aNum);
    }
    
}

//どのオブジェクトに衝突したかによって、プレイヤーの速度の減衰率を調整
void CContactListener::setPlayerBoundRatio(b2Body *playerBody, int npType, b2Body *npBody, int npNum) {
    //プレイヤーの速度を取得
    b2Vec2 playerVec = playerBody->GetLinearVelocity();
    if ((-30 < playerVec.x && playerVec.x < 30) && (-30 < playerVec.y && playerVec.y < 30)) {
        return;
    }
    
    HelloWorld* hw = HelloWorld::Instance;
    switch (npType) {
        case TYPE_PLAYER:
        {
            playerBody->SetLinearDamping(8.0f);
            npBody->SetLinearDamping(8.0f);
            break;
        }
        case TYPE_ENEMY:
        {
            playerBody->SetLinearDamping(hw->enemys[npNum]->getPlayerBoundRatio());
            break;
        }
        case TYPE_OBSTACLE:
        {
            playerBody->SetLinearDamping(hw->obstacles[npNum]->getPlayerBoundRatio());
            break;
        }
        default:
            break;
    }
    
}


