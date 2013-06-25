//
//  World.h
//  box2d
//
//  Created by TakuyaIida on 13/06/19.
//
//


/**************
 現状、シングルトンパターンで記述をしているためDeleteできない。
 ステージ以外を作ることになった場合はシングルトンはやめた方がいいかも
 ***************/

#ifndef __box2d__World__
#define __box2d__World__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GLES-Render.h"

class World : public cocos2d::CCLayer {
public:
    static World* getInstance(void);
    
    static b2World* getWorld();
    
    void createWall(cocos2d::CCRect);
    
    void deleteWall();
    
    void moveWall(cocos2d::CCPoint);
    
    void destroyBody(b2Body *destroyBody);
    
    //衝突検知を行う
    void setContactListener();
private:
    //コンストラクタの禁止
    World() {
        initWorld();
    }
    //デストラクタの禁止
    ~World(){}
    //コピーコンストラクタの禁止
    World(const World&);
    //代入演算子オーバーロード
    World operator=(const World&);

    //World初期化
    void initWorld();
    
    //インスタンス
    static World *worldInstance;
    
    static b2World* _world;
    
    cocos2d::CCRect _rectWall;
    
    b2Body *_wall;
    
    b2ContactListener* _contactListener;
    
    //デバッグ用変数
    GLESDebugDraw* _debugDraw;
};
#endif /* defined(__box2d__World__) */
