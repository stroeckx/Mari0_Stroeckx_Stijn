#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Enemy.h"

class Koopa : public Enemy
{
public:
	Koopa( const DOUBLE2 Center, const double width, const double height, const double velocity );
	virtual ~Koopa( );
	Koopa( const Koopa& ) = delete;
	Koopa& operator=( const Koopa& ) = delete;

	void Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr);
	void Paint();


private: 
	const int m_NrOfFrames = 1;
	const int m_NrFramesPerSec = 2;
	double m_AnimTime = 0.0;
	int m_AnimFrame = 0;
};
