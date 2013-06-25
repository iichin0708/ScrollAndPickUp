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
#include "RigidBody.h"
#include "Player.h"
#include "Field.h"
#include "Enemy.h"
#include "Coin.h"
#include "Ghost.h"
#include "Cursor.h"
#include "HitEffect.h"
#include "Obstacle.h"

#define ENEMY_NUM 15
#define COIN_NUM 50
#define HIT_EF_NUM 10
#define OBSTACLE_NUM 12

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
    Cursor *cursor;
    Field *field;
    
    b2Body *backgroud;
    
    cocos2d::CCSprite *arrow;
    
    static int sprite_id;
    
    //float SPEED_DECREASE_RATIO;
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    void initPhysics();
    // adds a new sprite at a given coordinate
    void createNewBody(RigidBody *&body, cocos2d::CCPoint p);
    
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
    
    void moveMapSlow(cocos2d::CCPoint);
    
    // ゲーム終了イベント
    void menuCloseCallback(CCObject*);
    
    // ぶつかった敵キャラ
    int contactEnemyindex;
    
    //移動中かどうか
    bool isMoving;
    
    //移動を追うフラグ
    bool isTrace;
    
    //次のプレイヤーのためにマップを動かすフラグ１
    bool isShowedNextPlayer;
    
    cocos2d::CCPoint speedVec;
    
    // ぶつかり処理を行うフラグ
    bool isContacted;
    
    bool blink;
    
    //移動後のポインタ
    cocos2d::CCPoint movedPosition;
    
    // 攻撃をしたプレイヤーのインデックス
    int pIndex;
    
private:
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
    
    void moveMapWithObject(b2Body *moveObjectBody);
    
    void setObstacle();
    
};

#endif // __HELLO_WORLD_H__
