#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Enemy.h"

class Goomba : public Enemy
{
public:
	Goomba(const DOUBLE2 Center, const double width, const double height, const double velocity);
	virtual ~Goomba( );
	Goomba( const Goomba& ) = delete;
	Goomba& operator=( const Goomba& ) = delete;

	void Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr);
	void Paint();
	bool IsHit(PhysicsActor* actOtherPtr);
	//bool IsDead();
	//void SetDead();

private: 
	const int m_NrOfFrames = 1;
	const int m_NrFramesPerSec = 2;
	double m_AnimTime = 0.0;
	int m_AnimFrame = 0;
	//bool m_IsDieing = false;
	//double m_TimeDead = 0;
	//bool m_IsDead = false;
};