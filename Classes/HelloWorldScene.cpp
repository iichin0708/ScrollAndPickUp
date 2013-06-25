//
//  HelloWorldScene.cpp
//  box2d
//
//  Created by TakuyaIida on 13/06/18.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CContactListener.h"
#include "PhysicsSprite.h"
#include "World.h"
#include "Ghost.h"
#include "Cursor.h"


#define SPEE_MAX 25
#define ARROW_SCALE_RATIO 100

using namespace cocos2d;
using namespace CocosDenshion;


HelloWorld* HelloWorld::Instance;

enum {
    kTagParentNode = 1,
};

int HelloWorld::sprite_id = 2;

HelloWorld::HelloWorld()
{
    /*
    float x=0, y=0, z=0;
    CCCamera* pCamera = this->getCamera();

    pCamera->getCenterXYZ(&x, &y, &z);
    pCamera->setCenterXYZ(x, y+0.0000001, z);

    pCamera->getCenterXYZ(&x, &y, &z);
    pCamera->setEyeXYZ(x, y, 500);
    pCamera->getCenterXYZ(&x, &y, &z);
    CCLOG("z = %f", z);
    */

    isMoving = false;
    
    isContacted = false;
    
    isTrace = true;
    
    // 自分自身を静的メンバーから参照できるように設定
    HelloWorld:Instance = this;
    
    sprite_id = 2;
    
    setTouchEnabled( true );
    setAccelerometerEnabled( true );
    
    // 画面サイズ取得
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    // init physics
    this->initPhysics();
    
    // フィールドの用意
    field = new Field();
    field->setPosition(s.width/2, field->height/2);
    //field->getSprite()->setVisible(false);
    addChild(field->getSprite());
    
    
    // ワールド（キャラクター行動可能範囲）の用意
    World *world = World::getInstance();
    world->deleteWall();
    world->createWall(field->getRect());
    
    cursor = new Cursor();
    cursor->setPosition(100, 100);
    cursor->setVisible(false);
    addChild(cursor->getSprite());
    
    // 敵の用意
    for(int i = 0; i < ENEMY_NUM; i++) {
        int x = (rand() % (int)(field->getRect().getMaxX() - field->getRect().getMinX())) + field->getRect().getMinX();
        int y = rand() % (int)(field->getRect().getMaxY() - field->getRect().getMinY() - s.height / 1.5f) + field->getRect().getMinY() + s.height / 1.5f;
        
        enemys[i] = new Enemy(this,
                              rand() % 3,
                              ccp(x, y),
                              1.5f,
                              0.5f,
                              0.6f);
    }
    
    // コインの用意
    for(int i = 0; i < COIN_NUM; i++) {
        coins[i] = new Coin();
        coins[i]->setVisible(false, 0);
        //coins[i]->setPosition(100, 100);
        addChild(coins[i]->getSprite());
    }
    
    for(int i = 0; i < ENEMY_NUM; i++) {
        ghosts[i] = new Ghost();
        ghosts[i]->setVisible(false);
        addChild(ghosts[i]->getSprite());
    }
    
    //矢印の画像を読み込む
    arrow = CCSprite::create("arrow_m.png", CCRectMake(0, 0, 100, 60));
    //表示位置の設定
    arrow->setPosition(ccp(0, 0));
    
    arrow->setVisible(false);
    
    arrow->setAnchorPoint(CCPointMake(0, 0.5f));
    
    //スプライトをレイヤに追加
    this->addChild(arrow);
    
    // 当たったときのエフェクト追加
    for(int i = 0; i < HIT_EF_NUM; i++) {
        hitEfs[i] = new HitEffect();
        hitEfs[i]->setVisible(false);
        addChild(hitEfs[i]->getSprite());
    }
    
    // プレイヤーの用意
    for(int i =0; i < PLAYER_NUM; i++) {
        monkeys[i] = new Player(this, ccp(s.width/2 -150 + 150 * i, s.height/2 + -100), 1.5f, 0.6f, 1.0f);
    }
    
    // 終了ボタン
    CCMenuItemImage *pCloseItem =
    CCMenuItemImage::create(
                            "button_close.png",
                            "button_close_pressed.png",
                            this,
                            menu_selector(HelloWorld::menuCloseCallback));
    pCloseItem->setPosition(ccp(s.width - pCloseItem->getContentSize().width / 2,
                                pCloseItem->getContentSize().height / 2));
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu);
    
    isObjectTouched = false;
    
    shootRadian = 0;
    
    blink = false;
        
    world->setContactListener();
    
    scheduleUpdate();
}

HelloWorld::~HelloWorld()
{
    //    delete world;
    //    world = NULL;
    
    //    delete m_debugDraw;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::initPhysics()
{
    World *world = World::getInstance();
    //CCPoint delta = CCPointMake(-50.0f, 50.0f);
    //world->moveWall(delta);
    
}

void HelloWorld::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    World *world = World::getInstance();
    world->getWorld()->DrawDebugData();
    
    kmGLPopMatrix();
}

//idを持つ剛体を終点から始点のベクトルにボディを飛ばす
void HelloWorld::flickBody(CCPoint start, CCPoint end, b2Body* object)
{
    //タッチしたオブジェクトの中心座標を取得
    CCPoint objectPoint = CCPointMake(touchObjectBody->GetPosition().x * PTM_RATIO, touchObjectBody->GetPosition().y * PTM_RATIO);
    
    //startとendの距離を測る
    float diffX = end.x - objectPoint.x;
    float diffY = end.y - objectPoint.y;
    float distance = sqrt(diffX * diffX + diffY * diffY);
    
    int x_plusFlag;
    int y_plusFlag;
    
    if (end.x - objectPoint.x <= 0) {
        x_plusFlag = 1;
    } else if (end.x == objectPoint.x) {
        x_plusFlag = 0;
    } else {
        x_plusFlag = -1;
    }
    
    if (end.y - objectPoint.y <= 0) {
        y_plusFlag = 1;
    } else if (end.y == objectPoint.y){
        y_plusFlag = 0;
    } else {
        y_plusFlag = -1;
    }
    
    //    CCLog("distance %f", distance);
    
    //CCLog("x_plusFlag = %d, y_plusFlag = %d", x_plusFlag, y_plusFlag);
    
    //角度と速度
    b2Vec2 angleAndSpeed = b2Vec2(-diffX, -diffY);
    
    
    //距離によって物体の初速度を変更する
    if(distance < 75) {
        angleAndSpeed.x = angleAndSpeed.x * 0.15;
        angleAndSpeed.y = angleAndSpeed.y * 0.15;
    } else if(75 <= distance && distance < 100) {
        angleAndSpeed.x = angleAndSpeed.x * 0.25;
        angleAndSpeed.y = angleAndSpeed.y * 0.25;
    } else if(100 <= distance && distance < 125) {
        angleAndSpeed.x = angleAndSpeed.x * 0.35;
        angleAndSpeed.y = angleAndSpeed.y * 0.35;
    } else if(125 <= distance && distance < 150) {
        angleAndSpeed.x = angleAndSpeed.x * 0.35;
        angleAndSpeed.y = angleAndSpeed.y * 0.35;
    } else if( 150 <= distance && distance < 175) {
        angleAndSpeed.x = angleAndSpeed.x * 0.35;
        angleAndSpeed.y = angleAndSpeed.y * 0.35;
    } else if (175 <= distance) {
        angleAndSpeed.x = angleAndSpeed.x * 1.0f;
        angleAndSpeed.y = angleAndSpeed.y * 1.0f;
    }
    
    //CCLog("angeleX = %f, angleY = %f", angleAndSpeed.x, angleAndSpeed.y);
    
    //CCLog("x->%f, y->%f", speed.x, speed.y);
    //CCLog("shoot = %f", CC_RADIANS_TO_DEGREES(shootRadian));
    object->SetTransform(object->GetPosition(), shootRadian);
    
    object->SetLinearVelocity(angleAndSpeed);
    isMoving = true;
    
    Player::playerTurnId++;
}

void HelloWorld::update(float dt)
{
    if(isContacted && // 衝突した
       enemys[contactEnemyindex] != NULL && // 指定の敵配列が空でない
       ! enemys[contactEnemyindex]->isInvincible ) // 敵が無敵でない
    {
        enemys[contactEnemyindex]->damaged();
        
        // 敵キャラが倒れたときの処理
        if(enemys[contactEnemyindex]->hp <= 0) {
            int countCoin = 0;
            
            // コインを出す
            for(int k = 0; k < COIN_NUM; k++) {
                if(coins[k]->getVisible()) continue;
                coins[k]->setVisible(true, enemys[contactEnemyindex]->getBody()->GetPosition().y * PTM_RATIO);
                coins[k]->setPosition(enemys[contactEnemyindex]->getBody()->GetPosition().x * PTM_RATIO,
                                      enemys[contactEnemyindex]->getBody()->GetPosition().y * PTM_RATIO);
                coins[k]->tarIndex = pIndex;
                countCoin++;
                if(countCoin >= 5) break;
            }
            
            // 幽霊を出す
            for(int i = 0; i < ENEMY_NUM; i++) {
                if(ghosts[i]->getVisible()) continue;
                ghosts[i]->setVisible(true);
                ghosts[i]->setPosition(enemys[contactEnemyindex]->getBody()->GetPosition().x * PTM_RATIO,
                                       enemys[contactEnemyindex]->getBody()->GetPosition().y * PTM_RATIO);
                break;
            }
            
            removeChild((PhysicsSprite*)enemys[contactEnemyindex]->getBody()->GetUserData());
            destroyObject((RigidBody *&)enemys[contactEnemyindex]);
        }
        
        isContacted = false;
    }
    
    //動いている物体と同時にマップを動かす
    if (isMoving) {
        b2Vec2 moveObjectVec = touchObjectBody->GetLinearVelocity();
        if( (-1.0f < moveObjectVec.x && moveObjectVec.x < 1.0f) && (-1.0f < moveObjectVec.y && moveObjectVec.y < 1.0f)) {
            isMoving = false;
        }
        
        CCSize dispSize = CCDirector::sharedDirector()->getWinSize();
        CCPoint dispCenter = CCPointMake(dispSize.width / 2, dispSize.height / 2);
        
        movedPosition = CCPointMake(touchObjectBody->GetPosition().x * PTM_RATIO, touchObjectBody->GetPosition().y * PTM_RATIO);
        
        float delta = ccpDistance(dispCenter, movedPosition);
        
        //CCLog("delta = %f", delta);
        
        if(100 < delta) {
            isTrace = true;
        }
        
        if(isTrace) {
            CCPoint gap = CCPointMake(dispCenter.x - movedPosition.x, dispCenter.y - movedPosition.y);
            speedVec = CCPointMake(gap.x / 10, gap.y / 10);
            if (delta < 10) {
                isTrace = false;
            } else {
                moveMap(speedVec);
            }
        }
    }
    
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    World *world = World::getInstance();
    world->getWorld()->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body* b = world->getWorld()->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //CCLog("getLinearVelocity.x -> %f, getLinearVelocity.y -> %f", b->GetLinearVelocity().x, b->GetLinearVelocity().y);
            
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }
    }
    
    //コインの毎フレーム実行する処理
    for(int i = 0; i < COIN_NUM; i++) {
        if(coins[i] == NULL) continue;
        if(coins[i]->getVisible()) {
            coins[i]->update();
        }
    }
    
    //おばけの毎フレーム実行する処理
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(ghosts[i] == NULL) continue;
        if(ghosts[i]->getVisible()) {
            ghosts[i]->update();
        }
    }
    
    // プレイヤーの毎フレーム実行する処理
    for(int i = 0; i < PLAYER_NUM; i++) {
        if(monkeys[i] == NULL) continue;
        //回転を止める
        monkeys[i]->getBody()->SetTransform(b2Vec2(monkeys[i]->getBody()->GetPosition().x,
                                                   monkeys[i]->getBody()->GetPosition().y), 0);

        //そのターンのプレイヤーにポインタの表示.　かつ、マップの移動
        if(i == Player::getPlayerTurnId()){
            //カーソルのひょうじ
            cursor->setPosition(monkeys[i]->getBody()->GetPosition().x * PTM_RATIO,
                                monkeys[i]->getBody()->GetPosition().y * PTM_RATIO);
            cursor->setVisible(true);
            //cursor->getSprite()->runAction(cocos2d::CCBlink::create(0.7f, 5));
            
            //プレイヤーの存在するマップまでの移動
            CCPoint playerPosition = monkeys[i]->getPlayerPosition(i);
            
            CCSize dispSize = CCDirector::sharedDirector()->getWinSize();
            CCPoint dispCenter = CCPointMake(dispSize.width / 2, dispSize.height / 2);
            float delta = ccpDistance(dispCenter, playerPosition);
            
            if(100 < delta) {
                isTrace = true;
            }
            
            if(isTrace) {
                CCPoint gap = CCPointMake(dispCenter.x - playerPosition.x, dispCenter.y - playerPosition.y);
                speedVec = CCPointMake(gap.x / 10, gap.y / 10);
                if (delta < 10) {
                    isTrace = false;
                } else {
                    moveMap(speedVec);
                }
            }
        }
    }
    
    // 敵キャラの毎フレーム実行する処理
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(enemys[i] == NULL) continue;
        //回転を止める
        enemys[i]->getBody()->SetTransform(b2Vec2(enemys[i]->getBody()->GetPosition().x,
                                                  enemys[i]->getBody()->GetPosition().y), 0);
        //CCSprite sprite = enemys[i]->getBody()->GetUserData();
        enemys[i]->hpSprite->setPosition(ccp(enemys[i]->getBody()->GetPosition().x * PTM_RATIO,
                                             enemys[i]->getBody()->GetPosition().y * PTM_RATIO - enemys[i]->height / 2));
    }
    
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
    
    CCTouch *touch = (CCTouch *)pTouches->anyObject();
    //タッチ座標を取得
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
    //タッチしはじめの点を取得
    startPoint = touchLocation;
    moveTouchPoint = touchLocation;
    
    //タッチした矩形の大きさを定義.
    CCRect touchRect = CCRectMake(startPoint.x - 10, startPoint.y - 10, 20, 20);
    
    //Playerにタッチしているかどうか判定する.(タッチしていればstartPointとendPointから剛体を飛ばす)
    for(int i = 0; i < PLAYER_NUM; i++) {
        if(monkeys[i] == NULL) continue;
        //PhysicsSpriteを得る
        PhysicsSprite *playerSprite = (PhysicsSprite*)monkeys[i]->getBody()->GetUserData();
        //剛体の矩形を得る
        CCRect playerRect = CCRectMake(monkeys[i]->getBody()->GetPosition().x * PTM_RATIO - playerSprite->getContentSize().width / 2,
                                       monkeys[i]->getBody()->GetPosition().y * PTM_RATIO - playerSprite->getContentSize().height/2,
                                       playerSprite->getContentSize().width, playerSprite->getContentSize().height);
        
        //タッチした範囲に剛体の矩形が入っているかどうか判定する. 且つ,そのターンのプレイヤーかどうか判定.
        if(touchRect.intersectsRect(playerRect) && i == Player::getPlayerTurnId()) {
            isObjectTouched = true;
            touchObjectBody = monkeys[i]->getBody();
            arrow->setScale(0.35f);
            
            break;
        } else {
            isObjectTouched = false;
        }
    }
}

void HelloWorld::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    CCTouch *touch = (CCTouch *)pTouches->anyObject();
    
    //タッチ座標を取得
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    if(isObjectTouched) {
        //movePointがプラスかマイナスか判別するフラグ
        int x_plusFlag;
        int y_plusFlag;
        
        PhysicsSprite *pSprite = (PhysicsSprite *)touchObjectBody->GetUserData();
        
        //タッチしたオブジェクトの半径(正確には矩形)
        float radius = pSprite->getContentSize().width / 2;
        
        //タッチしたオブジェクトの中心座標を取得
        CCPoint objectPoint = CCPointMake(touchObjectBody->GetPosition().x * PTM_RATIO, touchObjectBody->GetPosition().y * PTM_RATIO);
        /*
         CCLog("radius = %f", radius);
         CCLog("move.x   = %f, move.y   = %f", touchLocation.x, touchLocation.y);
         CCLog("object.x = %f, ovject.y = %f", objectPoint.x, objectPoint.y);
         */
        
        //startとendの距離を測る
        float diffX = touchLocation.x - objectPoint.x;
        float diffY = touchLocation.y - objectPoint.y;
        float distance = sqrt(diffX * diffX + diffY * diffY);
        
        //表示位置の計算
        float x = sqrt( ((touchLocation.x - objectPoint.x) * (touchLocation.x - objectPoint.x) * radius * radius) / ((touchLocation.x - objectPoint.x) * (touchLocation.x - objectPoint.x) + (touchLocation.y - objectPoint.y) * (touchLocation.y - objectPoint.y)) );
        float y = sqrt( ((touchLocation.y - objectPoint.y) * (touchLocation.y - objectPoint.y) * radius * radius) / ((touchLocation.x - objectPoint.x) * (touchLocation.x - objectPoint.x) + (touchLocation.y - objectPoint.y) * (touchLocation.y - objectPoint.y)) );
        
        //矢印の回転
        float arcRadian = atanf(((touchLocation.y - objectPoint.y) / (touchLocation.x - objectPoint.x)));
        shootRadian = arcRadian;
        //ラジアンを角度に変換
        float arcDegree = -1 * CC_RADIANS_TO_DEGREES(arcRadian);
        //        CCLog("arcDegree => %f", arcDegree);
        
        
        if (objectPoint.x < touchLocation.x) {
            x_plusFlag = -1;
            x *= -1;
            if(objectPoint.y < touchLocation.y) {
                y_plusFlag = -1;
                arcDegree += 180;
                y *= -1;
            } else if(objectPoint.y == touchLocation.y) {
                y_plusFlag = 0;
                arcDegree += 180;
            } else {
                y_plusFlag = 1;
                arcDegree += 180;
            }
        } else  if(objectPoint.x == touchLocation.x){
            x_plusFlag = 0;
            if(objectPoint.y < touchLocation.y) {
                y_plusFlag = -1;
                //arcDegree += 90;
                y *= -1;
            } else if(objectPoint.y == touchLocation.y) {
                y_plusFlag = 0;
            } else {
                y_plusFlag = 1;
                //arcDegree += 270;
            }
        } else {
            x_plusFlag = 1;
            if(objectPoint.y < touchLocation.y) {
                y_plusFlag = -1;
                arcDegree += 360;
                y *= -1;
            } else if(objectPoint.y == touchLocation.y) {
                y_plusFlag = 0;
            } else {
                y_plusFlag = 1;
            }
        }
        
        //CCLog("distance => %f", distance);
        if(distance < 200) {
            arrow->setScale(distance/ARROW_SCALE_RATIO);
        }
        
        //表示位置の設定
        arrow->setPosition(ccp(x + objectPoint.x, y + objectPoint.y));
        
        arrow->setRotation(arcDegree);
        
        //半径以下の時矢印を表示しない
        if (radius < ccpDistance(startPoint, touchLocation)) {
            arrow->setColor(ccc3(255, 200, 100));
            arrow->setVisible(true);
        } else {
            arrow->setColor(ccc3(0, 200, 100));
            arrow->setVisible(true);
        }
        
    } else {
        // 画面をタッチして動かす処理
        CCPoint touchGap = ccp(touchLocation.x - moveTouchPoint.x,
                               touchLocation.y - moveTouchPoint.y);
        
        moveMap(touchGap);
        
        moveTouchPoint = touchLocation;
    }
}


void HelloWorld::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
}


void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    //Add a new body/atlas sprite at the touched location
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        endPoint = touchLocation;
        
        //タッチしていたオブジェクトを飛ばす
        if(isObjectTouched) {
            PhysicsSprite *pSprite = (PhysicsSprite *)touchObjectBody->GetUserData();
            //タッチしたオブジェクトの半径(正確には矩形)
            float radius = pSprite->getContentSize().width / 2;
            //ただし、フリック距離が半径以下ならば飛ばさない
            if(ccpDistance(startPoint, endPoint) < radius) {
                break;
            }
            flickBody(startPoint, endPoint, touchObjectBody);
        }
    }
    
    cursor->setVisible(false);
    arrow->setVisible(false);
}


void HelloWorld::moveMap(CCPoint touchGap) {
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    // x方向にフィールドが動かせるかどうか
    if(field->getPosition().x + touchGap.x <= field->getSprite()->getContentSize().width/2 &&
       field->getPosition().x + field->getSprite()->getContentSize().width / 2 + touchGap.x >= s.width) {
        
        // フィールドスライド
        field->setPosition(field->getPosition().x + touchGap.x,
                           field->getPosition().y);

        // プレイヤースライド
        for(int i = 0; i < PLAYER_NUM; i++) {
            if(monkeys[i] == NULL) continue;
            monkeys[i]->getBody()->SetTransform(b2Vec2(monkeys[i]->getBody()->GetPosition().x + touchGap.x / PTM_RATIO,
                                                       monkeys[i]->getBody()->GetPosition().y), 0);
        }
        
        // 敵スライド
        for(int i = 0; i < ENEMY_NUM; i++) {
            if(enemys[i] == NULL) continue;
            enemys[i]->getBody()->SetTransform(b2Vec2(enemys[i]->getBody()->GetPosition().x + touchGap.x / PTM_RATIO,
                                                      enemys[i]->getBody()->GetPosition().y), 0);
        }
        
        // コインスライド
        for(int i = 0; i < COIN_NUM; i++) {
            if(coins[i] == NULL) continue;
            coins[i]->setPosition(coins[i]->getPosition().x + touchGap.x,
                                  coins[i]->getPosition().y);
        }
        
        // 幽霊スライド
        for(int i = 0; i < ENEMY_NUM; i++) {
            if(ghosts[i] == NULL) continue;
            ghosts[i]->setPosition(ghosts[i]->getPosition().x + touchGap.x,
                                   ghosts[i]->getPosition().y);
        }
        
        // エフェクトスライド
        for(int i = 0; i < HIT_EF_NUM; i++) {
            if(hitEfs[i] == NULL) continue;
            hitEfs[i]->setPosition(hitEfs[i]->getPosition().x + touchGap.x,
                                   hitEfs[i]->getPosition().y);
        }
    }
    
        
    // y方向にフィールドが動かせるかどうか
    if(field->getPosition().y + touchGap.y <= field->height / 2 &&
       field->getPosition().y + field->height / 2 + touchGap.y >= s.height) {
        
        // フィールドスライド
        field->setPosition(field->getPosition().x,
                           field->getPosition().y + touchGap.y);
        
        // プレイヤースライド
        for(int i = 0; i < PLAYER_NUM; i++) {
            if(monkeys[i] == NULL) continue;
            monkeys[i]->getBody()->SetTransform(b2Vec2(monkeys[i]->getBody()->GetPosition().x,
                                                       monkeys[i]->getBody()->GetPosition().y + touchGap.y / PTM_RATIO), 0);
        }
        
        // 敵スライド
        for(int i = 0; i < ENEMY_NUM; i++) {
            if(enemys[i] == NULL) continue;
            enemys[i]->getBody()->SetTransform(b2Vec2(enemys[i]->getBody()->GetPosition().x,
                                                      enemys[i]->getBody()->GetPosition().y + touchGap.y / PTM_RATIO), 0);
        }
        
        // コインスライド
        for(int i = 0; i < COIN_NUM; i++) {
            if(coins[i] == NULL) continue;
            coins[i]->setPosition(coins[i]->getPosition().x,
                                  coins[i]->getPosition().y + touchGap.y);
            coins[i]->groundHeight += touchGap.y;
        }
        
        // 幽霊スライド
        for(int i = 0; i < ENEMY_NUM; i++) {
            if(ghosts[i] == NULL) continue;
            ghosts[i]->setPosition(ghosts[i]->getPosition().x,
                                   ghosts[i]->getPosition().y + touchGap.y);
        }
        
        // エフェクトスライド
        for(int i = 0; i < HIT_EF_NUM; i++) {
            if(hitEfs[i] == NULL) continue;
            hitEfs[i]->setPosition(hitEfs[i]->getPosition().x,
                                   hitEfs[i]->getPosition().y + touchGap.y);
        }
    }
    
    World *world =  World::getInstance();
    world->deleteWall();
    
    world->createWall(field->getRect());
}

void HelloWorld::destroyObject(RigidBody *&object) {
    if (object == NULL) {
        return;
    }
    World *world = World::getInstance();
    world->destroyBody(object->getBody());
    object = NULL;
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new HelloWorld();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

