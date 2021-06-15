#include "BattleScene.h"
#include "Knight.h"

//#include "weapon.h"
#define schedule_selector(_SELECTOR) static_cast<cocos2d::SEL_SCHEDULE>(&_SELECTOR)
cocos2d::Scene* Knight::createScene()
{
	auto scene = Scene::create();
	auto layer0 = Knight::create();
	//auto layer1 = createShot::create();
	//auto layerd = d::create();

	//layer1->startx = &layerd->pl->getPosition().x;
	//layer1->starty = &layerd->pl->getPosition().y;

	scene->addChild(layer0);
	//scene->addChild(layer1);
	//scene->addChild(layerd);

	return scene;
}
bool Knight::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();//居中坐标
	Vec2 position = Vec2(0.25 * visibleSize.width, 1.8 * visibleSize.height);

	_Knight = Sprite::create("Knight1.png");//创建精灵
	_Knight->setPosition(position);//初始坐标
	this->addChild(_Knight);

	auto keyListener = EventListenerKeyboard::create();//键盘监听
	keyListener->onKeyPressed = CC_CALLBACK_2(Knight::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(Knight::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	schedule(schedule_selector(Knight::knightMove), 0.25f);//定时器执行动画
	this->scheduleUpdate();
	return true;
}

Animate* Knight::getAnimateRight()//右移动画
{
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("Knight.plist");

	Vector<SpriteFrame*>images;
	images.pushBack(cache->getSpriteFrameByName("Knight1.png"));
	images.pushBack(cache->getSpriteFrameByName("Knight2.png"));
	images.pushBack(cache->getSpriteFrameByName("Knight3.png"));
	images.pushBack(cache->getSpriteFrameByName("Knight4.png"));

	Animation* animation = Animation::createWithSpriteFrames(images, 0.25 / images.size());
	Animate* animate = Animate::create(animation);
	return animate;
}

Animate* Knight::getAnimateLeft() //左移动画
{
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("KnightLeft.plist");

	Vector<SpriteFrame*>images;
	images.pushBack(cache->getSpriteFrameByName("KnightLeft1.png"));
	images.pushBack(cache->getSpriteFrameByName("KnightLeft2.png"));
	images.pushBack(cache->getSpriteFrameByName("KnightLeft3.png"));
	images.pushBack(cache->getSpriteFrameByName("KnightLeft4.png"));

	Animation* animation = Animation::createWithSpriteFrames(images, 0.25 / images.size());
	Animate* animate = Animate::create(animation);
	return animate;
}

void Knight::onKeyPressed(EventKeyboard::KeyCode Code, Event* events)
{
	keyMap[Code] = true;
}

void Knight::onKeyReleased(EventKeyboard::KeyCode Code, Event* event)
{
	keyMap[Code] = false;
}

void Knight::knightMove(float dt)
{
	log("1");
	auto w = EventKeyboard::KeyCode::KEY_W;
	auto s = EventKeyboard::KeyCode::KEY_S;
	auto a = EventKeyboard::KeyCode::KEY_A;
	auto d = EventKeyboard::KeyCode::KEY_D;
	int offsetx = 0;
	int offsety = 0;
	if (Limit == 0)
	{
		if (keyMap[a])
		{
			offsetx = -40;
			direction = 1;
			_Knight->runAction(getAnimateLeft());
		}
		if (keyMap[d])
		{
			offsetx = 40;
			direction = 0;
			_Knight->runAction(getAnimateRight());
		}
		else if (keyMap[w])
		{
			offsety = 40;
		}
		else if (keyMap[s])
		{
			offsety = -40;
		}
	}
	if (offsetx == 0 && offsety == 0)
	{
		return;
	}
	if (direction == 0)
	{
		_Knight->runAction(getAnimateRight());
	}
	else
	{
		_Knight->runAction(getAnimateLeft());
	}
	auto moveto = MoveTo::create(0.25, Vec2(_Knight->getPosition().x + offsetx, _Knight->getPosition().y + offsety));
	_Knight->runAction(moveto);
}

Knight::~Knight()
{
}

bool Knight::isinPortal(Sprite* portal)
{
	Rect entityRect = this->_Knight->getBoundingBox();
	Point PortalPos = portal->getPosition();
	//judge if the boundingbox of the knight has touched the portal center.
	return entityRect.containsPoint(PortalPos);
}

void Knight::update(float dt)
{
	if (portal == nullptr)
		return;
	else if (isinPortal(this->portal))
	{
		Director::getInstance()->replaceScene(TransitionSlideInT::create(3.0f, BattleScene::create()));
	}
}
void Knight::bindSprite(Sprite* sprite)
{
	this->portal = sprite;
}
