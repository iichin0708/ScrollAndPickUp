#include "Cursor.h"

USING_NS_CC;


Cursor::Cursor()
{
    width = 182;
    height = 182;
    sprite = CCSprite::create("circle.png", CCRectMake(0, 0, width, height) );
    
    // 透明度(0~255)
    sprite->setOpacity(220);
    
    /*
    CCActionInterval* actionRoll = CCRotateBy::create(1.0f, 360);
    CCActionInterval* actionRollFoever = CCRepeatForever::create(actionRoll);
    sprite->runAction(actionRollFoever);
    */
}

cocos2d::CCSprite* Cursor::getSprite()
{
    return sprite;
}

cocos2d::CCPoint Cursor::getPosition()
{
    return sprite->getPosition();
}

bool Cursor::getVisible()
{
    return isVisible;
}

CCRect Cursor::getRect()
{
    CCRect ccr = CCRectMake(sprite->getPosition().x - width / 2,
                            sprite->getPosition().y - height / 2,
                            width,
                            height);
    return ccr;
}

void Cursor::setPosition(float x, float y)
{
    sprite->setPosition(ccp(x, y));
}

void Cursor::setVisible(bool isVisible)
{
    this->isVisible = isVisible;
    sprite->setVisible(this->isVisible);
}