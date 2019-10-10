#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Enemy.h"

class EnemyStar : public Enemy
{
public:
	EnemyStar(const DOUBLE2 Center, const double width, const double height, const double velocity);
	virtual ~EnemyStar( );
	EnemyStar( const EnemyStar& ) = delete;
	EnemyStar& operator=( const EnemyStar& ) = delete;

	virtual void Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr);
	virtual void Paint();
	virtual void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);

private: 
	const int m_NrOfFrames = 4;
	const int m_NrFramesPerSec = 4;
	double m_AnimTime = 0.0;
	int m_AnimFrame = 0;
	double m_BounceHeight = 600;
	void FlipDirection();

};
