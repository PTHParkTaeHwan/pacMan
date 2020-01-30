#pragma once
#include"gameNode.h"

enum MOTION
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct eatPoint
{
	image * _eatPoint;
	RECT _rcPoint;
};

struct enemy
{
	image * _enemyImg;
	RECT _enemyRc;
	bool _enemyLife;
	bool _moveOn;
	int _enemyDirection;
	int _enemyIndex;
};
class pixelCollisionScene : public gameNode
{
private:

	vector<eatPoint> _vpointRc;
	vector<eatPoint>::iterator _vipointRc;

	vector<enemy> _vEnemy;
	vector<enemy>::iterator _viEnemy;


	image * _ball;
	RECT _rc;
	float _x, _y;
	int _probeX;	//픽셀검사용
	int _probeY;	//픽셀검사용
	bool debug;

	//팩맨 모션 변수
	bool _left;
	bool _right;
	bool _up;
	bool _down;
	
	int _inputKey;
	int _count;
	int _ballIndex;


public:
	pixelCollisionScene();
	~pixelCollisionScene();

	HRESULT init();
	void release();
	void update();
	void render();

	//ball 상자 업데이트
	void rectUpdate();
	//ball의 프레임 이미지 업데이트
	void ballMotionUpdate();
	//ball과 벽 충돌 확인
	void checkCollision();
	//ball과 포인트 충돌 확인
	void checkPoint();


	void creatEnemay();
	void enemyMove();
	void checkEnemyCollision();

};

