// GameController.cpp
#include "GameController.h"
#include "GameScene.h"

GameController::GameController(GameScene* scene)
    : _scene(scene) {}

bool GameController::handleCardClick(int cardIndex) {
    if (!_scene) return false;

    // 这里写你的业务逻辑判断，比如是否允许点击
    auto& handCards = _scene->getHandCards();
    if (cardIndex < 0 || cardIndex >= (int)handCards.size()) return false;

    auto& clickedCard = handCards[cardIndex];

    // 只处理♥A点击
    if (clickedCard.face == CFT_ACE && clickedCard.suit == CST_HEARTS) {
        // 执行替换动画等逻辑
        _scene->replaceTopCardWith(cardIndex);
        return true;
    }
    return false;
}
