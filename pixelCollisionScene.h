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
	int _probeX;	//�ȼ��˻��
	int _probeY;	//�ȼ��˻��
	bool debug;

	//�Ѹ� ��� ����
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

	//ball ���� ������Ʈ
	void rectUpdate();
	//ball�� ������ �̹��� ������Ʈ
	void ballMotionUpdate();
	//ball�� �� �浹 Ȯ��
	void checkCollision();
	//ball�� ����Ʈ �浹 Ȯ��
	void checkPoint();


	void creatEnemay();
	void enemyMove();
	void checkEnemyCollision();

};

