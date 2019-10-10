#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "ContactListener.h"
#include "Enums.h"
class EnemyManager;
class Level;
class HUD;

class Enemy : public ContactListener
{
public:
	Enemy( const DOUBLE2 Center, const double width, const double height, const double velocity, const String& bitmap );
	virtual ~Enemy( );
	Enemy( const Enemy& ) = delete;
	Enemy& operator=( const Enemy& ) = delete;

	virtual void Paint();
	virtual void Paint(RECT2 rect);
	virtual void Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr);
	virtual bool IsHit(PhysicsActor* actOtherPtr);
	virtual void SetDead();
	virtual bool IsDead();
	virtual void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);
	virtual void SetSpecialDead();
	bool IsOverlapping(PhysicsActor* actOtherPtr);
	void SetVelocity(double velocity);
	void SetFlipDirection(bool flag);
	bool GetFlipDirection();
	void IncreaseEndContactcount();
	void SetEndContactCount(int value);
	int GetEndContactcount();
	bool GetReadyForTeleport();
	void SetReadyForTeleport(bool flag);
	void Teleport(DOUBLE2 position, DOUBLE2 velocity);
	DOUBLE2 GetPosition();
	PhysicsActor* GetActor();

protected:
	double m_Velocity = 0;
	DOUBLE2 m_Center;
	double m_Width = 0;
	double m_Height = 0;
	Bitmap* m_BmpPtr = nullptr;
	PhysicsActor* m_ActPtr = nullptr;
	bool m_IsDieing = false;
	double m_TimeDead = 0;
	bool m_IsDead = false;
	const int m_BitmapOffsetY = 2;
	bool m_FlipDirection = false;
	bool m_IsSpecialDead = false;
	int m_ScoreValue = 100;
	int m_AddScore = 0;
	FmodSound* m_SndSpecialDeadPtr{};
	virtual void FlipDirection();


private: 
	DOUBLE2 m_TeleportPosition;
	bool m_ShouldTeleport = false;
	DOUBLE2 m_TeleportVelocity;
	int m_EndContactcount = 0;
	bool m_ReadyForNextTeleport = true;
	FmodSound* m_SndDeadPtr{};

};
