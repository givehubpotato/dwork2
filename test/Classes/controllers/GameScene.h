#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include"views/CardView.h"
#include<vector>
#include<algorithm>

struct CardData {
    int face;          // 牌点数，如 CFT_ACE
    int suit;          // 花色，如 CST_HEARTS
    CardView* view;    // 对应的显示节点
};

//回退相关
// 操作类型
enum class OperationType {
    SwapHandCards,
    MatchCards
};

// 操作记录结构体
struct OperationRecord {
    OperationType type;

    // 交换操作相关数据
    int handCardIndex1;
    int handCardIndex2;
    cocos2d::Vec2 pos1Before;
    cocos2d::Vec2 pos2Before;
    cocos2d::Vec2 pos1After;
    cocos2d::Vec2 pos2After;

    // 匹配操作相关数据
    CardData matchedCard;       // 匹配的牌数据
    cocos2d::Vec2 posBefore;    // 匹配牌原始位置
    cocos2d::Vec2 posAfter;     // 匹配牌移动到顶部牌位置
};
//

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
    void onHandCardClicked(CardView* clickedCard);

    void onMainCardClicked(CardView* clickedCard);

    //顶牌栈
    std::stack<int>st;//顶牌数据栈

    //回退
    //void onButtonClicked();
    //回退1
     // 操作记录栈
    std::stack<OperationRecord> OpStack;

    // 回退按钮点击
    void onButtonClicked();

    // 刷新手牌显示（根据handCards顺序）
    void refreshHandCardsDisplay();

    //修正位置
    void ResetPos();

};

#endif // __GAME_SCENE_H__
