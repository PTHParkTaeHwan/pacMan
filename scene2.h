#pragma once
#include"gameNode.h"
class scene2 : public gameNode
{
public:
	scene2();
	~scene2();

	HRESULT init();
	void release();
	void update();
	void render();
};

