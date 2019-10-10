#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Block.h"

class BlockMushroom : public Block
{
public:
	BlockMushroom(const DOUBLE2 Center, const double width, const double height, EnemyManager* enemyManager);
	virtual ~BlockMushroom( );
	BlockMushroom( const BlockMushroom& ) = delete;
	BlockMushroom& operator=( const BlockMushroom& ) = delete;

	void Tick(double deltaTime, HUD* HUDPtr);
	void Paint();
	void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);

private: 
	const int m_NrOfFrames = 3;
	const int m_NrFramesPerSec = 2;
	double m_AnimTime = 0.0;
	int m_AnimFrame = 0;
	bool m_IsEmpty = false;
	bool m_SpawnMushroom = false;
	int m_Counter = 0;
	double m_MushroomOffsetY;
	Bitmap* m_BmpActivePtr = new Bitmap(String("Resources/Question.png"));
	Bitmap* m_BmpMushroomPtr = new Bitmap(String("Resources/Mushroom.png"));
	FmodSound* m_SndSpawnPtr{};

};
