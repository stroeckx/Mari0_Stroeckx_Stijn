#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "ContactListener.h"
#include "Enums.h"
#include "EnemyManager.h"
class HUD;

enum class BlockType
{
	WALL,
	SINGLECOIN,
	MULTICOIN,
	STAR,
};

class Block : public ContactListener
{
public:
	Block( const DOUBLE2 Center, const double width, const double height, const String& bitmap, EnemyManager* enemyManager);
	virtual ~Block( );
	Block( const Block& ) = delete;
	Block& operator=( const Block& ) = delete;

	virtual void Paint();
	virtual void Tick(double deltaTime, HUD* HUDPtr);
	virtual bool IsHit();
	bool IsOverlapping(PhysicsActor* actOtherPtr);
	virtual void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);
	virtual EnemyType SpawnEnemy();
	virtual DOUBLE2 GetPosition();
	virtual double GetWidth();
	virtual double GetHeight();
	virtual void Break();
	virtual bool IsDestroyed();
	virtual bool Raycast(DOUBLE2 point1, DOUBLE2 point2, DOUBLE2 &intersectionRef, DOUBLE2 &normalRef, double &fractionRef);

protected:
	DOUBLE2 m_Center;
	double m_Width = 0;
	double m_Height = 0;
	Bitmap* m_BmpPtr = nullptr;
	PhysicsActor* m_ActPtr = nullptr;
	bool m_Ishit = false;
	bool m_AnimUpwards = false;
	int m_BmpOffsetY = 0;
	EnemyManager* m_EnemyManager;
	bool m_SpawnEnemy = false;
	bool m_Broken = false;
	bool m_IsDestroyed = false;
private:
	FmodSound * m_SndHitPtr{};
};
