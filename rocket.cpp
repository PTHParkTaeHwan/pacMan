#include "stdafx.h"
#include "rocket.h"
#include"enemyManager.h"
#pragma warning(disable : 4996)

//cpp에 인클루드 해야 진정한 상호참조...
rocket::rocket()
{
}

rocket::~rocket()
{
}

HRESULT rocket::init()
{

	_player = IMAGEMANAGER->addImage("rocket", "images/rocket.bmp", WINSIZEX / 2, WINSIZEY - 200, 52, 64, true, RGB(255, 0, 255));

	rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, _player->getWidth(), _player->getHeight());


	_missile = new missile;
	_missile->init(30, 1000);

	_nuclear = new nuclear;
	_nuclear->init(30, 1000);


	_maxHP = _currentHP = 100;
	_hpBar = new progressBar;
	_hpBar->init("images/progressBarFront.bmp", "images/progressBarBack.bmp", 0, 0, 50, 10);
	_hpBar->setGauge(_currentHP, _maxHP);
	return S_OK;
}

void rocket::release()
{
	SAFE_DELETE(_missile);
	SAFE_DELETE(_nuclear);
	SAFE_DELETE(_hpBar);
}

void rocket::update()
{
	//왼쪽
	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _player->getX() > 0)
	{
		_player->setX(_player->getX() - PLAYERSPEED);
	}

	//오른쪽
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _player->getX() + _player->getWidth() < WINSIZEX)
	{
		_player->setX(_player->getX() + PLAYERSPEED);
	}

	//위
	if (KEYMANAGER->isStayKeyDown(VK_UP) && _player->getY() > 0)
	{
		_player->setY(_player->getY() - PLAYERSPEED);
	}

	//아래
	if (KEYMANAGER->isStayKeyDown(VK_DOWN) && _player->getY() + _player->getHeight() < WINSIZEY)
	{
		_player->setY(_player->getY() + PLAYERSPEED);
	}

	//if (KEYMANAGER->isStayKeyDown(VK_LEFT) )
	//{
	//	rc.left -= PLAYERSPEED;
	//	rc.right -= PLAYERSPEED;
	//}

	////오른쪽
	//if (KEYMANAGER->isStayKeyDown(VK_RIGHT) )
	//{
	//	rc.left += PLAYERSPEED;
	//	rc.right += PLAYERSPEED;
	//}

	////위
	//if (KEYMANAGER->isStayKeyDown(VK_UP))
	//{
	//	rc.top -= PLAYERSPEED;
	//	rc.bottom -= PLAYERSPEED;
	//}

	////아래
	//if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	//{
	//	rc.top += PLAYERSPEED;
	//	rc.bottom += PLAYERSPEED;
	//}

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		//_missile->fire(rc.right - (rc.right - rc.left) / 2, rc.top - 50);
		_missile->fire(_player->getX() + _player->getWidth() / 2, _player->getY() - 50);
	}
	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		//_nuclear->fire(rc.right - (rc.right - rc.left) / 2, rc.top - 50);
		_nuclear->fire(_player->getX() + _player->getWidth() / 2, _player->getY() - 50);
	}
	_missile->update();
	_nuclear->update();


	collision();
	//프로그래스
	_hpBar->setX(_player->getX());
	_hpBar->setY(_player->getY() - 15);
	_hpBar->setGauge(_currentHP, _maxHP);
	_hpBar->update();

	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		char temp[64];
		vector<string>vStr;

		vStr.push_back(_itoa(_currentHP, temp, 10));
		vStr.push_back(_itoa(_maxHP, temp, 10));
		vStr.push_back(_itoa(_player->getX(), temp, 10));
		vStr.push_back(_itoa(_player->getY(), temp,10));

		TXTDATA->txtSave("save/rocketData.txt", vStr);
	}
	if (KEYMANAGER->isOnceKeyDown('L'))
	{
		vector<string> vStr (TXTDATA->txtLoad("save/rocketData.txt"));

		_currentHP = stoi(vStr[0]);
		_maxHP = stoi(vStr[1]);
		_player->setX(stoi(vStr[2]));
		_player->setY(stoi(vStr[3]));
	}
}

void rocket::render()
{
	_player->render(getMemDC(), _player->getX(), _player->getY());
	//_player->render(getMemDC(), rc.left, rc.top);

	_missile->render();
	_nuclear->render();

	_hpBar->render();
}

void rocket::removeMissile(int arrNum)
{
	_nuclear->removeNuclear(arrNum);
}

void rocket::collision()
{
	for (int i = 0; i < _nuclear->getVBullet().size(); i++)
	{

		for (int j = 0; j < _em->getVMinion().size(); j++)
		{
			RECT rc;

			if (IntersectRect(&rc, &_nuclear->getVBullet()[i].rc, &_em->getVMinion()[j]->getRect()))
			{
				_nuclear->removeNuclear(i);
				_em->removeMinion(j);
				break;
			}
		}
	}
}

void rocket::hitDamage(float damage)
{
	_currentHP -= damage;
}

