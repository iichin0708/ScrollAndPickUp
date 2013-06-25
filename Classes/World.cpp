//
//  World.cpp
//  box2d
//
//  Created by TakuyaIida on 13/06/19.
//
//


#include "CContactListener.h"
#include "PhysicsSprite.h"
#include "World.h"

using namespace cocos2d;

World *World::worldInstance = NULL;
b2World *World::_world = NULL;

World* World::getInstance() {
    if(!worldInstance) {
      worldInstance = new World();
    }

    return worldInstance;
}

void World::initWorld() {
    //初期化
    _wall = NULL;
    
    //重力なし
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);

    /*
    b2Vec3 gravity;
    gravity.Set(0.0f, 0.0f, -9.8f);
    */
    _world = new b2World(gravity);

    //接触判定クラスの宣言
    _contactListener = new CContactListener();
    
    /*
    //接触判定を世界に追加
    _world->SetContactListener(_contactListener);
    */
     
    //Sleepしているオブジェクトは計算しない(動作の軽量化)
    //小さな力を無視して止まってしまったオブジェクトに対して余計な計算しない.
    _world->SetAllowSleeping(true);
    
    //CCDでオブジェクト同士の衝突を判断(衝突の判断を連続的に行う)
    _world->SetContinuousPhysics(true);
    
    //ディスプレイのサイズを得る
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    //物理世界の壁を作る(初期化では画面の大きさの壁を作ることにする)
    createWall(CCRect(0, 0, s.width, s.height));
    
    //以下、デバッグ用の設定(使用しないならコメントアウトする)
    /******* Debug ********/
    
    _debugDraw = new GLESDebugDraw( PTM_RATIO );
    _world->SetDebugDraw(_debugDraw);
    
    uint32 flags = 0;
    //オブジェクトのポリゴンの形を描画
    flags += b2Draw::e_shapeBit;
    
    //オブジェクト同士の連結部分の描画
    //flags += b2Draw::e_jointBit;
    
    //オブジェクトのバウンディングボックスを包括するような座標軸でできた四角形を描く
    //flags += b2Draw::e_aabbBit;
    
    //オブジェクトのペア関係の線を描画
    //flags += b2Draw::e_pairBit;
    
    //質量の中心を描画
    //flags += b2Draw::e_centerOfMassBit;

    _debugDraw->SetFlags(flags);
    /******* Debug ********/
    
}

void World::setContactListener() {
    _world->SetContactListener(_contactListener);
}

b2World* World::getWorld() {
    return _world;
}

//新たな壁を作成する.(ただし壁は常に１つとする)
void World::createWall(CCRect rect) {
    if(_wall != NULL) {
        CCLog("Error:既に壁があります.");
        return;
    }

    _rectWall = rect;
    
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    groundBodyDef.type = b2_staticBody;
    
    _wall = _world->CreateBody(&groundBodyDef);
    // Define the ground box shape.
    b2EdgeShape groundBox;
    
    /*
    b2MassData *mass;
    groundBox.ComputeMass(mass, 0);
    */
    
    // bottom
    groundBox.Set(b2Vec2(_rectWall.getMinX()/PTM_RATIO, _rectWall.getMinY()/PTM_RATIO), b2Vec2(_rectWall.getMaxX()/PTM_RATIO, _rectWall.getMinY()/PTM_RATIO));
    _wall->CreateFixture(&groundBox,0);
    
    // top
    groundBox.Set(b2Vec2(_rectWall.getMinX()/PTM_RATIO, _rectWall.getMaxY()/PTM_RATIO), b2Vec2(_rectWall.getMaxX()/PTM_RATIO, _rectWall.getMaxY()/PTM_RATIO));
    _wall->CreateFixture(&groundBox,0);
    
    /*
    // left
    groundBox.Set(b2Vec2(_rectWall.getMinX()/PTM_RATIO, _rectWall.getMaxY()/PTM_RATIO), b2Vec2(_rectWall.getMinX()/PTM_RATIO, _rectWall.getMinY()/PTM_RATIO));
    _wall->CreateFixture(&groundBox,0);
    
    // right
    groundBox.Set(b2Vec2(_rectWall.getMaxX()/PTM_RATIO, _rectWall.getMaxY()/PTM_RATIO), b2Vec2(_rectWall.getMaxX()/PTM_RATIO, _rectWall.getMinY()/PTM_RATIO));
    _wall->CreateFixture(&groundBox,0);
    */
}

void World::deleteWall() {
    _world->DestroyBody(_wall);
    _wall = NULL;
}

//差分(delta)だけ壁を移動させる
void World::moveWall(CCPoint delta) {
    deleteWall();
    
    /*
    //壁の移動距離を加算
    _wallWidth += delta.x;
    _wallHeight += delta.y;
    */
    
    //新たに壁を作成
    // Define the ground box shape.
    //b2EdgeShape groundBox;
    
    //初期位置を差分だけズラし、幅と高さは変化させない.
    CCRect newRect = CCRect(_rectWall.getMinX() + delta.x, _rectWall.getMinY() + delta.y, _rectWall.getMaxX() - _rectWall.getMinX(), _rectWall.getMaxY() -  _rectWall.getMinY());
    
    createWall(newRect);

}

//bodyを消す
void World::destroyBody(b2Body *destroyBody) {
    _world->DestroyBody(destroyBody);
}
