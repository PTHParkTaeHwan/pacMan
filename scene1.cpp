#include "stdafx.h"
#include "scene1.h"


scene1::scene1()
{
}


scene1::~scene1()
{
}

HRESULT scene1::init()
{

	IMAGEMANAGER->addImage("사나1", "images/사나1.bmp", WINSIZEX, WINSIZEY,true, RGB(255,0,255));

	return S_OK;
}

void scene1::release()
{
}

void scene1::update()
{
	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		//씬체인지
		SCENEMANAGER->changeScene("scene2");
	}
}

void scene1::render()
{
	IMAGEMANAGER->render("사나1", getMemDC());
}
