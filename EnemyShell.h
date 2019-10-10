#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Enemy.h"

class EnemyShell : public Enemy
{
public:
	EnemyShell(const DOUBLE2 Center, const double width, const double height, const double velocity);
	virtual ~EnemyShell( );
	EnemyShell( const EnemyShell& ) = delete;
	EnemyShell& operator=( const EnemyShell& ) = delete;

	void Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr);
	void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);

private: 
	bool m_IsActivated = false;
	double m_DistanceMario = 0;
	double m_TimeActivated = 0;
	const double m_ActivatedDelay = 1;
};
