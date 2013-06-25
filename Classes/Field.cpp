#include "Field.h"

USING_NS_CC;


Field::Field()
{
    sprite = CCSprite::create("field.jpg", CCRectMake(0, 0, 1674, 2048) );
    width = 640;
    height = sprite->getContentSize().height;
}

cocos2d::CCSprite* Field::getSprite()
{
    return sprite;
}

cocos2d::CCPoint Field::getPosition()
{
    return sprite->getPosition();
}

CCRect Field::getRect()
{
    CCRect ccr = CCRectMake(sprite->getPosition().x - width / 2,
                            sprite->getPosition().y - height / 2,
                            width,
                            height);
    return ccr;
}

void Field::setPosition(float x, float y)
{
    sprite->setPosition(ccp(x, y));
}

