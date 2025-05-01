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

    // 1. 创建SpriteBatchNode，绑定一张大图（这里用背景图）
    auto batchNode = SpriteBatchNode::create("res/card_general.png");
    this->addChild(batchNode);

    // 2. 创建背景Sprite，添加到batchNode
    background = Sprite::create("res/card_general.png");
    background->setAnchorPoint(Vec2::ZERO);
    batchNode->addChild(background);

    // 3. 判断是否支持的牌
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

    // 4. 创建小数字Sprite，单独加载
    std::string smallNumFile = "res/number/small_" + colorStr + "_" + faceName + ".png";
    smallNumber = Sprite::create(smallNumFile);
    if (smallNumber) {
        smallNumber->setAnchorPoint(Vec2(0, 1));
        smallNumber->setPosition(Vec2(30, background->getContentSize().height - 2));
        this->addChild(smallNumber);
    }
    else {
        CCLOG("Error: %s not found", smallNumFile.c_str());
    }

    // 5. 创建大数字Sprite，单独加载
    std::string bigNumFile = "res/number/big_" + colorStr + "_" + faceName + ".png";
    bigNumber = Sprite::create(bigNumFile);
    if (bigNumber) {
        bigNumber->setAnchorPoint(Vec2(0.5f, 0.5f));
        bigNumber->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height / 2));
        this->addChild(bigNumber);
    }
    else {
        CCLOG("Error: %s not found", bigNumFile.c_str());
    }

    // 6. 创建花色图标Sprite，单独加载
    std::string suitFile = "res/suits/" + suitName + ".png";
    suitIcon = Sprite::create(suitFile);
    if (suitIcon) {
        suitIcon->setAnchorPoint(Vec2(1, 1));
        suitIcon->setPosition(Vec2(background->getContentSize().width - 30, background->getContentSize().height - 2));
        this->addChild(suitIcon);
    }
    else {
        CCLOG("Error: %s not found", suitFile.c_str());
    }

    // 7. 设置内容大小和缩放
    this->setContentSize(background->getContentSize());
    this->setScale(1.4f);

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


void CardView::onEnter() {
    Node::onEnter();
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);

    _touchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        // 将触摸点转换到CardView节点坐标系
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Rect rect(Vec2::ZERO, this->getContentSize());

        if (rect.containsPoint(locationInNode)) {
            if (_clickCallback) {
                _clickCallback(this);
            }
            
            return true; // 捕获事件
        }
        return false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    
}

void CardView::onExit() {
    Node::onExit();
    _eventDispatcher->removeEventListenersForTarget(this);
}

void CardView::setClickCallback(const ClickCallback& cb) { _clickCallback = cb; }