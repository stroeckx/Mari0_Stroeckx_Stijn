#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Enemy.h"
#include "Enums.h"
class Level;
class FlipDirectionSensor;
class HUD;
class Mario;

class EnemyManager 
{
public:
	EnemyManager( );
	virtual ~EnemyManager( );
	EnemyManager( const EnemyManager& ) = delete;
	EnemyManager& operator=( const EnemyManager& ) = delete;

	Enemy * Add(const DOUBLE2 Center, const double width, const double height, const double velocity, EnemyType type);
	bool Remove(Enemy * itemPtr);
	void Tick(double deltaTime, Level* levelPtr, HUD* HUDPtr, Mario* marioPtr);
	void Paint();
	void IsHit(PhysicsActor* actOtherPtr);
	std::string Getvalue(const std::string & nameRef, const std::string & blockRef);
	DOUBLE2 StringToDOUBLE2(const std::string & DOUBLE2Ref);
	void CreateEnemy(const std::string & shapeRef);
	void CreateGoomba(const std::string & shapeRef);
	void CreateKoopa(const std::string & shapeRef);
	void CreateFlipSensor(const std::string & shapeRef);
	void QueueEnemy(const DOUBLE2 Center, const double width, const double height, const double velocity, EnemyType type);



private: 
	std::vector<Enemy *> m_ItemPtrArr;
	Enemy * Create(const DOUBLE2 Center, const double width, const double height, const double velocity, EnemyType type);
	std::vector<FlipDirectionSensor*> m_FlipSensorPtrArr;

	bool m_EnemyQueued = false;
	DOUBLE2 m_QueueCenter;
	double m_QueueWidth = 0;
	double m_QueueHeight = 0;
	double m_QueueVelocity = 0;
	EnemyType m_QueueType;
	int m_DistanceToActivate = 300;


};
