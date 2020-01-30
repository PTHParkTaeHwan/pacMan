#include "stdafx.h"
#include "loadingTest.h"


loadingTest::loadingTest()
{
}


loadingTest::~loadingTest()
{
}

HRESULT loadingTest::init()
{

	_loading = new loading;
	_loading->init();

	this->totalLoading();
	return S_OK;
}

void loadingTest::release()
{
	SAFE_DELETE(_loading);
}

void loadingTest::update()
{
	_loading->update();
	//로딩이 끝나면
	if (_loading->loadingDone())
	{
		SCENEMANAGER->changeScene("scene1");
	}

}

void loadingTest::render()
{
	_loading->render();
}

void loadingTest::totalLoading()
{
	for (int i = 0; i < 500; i++)
	{
		_loading->loadImage("1", WINSIZEX, WINSIZEY);
	}
}
