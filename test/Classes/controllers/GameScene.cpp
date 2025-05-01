#include "GameScene.h"
#include"views/CardView.h"

USING_NS_CC;

Scene* GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init()) return false;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float borderWidth = 20.0f;
    Size contentSize = Size(visibleSize.width - 2 * borderWidth, visibleSize.height - 2 * borderWidth);

    // 创建主牌区显示节点（棕色背景）
    float playfieldHeight = contentSize.height * 0.7f;
    playfieldArea = LayerColor::create(Color4B(139, 69, 19, 255), contentSize.width, playfieldHeight);
    playfieldArea->setAnchorPoint(Vec2::ZERO);
    playfieldArea->setPosition(origin.x + borderWidth, origin.y + borderWidth + contentSize.height * 0.3f);
    this->addChild(playfieldArea, 10);

    // 创建手牌区显示节点（紫色背景）
    float handAreaHeight = contentSize.height * 0.3f;
    handArea = LayerColor::create(Color4B(128, 0, 128, 255), contentSize.width, handAreaHeight);
    handArea->setAnchorPoint(Vec2::ZERO);
    handArea->setPosition(origin.x + borderWidth, origin.y + borderWidth);
    this->addChild(handArea, 10);

    // 初始化牌容器
    mainDeck.clear();
    handCards.clear();

    // 创建示例卡牌
    createMainDeckCards();
    createHandCards();

    return true;
}

void GameScene::createMainDeckCards() {
    std::vector<std::pair<int, int>> col1 = {
        {CFT_THREE, CST_DIAMONDS},
        {CFT_THREE, CST_CLUBS},
        {CFT_KING, CST_CLUBS}
    };
    std::vector<std::pair<int, int>> col2 = {
        {CFT_TWO, CST_SPADES},
        {CFT_THREE, CST_CLUBS},
        {CFT_THREE, CST_CLUBS}
    };

    Size areaSize = playfieldArea->getContentSize();

    float col1X = areaSize.width * 0.3f;
    float col2X = areaSize.width * 0.6f;

    auto tempCard = CardView::create(CFT_THREE, CST_CLUBS);
    Size cardSize = tempCard ? tempCard->getContentSize() * tempCard->getScale() : Size(100, 150);
    if (tempCard) tempCard->removeFromParent();

    int count = (int)col1.size();
    float totalHeight = cardSize.height * count;
    float maxSpacing = 30.0f;
    float spacing = (areaSize.height - totalHeight) / (count + 1);
    if (spacing > maxSpacing) spacing = maxSpacing;

    float startY = spacing;

    for (int i = 0; i < count; ++i) {
        CardData cardData;
        cardData.face = col1[i].first;
        cardData.suit = col1[i].second;
        cardData.view = CardView::create(cardData.face, cardData.suit);
        cardData.view->setPosition(Vec2(col1X, startY + i * (cardSize.height + spacing)));

        // 绑定点击回调
        cardData.view->setClickCallback([this](CardView* clickedCard) {
            MessageBox("ccc", "ddd");
            });

        playfieldArea->addChild(cardData.view);
        mainDeck.push_back(cardData);
    }

    for (int i = 0; i < count; ++i) {
        CardData cardData;
        cardData.face = col2[i].first;
        cardData.suit = col2[i].second;
        cardData.view = CardView::create(cardData.face, cardData.suit);
        cardData.view->setPosition(Vec2(col2X, startY + i * (cardSize.height + spacing)));

        cardData.view->setClickCallback([this](CardView* clickedCard) {
            MessageBox("ccc", "ddd");
            });

        playfieldArea->addChild(cardData.view);
        mainDeck.push_back(cardData);
    }
}


void GameScene::createHandCards() {
    std::vector<std::pair<int, int>> cards = {
        {CFT_THREE, CST_CLUBS},
        {CFT_ACE, CST_HEARTS},
        {CFT_FOUR, CST_CLUBS}
    };

    Size areaSize = handArea->getContentSize();

    auto tempCard = CardView::create(CFT_THREE, CST_CLUBS);
    Size cardSize = tempCard ? tempCard->getContentSize() * tempCard->getScale() : Size(100, 150);
    if (tempCard) tempCard->removeFromParent();

    int count = (int)cards.size();
    float totalWidth = cardSize.width * count;
    float maxSpacing = 50.0f;
    float spacing = (areaSize.width - totalWidth) / (count + 1);
    if (spacing > maxSpacing) spacing = maxSpacing;

    float startX = (areaSize.width - (cardSize.width * count + spacing * (count - 1))) / 2;
    float posY = areaSize.height / 2;

    for (int i = 0; i < count; ++i) {
        CardData cardData;
        cardData.face = cards[i].first;
        cardData.suit = cards[i].second;
        cardData.view = CardView::create(cardData.face, cardData.suit);
        cardData.view->index = i;
        cardData.view->setPosition(Vec2(startX + i * (cardSize.width + spacing), posY));

        cardData.view->setClickCallback([this](CardView* clickedCard) {
            this->onHandCardClicked(clickedCard);
            //MessageBox("hand", "card");
            });

        handArea->addChild(cardData.view);
        handCards.push_back(cardData);
    }
}

void GameScene::updateHandCardsIndex() {
    for (int i = 0; i <handCards.size(); i++) {
        handCards[i].view->index=i;
    }
}


void GameScene::onHandCardClicked(CardView* clickedCard) {

    int topIndex = (int)handCards.size() - 1;
    int currentIndex = clickedCard->index;
    
    if (currentIndex == topIndex) {
        return;
    }

    Vec2 clickedPos = handCards[currentIndex].view->getPosition();
    Vec2 topPos = handCards[topIndex].view->getPosition();

    // 动画：点击牌移动到顶部牌位置,顶部牌到点击牌位置
    auto moveClickedToTop = MoveTo::create(0.5f, topPos);
    handCards[currentIndex].view->runAction(moveClickedToTop);
    auto moveTop = MoveTo::create(0.5f, clickedPos);
    handCards[topIndex].view->runAction(moveTop);

    //数据交换
    // 立即交换容器数据和更新索引
    std::swap(handCards[currentIndex], handCards[topIndex]);

    // 更新CardView中的index，保持同步
    handCards[currentIndex].view->index = currentIndex;
    handCards[topIndex].view->index = topIndex;

    //顶部牌显示在最上层
    //handCards[topIndex].view->getParent()->reorderChild(handCards[topIndex].view, 100);
}
