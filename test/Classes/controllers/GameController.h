// GameController.h
#pragma once
#include"GameScene.h"
extern class GameScene;
class GameController {
public:
    GameController(GameScene* scene);

    // 处理卡牌点击事件，返回是否成功处理
    bool handleCardClick(int cardIndex);

private:
    GameScene* _scene;
};
