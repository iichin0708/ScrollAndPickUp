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
    //ランダム初期化
    srand((unsigned int)time(NULL));
    /*
    float x=0, y=0, z=0;
    CCCamera* pCamera = this->getCamera();

    pCamera->getCenterXYZ(&x, &y, &z);
    pCamera->setCenterXYZ(x, y+0.0000001, z);

    pCamera->getCenterXYZ(&x, &y, &z);
    pCamera->setEyeXYZ(x, y, 500);
    pCamera->getCenterXYZ(&x, &y, &z);
    CCLOG("z = %f", z);
     iichinのsourceTreeの編集が反映さるのか
     変更してる
    */

    isMoving = false;
    isContacted = false;
    isTrace = true;
    // ゲームイントロ（開始前の導入シーン）フラグ
    isGameintro = true;
    isGameintroMove = false;
    isGameWait = false;
    isGameReady = false;
    
    // ゲームオーバー / ゲームクリアフラグ
    isGameover = false;
    isGameclear = false;
    
    isShotEnemy = false;
    
    // 自分自身を静的メンバーから参照できるように設定
    HelloWorld:Instance = this;
    
    sprite_id = 2;
    
    setTouchEnabled( true );
    setAccelerometerEnabled( true );
    
    CCDirector* pDirector = CCDirector::sharedDirector();
    
    // 画面サイズ取得
    CCSize s = pDirector->getWinSize();
    
    // 表示できる画面サイズ取得
    CCPoint origin = pDirector->getVisibleOrigin();
    CCSize visibleSize = pDirector->getVisibleSize();
    
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
    
    // 幽霊の用意
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
    
    // 移動中エフェクト
    windEffect = CCSprite::create("effect_wind.png");
    windEffect->setOpacity(170);
    windEffect->setAnchorPoint(ccp((windEffect->getContentSize().width - 30) / windEffect->getContentSize().width,
                                    0.5));
    addChild(windEffect);
    
    // 障害物の配置
    setObstacle();
    
    // 終了ボタン
    CCMenuItemImage *pCloseItem =
    CCMenuItemImage::create("button_close.png",
                            "button_close_pressed.png",
                            this,
                            menu_selector(HelloWorld::menuCloseCallback));
    
    pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width / 2,
                                origin.y + pCloseItem->getContentSize().height / 2));
    
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu);
    reorderChild(pMenu, 100);
    
    isObjectTouched = false;
    
    isShowedNextPlayer = false;
    
    isShowedNextEnemy = true;
    
    isPlayerTurn = true;
    
    shootRadian = 0;
    
    blink = false;
        
    world->setContactListener();
    
    // マップ最上部の位置を画面最上部にあわせる
    moveMap(ccp(0, (origin.y + visibleSize.height - field->height/2) - field->getPosition().y));
    
    black = CCSprite::create("brack.png");
    black->setPosition(ccp(s.width / 2, s.height / 2));
    CCFadeOut* actionFadeOut = CCFadeOut::create(1.5f);
    CCCallFunc* actionStart = CCCallFunc::create(this, callfunc_selector(HelloWorld::offIntro));
    CCSequence* actionIntro = CCSequence::create(actionFadeOut, actionStart);
    black->runAction(actionIntro);
    
    stageLabel = CCSprite::create("stage.png");
    stageLabel->setPosition(ccp(s.width / 2, s.height / 2));
    
    addChild(black);
    addChild(stageLabel);
    reorderChild(black, 100);
    reorderChild(stageLabel, 100);
    
    // 表示の順番を決定
    // 背景
    reorderChild(field->getSprite(), 1);
    
    // 矢印
    reorderChild(arrow, 2);
    
    // カーソル
    reorderChild(cursor->getSprite(), 3);

    // 障害物
    for(int i = 0; i < OBSTACLE_NUM; i++) {
        if(obstacles[i] == NULL) continue;
        reorderChild((PhysicsSprite *)obstacles[i]->getBody()->GetUserData(), 4);
    }

    // 敵
    for(int i = 0; i < ENEMY_NUM; i++) {
        reorderChild((PhysicsSprite *)enemys[i]->getBody()->GetUserData(), 5);
    }
    
    // プレイヤー
    for(int i = 0; i < PLAYER_NUM; i++) {
        reorderChild((PhysicsSprite *)monkeys[i]->getBody()->GetUserData(), 6);
    }
    
    // 当たったときのエフェクト
    for(int i = 0; i < HIT_EF_NUM; i++) {
        reorderChild(hitEfs[i]->getSprite(), 7);
    }
    
    // 水
    for(int i = 0; i < WATER_NUM; i++) {
        reorderChild(waters[i]->getSprite(), 8);
    }
    
    // コイン
    for(int i = 0; i < COIN_NUM; i++) {
        reorderChild(coins[i]->getSprite(), 9);
    }
    
    // 幽霊
    for(int i = 0; i < ENEMY_NUM; i++) {
        reorderChild(ghosts[i]->getSprite(), 10);
    }

    scheduleUpdate();
}

void HelloWorld::offIntro()
{
    CCFadeOut* actionFadeOut2 = CCFadeOut::create(0.5f);
    stageLabel->runAction(actionFadeOut2);
    
    isGameintro = false;
    isGameintroMove = true;
}

void HelloWorld::offWait()
{
    isGameReady = true;
    isGameWait = false;
    
    // 画面サイズ取得
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    // Ready...の画像表示
    CCSprite* readyLabel = CCSprite::create("ready.png");
    readyLabel->setPosition(ccp(s.width / 2, s.height / 2));
    CCRotateBy* actionBlank = CCRotateBy::create(1.0f, 1);
    CCFadeOut* actionFadeOut = CCFadeOut::create(1.0f);
    CCCallFunc* actionStart = CCCallFunc::create(this, callfunc_selector(HelloWorld::offReady));
    CCSequence* actionIntro1 = CCSequence::create(actionFadeOut, actionStart);
    
    readyLabel->runAction(actionIntro1);
    
    
    addChild(readyLabel);
    reorderChild(readyLabel, 100);
}

void HelloWorld::offReady()
{
    // 画面サイズ取得
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* goLabel = CCSprite::create("go.png");
    goLabel->setPosition(ccp(s.width / 2, s.height / 2));
    CCFadeOut* actionFadeOut = CCFadeOut::create(0.5f);
    goLabel->runAction(actionFadeOut);
    addChild(goLabel);
    reorderChild(goLabel, 100);
    
    isGameReady = false;
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
    /*
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
     */
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
    
    // 風エフェクトの設定
    // 表示設定
    angleAndSpeed.y *= -1;
    float angle = monkeys[Player::playerTurnId]->getAngle(angleAndSpeed);
    windEffect->setRotation(angle);
    windEffect->setPosition(ccp(monkeys[Player::getPlayerTurnId()]->getBody()->GetPosition().x * PTM_RATIO,
                                monkeys[Player::getPlayerTurnId()]->getBody()->GetPosition().y * PTM_RATIO));
    windEffect->setVisible(true);
    reorderChild(windEffect, 100);
    // フェードアウト設定
    CCFadeOut* actionFadeOut = CCFadeOut::create(0.3f);
    windEffect->runAction(actionFadeOut);
    // 風エフェクトがついているオブジェクトの指定
    windTargetType = TYPE_PLAYER;
    windTargetIndex = Player::getPlayerTurnId();

}

void HelloWorld::update(float dt)
{
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
    
    if(isGameintro) {
        // ゲーム導入シーン
        return;
        
    } else if(isGameintroMove ) {
        // ステージ確認シーン
        
        // 一番したまできたとき
        if(field->getPosition().y >= field->height/2) {
            isGameWait = true;
            isGameintroMove = false;
            CCRotateBy* actionBlank = CCRotateBy::create(1.0f, 0);
            CCCallFunc* actionNext = CCCallFunc::create(this, callfunc_selector(HelloWorld::offWait));
            CCSequence* actionIntro = CCSequence::create(actionBlank, actionNext);
            
            this->runAction(actionIntro);
        } else {
            // スクロール
            moveMap(ccp(0, 6));
        }
        return;
        
    } else if (isGameWait) {
        // 待ち時間
        return;
    } else if(isGameReady) {
        // Ready… のシーン
        return;
    }
    
    int countPlayer = 0;
    // 対象のプレイヤーがいなければ順番を飛ばす
    for(int i = 0; i < PLAYER_NUM; i++) {
        if (monkeys[i] == NULL) {
            continue;
        }
        
        if(!monkeys[i]->isFalled) {
            countPlayer++;
        }
    }
    
    // ゲームオーバーチェック
    if(countPlayer < 1 && ! isGameover && ! isGameclear) {
        isGameover = true;
        CCLabelTTF* overLabel = CCLabelTTF::create("GAME OVER", "arial", 48);
        CCDirector* pDirector = CCDirector::sharedDirector();
        CCPoint origin = pDirector->getVisibleOrigin();
        CCSize visibleSize = pDirector->getVisibleSize();
        
        overLabel->setScale(2.0f);
        overLabel->setPosition(ccp(origin.x + visibleSize.width / 2,
                                   origin.y + visibleSize.height / 2));
        this->addChild(overLabel);
        reorderChild(overLabel, 100);
    }
    
    if(isContacted && // 衝突した
       enemys[contactEnemyindex] != NULL && // 指定の敵配列が空でない
       ! enemys[contactEnemyindex]->isInvincible
       ) // 敵が無敵でない
    {
        if(isPlayerTurn) {
            // ぶつかられた敵はぶつかったプレイヤーの攻撃力分のダメージを受ける
            enemys[contactEnemyindex]->damaged(contactPlayerOffence);
            
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
        } else {
            
            // ぶつかられたプレイヤーはぶつかった敵の攻撃力分のダメージを受ける
            monkeys[contactPlayerindex]->damaged(contactEnemyOffence);
            
            // プレイヤーが倒れたときの処理
            if(monkeys[contactPlayerindex]->hp <= 0) {
                
                // 幽霊を出す
                for(int i = 0; i < ENEMY_NUM; i++) {
                    if(ghosts[i]->getVisible()) continue;
                    ghosts[i]->setVisible(true);
                    ghosts[i]->setPosition(monkeys[contactPlayerindex]->getBody()->GetPosition().x * PTM_RATIO,
                                           monkeys[contactPlayerindex]->getBody()->GetPosition().y * PTM_RATIO);
                    break;
                }
                
                removeChild((PhysicsSprite*)monkeys[contactPlayerindex]->getBody()->GetUserData());
                destroyObject((RigidBody *&)monkeys[contactPlayerindex]);
            }
        }
        isContacted = false;
    }
    
    //動いている物体と同時にマップを動かす
    //プレイヤーの時
    if (isMoving && touchObjectBody != NULL && isPlayerTurn) {
        b2Vec2 vec = touchObjectBody->GetLinearVelocity();
        //CCLog("vec.x = %f, vec.y = %f", vec.x, vec.y);
        //最後にスーっと動くように。
        if((-10 < vec.x && vec.x < 10) && (-10 < vec.y && vec.y < 10)) {
            touchObjectBody->SetLinearDamping(4.0f);
        }
        //moveMapWithObject(touchObjectBody->GetLinearVelocity());
        moveMapWithObject(touchObjectBody);
    } else if (isShotEnemy && isMoving) { //敵プレイヤーの時
        if (enemys[Enemy::enemyTurnId] != NULL) {
            b2Vec2 vec = enemys[Enemy::enemyTurnId]->getBody()->GetLinearVelocity();
            //CCLog("vec.x = %f, vec.y = %f", vec.x, vec.y);
            //最後にスーっと動くように。
            if((-10 < vec.x && vec.x < 10) && (-10 < vec.y && vec.y < 10)) {
                enemys[Enemy::enemyTurnId]->getBody()->SetLinearDamping(4.0f);
            }
            moveMapWithObject(enemys[Enemy::enemyTurnId]->getBody());
        } else { //敵プレイヤーが自分で水に落ちた時
            this->scheduleOnce(schedule_selector(HelloWorld::enemyChange), 0.5f);
            isMoving = false;
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
        if(i == Player::getPlayerTurnId() && isPlayerTurn){
            //カーソルの表示
            cursor->setPosition(monkeys[i]->getBody()->GetPosition().x * PTM_RATIO,
                                monkeys[i]->getBody()->GetPosition().y * PTM_RATIO);
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
    if(!isShowedNextPlayer && isPlayerTurn & !isGameover){
        // カーソルの色
        cursor->setVisible(true);
        cursor->getSprite()->setColor(ccc3(255, 255, 255));
        
        CCSize dispSize = CCDirector::sharedDirector()->getWinSize();
        CCPoint dispCenter = CCPointMake(dispSize.width / 2, dispSize.height / 2);
        if(monkeys[Player::getPlayerTurnId()] != NULL) {
            CCPoint playerPosition = monkeys[Player::getPlayerTurnId()]->getRigidPosition();
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
    
    
    //次の敵プレイヤーにマップを一度だけ移動しポインタをあわせる
    if(!isShowedNextEnemy && !isPlayerTurn && !isGameclear) {
        // カーソルの色
        cursor->setVisible(true);
        cursor->getSprite()->setColor(ccc3(0, 0, 0));

        CCSize dispSize = CCDirector::sharedDirector()->getWinSize();
        CCPoint dispCenter = CCPointMake(dispSize.width / 2, dispSize.height / 2);
        if(enemys[Enemy::getEnemyTurnId()] != NULL) {
            CCPoint enemyPosition = enemys[Enemy::getEnemyTurnId()]->getRigidPosition();
            float delta = ccpDistance(dispCenter, enemyPosition);
            CCPoint gap = CCPointMake(dispCenter.x - enemyPosition.x, dispCenter.y - enemyPosition.y);
            speedVec = CCPointMake(gap.x / 10, gap.y / 10);
            if (delta < 20) {
                isShowedNextEnemy = true;
            } else {
                moveMap(speedVec);
            }
        }
    }
    
//    CCLog("isPlayerTurn = %d", isPlayerTurn);
//    CCLog("isGetEnemyTurnId = %d", Enemy::getEnemyTurnId());
    
    // 敵キャラの毎フレーム実行する処理
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(enemys[i] == NULL) {
            continue;
        }
        //回転を止める
        /*
        enemys[i]->getBody()->SetTransform(b2Vec2(enemys[i]->getBody()->GetPosition().x,
                                                  enemys[i]->getBody()->GetPosition().y), 0);
         */
        //CCSprite sprite = enemys[i]->getBody()->GetUserData();
        
        
        //そのターンの敵プレイヤーにポインタの表示.
        if(i == Enemy::getEnemyTurnId() && !isPlayerTurn){
            //カーソルの表示
            cursor->setPosition(enemys[i]->getRigidPosition().x,
                                enemys[i]->getRigidPosition().y);
            cursor->setVisible(true);
        }
        
        if(!isShotEnemy && !isPlayerTurn && isShowedNextEnemy && !isGameover) {
            moveEnemy(Enemy::getEnemyTurnId());
        }
        
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
    
    // 風のエフェクトが毎フレーム実行する処理
    if(windTargetType == TYPE_PLAYER) {
        if(monkeys[windTargetIndex] != NULL) {
        windEffect->setPosition(monkeys[windTargetIndex]->getRigidPosition());
        } else {
            windEffect->setVisible(false);
        }
    } else if(windTargetType == TYPE_ENEMY) {
        if(enemys[windTargetIndex] != NULL) {
            windEffect->setPosition(enemys[windTargetIndex]->getRigidPosition());
        } else {
            windEffect->setVisible(false);
        }
    }
}

void HelloWorld::playerChange() {
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
    
    int countEnemy = 0;
    // 残っている敵キャラの数を数える
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(enemys[i] == NULL || ! enemys[i]->isVisible()) continue;
        countEnemy++;
    }
    
    // ゲームクリアチェック
    if(countEnemy <= 0 && ! isGameover && ! isGameclear) {
        isGameclear = true;
        CCLabelTTF* clearLabel = CCLabelTTF::create("CLEAR!", "arial", 48);
        CCDirector* pDirector = CCDirector::sharedDirector();
        CCPoint origin = pDirector->getVisibleOrigin();
        CCSize visibleSize = pDirector->getVisibleSize();
        
        clearLabel->setScale(2.0f);
        clearLabel->setPosition(ccp(origin.x + visibleSize.width / 2,
                                    origin.y + visibleSize.height / 2));
        this->addChild(clearLabel);
        reorderChild(clearLabel, 100);
    }
    
    
    Player::prePlayerTurnId = Player::playerTurnId;
    Player::playerTurnId++;
    
    //次の敵プレイヤーのポインタを探す(死んでいた場合は次の敵プレイヤーを探す) プレイヤーが敵を殺した場合も考えて
    int loop = 0;
    for(int i = Player::playerTurnId; loop < PLAYER_NUM; i++) {
        if(monkeys[Player::getPlayerTurnId()] == NULL) {
            Player::playerTurnId++;
        } else {
            break;
        }
        loop++;
    }
   
    //プレイヤーターン終了
    isPlayerTurn = false;
    
    //次の敵プレイヤーのポインタを探す(死んでいた場合は次のプレイヤーを探す)
    int loopCount = 0;
    for(int i = Enemy::enemyTurnId; loopCount < ENEMY_NUM; i++) {
        if(enemys[Enemy::getEnemyTurnId()] == NULL) {
            Enemy::enemyTurnId++;
        } else {
            break;
        }
        
        loopCount++;
    }
    
    //次のプレイヤーにポインタを動かすためのフラグ
    isShowedNextPlayer = false;

    //次の敵プレイヤーにポインタを動かすためのフラグ
    isShowedNextEnemy = false;
    
}

void HelloWorld::enemyChange() {
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
    
    //次の敵プレイヤーにポインタを動かすためのフラグ
    isShowedNextEnemy = false;
    
    //敵プレイヤーのターン終了
    isPlayerTurn = true;
    
    //敵が動いた
    isShotEnemy = false;

    //減衰率を戻す
    if(enemys[Enemy::getEnemyTurnId()] != NULL) {
        enemys[Enemy::getEnemyTurnId()]->setDecreaseSpeedRatio(enemys[Enemy::getEnemyTurnId()]->decreaseRatio);
    }
    
    Enemy::preEnemyTurnId = Enemy::enemyTurnId;
    Enemy::enemyTurnId++;
    
    //次の敵プレイヤーのポインタを探す(死んでいた場合は次のプレイヤーを探す)
    int loopCount = 0;
    for(int i = Enemy::enemyTurnId; loopCount < ENEMY_NUM; i++) {
        if(enemys[Enemy::getEnemyTurnId()] == NULL) {
            Enemy::enemyTurnId++;
        } else {
            break;
        }
        
        loopCount++;
    }
    
    //次の敵プレイヤーのポインタを探す(死んでいた場合は次の敵プレイヤーを探す) プレイヤーが敵を殺した場合も考えて
    int loop = 0;
    for(int i = Player::playerTurnId; loop < PLAYER_NUM; i++) {
        if(monkeys[Player::getPlayerTurnId()] == NULL) {
            Player::playerTurnId++;
        } else {
            break;
        }
        loop++;
    }
}

void HelloWorld::moveEnemy(int enemyId) {
    int targetId = getNearestPlayerId(enemyId);
    //CCLog("targetId = %d", targetId);
    //CCLog("enemyId => %d", enemyId);
    CCPoint playerPoint = monkeys[targetId]->getRigidPosition();
    CCPoint enemyPoint = enemys[enemyId]->getRigidPosition();
    //float distance = ccpDistance(playerPoint, enemyPoint);
    
    
    b2Vec2 enemyShotAngle = b2Vec2(playerPoint.x - enemyPoint.x, playerPoint.y - enemyPoint.y);
    
    //距離によって持たせる速度を調整しようとしたけどやめる.
    /******
    if()
    */
    
    //敵の速度は固定とする.
    enemyShotAngle.x *= enemys[enemyId]->speed;
    enemyShotAngle.y *= enemys[enemyId]->speed;
    
    float angle = enemys[enemyId]->getAngle(enemyShotAngle);
    int direction = enemys[enemyId]->getDirectionFromAngle(angle);

    //これから向く方向に画像をあわせる
    enemys[enemyId]->setImage(direction);
    
    //発射
    enemys[enemyId]->getBody()->SetLinearVelocity(enemyShotAngle);
    isShotEnemy = true;
    isMoving = true;
    
    // 風エフェクトの設定
    // 表示設定
    enemyShotAngle.y *= -1;
    float reverseAngle = enemys[enemyId]->getAngle(enemyShotAngle);
    windEffect->setRotation(reverseAngle);
    windEffect->setPosition(ccp(enemys[enemyId]->getRigidPosition().x,
                                enemys[enemyId]->getRigidPosition().y));
    windEffect->setVisible(true);
    reorderChild(windEffect, 100);
    // フェードアウト設定
    CCFadeOut* actionFadeOut = CCFadeOut::create(0.3f);
    windEffect->runAction(actionFadeOut);
    // 風エフェクトがついているオブジェクトの指定
    windTargetType = TYPE_ENEMY;
    windTargetIndex = enemyId;

}


//敵プレイヤーから最も近いプレイヤーIdを返す
int HelloWorld::getNearestPlayerId(int enemyId) {
    float shortDistance = 10000.0f;
    int targetPlayerId = 0;
    for(int i = 0; i < PLAYER_NUM; i++) {
        if(monkeys[i] == NULL || isGameover) continue;

        CCPoint enemyPoint = enemys[enemyId]->getRigidPosition();
        CCPoint playerPoint = monkeys[i]->getRigidPosition();
        
        float diffX = enemyPoint.x - playerPoint.x;
        float diffY = enemyPoint.y - playerPoint.y;
        float distance = sqrt(diffX * diffX + diffY + diffY);
        
//        CCLog("distance = %f", distance);
        
//        CCLog("enemyPoint.x = %f, enemyPoint.y = %f, playerPoint.x = %f, playerPoint.y = %f", enemyPoint.x, enemyPoint.y, playerPoint.x, playerPoint.y);
//        CCLog("distance = %d", distance);
        if(distance < shortDistance) {
            targetPlayerId = i;
            shortDistance = distance;
        }
    }
    return targetPlayerId;
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
        
        //タッチした範囲に剛体の矩形が入っているかどうか判定する. 且つ,そのターンのプレイヤーかどうか判定. プレイヤーのターンかどうか
        if(touchRect.intersectsRect(playerRect) && i == Player::getPlayerTurnId() && isPlayerTurn) {
            isObjectTouched = true;
            touchObjectBody = monkeys[i]->getBody();
            arrow->setScale(0.35f);
            touchObjectNum = i;
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
    
    if (isPlayerTurn) {
        if(isObjectTouched && touchObjectBody != NULL) {
            //movePointがプラスかマイナスか判別するフラグ
            //int x_plusFlag;
            //int y_plusFlag;
            
            PhysicsSprite *pSprite = (PhysicsSprite *)touchObjectBody->GetUserData();
            
            //タッチしたオブジェクトの半径(正確には矩形)
            float radius = pSprite->getContentSize().width / 2;
            
            //タッチしたオブジェクトの中心座標を取得
            CCPoint objectPoint = CCPointMake(touchObjectBody->GetPosition().x * PTM_RATIO, touchObjectBody->GetPosition().y * PTM_RATIO);
            
            //進行方向の矢印を表示する
            showArrow(touchLocation, objectPoint, radius);
            
            b2Vec2 vec = b2Vec2(objectPoint.x - touchLocation.x, objectPoint.y - touchLocation.y);
            int direction = monkeys[touchObjectNum]->getDirectionFromVec(vec);
            monkeys[touchObjectNum]->setImage(direction);
        } else {
            // 画面をタッチして動かす処理
            CCPoint touchGap = ccp(touchLocation.x - moveTouchPoint.x,
                                   touchLocation.y - moveTouchPoint.y);
            
            moveMap(touchGap);
            
            moveTouchPoint = touchLocation;
        }
    }
}

//進行方向の矢印を表示する
void HelloWorld::showArrow(CCPoint start, CCPoint end, int radius) {
    //movePointがプラスかマイナスか判別するフラグ
    int x_plusFlag;
    int y_plusFlag;
    
    //startとendの距離を測る
    float diffX = start.x - end.x;
    float diffY = start.y - end.y;
    float distance = sqrt(diffX * diffX + diffY * diffY);
    
    //表示位置の計算
    float x = sqrt( ((start.x - end.x) * (start.x - end.x) * radius * radius) / ((start.x - end.x) * (start.x - end.x) + (start.y - end.y) * (start.y - end.y)) );
    float y = sqrt( ((start.y - end.y) * (start.y - end.y) * radius * radius) / ((start.x - end.x) * (start.x - end.x) + (start.y - end.y) * (start.y - end.y)) );
    
    //矢印の回転
    float arcRadian = atanf(((start.y - end.y) / (start.x - end.x)));
    shootRadian = arcRadian;
    //ラジアンを角度に変換
    float arcDegree = -1 * CC_RADIANS_TO_DEGREES(arcRadian);
    //        CCLog("arcDegree => %f", arcDegree);
    
    
    if (end.x < start.x) {
        x_plusFlag = -1;
        x *= -1;
        if(end.y < start.y) {
            y_plusFlag = -1;
            arcDegree += 180;
            y *= -1;
        } else if(end.y == start.y) {
            y_plusFlag = 0;
            arcDegree += 180;
        } else {
            y_plusFlag = 1;
            arcDegree += 180;
        }
    } else  if(end.x == start.x){
        x_plusFlag = 0;
        if(end.y < start.y) {
            y_plusFlag = -1;
            //arcDegree += 90;
            y *= -1;
        } else if(end.y == start.y) {
            y_plusFlag = 0;
        } else {
            y_plusFlag = 1;
            //arcDegree += 270;
        }
    } else {
        x_plusFlag = 1;
        if(end.y < start.y) {
            y_plusFlag = -1;
            arcDegree += 360;
            y *= -1;
        } else if(end.y == start.y) {
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
    arrow->setPosition(ccp(x + end.x, y + end.y));
    
    arrow->setRotation(arcDegree);
    
    //半径以下の時矢印の表示色を変える
    if (radius < ccpDistance(startPoint, start)) {
        arrow->setColor(ccc3(255, 200, 100));
        arrow->setVisible(true);
    } else {
        arrow->setColor(ccc3(0, 200, 100));
        arrow->setVisible(true);
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
    
    //cursor->setVisible(false);
    arrow->setVisible(false);
}


void HelloWorld::moveMapWithObject(b2Body *moveObjectBody) {
    b2Vec2 moveObjectVec = moveObjectBody->GetLinearVelocity();
    if( (-1.0f < moveObjectVec.x && moveObjectVec.x < 1.0f) && (-1.0f < moveObjectVec.y && moveObjectVec.y < 1.0f)) {
        //プレイヤーのターンの時
        if (isPlayerTurn) {
            isMoving = false;
            moveObjectBody->SetLinearDamping(8.0f);
            this->scheduleOnce(schedule_selector(HelloWorld::playerChange), 0.5f);
        } else {
            isMoving = false;
//            isShotEnemy = false;
            this->scheduleOnce(schedule_selector(HelloWorld::enemyChange), 0.5f);
        }
    }
    CCSize dispSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint dispCenter = CCPointMake(dispSize.width / 2, dispSize.height / 2);
    
    movedPosition = CCPointMake(moveObjectBody->GetPosition().x * PTM_RATIO, moveObjectBody->GetPosition().y * PTM_RATIO);
//    movedPosition = CCPointMake(touchObjectBody->GetPosition().x * PTM_RATIO, touchObjectBody->GetPosition().y * PTM_RATIO);
    
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
    
        // 当たったときのエフェクトスライド
        for(int i = 0; i < HIT_EF_NUM; i++) {
            if(hitEfs[i] == NULL) continue;
            hitEfs[i]->setPosition(hitEfs[i]->getPosition().x + touchGap.x,
                                   hitEfs[i]->getPosition().y);
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
        
        //カーソルスライド
        cursor->setPosition(cursor->getPosition().x + touchGap.x,
                            cursor->getPosition().y);
    }
    

    // y方向にフィールドが動かせるかどうか
    if(field->getPosition().y + touchGap.y <= field->getSprite()->getContentSize().height / 2 + field->getSprite()->getContentSize().height / 2 &&
           field->getPosition().y + field->getSprite()->getContentSize().height / 2 + touchGap.y + 100>= s.width) {
    
    //if(field->getPosition().y - field->height / 2 > 0 && field->getPosition().y + field->height / 2 < field->height) {
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
        
        // 当たったときのエフェクトスライド
        for(int i = 0; i < HIT_EF_NUM; i++) {
            if(hitEfs[i] == NULL) continue;
            hitEfs[i]->setPosition(hitEfs[i]->getPosition().x,
                                   hitEfs[i]->getPosition().y + touchGap.y);
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
    
        //カーソルスライド
        cursor->setPosition(cursor->getPosition().x,
                            cursor->getPosition().y + touchGap.y);
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
