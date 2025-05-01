#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include<functional>
// 枚举定义（简化版）
enum CardSuitType {
    CST_CLUBS,      // 梅花（黑色）
    CST_DIAMONDS,   // 方块（红色）
    CST_HEARTS,     // 红桃（红色）
    CST_SPADES      // 黑桃（黑色）
};

enum CardFaceType {
    CFT_ACE = 1,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_KING = 13
};
class CardView : public cocos2d::Node {
public:
    // 创建函数，传入点数和花色
    static CardView* create(int face, int suit);

    // 初始化函数
    virtual bool init(int face, int suit);

private:
    cocos2d::Sprite* background;
    cocos2d::Sprite* smallNumber;
    cocos2d::Sprite* bigNumber;
    cocos2d::Sprite* suitIcon;

    // 辅助函数，获取资源名
    std::string getFaceName(int face);
    std::string getSuitName(int suit);
public:

    bool clickable = true; // 是否可点击
    void onEnter()override;
    void onExit()override;

    //
    using ClickCallback = std::function<void(CardView*)>;
    ClickCallback _clickCallback = NULL;
    int _face = -1;
    int _suit = -1;
    void setClickCallback(const ClickCallback& cb) { _clickCallback = cb; }
    int getFace() { return _face; }
    int getSuit() { return _suit; }

    //
private:
    cocos2d::EventListenerTouchOneByOne* _touchListener = nullptr;

public:
    int index;//索引
    void setTouchEnabled(bool enabled) {
        if (_touchListener) {
            _touchListener->setEnabled(enabled);
        }
    }

};

#endif // __CARD_VIEW_H__
