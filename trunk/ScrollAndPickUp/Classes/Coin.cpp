//
//  Coin.cpp
//  ScrollAndPickUp
//
//  Created by Kenji Kawanabe on 2013/06/21.
//
//

#include "Coin.h"
#include "HelloWorldScene.h"
#include "Box2D/Box2d.h"

Coin::Coin()
{
    sprite = cocos2d::CCSprite::create("coin.png", cocos2d::CCRectMake(0, 0, 40, 40));
}

void Coin::setPosition(float x, float y)
{
    sprite->setPosition(ccp(x, y));
}

void Coin::setVisible(bool isVisible, int y)
{
    this->isVisible = isVisible;
    sprite->setVisible(this->isVisible);
    groundHeight = y - BOUND_HEIGHT - rand() % 10;
    frameCount = 0;
    speedY = FIRST_SPEED - rand() % 5;
    speedX = rand() % SIDE_SPEED * 2 - SIDE_SPEED;
    isCorrect = false;
}

void Coin::setGroundHeight(int groundHeight)
{
    this->groundHeight = groundHeight;
}

cocos2d::CCSprite* Coin::getSprite()
{
    return sprite;
}

bool Coin::getVisible()
{
    return isVisible;
}

cocos2d::CCPoint Coin::getPosition()
{
    return sprite->getPosition();
}

int Coin::getGroundHeight()
{
    return groundHeight;
}

void Coin::update()
{
    if(!isCorrect) {
        sprite->setPosition(ccp(sprite->getPositionX() + speedX,
                                sprite->getPositionY() + speedY));
        speedY += GRAVITY;
        
        if(getPosition().y < groundHeight) {
            if(speedY > -5) {
                isCorrect = true;
                //setVisible(false, 0);
                frameCount = 0;
            } else {
                speedY = -speedY / 1.5f;
            }
        }
        
        if(frameCount == 120) {
            isCorrect = true;
            //setVisible(false, 0);
            frameCount = 0;
        }
        frameCount++;
    } else {
        HelloWorld* hw = HelloWorld::Instance;
        int tx = hw->monkeys[tarIndex]->getBody()->GetPosition().x * PTM_RATIO;
        int ty = hw->monkeys[tarIndex]->getBody()->GetPosition().y * PTM_RATIO;
        int x = getPosition().x;
        int y = getPosition().y;
        setPosition(x + (tx - x) / 10,
                    y + (ty - y) / 10);
        if(frameCount > 12) {
            frameCount = 0;
            setVisible(false, 0);
            isCorrect = false;
        }
        frameCount++;
        
        /*
        cocos2d::CCRect cr = CCRectMake(x, y, 40, 40);
        cocos2d::CCRect pr = CCRectMake(tx, ty, 120, 139);
        
        if(pr.intersectsRect(cr)) {
            frameCount = 0;
            setVisible(false, 0);
        }
         */
    }
}