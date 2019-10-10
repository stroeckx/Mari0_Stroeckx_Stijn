#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Block.h"

class BlockWall : public Block
{
public:
	BlockWall( const DOUBLE2 Center, const double width, const double height, EnemyManager* enemyManager);
	virtual ~BlockWall( );
	BlockWall( const BlockWall& ) = delete;
	BlockWall& operator=( const BlockWall& ) = delete;

	void Paint();
	virtual void Tick(double deltaTime, HUD* HUDPtr);
	void PaintQuarter(PhysicsActor* actPtr, DOUBLE2 rect);
	void Break();

private: 
	PhysicsActor* m_ActTopLeftPtr = nullptr;
	DOUBLE2 m_TopLeftPosOrig;
	PhysicsActor* m_ActTopRightPtr = nullptr;
	DOUBLE2 m_TopRightPosOrig;
	PhysicsActor* m_ActBottomLeftPtr = nullptr;
	DOUBLE2 m_BottomLeftPosOrig;
	PhysicsActor* m_ActBottomRightPtr = nullptr;
	DOUBLE2 m_BottomRightPosOrig;
	double m_Angle = 0;
	FmodSound* m_SndBreakPtr{};
};
