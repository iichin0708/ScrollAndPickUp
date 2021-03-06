//
//  HelloWorldScene.h
//  box2d
//
//  Created by TakuyaIida on 13/06/18.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "AppDelegate.h"
#include "RigidBody.h"
#include "Player.h"
#include "Field.h"
#include "Enemy.h"
#include "Coin.h"
#include "Ghost.h"
#include "Cursor.h"
#include "HitEffect.h"
#include "Obstacle.h"
#include "Water.h"

class HelloWorld : public cocos2d::CCLayer {
public:
    ~HelloWorld();
    HelloWorld();
    
    static HelloWorld* Instance;
    
    RigidBody *player[PLAYER_NUM];
    Player *monkeys[PLAYER_NUM];
    Enemy *enemys[ENEMY_NUM];
    Coin *coins[COIN_NUM];
    Ghost *ghosts[ENEMY_NUM];
    HitEffect *hitEfs[HIT_EF_NUM];
    Obstacle *obstacles[OBSTACLE_NUM];
    Water *waters[WATER_NUM];
    Cursor *cursor;
    Field *field;
    cocos2d::CCSprite *windEffect;
    
    b2Body *backgroud;
    
    cocos2d::CCSprite *arrow;
    
    static int sprite_id;
    
    //float SPEED_DECREASE_RATIO;
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    // 実際の画面サイズ
    static int realDispWidth;
    static int realDispHeight;
    
    void initPhysics();
  
    void flickBody(cocos2d::CCPoint start, cocos2d::CCPoint end, b2Body* object);
    
    void destroyObject(RigidBody *&object);
    
    virtual void draw();
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void update(float dt);
    
    //マップ移動メソッド
    void moveMap(cocos2d::CCPoint);
    
    // ゲーム終了イベント
    void menuCloseCallback(CCObject*);

    //敵キャラの移動
    void moveEnemy(int enemyId);
    
    //敵キャラから最も近いプレイヤーIDを返す
    int getNearestPlayerId(int enemyId);
    
    // ぶつかった敵キャラ
    int contactEnemyindex;
    
    // ぶつけたプレイヤーの攻撃力
    int contactPlayerOffence;
    
    // ぶつかったプレイヤー
    int contactPlayerindex;
    
    // ぶつけられた敵キャラの攻撃力
    int contactEnemyOffence;
    
    //移動中かどうか
    bool isMoving;
    
    //移動を追うフラグ
    bool isTrace;
    
    //プレイヤーのターンかどうか
    bool isPlayerTurn;
    
    //次のプレイヤーのためにマップを動かすフラグ
    bool isShowedNextPlayer;
    
    bool isShotEnemy;
    
    //次の敵にマップを動かすフラグ
    bool isShowedNextEnemy;
    
    cocos2d::CCPoint speedVec;
    
    // ぶつかり処理を行うフラグ
    bool isContacted;
    
    bool blink;
    
    //移動後のポインタ
    cocos2d::CCPoint movedPosition;
    
    // 攻撃をしたプレイヤーのインデックス
    int pIndex;

    
private:
    // ゲームイントロフラグ
    bool isGameintro;
    bool isGameintroMove;
    bool isGameWait;
    bool isGameReady;
    
    // ゲームオーバーフラグ
    bool isGameover;
    
    // ゲームクリアフラグ
    bool isGameclear;
    
    //    b2World* world;
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
    cocos2d::CCTexture2D* m_pSpriteTexture2;
    cocos2d::CCPoint startPoint;
    cocos2d::CCPoint endPoint;
    cocos2d::CCPoint moveTouchPoint;
    
    b2ContactListener* _contactListener;
    
    //デバッグ変数
    //GLESDebugDraw* m_debugDraw;
    
    bool isObjectTouched;
    
    b2Body *touchObjectBody;
    
    float shootRadian;
        
    void addNewBody(float density, char* fileName);
    
    void playerChange();
    
    void enemyChange();
    
    void showArrow(cocos2d::CCPoint start, cocos2d::CCPoint end, int radius);
    
    void moveMapWithObject(b2Body *moveObjectBody);
    
    void setObstacle();
    
    int touchObjectNum;

    // イントロ（はじめに画面が表示される状態）フラグをおろす
    void offIntro();
    
    // ウェイト（ステージが表示され終わったあとの間の状態）フラグをおろす
    void offWait();
    
    // レディ（Ready...の状態）フラグをおろす
    void offReady();
    
    cocos2d::CCSprite* black;
    cocos2d::CCSprite* stageLabel;
    
    // 風エフェクトが追いかけるオブジェクト
    int windTargetType;
    int windTargetIndex;
};

#endif // __HELLO_WORLD_H__
