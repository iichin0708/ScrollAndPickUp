#include "Ghost.h"

USING_NS_CC;


Ghost::Ghost()
{
    sprite = CCSprite::create("ghost.png", CCRectMake(0, 0, 140, 140) );
    width = 140;
    height = 140;
}

cocos2d::CCSprite* Ghost::getSprite()
{
    return sprite;
}

cocos2d::CCPoint Ghost::getPosition()
{
    return sprite->getPosition();
}

bool Ghost::getVisible()
{
    return isVisible;
}

CCRect Ghost::getRect()
{
    CCRect ccr = CCRectMake(sprite->getPosition().x - width / 2,
                            sprite->getPosition().y - height / 2,
                            width,
                            height);
    return ccr;
}

void Ghost::setPosition(float x, float y)
{
    sprite->setPosition(ccp(x, y));
}

void Ghost::setVisible(bool isVisible)
{
    this->isVisible = isVisible;
    sprite->setVisible(this->isVisible);
    // 透明度(0~255)
    //sprite->setOpacity(50);

    frameCount = 0;
    
    CCActionInterval* actionAlpha = CCFadeOut::create(1.0f);
    CCActionInterval* actionMove = CCMoveTo::create(0.5f,
                                            ccp(sprite->getPosition().x,
                                                sprite->getPosition().y + 50));
    sprite->runAction(actionAlpha);
    sprite->runAction(actionMove);
}

void Ghost::update()
{
    /*
    if(frameCount > 5 && frameCount < 10) {
        setPosition(getPosition().x, getPosition().y + 3);
    } else if(frameCount >= 10) {
        setVisible(false);
    }
     */
    frameCount++;
}

