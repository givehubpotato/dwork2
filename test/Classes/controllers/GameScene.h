#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include"views/CardView.h"
#include<vector>

struct CardData {
    int face;          // 牌点数，如 CFT_ACE
    int suit;          // 花色，如 CST_HEARTS
    CardView* view;    // 对应的显示节点
};

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    CREATE_FUNC(GameScene);

private:
    // 主牌区容器，存储主牌堆的牌数据
    std::vector<CardData> mainDeck;

    // 手牌区容器，存储手牌区的牌数据
    std::vector<CardData> handCards;

    // 主牌区和手牌区显示节点
    Node* playfieldArea = nullptr;
    Node* handArea = nullptr;


    // 示例函数：创建卡牌并添加到主牌区
    void createMainDeckCards();

    // 示例函数：创建卡牌并添加到手牌区
    void createHandCards();

public:
    void GameScene::onHandCardClicked(CardView* clickedCard);
    void GameScene::updateHandCardsIndex();

};

#endif // __GAME_SCENE_H__
