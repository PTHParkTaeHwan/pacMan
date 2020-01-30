#pragma once
#include"gameNode.h"
#include"loading.h"
class loadingTest: public gameNode
{
private:
	loading * _loading;
public:
	loadingTest();
	~loadingTest();

	HRESULT init();
	void release();
	void update();
	void render();
	
	void totalLoading();
};

