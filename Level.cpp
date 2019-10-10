//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Level.h"
#include "BlockSingleCoin.h"
#include "BlockWall.h"
#include "BlockMushroom.h"
#include "EnemyManager.h"
#include "ContactListener.h"
#include "BlockManager.h"
#include "HUD.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Level::Level(EnemyManager* enemyManager)
{
	m_BmpBackgroundPtr = new Bitmap(String("Resources/Level.png"));
	m_ActMainPtr = new PhysicsActor(0, 0, 0, BodyType::STATIC);
	m_ActMainPtr->AddSVGFixture(String("Resources/Level_12.svg"), false, 0);
	m_ActMainPtr->SetUserData(int(ActorType::TERRAIN));
	m_ActLeftBorderPtr = new PhysicsActor(-2, 0, 0, BodyType::STATIC);
	m_ActLeftBorderPtr->AddBoxFixture(1, 1000, 0, 0);
	m_ActLeftBorderPtr->AddContactListener(this);
	m_ActLeftBorderPtr->SetUserData(int(ActorType::TERRAIN));
	b2Filter collisionFilter;
	collisionFilter.categoryBits = ActorCollideType::BOUNDARY;
	collisionFilter.maskBits = ActorCollideType::MARIO;
	m_ActLeftBorderPtr->SetCollisionFilter(collisionFilter);
	m_ActRightBorderPtr = new PhysicsActor(m_BmpBackgroundPtr->GetWidth()+2, 0, 0, BodyType::STATIC);
	m_ActRightBorderPtr->AddBoxFixture(1, 1000, 0, 0);
	m_ActRightBorderPtr->SetUserData(int(ActorType::TERRAIN));
	m_ActRightBorderPtr->SetCollisionFilter(collisionFilter);
	/*m_TestBlockPtr = new BlockMushroom({ 216,200 }, 16, 16, enemyManager);
	m_TestBlock2Ptr = new BlockWall({ 200,200 }, 16, 16, enemyManager);
	m_TestBlock3Ptr = new BlockWall({ 232,200 }, 16, 16, enemyManager);
	m_TestBlock4Ptr = new BlockSingleCoin({ 248,200 }, 16, 16, enemyManager);*/
	m_BlockManagerPtr = new BlockManager(enemyManager);
}

Level::~Level()
{
	delete m_BmpBackgroundPtr;
	m_BmpBackgroundPtr = nullptr;
	delete m_ActMainPtr;
	m_ActMainPtr = nullptr;
	delete m_ActLeftBorderPtr;
	m_ActLeftBorderPtr = nullptr;
	delete m_ActRightBorderPtr;
	m_ActRightBorderPtr = nullptr;
	/*delete m_TestBlockPtr;
	m_TestBlockPtr = nullptr;
	delete m_TestBlock2Ptr;
	m_TestBlock2Ptr = nullptr;
	delete m_TestBlock3Ptr;
	m_TestBlock3Ptr = nullptr;
	delete m_TestBlock4Ptr;
	m_TestBlock4Ptr = nullptr;*/
	delete m_BlockManagerPtr;
	m_BlockManagerPtr = nullptr;
}

void Level::Tick(double deltaTime, EnemyManager* enemyManager, HUD* HUDPtr)
{

	/*m_TestBlockPtr->Tick(deltaTime);
	m_TestBlock2Ptr->Tick(deltaTime);
	m_TestBlock3Ptr->Tick(deltaTime);
	m_TestBlock4Ptr->Tick(deltaTime);*/
	m_BlockManagerPtr->Tick(deltaTime, enemyManager, HUDPtr);
}

void Level::PaintBackground()
{
	GAME_ENGINE->DrawBitmap(m_BmpBackgroundPtr);
	/*m_TestBlockPtr->Paint();
	m_TestBlock2Ptr->Paint();
	m_TestBlock3Ptr->Paint();
	m_TestBlock4Ptr->Paint();*/
	m_BlockManagerPtr->Paint();
}

void Level::PaintForeground()
{
}

bool Level::IsOnGround(PhysicsActor* actOtherPtr)
{
	if (m_ActMainPtr->IsOverlapping(actOtherPtr))
	{
		return true;
	}
	/*if (m_TestBlockPtr->IsOverlapping(actOtherPtr))
	{
		return true;
	}
	if (m_TestBlock2Ptr->IsOverlapping(actOtherPtr))
	{
		return true;
	}
	if (m_TestBlock3Ptr->IsOverlapping(actOtherPtr))
	{
		return true;
	}
	if (m_TestBlock4Ptr->IsOverlapping(actOtherPtr))
	{
		return true;
	}*/
	if (m_BlockManagerPtr->IsOverlapping(actOtherPtr))
	{
		return true;
	}
	return false;
}

double Level::GetWidth()
{
	return m_BmpBackgroundPtr->GetWidth();
}

double Level::GetHeight()
{
	return m_BmpBackgroundPtr->GetHeight();
}

bool Level::EndReached()
{
	return false;
}

void Level::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	if (actOtherPtr->GetUserData() == int(ActorType::MARIO) || actOtherPtr->GetUserData() == int(ActorType::BIGMARIO))
	{
		enableContactRef = true;
	}
	else enableContactRef = false;
}

bool Level::Raycast(DOUBLE2 point1, DOUBLE2 point2, DOUBLE2 &intersectionRef, DOUBLE2 &normalRef, double &fractionRef)
{
	bool returnFlag = false;
	
	if (m_ActMainPtr->Raycast(point1, point2, intersectionRef, normalRef, fractionRef))
	{
		returnFlag = true;
	}
	//first contact is stored in reference variables, after that they are stored locally to return the contact closest to point1
	DOUBLE2 intersectReturn = intersectionRef, normalReturn = normalRef;
	double fractionReturn = fractionRef;

	if (m_BlockManagerPtr->Raycast(point1, point2, intersectionRef, normalRef, fractionRef))
	{
		if (fractionRef > fractionReturn)
		{
			intersectionRef = intersectReturn;
			normalRef = normalReturn;
			fractionRef = fractionReturn;
		}
		else
		{
			intersectReturn = intersectionRef;
			normalReturn = normalRef;
			fractionReturn = fractionRef;
		}
		returnFlag = true;
	}
	return returnFlag;
}
