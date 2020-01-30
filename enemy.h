#pragma once
#include"gameNode.h"
//에너미 클래스를 상속받아서 보스나 부하들을 만들어된다.
class enemy : public gameNode
{
private :

	image * _image;
	RECT _rc;

	int _currentFrameX;
	int _currentFrameY;
	int _count;

	//랜덤으로 총알 발사할 변수들
	int _fireCount;
	int _rndFireCount;

public:
	enemy();
	~enemy();

	HRESULT init();
	HRESULT init(const char* imageName, POINT position);
	void release();
	void update();
	void render();
	

	virtual void draw();
	virtual void animation();

	bool bulletCountFire();

	RECT getRect() { return _rc; }

};

