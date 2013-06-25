//
//  Obstacle.cpp
//  ScrollAndPickUp
//
//  Created by GRENGE-MAC0015 on 13/06/25.
//
//

#include "Obstacle.h"

cocos2d::CCSize Obstacle::getImgSize() {
    return cocos2d::CCSizeMake(width, height);
}

cocos2d::CCPoint Obstacle::getObstaclePosition() {
    cocos2d::CCPoint obstaclePoint = cocos2d::CCPointMake(_body->GetPosition().x * PTM_RATIO, _body->GetPosition().y * PTM_RATIO);
    return obstaclePoint;
}
