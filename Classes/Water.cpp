//
//  Warter.cpp
//  ScrollAndPickUp
//
//  Created by Kenji Kawanabe on 2013/06/25.
//
//

#include "Water.h"
#include "HelloWorldScene.h"
#include "Box2D/Box2d.h"

USING_NS_CC;


Water::Water()
{
    sprite = cocos2d::CCSprite::create("water.png", cocos2d::CCRectMake(0, 0, 25, 25));
    sprite->setOpacity(150);
}

void Water::setPosition(float x, float y)
{
    sprite->setPosition(ccp(x, y));
}

void Water::setVisible(bool isVisible, int y)
{
    this->isVisible = isVisible;
    sprite->setVisible(this->isVisible);
    groundHeight = y - W_BOUND_HEIGHT - rand() % 10;
    frameCount = 0;
    speedY = W_FIRST_SPEED - rand() % 5;
    speedX = rand() % SIDE_SPEED * 2 - SIDE_SPEED;
    isCorrect = false;
}

void Water::setGroundHeight(int groundHeight)
{
    this->groundHeight = groundHeight;
}

cocos2d::CCSprite* Water::getSprite()
{
    return sprite;
}

bool Water::getVisible()
{
    return isVisible;
}

cocos2d::CCPoint Water::getPosition()
{
    return sprite->getPosition();
}

int Water::getGroundHeight()
{
    return groundHeight;
}

void Water::update()
{
    if(!isCorrect) {
        sprite->setPosition(ccp(sprite->getPositionX() + speedX,
                                sprite->getPositionY() + speedY));
        speedY += W_GRAVITY;
        
        if(getPosition().y < groundHeight) {
            setVisible(false, 0);
        }
    }
}


/*
Water::Water()
{
    width = 36;
    height = 36;
    sprite = CCSprite::create("effect_attack.png", CCRectMake(0, 0, width, height) );
    
    // 透明度(0~255)
    sprite->setOpacity(220);
}

cocos2d::CCSprite* Water::getSprite()
{
    return sprite;
}

cocos2d::CCPoint Water::getPosition()
{
    return sprite->getPosition();
}

bool Water::getVisible()
{
    return isVisible;
}

CCRect Water::getRect()
{
    CCRect ccr = CCRectMake(sprite->getPosition().x - width / 2,
                            sprite->getPosition().y - height / 2,
                            width,
                            height);
    return ccr;
}

void Water::setPosition(float x, float y)
{
    sprite->setPosition(ccp(x, y));
}

void Water::setVisible(bool isVisible)
{
    this->isVisible = isVisible;
    sprite->setVisible(this->isVisible);
    
    float speedX = rand() % 100 - 50;
    float height = rand() % 100 + 100;
    CCLOG("spx = %f  :  hgt = %f", speedX, height);
    
    // ジャンプ
    CCActionInterval* actionJump = CCJumpBy::create(0.7f,
                                                    ccp(0, speedX),
                                                    height,
                                                    1);
    
    // 非表示状態
    cocos2d::CCCallFunc* actionNoVisible = cocos2d::CCCallFunc::create(this, callfunc_selector(Water::setNoVisible));
    // 処理順を並べる
    CCSequence* actionJumpOut = CCSequence::create(actionJump, actionNoVisible);
    
    sprite->runAction(actionJumpOut);
}

void Water::setNoVisible()
{
    isVisible = false;
    sprite->setVisible(false);
}
*/