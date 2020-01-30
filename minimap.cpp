#include "stdafx.h"
#include "minimap.h"
minimap::minimap()
{
}


minimap::~minimap()
{
}

HRESULT minimap::init()
{

	IMAGEMANAGER->addImage("배경화면", "images/배경.bmp", WINSIZEX * 2, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("카메라", "images/카메라.bmp", WINSIZEX / 10, WINSIZEY / 10, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("미니맵", "images/배경.bmp", WINSIZEX * 2/ 10, WINSIZEY / 10, true, RGB(255, 0, 255));

	_rockman = IMAGEMANAGER->addFrameImage("록맨", "images/록맨.bmp", 960, 200, 10, 2, true, RGB(255, 0, 255));
	mini_rc = RectMakeCenter(100 + _rockman->getX() / 10 - _camera / 10, 50 + _rockman->getY() / 10, 10, 10);
	_rockman->setX(WINSIZEX / 2);
	_rockman->setY(WINSIZEY / 2);


	 _index = 0;
	 _count = 0;
	 _camera = 0;
	 _isLeft =  false;

	return S_OK;
}

void minimap::release()
{
}

void minimap::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _rockman->getX() > 0)
	{
		_isLeft = true;

		if (_rockman->getX() >= WINSIZEX / 2)
		{
			_rockman->setX(_rockman->getX() - PLAYERSPEED);
		}
		else if (_camera < 0)
		{
			_camera += CAMERASPEED;
		}
		else
		{
			_rockman->setX(_rockman->getX() - PLAYERSPEED);
		}

	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _rockman->getX()+80 < WINSIZEX)
	{
		_isLeft = false;

		if (_rockman->getX() <= WINSIZEX / 2)
		{
			_rockman->setX(_rockman->getX() + PLAYERSPEED);
		}
		else if (_camera > -WINSIZEX)
		{
			_camera -= CAMERASPEED;
		}
		else
		{
			_rockman->setX(_rockman->getX() + PLAYERSPEED);
		}
	}
	if (_isLeft)
	{
		_count++;
		_rockman->setFrameY(1);
		if (_count % 5 == 0)
		{
			_count = 0;
			_index--;
			if (_index < _rockman->getMaxFrameY())
			{
				_index = 9;
			}
			_rockman->setFrameX(_index);
		}
	}
	else
	{
		_count++;
		_rockman->setFrameY(0);
		if (_count % 5 == 0)
		{
			_count = 0;
			_index++;
			if (_index >= _rockman->getMaxFrameX())
			{
				_index = 0;
			}
			_rockman->setFrameX(_index);
		}
	}
	mini_rc = RectMakeCenter(100 + _rockman->getX() / 10 - _camera / 10, 50 + _rockman->getY() / 10, 10, 10);
}

void minimap::render()
{
	IMAGEMANAGER->render("배경화면", getMemDC(), _camera, 0);
	IMAGEMANAGER->render("미니맵", getMemDC(), 100, 50);
	IMAGEMANAGER->frameRender("록맨", getMemDC(), _rockman->getX(), _rockman->getY(), _rockman->getFrameX(), _rockman->getFrameY());
	Rectangle(getMemDC(), mini_rc.left, mini_rc.top, mini_rc.right, mini_rc.bottom);
	IMAGEMANAGER->render("카메라", getMemDC(), 100 - _camera / 10, 50);
}
