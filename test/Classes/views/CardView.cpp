#include "CardView.h"

USING_NS_CC;

CardView* CardView::create(int face, int suit) {
    CardView* ret = new (std::nothrow) CardView();
    if (ret && ret->init(face, suit)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}


bool CardView::init(int face, int suit) {
    if (!Node::init()) return false;

    // 1. 底图，路径加上 "res/"
    background = Sprite::create("res/card_general.png");
    if (!background) {
        CCLOG("Error: res/card_general.png not found");
        return false;
    }
    background->setAnchorPoint(Vec2::ZERO);
    this->addChild(background);

    // 2. 判断是否支持的牌
    std::string faceName = getFaceName(face);
    std::string suitName = getSuitName(suit);

    bool supported = false;
    if (faceName != "" && suitName != "") {
        if ((face == CFT_THREE && (suit == CST_CLUBS || suit == CST_DIAMONDS)) ||
            (face == CFT_ACE && suit == CST_HEARTS) ||
            (face == CFT_FOUR && suit == CST_CLUBS) ||
            (face == CFT_KING && suit == CST_CLUBS) ||
            (face == CFT_TWO && suit == CST_SPADES)) {
            supported = true;
        }
    }

    if (!supported) {
        CCLOG("Unsupported card face=%d suit=%d", face, suit);
        return true; // 只显示空底图
    }

    // 3. 根据花色判断颜色，方块和红桃为红色，其余为黑色
    std::string colorStr = (suit == CST_DIAMONDS || suit == CST_HEARTS) ? "red" : "black";

    // 4. 小数字图片名，路径加上 "res/"
    std::string smallNumFile = "res/number/small_" + colorStr + "_" + faceName + ".png";
    smallNumber = Sprite::create(smallNumFile);
    if (smallNumber) {
        smallNumber->setAnchorPoint(Vec2(0, 1)); // 左上角锚点
        // 位置：左上角，距离左边30
        smallNumber->setPosition(Vec2(30, background->getContentSize().height-2));
        background->addChild(smallNumber);
    }
    else {
        CCLOG("Error: %s not found", smallNumFile.c_str());
    }

    // 5. 大数字图片名，路径加上 "res/"
    std::string bigNumFile = "res/number/big_" + colorStr + "_" + faceName + ".png";
    bigNumber = Sprite::create(bigNumFile);
    if (bigNumber) {
        bigNumber->setAnchorPoint(Vec2(0.5f, 0.5f));
        bigNumber->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height / 2));
        background->addChild(bigNumber);
    }
    else {
        CCLOG("Error: %s not found", bigNumFile.c_str());
    }

    // 6. 花色图标图片名，路径加上 "res/"
    std::string suitFile = "res/suits/" + suitName + ".png";
    suitIcon = Sprite::create(suitFile);
    if (suitIcon) {
        suitIcon->setAnchorPoint(Vec2(1, 1)); // 右上角锚点
        // 位置：右上角，距离右边30
        suitIcon->setPosition(Vec2(background->getContentSize().width - 30, background->getContentSize().height-2));
        background->addChild(suitIcon);
    }
    else {
        CCLOG("Error: %s not found", suitFile.c_str());
    }


    // 7. 设置CardView内容大小为背景大小，方便布局
    this->setContentSize(background->getContentSize());

    // 8. 整体放大1.4倍
    this->setScale(1.4f);

    //
    


    return true;
}


std::string CardView::getFaceName(int face) {
    switch (face) {
    case CFT_ACE: return "A";
    case CFT_TWO: return "2";
    case CFT_THREE: return "3";
    case CFT_FOUR: return "4";
    case CFT_KING: return "K";
    default: return "";
    }
}

std::string CardView::getSuitName(int suit) {
    switch (suit) {
    case CST_CLUBS: return "club";
    case CST_DIAMONDS: return "diamond";
    case CST_HEARTS: return "heart";
    case CST_SPADES: return "spade";
    default: return "";
    }
}

//回调
void test() {
    MessageBox("aa", "bb");
}

void CardView::onEnter() {
    Node::onEnter();

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Vec2 anchor = this->getAnchorPoint();
        Rect rect(-size.width * anchor.x, -size.height * anchor.y, size.width, size.height);

        if (rect.containsPoint(locationInNode)) {
            test();
            return true; // 捕获事件
        }
        return false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CardView::onExit() {
    Node::onExit();
    _eventDispatcher->removeEventListenersForTarget(this);
}


