#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
class Platform;
class ExitSign;
class BlockSingleCoin;
class BlockWall;
class BlockMushroom;
class EnemyManager;
class BlockManager;
#include "ContactListener.h"
class HUD;

class Level : public ContactListener
{
public:
	Level(EnemyManager* enemyManager);
	virtual ~Level( );
	Level( const Level& ) = delete;
	Level& operator=( const Level& ) = delete;

	void Tick(double deltaTime, EnemyManager* enemyManager, HUD* HUDPtr);
	void PaintBackground();
	void PaintForeground();
	double GetWidth();
	double GetHeight();
	bool IsOnGround(PhysicsActor* actOtherPtr);
	bool EndReached();
	virtual void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);
	bool Raycast(DOUBLE2 point1, DOUBLE2 point2, DOUBLE2 &intersectionRef, DOUBLE2 &normalRef, double &fractionRef);

private: 
	Bitmap * m_BmpBackgroundPtr;
	Bitmap * m_BmpFencePtr;
	DOUBLE2 m_FencePos = { 200.0, 240.0 };
	PhysicsActor * m_ActMainPtr;
	PhysicsActor * m_ActLeftBorderPtr;
	PhysicsActor * m_ActRightBorderPtr;
	BlockManager* m_BlockManagerPtr = nullptr;
};

