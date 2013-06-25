//
//  RigidBody.cpp
//  box2d
//
//  Created by TakuyaIida on 13/06/20.
//
//

#include "RigidBody.h"
#include "World.h"

using namespace cocos2d;



RigidBody::RigidBody(cocos2d::CCPoint location, PhysicsSprite *pSprite, float density, float friction, float restitution) {
    createDynamicBody(location, pSprite, density, friction, restitution);
}

RigidBody::~RigidBody() {
    
}

void RigidBody::createDynamicBody(CCPoint location, PhysicsSprite *pSprite, float density, float friction, float restitution) {
    //ボディを定義する
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(location.x/PTM_RATIO, location.y/PTM_RATIO);

    //一律で減衰率は8.0
    bodyDef.linearDamping = 8.0;
    
    World *world = World::getInstance();
    
    //PhysicsSpriteを保存する.
    _body = world->getWorld()->CreateBody(&bodyDef);
    _body->SetUserData(pSprite);
    
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.5f, 1.5f);//These are mid points for our 1　m box

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    _body->CreateFixture(&fixtureDef);

}

b2Body* RigidBody::getBody() {
    return _body;
}

void RigidBody::setDecreaseSpeedRatio(float ratio) {
    //b2Fixture *fixture = _body->GetFixtureList();
    _body->SetLinearDamping(ratio);
}