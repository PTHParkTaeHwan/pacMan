#include "stdafx.h"
#include "scene2.h"
scene2::scene2()
{
}

scene2::~scene2()
{
}

HRESULT scene2::init()
{
	IMAGEMANAGER->addImage("사나", "images/사나.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	return S_OK;
}
void scene2::release()
{
}

void scene2::update()
{
	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		//씬체인지
		SCENEMANAGER->changeScene("scene1");
	}
}

void scene2::render()
{
	IMAGEMANAGER->render("사나", getMemDC());
}
