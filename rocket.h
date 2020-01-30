#pragma once
#include"gameNode.h"
#include"bullet.h"
#include"progressBar.h"
#define PLAYERSPEED 5


//참고 :  헤더파일이 많아 질수록 컴파일 속도가 저하된다.
/*
	class A가 a.h있고 class B가 b.h에 있는데
	B가 A를 사용하고 A가 B를 사용할 경우 :설계가 잘못되었다고 볼수 있다.
	요로코롬 하면 이해하기도 힘들고 추적이 힘들다.

	에이는 비의 데이터를 알기위해서 비의 구조를 알아야하고 즉 비 헤더를 인클루드 해야한다.
	하지만 비에서 이미 에이라는 헤더를 인클루드를 하고 있기에 에이에서 비의 헤더를 인클루드 하면 상호 참조 에러가 된다.
	구조를 바꾸지 않으면 인클루드 할수 없다.
	에이에서 비를 인클루드 한다고 할때 만약 비 헤더가 수정되면 인클루드 부분도 
	이영향을 받기 때문에 수정이 많아질수록 컴파일 속도도 저하된다.
	이것을 해결하기 위한 방법은 전방선언이다.
	(장점 :  참조하려는 헤더파일에 변경이 생겨도 참조하는 헤더파일에서는 재컴파일이 이루어 지지 않는다.)
	단 주의 해야 할점 :  전방선언할 경우 그 클래스 관련 객체는 포인터형으로 선언해야 한다.
	만약 포인터형이 아닌 객체를 생성할 경우 전방선언의 특징상 그 객체의 크기를 정확히 파악하여 할당을 못해주기
	때문이다.(해당 클래스가 있다는 정보만 사이즈를 모른다.)
*/

//상호참조용 적 매니저 클래스 전방선언
class enemyManager;
class rocket :public gameNode
{
private:

	image * _player;
	missile* _missile;
	nuclear* _nuclear;
	RECT rc;

	//프로그레스바 
	progressBar* _hpBar;
	float _maxHP;
	float _currentHP;


	enemyManager* _em;		//적 매니저 클래스 선언

public:
	rocket();
	~rocket();

	HRESULT init();
	void release();
	void update();
	void render();

	//미슬 삭제
	void removeMissile(int arrNum);
	//충돌함수
	void collision();
	//피깎는 함수
	void hitDamage(float damage);

	//뉴클리어 가져오기
	nuclear* getMissile() { return _nuclear; }
	//로켓 이미지 가져오기
	image* getRocketImage() { return _player; }

	void setEnemyMemoryLink(enemyManager* em) { _em = em; }
};

