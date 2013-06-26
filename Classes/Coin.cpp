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
    // バウンド状態
        sprite->setPosition(ccp(sprite->getPositionX() + speedX,
                                sprite->getPositionY() + speedY));
        speedY += GRAVITY;
        
        // 基準線を下回ったとき
        if(getPosition().y < groundHeight) {
            if(speedY > -5) {
            // スピードが衰えているとき
                isCorrect = true;
                frameCount = 0;
            } else {
            // 通常の動作（はねる）
                speedY = -speedY / 1.5f;
            }
        }
        
        // 一定フレーム経過
        if(frameCount == 120) {
            isCorrect = true;
            frameCount = 0;
        }
        frameCount++;
        
    } else {
    // 集まり状態
        HelloWorld* hw = HelloWorld::Instance;
        
        if(hw->monkeys[tarIndex] == NULL || hw->monkeys[tarIndex]->isFalled) {
        // 既に集合対象のキャラクターが消えている場合
            frameCount = 0;
            setVisible(false, 0);
            isCorrect = false;
        } else {
        // 通常（集まる）
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
        }
    }
}