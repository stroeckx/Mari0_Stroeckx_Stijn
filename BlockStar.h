#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Block.h"

class BlockStar : public Block
{
public:
	BlockStar(const DOUBLE2 Center, const double width, const double height, EnemyManager* enemyManager);
	virtual ~BlockStar( );
	BlockStar( const BlockStar& ) = delete;
	BlockStar& operator=( const BlockStar& ) = delete;

	void Tick(double deltaTime, HUD* HUDPtr);
	void Paint();
	void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);
	EnemyType SpawnEnemy();

private: 
	const int m_NrOfFrames = 4;
	const int m_NrFramesPerSec = 1;
	double m_AnimTime = 0.0;
	int m_AnimFrame = 0;
	int m_Counter = 0;
	bool m_IsEmpty = false;
	bool m_SpawnStar = false;
	double m_StarOffsetY = 0;
	Bitmap* m_BmpStarPtr = new Bitmap(String("Resources/Star.png"));
	Bitmap* m_BmpActivePtr = new Bitmap(String("Resources/Wall.png"));
};
