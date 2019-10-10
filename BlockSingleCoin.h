#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Block.h"

class BlockSingleCoin : public Block
{
public:
	BlockSingleCoin( const DOUBLE2 Center, const double width, const double height, EnemyManager* enemyManager, int numberOfCoins = 1);
	virtual ~BlockSingleCoin( );
	BlockSingleCoin( const BlockSingleCoin& ) = delete;
	BlockSingleCoin& operator=( const BlockSingleCoin& ) = delete;

	void Tick(double deltaTime, HUD* HUDPtr);
	void Paint();
	void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);


private:
	const int m_NrOfFrames = 3;
	const int m_NrFramesPerSec = 2;
	double m_AnimTime = 0.0;
	int m_AnimFrame = 0;
	bool m_IsEmpty = false;
	const double m_CoinWidth = 8;
	int m_CoinFrame = 0;
	bool m_CoinActive = false;
	Bitmap* m_BmpActivePtr = new Bitmap(String("Resources/Question.png"));
	Bitmap* m_BmpCoinPtr = new Bitmap(String("Resources/coinblockanimation.png"));
	int m_AddScore = 0;
	int m_ScoreValue = 200;
	FmodSound* m_SndCoinPtr{};
	int m_NrOfCoins = 0;
};
