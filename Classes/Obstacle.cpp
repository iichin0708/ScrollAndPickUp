//
//  Obstacle.cpp
//  ScrollAndPickUp
//
//  Created by GRENGE-MAC0015 on 13/06/25.
//
//

#include "Obstacle.h"

using namespace cocos2d;

CCDictionary* Obstacle::imgDict = CCDictionary::create();

CCSize Obstacle::getImgSize() {
    return CCSizeMake(width, height);
}

CCPoint Obstacle::getObstaclePosition() {
    CCPoint obstaclePoint = CCPointMake(_body->GetPosition().x * PTM_RATIO, _body->GetPosition().y * PTM_RATIO);
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

//画像の定義
void Obstacle::initImgDictionary() {
    setImgDictionary("wood01.png", 228, 285);
    setImgDictionary("wood02.png", 183, 248);
    setImgDictionary("wood03.png", 157, 170);
    setImgDictionary("wood04.png", 190, 135);
    setImgDictionary("wood05.png", 174, 145);
    setImgDictionary("wood06.png", 95, 120);
    setImgDictionary("wood07.png", 278, 246);
    setImgDictionary("wood08.png", 97, 40);
    setImgDictionary("saku01.png", 55, 392);
    setImgDictionary("saku02.png", 49, 279);
    setImgDictionary("iwa01.png", 192, 131);
    setImgDictionary("iwa02.png", 268, 168);
}

//辞書に登録
void Obstacle::setImgDictionary(const char *imgName, int width, int height) {
    CCArray *imgSizeArray = CCArray::create();
    imgSizeArray->addObject(CCInteger::create(width));
    imgSizeArray->addObject(CCInteger::create(height));
    
    imgDict->setObject(imgSizeArray, imgName);
}

//指定した画像名の画像サイズを取得
CCSize Obstacle::getImgSize(const char *imgName) {
    CCArray *img = (CCArray *)imgDict->objectForKey(imgName);
    if(img == NULL) {
        CCLog("Error:%sの画像は登録されていません。", imgName);
        return CCSizeMake(0, 0);
    }
    
    CCInteger *width = (CCInteger *)img->objectAtIndex(0);
    CCInteger *height = (CCInteger *)img->objectAtIndex(1);
    return CCSizeMake(width->getValue(), height->getValue());
}
