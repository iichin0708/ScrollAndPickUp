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

// プレイヤーのバウンド率の調整
int Obstacle::getPlayerBoundRatio() {
    switch (_kind) {
        //岩
        case 0: case 1:
            return 10.0f;
        //木と柵
        case 2: case 3: case 4: case 5: case 6:
            return 9.0f;
        //草むら系
       case 7:  case 8:  case 9: case 10: case 11:
            return 8.0f;
        default:
            return 0;
    }
}

void Obstacle::setKind(const char *imgName) {
    if (strcmp(imgName, "iwa01.png")) {
        _kind = 0;
    } else if(strcmp(imgName, "iwa02.png")) {
        _kind = 1;
    } else if(strcmp(imgName, "saku01.png")) {
        _kind = 2;
    } else if(strcmp(imgName, "saku02.png")) {
        _kind = 3;
    } else if(strcmp(imgName, "wood01.png")) {
        _kind = 4;
    } else if(strcmp(imgName, "wood02.png")) {
        _kind = 5;
    } else if(strcmp(imgName, "wood03.png")) {
        _kind = 6;
    } else if(strcmp(imgName, "wood04.png")) {
        _kind = 7;
    } else if(strcmp(imgName, "wood05.png")) {
        _kind = 8;
    } else if(strcmp(imgName, "wood06.png")) {
        _kind = 9;
    } else if(strcmp(imgName, "wood07.png")) {
        _kind  = 10;
    } else if(strcmp(imgName, "wood08.png")) {
        _kind = 11;
    } else {
        _kind = -1;
    }
                   
}

int Obstacle::getKind() {
    return _kind;
}