#include "stdafx.h"
#include "soundTest.h"


soundTest::soundTest()
{
}


soundTest::~soundTest()
{
}

HRESULT soundTest::init()
{
	SOUNDMANAGER->addSound("¾Ë·¯ºä", "sound/snow_flower.mp3", true, true);
	return S_OK;
}

void soundTest::release()
{
}

void soundTest::update()
{
	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		SOUNDMANAGER->play("¾Ë·¯ºä", 1.0f);
	}
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		SOUNDMANAGER->stop("¾Ë·¯ºä");
	}
	if (KEYMANAGER->isOnceKeyDown('T'))
	{
		SOUNDMANAGER->resume("¾Ë·¯ºä");
	}
	if (KEYMANAGER->isOnceKeyDown('P'))
	{
		SOUNDMANAGER->pause("¾Ë·¯ºä");
	}

}
