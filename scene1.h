#pragma once
#include"gameNode.h"
class scene1 : public gameNode
{
public:
	scene1();
	~scene1();

	HRESULT init();
	void release();
	void update();
	void render();
};

