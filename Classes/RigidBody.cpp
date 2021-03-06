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
    //createBody(location, pSprite, density, friction, restitution);
}

RigidBody::~RigidBody() {
    
}

void RigidBody::createCircleBody(CCPoint location, PhysicsSprite *pSprite, float collisionRadius, float density, float friction, float restitution) {
    //ボディを定義する
    b2BodyDef bodyDef;
    //bodyDef.type = b2_dynamicBody;
    switch(pSprite->_tag) {
        case TAG_PLAYER:
        case TAG_ENEMY:
            bodyDef.type = b2_dynamicBody;
            break;
        case TAG_OBSTACLE:
            bodyDef.type = b2_dynamicBody;
            density = 10000.0f;
            break;
        default:
            break;
    }
    bodyDef.position.Set(location.x/PTM_RATIO, location.y/PTM_RATIO);

    //一律で減衰率は8.0
    bodyDef.linearDamping = 8.0;

    //回転を許さない
    bodyDef.fixedRotation = true;
    
    World *world = World::getInstance();
    
    //PhysicsSpriteを保存する.
    _body = world->getWorld()->CreateBody(&bodyDef);
    _body->SetUserData(pSprite);
    
    /*
    // ボックス型
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.5f, 1.5f);//These are mid points for our 1　m box
    */
    
    
    // 円形型
    b2CircleShape circleShape;
    circleShape.m_radius = collisionRadius / PTM_RATIO;
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
//    fixtureDef.shape = &dynamicBox;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    _body->CreateFixture(&fixtureDef);

}

void RigidBody::createBoxBody(CCPoint location, PhysicsSprite *pSprite, CCSize collisionSize, float density, float friction, float restitution) {
    //ボディを定義する
    b2BodyDef bodyDef;
    //bodyDef.type = b2_dynamicBody;
    switch(pSprite->_tag) {
        case TAG_PLAYER:
        case TAG_ENEMY:
            bodyDef.type = b2_dynamicBody;
            break;
        case TAG_OBSTACLE:
            bodyDef.type = b2_dynamicBody;
            density = 10000.0f;
            break;
        default:
            break;
    }
    bodyDef.position.Set(location.x/PTM_RATIO, location.y/PTM_RATIO);
    
    //一律で減衰率は8.0
    bodyDef.linearDamping = 8.0;
    
    //回転を許さない
    bodyDef.fixedRotation = true;
    
    World *world = World::getInstance();
    
    //PhysicsSpriteを保存する.
    _body = world->getWorld()->CreateBody(&bodyDef);
    _body->SetUserData(pSprite);
    
    
    // ボックス型
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(collisionSize.width / PTM_RATIO, collisionSize.height / PTM_RATIO);//These are mid points for our 1　m box
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    //    fixtureDef.shape = &dynamicBox;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    _body->CreateFixture(&fixtureDef);
    
}


b2Body* RigidBody::getBody() {
    return _body;
}

//ベクトルから向いている角度を取得
float RigidBody::getAngle(b2Vec2 vec) {
    //矢印の回転
    float arcRadian = atanf(vec.y / vec.x);
    //ラジアンを角度に変換
    float arcDegree =  CC_RADIANS_TO_DEGREES(arcRadian);
    
    //ラジアンを360度に変換する.
    if (vec.x < 0) {
        arcDegree += 180;
    } else {
        if(vec.y < 0) {
            arcDegree += 360;
        }
    }
    
    return arcDegree;
}

//角度から向いている方角を取得
int RigidBody::getDirectionFromAngle(float angle) {
    if (0 < angle && angle < 60) {
        return RIGHT;
    } else if(60 <= angle && angle <= 120) {
        return UP;
    } else if(120 < angle && angle < 240) {
        return LEFT;
    } else if(240 <= angle && angle <= 300) {
        return DOWN;
    } else if(240 < angle && angle <360) {
        return RIGHT;
    }
}

//ベクトルから向いている方角を取得
int RigidBody::getDirectionFromVec(b2Vec2 vec) {
    /** x,y方向のベクトルのうち、大きい方のベクトルを自分の向きとする **/
    //左方向
    if(vec.x < 0) {
        //上方向
        if(0 <= vec.y) {
            if(-vec.x <= vec.y) {
                return UP;
            }
        }
        //下方向
        else
        {
            if(-vec.x < -vec.y) {
                return DOWN;
            }
        }
        return LEFT;
    }
    //右方向
    else
    {
        //上方向
        if(0 <= vec.y) {
            if(vec.x <= vec.y) {
                return UP;
            }
        }
        //下方向
        else
        {
            if(vec.x < -vec.y) {
                return DOWN;
            }
        }
        return RIGHT;
    }
    
}


//渡された方向と逆の方向を返す.
int RigidBody::getReverseDirection(int direction) {
    if(direction == UP) {
        return DOWN;
    } else if(direction == DOWN) {
        return UP;
    } else if(direction == RIGHT) {
        return LEFT;
    } else if(direction == LEFT) {
        return RIGHT;
    }
    
    //エラー
    return -1;
}


//どこにぶつかられたか(正面、背後、左右)をセットする
void RigidBody::setContactedDirection() {
    switch(_preDirection) {
        case UP:
            switch (_postDirection) {
                case UP:
                    _contactedDirection = BACK;
                    break;
                case DOWN:
                    _contactedDirection = FRONT;
                    break;
                default:
                    _contactedDirection = SIDE;
                    break;
            }
            break;
        case DOWN:
            switch (_postDirection) {
                case UP:
                    _contactedDirection = FRONT;
                    break;
                case DOWN:
                    _contactedDirection = BACK;
                    break;
                default:
                    _contactedDirection = SIDE;
                    break;
            }
            break;
        case LEFT:
            switch (_postDirection) {
                case LEFT:
                    _contactedDirection = BACK;
                    break;
                case RIGHT:
                    _contactedDirection = RIGHT;
                    break;
                default:
                    _contactedDirection = SIDE;
                    break;
            }
            break;
        case RIGHT:
            switch (_postDirection) {
                case LEFT:
                    _contactedDirection = FRONT;
                    break;
                case RIGHT:
                    _contactedDirection = BACK;
                    break;
                default:
                    _contactedDirection = SIDE;
                    break;
            }
            break;
    }
}

//当たった時の向きによって速度を変化させる.
void RigidBody::setReviceVelocity(int contactedDirection) {
    b2Vec2 objBodyVec = _body->GetLinearVelocity();
    CCLog("_pre = %d, _post = %d", _preDirection, _postDirection);
    CCLog("contactedDirection = %d", contactedDirection);
    switch (contactedDirection) {
        case FRONT:
            objBodyVec.x *= 1.5;
            objBodyVec.y *= 1.5;
            break;
        case BACK:
            objBodyVec.x *= 3.0;
            objBodyVec.y *= 3.0;
            break;
        case SIDE:
            objBodyVec.x *= 2.25;
            objBodyVec.y *= 2.25;
            break;
    }
    _body->SetLinearVelocity(objBodyVec);
}

CCPoint RigidBody::getRigidPosition() {
    CCPoint rigidPoint = CCPointMake(_body->GetPosition().x * PTM_RATIO, _body->GetPosition().y * PTM_RATIO);
    return rigidPoint;
}

void RigidBody::setDecreaseSpeedRatio(float ratio) {
    //b2Fixture *fixture = _body->GetFixtureList();
    _body->SetLinearDamping(ratio);
}
