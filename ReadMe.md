# 程序设计文档

## 项目名称
**扑克牌游戏**

## 版本
1.0

## 概述
本模块基于 Cocos2d-x 框架，实现扑克牌游戏中的核心场景管理功能，包括牌的创建、显示、用户交互、操作回退等。通过 `GameScene` 类管理游戏主牌区和手牌区，`CardView` 类负责单张牌的显示和点击事件。

---

## 1. 模块结构

### 1.1 CardView 类

#### 1.1.1 作用
- 负责显示单张扑克牌的图像（点数和花色）
- 处理用户点击事件，支持外部注册点击回调

#### 1.1.2 主要成员

| 成员变量               | 说明                         |
|------------------------|------------------------------|
| `_face`                | 牌点数（如 A、2、K 等）      |
| `_suit`                | 牌花色（梅花、红桃等）        |
| `background`           | 牌面背景精灵                 |
| `smallNumber`, `bigNumber` | 牌面数字精灵               |
| `suitIcon`             | 花色图标精灵                 |
| `_clickCallback`       | 点击事件回调函数             |
| `_touchListener`       | 触摸事件监听器               |
| `_index`               | 牌在容器中的索引             |

#### 1.1.3 主要方法

- `static CardView* create(int face, int suit)`  
  创建并初始化一张牌的显示节点。

- `bool init(int face, int suit)`  
  初始化牌的点数和花色，加载对应资源，使用SpriteBatchNode。

- `void setClickCallback(const ClickCallback& cb)`  
  注册点击事件回调。

- `void onEnter()` 和 `void onExit()`  
  注册和注销触摸事件监听器。

---

### 1.2 GameScene 类

#### 1.2.1 作用
- 管理主牌区和手牌区的牌数据及显示
- 处理用户点击牌的交互逻辑
- 维护操作记录，实现操作回退功能
- 控制牌的显示顺序和位置布局

#### 1.2.2 主要数据结构

- `struct CardData`  
  封装单张牌的点数、花色及对应的显示节点。

- `enum class OperationType`  
  定义操作类型，包括交换手牌和匹配牌。

- `struct OperationRecord`  
  记录一次操作的详细信息，用于回退。

#### 1.2.3 主要成员变量

| 成员变量           | 说明                             |
|--------------------|----------------------------------|
| `_mainDeck`        | 主牌区牌数据容器                  |
| `_handCards`       | 手牌区牌数据容器                  |
| `playfieldArea`    | 主牌区显示节点                   |
| `handArea`         | 手牌区显示节点                   |
| `_st`              | 顶牌数据栈                       |
| `_OpStack`         | 操作记录栈，用于回退             |

#### 1.2.4 主要方法

- `static cocos2d::Scene* createScene()`  
  创建并返回游戏场景实例。

- `bool init()`  
  初始化游戏场景，创建牌堆和手牌。

- `void createMainDeckCards()`  
  创建主牌区的牌并添加到场景。

- `void createHandCards()`  
  创建手牌区的牌并添加到场景。

- `void onHandCardClicked(CardView* clickedCard)`  
  处理手牌点击事件。

- `void onMainCardClicked(CardView* clickedCard)`  
  处理主牌点击事件。

- `void onButtonClicked()`  
  回退按钮点击事件，撤销上一步操作。

- `void refreshHandCardsDisplay()`  
  根据当前手牌顺序刷新显示。

- `void resetPos()`  
  修正牌的位置布局。

---

## 2. 设计细节

### 2.1 牌的显示与交互

- 每张牌由 `CardView` 负责显示，包含点数和花色的图像资源。
- 牌支持单点触摸事件，点击时通过回调通知 `GameScene` 处理逻辑。
- `GameScene` 根据点击事件执行相应操作，如选牌、交换、匹配等。

### 2.2 操作回退机制

- 每次用户操作（交换手牌、匹配牌）都会生成一个 `OperationRecord`，记录操作类型和相关数据（牌索引、位置变化等）。
- 操作记录入栈 `_OpStack`，支持多步回退。
- 回退时弹出操作记录，恢复牌的位置和状态。

### 2.3 牌堆和手牌管理

- `_mainDeck` 和 `_handCards` 分别存储主牌堆和手牌区的牌数据。
- 通过 `createMainDeckCards()` 和 `createHandCards()` 初始化牌堆。
- 牌的显示顺序和位置通过 `refreshHandCardsDisplay()` 和 `resetPos()` 维护，保证界面整齐。

---

## 3. 交互流程示例

1. 游戏开始，调用 `createScene()` 创建场景，初始化牌堆和手牌。
2. 用户点击手牌，触发 `onHandCardClicked()`，执行手牌区交换逻辑。
3. 操作完成后，生成 `OperationRecord` 并入栈，方便回退。
4. 用户点击回退按钮，调用 `onButtonClicked()`，弹出操作记录，恢复牌状态。
5. 牌的显示通过 `refreshHandCardsDisplay()` 和 `resetPos()` 保持更新。

---

## 4. 依赖关系

- 依赖 Cocos2d-x 引擎提供的场景管理、节点、事件监听等功能。
- 依赖 `CardView` 类实现单张牌的显示和交互。
- 使用标准库容器如 `std::vector` 和 `std::stack` 管理牌数据和操作记录。

---

## 5. 新加入卡牌和新类型回退说明
###一、新增加卡牌
1. 添加卡牌花色和牌面的枚举类型

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

2.添加花色和牌面之后，在CardView::getFaceName 和 CardView::getSuitName中添加对应的映射

	std::string CardView::getFaceName(int face) {
		switch (face) {
		case CFT_ACE: return "A";
		case CFT_TWO: return "2";
		case CFT_THREE: return "3";
		case CFT_FOUR: return "4";
		case CFT_KING: return "K";
		case CFT_JACK: return "J";  // 新增
		default: return "";
		}
	}

	std::string CardView::getSuitName(int suit) {
		switch (suit) {
		case CST_CLUBS: return "club";
		case CST_DIAMONDS: return "diamond";
		case CST_HEARTS: return "heart";
		case CST_SPADES: return "spade";
		case CST_STAR: return "star";  // 新增
		default: return "";
		}
	}


3.添加资源文件
根据新增的点数和花色，准备对应的图片资源,我这里是我工程目录下的资源路径，根据需要适当修改：
小数字图：res/number/small_red_J.png、res/number/small_black_J.png（根据花色颜色区分）
大数字图：res/number/big_red_J.png、res/number/big_black_J.png
花色图标：res/suits/star.png

4.修改支持牌判断逻辑（如果有特殊规则）
CardView::init 中有一段判断是否支持的牌：

	if ((face == CFT_THREE && (suit == CST_CLUBS || suit == CST_DIAMONDS)) ||
		(face == CFT_ACE && suit == CST_HEARTS) ||
		(face == CFT_FOUR && suit == CST_CLUBS) ||
		(face == CFT_KING && suit == CST_CLUBS) ||
		(face == CFT_TWO && suit == CST_SPADES)) {
		supported = true;
	}


5.在游戏逻辑中使用新卡牌
在 GameScene::createMainDeckCards 或 createHandCards 中添加新卡牌数据：

	std::vector<std::pair<int, int>> newCards = {
		{CFT_JACK, CST_STAR},
		// 其他新卡牌
	};

然后创建对应的 CardView 并添加到场景。

###二、新增一个新的回退功能
1.设计新的操作类型
当前回退功能是根据 OperationRecord 结构体和 OperationType 枚举实现的：

		enum class OperationType {
		SwapHandCards,
		MatchCards,
		// 未来可新增类型
		NewOperationType
		};
		
		struct OperationRecord {
		OperationType type;
		// 具体操作相关数据
		};

新增回退功能时，先在 OperationType 中添加新的操作类型，比如：

	OperationType::NewOperationType

2.定义新的操作记录结构
根据新功能的需求，扩展 OperationRecord，添加必要的字段来保存操作前后的状态，方便回退时恢复。

例如，如果是“新增卡牌”操作，可能需要保存新增卡牌的索引、位置等。

3.在执行新操作时，记录操作信息
在执行新操作的函数中，创建对应的 OperationRecord，填充操作前后状态，压入 _OpStack。

示例：

	OperationRecord op;
	op.type = OperationType::NewOperationType;
	// 填充op的其他字段
	_OpStack.push(op);

4.修改 GameScene::onButtonClicked 实现回退逻辑
在回退按钮的回调函数中，增加对新操作类型的判断和处理：

	if (op.type == OperationType::NewOperationType) {
		// 播放回退动画
		// 恢复数据状态
	}****

5.播放回退动画和恢复数据
根据新操作的特点，设计合适的动画效果和数据恢复逻辑，保证回退操作流畅且状态正确。

