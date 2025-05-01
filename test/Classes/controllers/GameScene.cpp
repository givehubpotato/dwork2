#include "GameScene.h"
#include"views/CardView.h"
#include"ui/CocosGUI.h"

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
    this->addChild(playfieldArea, 20);//20

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
    //初始化栈
    if (st.empty()) {
        st.push(handCards[handCards.size()-1].view->_face);
    }

    //回退按钮
    auto backButton = ui::Button::create();
    backButton->setTitleText("back");
    backButton->setTitleFontSize(24);
    backButton->setTitleColor(Color3B::BLACK);
    backButton->setPosition(Vec2(contentSize.width - 80, handAreaHeight / 2-40)); // 右下
    backButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    handArea->addChild(backButton);

    // 绑定按钮点击事件
    backButton->addClickEventListener([this](Ref* sender) {
        this->onButtonClicked();
        });


    return true;
}


//回退1
// 刷新手牌显示，按handCards顺序排列
void GameScene::refreshHandCardsDisplay() {
    for (int i = 0; i < handCards.size(); ++i) {
        auto cardView = handCards[i].view;
        auto targetPos = Vec2(100 + i * 120, 200);
        cardView->stopAllActions();
        cardView->runAction(MoveTo::create(0.3f, targetPos));
    }
}

void GameScene::ResetPos() {
    float offsetX = 60.0f;
    float offsetY = 80.0f;
    for (auto& card : handCards) {
        Vec2 curPos = card.view->getPosition();
        Vec2 newPos = Vec2(curPos.x - offsetX, curPos.y - offsetY);
        card.view->setPosition(newPos);
    }
}

// 回退操作，播放相反动画
void GameScene::onButtonClicked() {
    if (OpStack.empty()) {
        MessageBox("no operation can back", "GameTips");
        return;
    }

    OperationRecord op = OpStack.top();
    OpStack.pop();

    if (op.type == OperationType::SwapHandCards) {
        auto card1 = handCards[op.handCardIndex1].view;
        auto card2 = handCards[op.handCardIndex2].view;

        // 播放回退动画：从 posAfter 移动回 posBefore
        auto moveBack1 = MoveTo::create(0.5f, op.pos1Before);
        auto moveBack2 = MoveTo::create(0.5f, op.pos2Before);

        auto swapBack = CallFunc::create([this, op]() {
            // 交换数据结构中的牌，恢复之前顺序
            std::swap(handCards[op.handCardIndex1], handCards[op.handCardIndex2]);

            // 更新CardView中的index，保持同步
            handCards[op.handCardIndex1].view->index = op.handCardIndex1;
            handCards[op.handCardIndex2].view->index = op.handCardIndex2;

            // 顶部牌显示在最上层
            handCards[op.handCardIndex2].view->getParent()->reorderChild(handCards[op.handCardIndex2].view, 200);

            // 刷新手牌显示位置
            refreshHandCardsDisplay();

            //修正
            ResetPos();
            });

        card1->runAction(Sequence::create(moveBack1, swapBack, nullptr));
        card2->runAction(moveBack2);
    }
    else {
        //匹配回退
        CardView* card = op.matchedCard.view;

        // 先让牌显示出来（如果之前隐藏了）
        card->setVisible(true);

        // 播放回退动画：从顶部牌位置移动回原来位置
        auto moveBack = MoveTo::create(0.5f, op.posBefore);

        auto matchBack = CallFunc::create([this, op, card]() {
            // 数据回退：弹出栈顶点数
            if (!st.empty()) {
                st.pop();
            }

            });

        card->runAction(Sequence::create(moveBack, matchBack, nullptr));

    }
}

//

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
        cardData.view->_face = col1[i].first;

        cardData.view->setPosition(Vec2(col1X, startY + i * (cardSize.height + spacing)));

        // 绑定点击回调
        cardData.view->setClickCallback([this](CardView* clickedCard) {
            this->onMainCardClicked(clickedCard);
            });

        playfieldArea->addChild(cardData.view);
        mainDeck.push_back(cardData);
    }

    for (int i = 0; i < count; ++i) {
        CardData cardData;
        cardData.face = col2[i].first;
        cardData.suit = col2[i].second;
        cardData.view = CardView::create(cardData.face, cardData.suit);
        cardData.view->_face = col2[i].first;

        cardData.view->setPosition(Vec2(col2X, startY + i * (cardSize.height + spacing)));

        cardData.view->setClickCallback([this](CardView* clickedCard) {
            this->onMainCardClicked(clickedCard);
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
        cardData.view->_face= cards[i].first;//
        cardData.view->setPosition(Vec2(startX + i * (cardSize.width + spacing), posY));

        cardData.view->setClickCallback([this](CardView* clickedCard) {
            this->onHandCardClicked(clickedCard);
            //MessageBox("hand", "card");
            });

        handArea->addChild(cardData.view);
        handCards.push_back(cardData);
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
    //
    // 记录操作信息
    // 记录交换前位置
    Vec2 posCurrentBefore = handCards[currentIndex].view->getPosition();
    Vec2 posTopBefore = handCards[topIndex].view->getPosition();
    OperationRecord op;
    op.type = OperationType::SwapHandCards;
    op.handCardIndex1 = currentIndex;
    op.handCardIndex2 = topIndex;
    op.pos1Before = posCurrentBefore;
    op.pos2Before = posTopBefore;
    op.pos1After = posTopBefore;
    op.pos2After = posCurrentBefore;

    OpStack.push(op);
    //
    
    //数据交换
    // 立即交换容器数据和更新索引
    std::swap(handCards[currentIndex], handCards[topIndex]);

    // 更新CardView中的index，保持同步
    handCards[currentIndex].view->index = currentIndex;
    handCards[topIndex].view->index = topIndex;

    //更新栈
    st.pop();
    st.push(handCards[topIndex].view->_face);
    //顶部牌显示在最上层
    handCards[topIndex].view->getParent()->reorderChild(handCards[topIndex].view, 200);
}

void GameScene::onMainCardClicked(CardView* clickedCard) {
    
    int topIndex = (int)handCards.size() - 1;
    int currentFace = clickedCard->_face;
    
    int topFace = st.top();
    if (std::abs(currentFace-topFace) > 1) {
        return;
    }
    //目标坐标
    //Vec2 topPos = handCards[topIndex].view->getPosition();
    Vec2 topPos = handCards[topIndex].view->getParent()->convertToWorldSpace(
        handCards[topIndex].view->getPosition()
    );
    topPos = clickedCard->getParent()->convertToNodeSpace(topPos);
    //回退操作记录
    OperationRecord op;
    op.type = OperationType::MatchCards;
    op.matchedCard = { clickedCard->_face, clickedCard->_suit, clickedCard };
    op.posBefore = clickedCard->getPosition();
    op.posAfter = topPos;
    OpStack.push(op);

    // 
    // 动画：点击牌移动到顶部牌位置,顶部牌被覆盖
    auto moveClickedToTop = MoveTo::create(0.5f, topPos);
    clickedCard->runAction(moveClickedToTop);
    //数据部分
    st.push(clickedCard->_face);
    
}