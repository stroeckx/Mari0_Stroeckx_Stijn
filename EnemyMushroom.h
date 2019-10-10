#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Enemy.h"

class EnemyMushroom : public Enemy
{
public:
	EnemyMushroom(const DOUBLE2 Center, const double width, const double height, const double velocity);
	virtual ~EnemyMushroom( );
	EnemyMushroom( const EnemyMushroom& ) = delete;
	EnemyMushroom& operator=( const EnemyMushroom& ) = delete;

	virtual void Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr);
	virtual void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);

private: 
	FmodSound* m_SndEatPtr{};
};
