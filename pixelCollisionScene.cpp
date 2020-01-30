#include "stdafx.h"
#include "pixelCollisionScene.h"
pixelCollisionScene::pixelCollisionScene(){}
pixelCollisionScene::~pixelCollisionScene(){}

HRESULT pixelCollisionScene::init()
{
	IMAGEMANAGER->addImage("배경화면", "images/팩맨 배경.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));  //33,33,255

	_ball = IMAGEMANAGER->addFrameImage("ball", "images/move.bmp", 84, 170, 2, 4, true, RGB(255, 0, 255));

	_ball->setX(50);
	_ball->setY(95);
	_ball->setFrameX(1);
	_ball->setFrameY(1);

	_x = _ball->getX();
	_y = _ball->getY();

	rectUpdate();

	_probeX = _x + _ball->getFrameWidth() / 2;
	_probeY = _y + _ball->getFrameHeight() / 2;

	//=====================
	//  팩맨 변수 초기화  //
	//=====================

	_inputKey = 0;
	_count = 0;
	_ballIndex = 0;

	//=====================
	//  적군 변수 초기화  //
	//=====================
	for (int i = 0; i < 100; i++)
	{
		enemy _enemy;
		_enemy._enemyImg = IMAGEMANAGER->addFrameImage("적", "images/red_enemy.bmp", 296 + 24, 35, 8, 1, true, RGB(255, 0, 255));
		_enemy._enemyImg->setFrameX(0);
		_enemy._enemyImg->setFrameY(0);
		_enemy._enemyImg->setX(45);
		_enemy._enemyImg->setY(90+i*2);
		_enemy._enemyRc = RectMake(_enemy._enemyImg->getX(), _enemy._enemyImg->getY(), _enemy._enemyImg->getFrameWidth(), _enemy._enemyImg->getFrameHeight());
		_enemy._enemyLife = false;
		_enemy._moveOn = false;
		_enemy._enemyDirection = 0;
		_enemy._enemyIndex = 0;
		_vEnemy.push_back(_enemy);
	}

	debug = false;
	return S_OK;
}
void pixelCollisionScene::release()
{
}

void pixelCollisionScene::update()
{
	//=================
	//  플레이어 관련 //
	//=================
	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && !_right && !_up && !_down)
	{
		_left = true;
		_x -= 2.0f;
		_ball->setX(_x);
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LEFT)) _left = false;

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && !_left && !_up && !_down)
	{
		_right = true;
		_x += 2.0f;
		_ball->setX(_x);
	}
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT)) _right = false;

	if (KEYMANAGER->isStayKeyDown(VK_UP) && !_left && !_right && !_down)
	{
		_up = true;
		_y -= 2.0f;
		_ball->setY(_y);
	}
	if (KEYMANAGER->isOnceKeyUp(VK_UP)) _up = false;

	if (KEYMANAGER->isStayKeyDown(VK_DOWN) && !_left && !_right && !_up)
	{
		_down = true;
		_y += 2.0f;
		_ball->setY(_y);
	}
	if (KEYMANAGER->isOnceKeyUp(VK_DOWN)) _down = false;

	_probeX = _x + _ball->getFrameWidth() / 2;
	_probeY = _y + _ball->getHeight() / 2;
	
	checkCollision();
	ballMotionUpdate();
	checkPoint();

	rectUpdate();

	//============
	//  적 관련 //
	//============
	creatEnemay();
	//checkEnemyCollision();
	//enemyMove();

	//===============
	//  디버깅 관련 //
	//===============
	if (KEYMANAGER->isOnceKeyDown(VK_F1))	debug = true;
	if (KEYMANAGER->isOnceKeyDown(VK_F2))	debug = false;
}

void pixelCollisionScene::render()
{
	IMAGEMANAGER->findImage("배경화면")->render(getMemDC());
	//IMAGEMANAGER->findImage("산")->render(getMemDC());
	if (debug)
	{
		char str[100];
		Rectangle(getMemDC(), _rc.left, _rc.top, _rc.right, _rc.bottom);
		wsprintf(str, "%d", _inputKey);
		TextOut(getMemDC(), _rc.left, _rc.top + 10, str, strlen(str));
		wsprintf(str, "%d", _count);
		TextOut(getMemDC(), 10, 10, str, strlen(str));
		wsprintf(str, "%d", _vEnemy.size());
		TextOut(getMemDC(), 10, 30, str, strlen(str));
	}

	for (_vipointRc = _vpointRc.begin(); _vipointRc < _vpointRc.end(); ++_vipointRc)
	{
		_vipointRc->_eatPoint->render(getMemDC(), _vipointRc->_rcPoint.left, _vipointRc->_rcPoint.top);
		//Rectangle(getMemDC(), _vipointRc->_rcPoint.left, _vipointRc->_rcPoint.top, _vipointRc->_rcPoint.right, _vipointRc->_rcPoint.bottom);
	}

	_ball->frameRender(getMemDC(), _ball->getX(), _ball->getY());
	_viEnemy = _vEnemy.begin();
	for (_viEnemy; _viEnemy < _vEnemy.end(); ++_viEnemy)
	{
		char str[100];
		_viEnemy->_enemyImg->frameRender(getMemDC(), _viEnemy->_enemyImg->getX(), _viEnemy->_enemyImg->getY());
		wsprintf(str, "%d", _viEnemy->_moveOn);
		TextOut(getMemDC(), _viEnemy->_enemyImg->getX(), _viEnemy->_enemyImg->getY(), str, strlen(str));
	}


}

void pixelCollisionScene::rectUpdate()
{
	_rc = RectMake(_x, _y, _ball->getFrameWidth(), _ball->getFrameHeight());
}

void pixelCollisionScene::ballMotionUpdate()
{
	//해당되는 방향을 누르면 스위치 문에서 해당되는 _inputKey를 넘겨준다

	if (_left && !_right && !_up && !_down) _inputKey = 0;
	else if (!_left && _right && !_up && !_down) _inputKey = 1;
	else if (!_left && !_right && _up && !_down) _inputKey = 2;
	else if (!_left && !_right && !_up && _down) _inputKey = 3;

	switch (_inputKey)
	{
	case LEFT:
		_ball->setFrameY(1);
		_count++;
		if (_count % 10 == 0)
		{
			_ball->setFrameX(_ballIndex);
			_ballIndex++;
			if (_ballIndex > 1) _ballIndex = 0;
		}
		break;
	case RIGHT:
		_ball->setFrameY(0);
		_count++;
		if (_count % 10 == 0)
		{
			_ball->setFrameX(_ballIndex);
			_ballIndex++;
			if (_ballIndex > 1) _ballIndex = 0;
		}
		break;
	case UP:
		_ball->setFrameY(2);
		_count++;
		if (_count % 10 == 0)
		{
			_ball->setFrameX(_ballIndex);
			_ballIndex++;
			if (_ballIndex > 1) _ballIndex = 0;
		}
		break;
	case DOWN:
		_ball->setFrameY(3);
		_count++;
		if (_count % 10 == 0)
		{
			_ball->setFrameX(_ballIndex);
			_ballIndex++;
			if (_ballIndex > 1) _ballIndex = 0;
		}
		break;
	}
	if (_count > 100000) _count = 0;
}

void pixelCollisionScene::checkCollision()
{
	//오른쪽 벽 체크
	for (int i = _ball->getY(); i < _ball->getY() + _ball->getFrameHeight(); i++)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), _x + _ball->getFrameWidth(), i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 33 && g == 33 && b == 255)
		{
			_x -= 2.0f;
			_ball->setX(_x);
			break;
		}
	}
	//왼쪽 벽 체크
	for (int i = _ball->getY(); i < _ball->getY() + _ball->getFrameHeight(); i++)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), _x, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 33 && g == 33 && b == 255)
		{
			_x += 2.0f;
			_ball->setX(_x);
			break;
		}
	}
	
	//위쪽 벽 체크
	for (int i = _ball->getX(); i < _ball->getX() + _ball->getFrameWidth(); i++)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), i, _y);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 33 && g == 33 && b == 255)
		{
			_y += 2.0f;
			_ball->setY(_y);
			break;
		}
	}
	//아래쪽 벽 체크
	for (int i = _ball->getX(); i < _ball->getX() + _ball->getFrameWidth(); i++)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), i, _y+_ball->getFrameHeight());

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 33 && g == 33 && b == 255)
		{
			_y -= 2.0f;
			_ball->setY(_y);
			break;
		}
	}

}

void pixelCollisionScene::checkPoint()
{
	//왼쪽
	if (_inputKey == 0) 
	{
		for (int i = _ball->getY(); i < _ball->getY() + _ball->getFrameHeight(); i++)
		{
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), _x+_ball->getFrameWidth()/2, i);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 73 && g == 214 && b == 45)
			{
				eatPoint _point;
				_point._rcPoint = RectMake(_x + _ball->getFrameWidth() / 2 - 1, i,10,10);
				_point._eatPoint = IMAGEMANAGER->addImage("먹이", "images/eatPoint.bmp", _point._rcPoint.left, _point._rcPoint.top, 10, 10, true, RGB(255, 0, 255));
				_vpointRc.push_back(_point);
				break;
			}
		}
	}
	//오른쪽
	else if (_inputKey == 1) 
	{
		for (int i = _ball->getY(); i < _ball->getY() + _ball->getFrameHeight(); i++)
		{
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), _x + _ball->getFrameWidth()/2+2, i);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 73 && g == 214 && b == 45)
			{
				eatPoint _point;
				_point._rcPoint = RectMake(_x + _ball->getFrameWidth() / 2, i, 10, 10);
				_point._eatPoint = IMAGEMANAGER->addImage("먹이", "images/eatPoint.bmp", _point._rcPoint.left, _point._rcPoint.top, 10, 10, true, RGB(255, 0, 255));
				_vpointRc.push_back(_point);
				break;
			}
		}
	}
	//위
	else if (_inputKey == 2) 
	{
		for (int i = _ball->getX(); i < _ball->getX() + _ball->getFrameWidth(); i++)
		{
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), i, _y + _ball->getFrameHeight()/2+2);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 73 && g == 214 && b == 45)
			{
				eatPoint _point;
				_point._rcPoint = RectMake(i, _y + _ball->getFrameHeight() / 2-1, 10, 10);
				_point._eatPoint = IMAGEMANAGER->addImage("먹이", "images/eatPoint.bmp", _point._rcPoint.left, _point._rcPoint.top, 10, 10, true, RGB(255, 0, 255));
				_vpointRc.push_back(_point);
				break;
			}
		}
	}
	//아래
	else if (_inputKey == 3) 
	{
		for (int i = _ball->getX(); i < _ball->getX() + _ball->getFrameWidth(); i++)
		{
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), i, _y + _ball->getFrameHeight()/2+2);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 73 && g == 214 && b == 45)
			{
				eatPoint _point;
				_point._rcPoint = RectMake(i, _y + _ball->getFrameHeight() / 2, 10, 10);
				_point._eatPoint = IMAGEMANAGER->addImage("먹이", "images/eatPoint.bmp", _point._rcPoint.left, _point._rcPoint.top, 10, 10, true, RGB(255, 0, 255));
				_vpointRc.push_back(_point);
				break;
			}
		}
	}

}

void pixelCollisionScene::creatEnemay()
{
	if (_count % 100 == 0)
	{
		_viEnemy = _vEnemy.begin();
		for (_viEnemy; _viEnemy != _vEnemy.end(); ++_viEnemy)
		{
			if (!_viEnemy->_enemyLife)
			{
				_viEnemy->_enemyLife = true;
				break;
			}
		}
	}

}

void pixelCollisionScene::checkEnemyCollision()
{
	//적이 벽을 만났는지 여부를 체크하는 함수. 벽을 만나면 직전 움직임 만큼 돌아가면서 moveOn이 false가 된다.

	_viEnemy = _vEnemy.begin();
	for (_viEnemy; _viEnemy != _vEnemy.end(); ++_viEnemy);
	{
		//if (_viEnemy->_moveOn)
		//{
		//	switch (_viEnemy->_enemyDirection)
		//	{
		//	case LEFT:
		//		//왼쪽
		//		for (int i = _viEnemy->_enemyImg->getY(); i < _viEnemy->_enemyImg->getY() + _viEnemy->_enemyImg->getFrameHeight(); i++)
		//		{
		//			COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), _viEnemy->_enemyImg->getX(), i);

		//			int r = GetRValue(color);
		//			int g = GetGValue(color);
		//			int b = GetBValue(color);

		//			if (r == 33 && g == 33 && b == 255)
		//			{
		//				_viEnemy->_enemyImg->setX(_viEnemy->_enemyImg->getX() + 2.0f);
		//				_viEnemy->_moveOn = false;
		//				break;
		//			}
		//		}
		//		break;
		//	case RIGHT:
		//		for (int i = _viEnemy->_enemyImg->getY(); i < _viEnemy->_enemyImg->getY() + _viEnemy->_enemyImg->getFrameHeight(); i++)
		//		{
		//			COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), _viEnemy->_enemyImg->getX() + _viEnemy->_enemyImg->getFrameWidth(), i);

		//			int r = GetRValue(color);
		//			int g = GetGValue(color);
		//			int b = GetBValue(color);

		//			if (r == 33 && g == 33 && b == 255)
		//			{
		//				_viEnemy->_enemyImg->setX(_viEnemy->_enemyImg->getX() - 2.0f);
		//				_viEnemy->_moveOn = false;
		//				break;
		//			}
		//		}
		//		break;
		//	case UP:
		//		for (int i = _viEnemy->_enemyImg->getX(); i < _viEnemy->_enemyImg->getX() + _viEnemy->_enemyImg->getFrameWidth(); i++)
		//		{
		//			COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), i, _viEnemy->_enemyImg->getY());

		//			int r = GetRValue(color);
		//			int g = GetGValue(color);
		//			int b = GetBValue(color);

		//			if (r == 33 && g == 33 && b == 255)
		//			{
		//				_viEnemy->_enemyImg->setY(_viEnemy->_enemyImg->getY() + 2.0f);
		//				_viEnemy->_moveOn = false;
		//				break;
		//			}
		//		}
		//		break;
		//	case DOWN:
		//		for (int i = _viEnemy->_enemyImg->getX(); i < _viEnemy->_enemyImg->getX() + _viEnemy->_enemyImg->getFrameWidth(); i++)
		//		{
		//			COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경화면")->getMemDC(), i, _viEnemy->_enemyImg->getY() + _viEnemy->_enemyImg->getFrameHeight());

		//			int r = GetRValue(color);
		//			int g = GetGValue(color);
		//			int b = GetBValue(color);

		//			if (r == 33 && g == 33 && b == 255)
		//			{
		//				_viEnemy->_enemyImg->setY(_viEnemy->_enemyImg->getY() - 2.0f);
		//				_viEnemy->_moveOn = false;
		//				break;
		//			}
		//		}
		//		break;
		//	}
		//}
	}
	
}

void pixelCollisionScene::enemyMove()
{
	_viEnemy = _vEnemy.begin();
	for (_viEnemy; _viEnemy != _vEnemy.end(); ++_viEnemy);
	{
		if (_viEnemy->_enemyLife)
		{
			if (_viEnemy->_moveOn)
			{
				switch (_viEnemy->_enemyDirection)
				{
				case LEFT:
					_viEnemy->_enemyImg->setX(_viEnemy->_enemyImg->getX() - 2.0f);
					break;
				case RIGHT:
					_viEnemy->_enemyImg->setX(_viEnemy->_enemyImg->getX() + 2.0f);
					break;
				case UP:
					_viEnemy->_enemyImg->setY(_viEnemy->_enemyImg->getY() - 2.0f);
					break;
				case DOWN:
					_viEnemy->_enemyImg->setY(_viEnemy->_enemyImg->getY() + 2.0f);
					break;
				}
			}
			else if (!_viEnemy->_moveOn)
			{
				for (int i = 0; i < 100; i++)
				{
					int temp = RND->getFromIntTo(0, 3);
					if (_viEnemy->_enemyDirection != temp)
					{
						_viEnemy->_moveOn = true;
						_viEnemy->_enemyDirection = temp;
						break;
					}
				}
			}
		}
	}
}

