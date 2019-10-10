//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "BlockSingleCoin.h"
#include "HUD.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

BlockSingleCoin::BlockSingleCoin(const DOUBLE2 Center, const double width, const double height, EnemyManager* enemyManager, int numberOfCoins) : m_NrOfCoins(numberOfCoins), Block(Center, width, height, String("Resources/EmptyQuestion.png"), enemyManager)
{
	m_SndCoinPtr = new FmodSound();
	m_SndCoinPtr->CreateSound(String("Resources/Sounds/coin.ogg"));
}

BlockSingleCoin::~BlockSingleCoin()
{
	delete m_BmpActivePtr;
	m_BmpActivePtr = nullptr;
	delete m_BmpCoinPtr;
	m_BmpCoinPtr = nullptr;
	delete m_SndCoinPtr;
	m_SndCoinPtr = nullptr;
}

void BlockSingleCoin::Paint()
{
	double width = m_BmpActivePtr->GetWidth()/3;
	double height = m_BmpActivePtr->GetHeight();

	if (m_CoinActive)
	{
		GAME_ENGINE->DrawBitmap(m_BmpCoinPtr, m_Center - DOUBLE2(m_CoinWidth / 2, m_BmpCoinPtr->GetHeight()), { m_CoinFrame * m_CoinWidth,0,(m_CoinFrame + 1) * m_CoinWidth, double(m_BmpCoinPtr->GetHeight()) });
	}

	if (!m_IsEmpty)
	{
		GAME_ENGINE->DrawBitmap(m_BmpActivePtr, m_Center - DOUBLE2(m_Width / 2, (m_Height / 2) + m_BmpOffsetY), { m_AnimFrame * width,0,(m_AnimFrame + 1) * width, height });
		return;
	}

	Block::Paint();
}

void BlockSingleCoin::Tick(double deltaTime, HUD* HUDPtr)
{
	if (m_AddScore != 0)
	{
		HUDPtr->AddCoin();
		m_SndCoinPtr->Play();
		HUDPtr->AddScore(m_AddScore, m_ActPtr->GetPosition() - DOUBLE2(m_Width/2, m_Height*2));
		m_AddScore = 0;
	}
	if (m_CoinActive)
	{
		m_CoinFrame++;
		if (m_CoinFrame == 30)
		{
			m_CoinFrame = 0;
			m_CoinActive = false;
		}
	}

	m_AnimTime += deltaTime;
	if (m_AnimTime >= 1.0 / m_NrFramesPerSec)
	{
		++m_AnimFrame;
		if (m_AnimFrame >= m_NrOfFrames) m_AnimFrame = 0;
		m_AnimTime -= m_AnimTime / m_NrFramesPerSec;
	}
	Block::Tick(deltaTime, HUDPtr);
}

void BlockSingleCoin::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	bool previousState = m_Ishit;
	Block::PreSolve(actThisPtr, actOtherPtr, enableContactRef);
	if (previousState == false)
	{
		if (m_Ishit)
		{
			if (!m_IsEmpty)
			{
				m_AddScore = m_ScoreValue;
				m_CoinActive = true;
			}
			m_NrOfCoins--;
			if(m_NrOfCoins == 0) m_IsEmpty = true;
		}
	}
}