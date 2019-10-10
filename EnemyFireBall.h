#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Enemy.h"

class EnemyFireBall : public Enemy
{
public:
	EnemyFireBall(const DOUBLE2 Center, const double width, const double height, const double velocity, bool autoExplode = false);
	virtual ~EnemyFireBall( );
	EnemyFireBall( const EnemyFireBall& ) = delete;
	EnemyFireBall& operator=( const EnemyFireBall& ) = delete;

	void Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr);
	void Paint();
	void FlipDirection();
	void Destroy();
	void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);

private: 
	const int m_NrOfFrames = 3;
	const int m_NrFramesPerSec = 6;
	double m_AnimTime = 0.0;
	int m_AnimFrame = 0;
	bool m_De_Stroyed = false;
	double m_SoundLength = 0.360;
	double m_DeathTimer = 0;
	FmodSound* m_SndDeathPtr{};
	Bitmap* m_BmpDestroyPtr{};
	double m_DestroyedSize = 16;
};
