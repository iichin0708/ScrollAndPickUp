//
//  HitEffect.cpp
//  ScrollAndPickUp
//
//  Created by Kenji Kawanabe on 2013/06/23.
//
//
#include "HitEffect.h"

USING_NS_CC;


HitEffect::HitEffect()
{
    width = 70;
    height = 70;
    sprite = CCSprite::create("shockwave.png", CCRectMake(0, 0, width, height) );
    
    // 透明度(0~255)
    sprite->setOpacity(220);
}

cocos2d::CCSprite* HitEffect::getSprite()
{
    return sprite;
}

cocos2d::CCPoint HitEffect::getPosition()
{
    return sprite->getPosition();
}

bool HitEffect::getVisible()
{
    return isVisible;
}

CCRect HitEffect::getRect()
{
    CCRect ccr = CCRectMake(sprite->getPosition().x - width / 2,
                            sprite->getPosition().y - height / 2,
                            width,
                            height);
    return ccr;
}

void HitEffect::setPosition(float x, float y)
{
    sprite->setPosition(ccp(x, y));
}

void HitEffect::setVisible(bool isVisible)
{
    this->isVisible = isVisible;
    sprite->setVisible(this->isVisible);
    sprite->setScale(1.0f);
    sprite->setOpacity(255 * 0.8);
    
    // 拡大
    CCActionInterval* actionScale = CCScaleBy::create(0.15f, 5.0f);
    
    // フェードアウト
    CCActionInterval* actionAlpha = CCFadeOut::create(0.4f);
    // 非表示状態
    cocos2d::CCCallFunc* actionNoVisible = cocos2d::CCCallFunc::create(this, callfunc_selector(HitEffect::setNoVisible));
    // 処理順を並べる
    //CCSequence* actionFadeOut = CCSequence::create(actionAlpha, actionNoVisible);
    CCSequence* actionFadeOut = CCSequence::create(actionScale, actionNoVisible);
    
    //sprite->runAction(actionScale);
    sprite->runAction(actionFadeOut);
}

void HitEffect::setNoVisible()
{
    isVisible = false;
    sprite->setVisible(false);
}