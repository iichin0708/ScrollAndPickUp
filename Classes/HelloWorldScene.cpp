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

int HelloWorld::realDispWidth = 0;
int HelloWorld::realDispHeight = 0;

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
    isGameover = false;
    
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
    //cursor->setVisible(false);
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
    
    // 水の用意
    for(int i = 0; i < WATER_NUM; i++) {
        waters[i] = new Water();
        waters[i]->setVisible(false, 0);
        addChild(waters[i]->getSprite());
    }
    
    // コインの用意
    for(int i = 0; i < COIN_NUM; i++) {
        coins[i] = new Coin();
        coins[i]->setVisible(false, 0);
        addChild(coins[i]->getSprite());
    }
    
    // 敵キャラの用意
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
    this->addChild(arrow, 2);
    
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

    // 障害物の配置
    setObstacle();
    
    // 終了ボタン
    CCMenuItemImage *pCloseItem =
    CCMenuItemImage::create("button_close.png",
                            "button_close_pressed.png",
                            this,
                            menu_selector(HelloWorld::menuCloseCallback));
    
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCPoint origin = pDirector->getVisibleOrigin();
    CCSize visibleSize = pDirector->getVisibleSize();
    
    pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width / 2,
                                origin.y + pCloseItem->getContentSize().height / 2));
    
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu);
    
    isObjectTouched = false;
    
    isShowedNextPlayer = false;
    
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
    World::getInstance();
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

void HelloWorld::setObstacle() {
    
    CCDictionary *imgDict = CCDictionary::create();

    for (int i = 0; i < OBSTACLE_NUM; i++) {
        obstacles[i] = NULL;
    }
    
    /*
    //障害物
    CCArray *imgSize = CCArray::createWithCapacity(2);
    imgSize->addObject(CCFloat::create(228));
    imgSize->addObject(CCFloat::create(285));
    imgDict->setObject(imgSize, "wood01.png");

    
    imgSize = new CCArray(2);
    imgSize[0] = 183;
    imgSize[1] = 248;
    imgDict->setObject(imgSize, "wood02.png");
    
    imgSize = new CCArray(2);
    imgSize[0] = 157;
    imgSize[1] = 170;
    imgDict->setObject(imgSize, "wood03.png");
    
    imgSize = new CCArray(2);
    imgSize[0] = 190;
    imgSize[1] = 135;
    imgDict->setObject(imgSize, "wood04.png");
    
    imgSize = new CCArray(2);
    imgSize[0] = 174;
    imgSize[1] = 145;
    imgDict->setObject(imgSize, "wood05.png");
    
    imgSize = new CCArray(2);
    imgSize[0] = 95;
    imgSize[1] = 120;
    imgDict->setObject(imgSize, "wood06.png");
    
    imgSize = new CCArray(2);
    imgSize[0] = 278;
    imgSize[1] = 246;
    imgDict->setObject(imgSize, "wood07.png");
    
    imgSize = new CCArray(2);
    imgSize[0] = 97;
    imgSize[1] = 40;
    imgDict->setObject(imgSize, "wood08.png");
    
    imgSize = new CCArray(2);
    imgSize[0] = 55;
    imgSize[1] = 392;
    imgDict->setObject(imgSize, "saku01.png");

    imgSize = new CCArray(2);
    imgSize[0] = 49;
    imgSize[1] = 279;
    imgDict->setObject(imgSize, "saku02.png");

    imgSize = new CCArray(2);
    imgSize[0] = 192;
    imgSize[1] = 131;
    imgDict->setObject(imgSize, "iwa01.png");
    
    imgSize = new CCArray(2);
    imgSize[0] = 268;
    imgSize[1] = 168;
    imgDict->setObject(imgSize, "iwa02.png");
    */
    

    //作成するShape(当たり判定の形)は丸とする.
    //(parent, 画像名, 画像サイズ, 当たり判定サイズ, 初期位置, Shapeの形, 質量, 摩擦, 反発係数)となっている. boxの場合は当たり判定に縦と横のサイズを渡すが、cricleの場合は半径を渡す.
    obstacles[8] = new Obstacle(this, "saku01.png", CCSizeMake(55, 392), CCSizeMake(55 / 2, 392 / 2), CCPointMake(40, 320), BOX_SHAPE, 0.0f, 0.0f, 0.0f);
    obstacles[9] = new Obstacle(this, "saku02.png", CCSizeMake(49, 279), CCSizeMake(49 / 2, 279 / 2), CCPointMake(40, 1200), BOX_SHAPE, 0.0f, 0.0f, 0.0f);
    
    obstacles[0] = new Obstacle(this, "wood01.png", CCSizeMake(228, 285), CCSizeMake(228 / 2, 228 / 2), CCPointMake(550, 300), CIRCLE_SHAPE, 0.0f, 0.0f, 0.0f);
    obstacles[1] = new Obstacle(this, "wood02.png", CCSizeMake(183, 248), CCSizeMake(183 / 2, 183 / 2), CCPointMake(100, 1600), CIRCLE_SHAPE, 0.0f, 0.0f, 0.0f);
    obstacles[2] = new Obstacle(this, "wood03.png", CCSizeMake(157, 170), CCSizeMake(157 / 2, 157 / 2), CCPointMake(400, 1050), CIRCLE_SHAPE, 0.0f, 0.0f, 0.0f);
    obstacles[3] = new Obstacle(this, "wood04.png", CCSizeMake(190, 135), CCSizeMake(135 / 2, 135 / 2), CCPointMake(180, 200), CIRCLE_SHAPE, 0.0f, 0.0f, 0.0f);
    obstacles[4] = new Obstacle(this, "wood05.png", CCSizeMake(174, 145), CCSizeMake(145 / 2, 145 / 2), CCPointMake(180, 1980), CIRCLE_SHAPE, 0.0f, 0.0f, 0.0f);
    obstacles[5] = new Obstacle(this, "wood06.png", CCSizeMake(95, 83), CCSizeMake(95 / 2, 95 / 2), CCPointMake(550, 1800), CIRCLE_SHAPE, 0.0f, 0.0f, 0.0f);
//    obstacles[6] = new Obstacle(this, "wood07.png", CCSizeMake(246, 278), CCPointMake(120, 750), 1.0f, 0.6f, 0.6f);
    obstacles[6] = new Obstacle(this, "wood07.png", CCSizeMake(278, 168), CCSizeMake(168 / 2, 168 / 2), CCPointMake(120, 780), CIRCLE_SHAPE, 0.0f, 0.0f, 0.0f);
    obstacles[7] = new Obstacle(this, "wood08.png", CCSizeMake(81, 40), CCSizeMake(40 / 2, 40 / 2), CCPointMake(550, 1500), CIRCLE_SHAPE, 0.0f, 0.0f, 0.0f);
    obstacles[10] = new Obstacle(this, "iwa01.png", CCSizeMake(192, 131), CCSizeMake(131 / 2, 131 / 2), CCPointMake(400, 550), CIRCLE_SHAPE, 0.0f, 0.0f, 0.0f);
    obstacles[11] = new Obstacle(this, "iwa02.png", CCSizeMake(268, 168), CCSizeMake(168 / 2, 0), CCPointMake(500, 1620), CIRCLE_SHAPE, 0.0f, 0.0f, 0.0f);
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
        angleAndSpeed.x = angleAndSpeed.x * 0.5f;
        angleAndSpeed.y = angleAndSpeed.y * 0.5f;
    }
    
    //CCLog("angeleX = %f, angleY = %f", angleAndSpeed.x, angleAndSpeed.y);
    
    //CCLog("x->%f, y->%f", speed.x, speed.y);
    //CCLog("shoot = %f", CC_RADIANS_TO_DEGREES(shootRadian));
    object->SetTransform(object->GetPosition(), shootRadian);
    
    object->SetLinearVelocity(angleAndSpeed);
    isMoving = true;
    
    Player::playerTurnId++;
    
    // 次ターンのプレイヤーがいなければ順番を飛ばす
    while(monkeys[Player::getPlayerTurnId()] == NULL ||
          monkeys[Player::getPlayerTurnId()]->isFalled) {
        Player::playerTurnId++;
    }
}

void HelloWorld::update(float dt)
{
    int countPlayer = 0;
    // 対象のプレイヤーがいなければ順番を飛ばす
    for(int i = 0; i < PLAYER_NUM; i++) {
        if(monkeys[Player::getPlayerTurnId()] == NULL ||
           monkeys[Player::getPlayerTurnId()]->isFalled) {
            Player::playerTurnId++;
        } else {
            countPlayer++;
        }
    }
    if(countPlayer < 1) {
        isGameover = true;
    }
    
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
    if (isMoving && touchObjectBody != NULL) {
        b2Vec2 vec = touchObjectBody->GetLinearVelocity();
        CCLog("vec.x = %f, vec.y = %f", vec.x, vec.y);
        //最後にスーっと動くように。
        if((-10 < vec.x && vec.x < 10) && (-10 < vec.y && vec.y < 10)) {
            touchObjectBody->SetLinearDamping(4.0f);
        }
        //moveMapWithObject(touchObjectBody->GetLinearVelocity());
        moveMapWithObject(touchObjectBody);
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
    
    // コインの毎フレーム実行する処理
    for(int i = 0; i < COIN_NUM; i++) {
        if(coins[i] == NULL) continue;
        if(coins[i]->getVisible()) {
            coins[i]->update();
        }
    }
    
    // 水しぶきの毎フレーム実行する処理
    for(int i = 0; i < WATER_NUM; i++) {
        if(waters[i] == NULL) continue;
        if(waters[i]->getVisible()) {
            waters[i]->update();
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
                
        //そのターンのプレイヤーにポインタの表示.
        if(i == Player::getPlayerTurnId()){
            //カーソルの表示
            cursor->setPosition(monkeys[i]->getBody()->GetPosition().x * PTM_RATIO,
                                monkeys[i]->getBody()->GetPosition().y * PTM_RATIO);
            cursor->setVisible(true);
        }
        
        // 画面端に行くと消える（落ちる）
        if (monkeys[i]->getBody()->GetPosition().x * PTM_RATIO > field->getPosition().x + field->width / 2||
            monkeys[i]->getBody()->GetPosition().x * PTM_RATIO < field->getPosition().x - field->width / 2) {
            
            int countWater = 0;
            // 水しぶきのエフェクト
            for(int j = 0; j < WATER_NUM; j++) {
                if(waters[j]->getVisible()) continue;
                waters[j]->setVisible(true, monkeys[i]->getBody()->GetPosition().y * PTM_RATIO);
                waters[j]->setPosition(monkeys[i]->getBody()->GetPosition().x * PTM_RATIO,
                                       monkeys[i]->getBody()->GetPosition().y * PTM_RATIO);
                waters[j]->tarIndex = pIndex;
                countWater++;
                if(countWater >= 5) break;
            }
            
            PhysicsSprite* pSprite = (PhysicsSprite*)monkeys[i]->getBody()->GetUserData();
            pSprite->setVisible(false);
            monkeys[i]->isFalled = true;
        }
    }
    
    //次のプレイヤーにマップを一度だけ移動しポインタをあわせる
    if(!isShowedNextPlayer) {
        CCSize dispSize = CCDirector::sharedDirector()->getWinSize();
        CCPoint dispCenter = CCPointMake(dispSize.width / 2, dispSize.height / 2);
        if(monkeys[Player::getPlayerTurnId()] != NULL) {
            CCPoint playerPosition = monkeys[Player::getPlayerTurnId()]->getPlayerPosition(Player::getPlayerTurnId());
            float delta = ccpDistance(dispCenter, playerPosition);
            CCPoint gap = CCPointMake(dispCenter.x - playerPosition.x, dispCenter.y - playerPosition.y);
            speedVec = CCPointMake(gap.x / 10, gap.y / 10);
            if (delta < 20) {
                isShowedNextPlayer = true;
            } else {
                moveMap(speedVec);
            }
        }
    }
     
    
    // 敵キャラの毎フレーム実行する処理
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(enemys[i] == NULL) continue;
        //回転を止める
        /*
        enemys[i]->getBody()->SetTransform(b2Vec2(enemys[i]->getBody()->GetPosition().x,
                                                  enemys[i]->getBody()->GetPosition().y), 0);
         */
        //CCSprite sprite = enemys[i]->getBody()->GetUserData();
        enemys[i]->hpSprite->setPosition(ccp(enemys[i]->getBody()->GetPosition().x * PTM_RATIO,
                                             enemys[i]->getBody()->GetPosition().y * PTM_RATIO - enemys[i]->height / 2));
        // 画面端に行くと消える（落ちる）
        if (enemys[i]->getBody()->GetPosition().x * PTM_RATIO > field->getPosition().x + field->width / 2||
            enemys[i]->getBody()->GetPosition().x * PTM_RATIO < field->getPosition().x - field->width / 2) {
            enemys[i]->hpSprite->setVisible(false);
            
            /*
            // 水しぶきのエフェクト(CCJump)
            for(int j = 0; j < WATER_NUM; j++) {
                if(waters[j]->getVisible()) continue;
                waters[j]->setPosition(enemys[i]->getBody()->GetPosition().x*PTM_RATIO,
                                       enemys[i]->getBody()->GetPosition().y*PTM_RATIO);
                CCLOG("x = %d  :  y = %d", enemys[i]->getBody()->GetPosition().x/PTM_RATIO, enemys[i]->getBody()->GetPosition().y/PTM_RATIO);
                waters[j]->setVisible(true);
                break;
            }
            */
            
            int countWater = 0;
            // 水しぶきのエフェクト
            for(int j = 0; j < WATER_NUM; j++) {
                if(waters[j]->getVisible()) continue;
                waters[j]->setVisible(true, enemys[i]->getBody()->GetPosition().y * PTM_RATIO);
                waters[j]->setPosition(enemys[i]->getBody()->GetPosition().x * PTM_RATIO,
                                      enemys[i]->getBody()->GetPosition().y * PTM_RATIO);
                waters[j]->tarIndex = pIndex;
                countWater++;
                if(countWater >= 5) break;
            }
            
            removeChild((PhysicsSprite*)enemys[i]->getBody()->GetUserData());
            destroyObject((RigidBody *&)enemys[i]);
            isContacted = false;
        }
    }
}

void HelloWorld::playerChange() {
    isShowedNextPlayer = false;
    
    // 溺れ状態のプレイヤーを除去する
    for(int i = 0; i < PLAYER_NUM; i++) {
        if(monkeys[i] != NULL &&
           monkeys[i]->isFalled) {
            cursor->setVisible(false);
            touchObjectBody = NULL;
            removeChild((PhysicsSprite*)monkeys[i]->getBody()->GetUserData());
            destroyObject((RigidBody *&)monkeys[i]);
        }
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
    
    if(isObjectTouched && touchObjectBody != NULL) {
        //movePointがプラスかマイナスか判別するフラグ
        int x_plusFlag;
        int y_plusFlag;
        
        PhysicsSprite *pSprite = (PhysicsSprite *)touchObjectBody->GetUserData();
        
        //タッチしたオブジェクトの半径(正確には矩形)
        float radius = pSprite->getContentSize().width / 2;
        
        //タッチしたオブジェクトの中心座標を取得
        CCPoint objectPoint = CCPointMake(touchObjectBody->GetPosition().x * PTM_RATIO, touchObjectBody->GetPosition().y * PTM_RATIO);
        
        // CCLog("radius = %f", radius);
        // CCLog("move.x   = %f, move.y   = %f", touchLocation.x, touchLocation.y);
        // CCLog("object.x = %f, ovject.y = %f", objectPoint.x, objectPoint.y);
        
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
        if(isObjectTouched && touchObjectBody != NULL) {
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


void HelloWorld::moveMapWithObject(b2Body *moveObjectBody) {
    b2Vec2 moveObjectVec = moveObjectBody->GetLinearVelocity();
    moveObjectVec = touchObjectBody->GetLinearVelocity();
    if( (-1.0f < moveObjectVec.x && moveObjectVec.x < 1.0f) && (-1.0f < moveObjectVec.y && moveObjectVec.y < 1.0f)) {
        isMoving = false;
        moveObjectBody->SetLinearDamping(8.0f);
        this->scheduleOnce(schedule_selector(HelloWorld::playerChange), 0.5f);
        CCLog("");
    }
    
    CCSize dispSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint dispCenter = CCPointMake(dispSize.width / 2, dispSize.height / 2);
    
    movedPosition = CCPointMake(touchObjectBody->GetPosition().x * PTM_RATIO, touchObjectBody->GetPosition().y * PTM_RATIO);
    
    CCPoint gap = CCPointMake(dispCenter.x - movedPosition.x, dispCenter.y - movedPosition.y);
    speedVec = CCPointMake(gap.x / 10, gap.y / 10);
    moveMap(speedVec);
}


void HelloWorld::moveMap(CCPoint touchGap) {
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    // x方向にフィールドが動かせるかどうか
    if(field->getPosition().x + touchGap.x <= field->getSprite()->getContentSize().width / 2 &&
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
    
        // 障害物スライド
        for(int i = 0; i < OBSTACLE_NUM; i++) {
            if(obstacles[i] == NULL) continue;
                obstacles[i]->getBody()->SetTransform(b2Vec2(obstacles[i]->getBody()->GetPosition().x + touchGap.x / PTM_RATIO, obstacles[i]->getBody()->GetPosition().y), 0);
        }


    //}
        // 水しぶきスライド
        for(int i = 0; i < WATER_NUM; i++) {
            if(waters[i] == NULL) continue;
            waters[i]->setPosition(waters[i]->getPosition().x + touchGap.x,
                                   waters[i]->getPosition().y);
            waters[i]->groundHeight += touchGap.y;
        }
    }
    
    /*
    // y方向にフィールドが動かせるかどうか
    if(field->getPosition().y + touchGap.y <= field->height / 2 &&
       field->getPosition().y + field->height / 2 + touchGap.y >= s.height) {
      */  
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
    

        // 障害物スライド
        for(int i = 0; i < OBSTACLE_NUM; i++) {
            if(obstacles[i] == NULL) continue;
            obstacles[i]->getBody()->SetTransform(b2Vec2(obstacles[i]->getBody()->GetPosition().x, obstacles[i]->getBody()->GetPosition().y  + touchGap.y/ PTM_RATIO), 0);
        }

        // 水しぶきスライド
        for(int i = 0; i < WATER_NUM; i++) {
            if(waters[i] == NULL) continue;
            waters[i]->setPosition(waters[i]->getPosition().x,
                                   waters[i]->getPosition().y + touchGap.y);
            waters[i]->groundHeight += touchGap.y;
        }
    

    //}
    
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

