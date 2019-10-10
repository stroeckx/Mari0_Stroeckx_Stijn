#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "Block.h"
#include "EnemyManager.h"
class BlockManager 
{
public:
	BlockManager(EnemyManager* enemyManager );
	virtual ~BlockManager( );
	BlockManager( const BlockManager& ) = delete;
	BlockManager& operator=( const BlockManager& ) = delete;
	void Tick(double deltaTime, EnemyManager* enemyManager, HUD* HUDPtr);
	void Paint();
	bool IsOverlapping(PhysicsActor* actOtherPtr);
	void SpawnEnemy();
	bool Remove(Block * itemPtr);
	std::string Getvalue(const std::string & nameRef, const std::string & blockRef);
	void CreateBlock(const std::string & shapeRef);
	void CreateWall(const std::string & shapeRef);
	void CreateSingleCoin(const std::string & shapeRef);
	void CreateMushroom(const std::string & shapeRef);
	void CreateStar(const std::string & shapeRef);
	void CreateInfiniteCoin(const std::string & shapeRef);
	DOUBLE2 StringToDOUBLE2(const std::string & DOUBLE2Ref);
	bool Raycast(DOUBLE2 point1, DOUBLE2 point2, DOUBLE2 &intersectionRef, DOUBLE2 &normalRef, double &fractionRef);

private: 
	std::vector<Block *> m_ItemPtrArr;
	EnemyManager* m_EnemyManager;
};
