//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "EnemyManager.h"
#include "Goomba.h"
#include "Koopa.h"
#include "EnemyMushroom.h"
#include "FlipDirectionSensor.h"
#include "EnemyShell.h"
#include "EnemyStar.h"
#include "HUD.h"
//#define GAME_ENGINE (GameEngine::GetSingleton())

EnemyManager::EnemyManager()
{
	m_ItemPtrArr = std::vector<Enemy *>(0);
	/*m_ItemPtrArr.push_back(nullptr);
	m_ItemPtrArr.push_back(nullptr);
	m_ItemPtrArr.push_back(nullptr);*/

	std::ifstream ifileStream;
	ifileStream.open("Resources/Enemies.txt");
	if (ifileStream.fail() == true)
	{
		std::cout << "could not open the file Resources/Enemys.txt";
		return;
	}

	std::string readLine;
	std::string saveEnemy;
	while (ifileStream.eof() == false)
	{
		std::getline(ifileStream, readLine);
		saveEnemy += readLine;
		if (readLine.size() >= 2)
		{
			if (readLine.substr(int(readLine.length()) - 2, 2) == std::string("/>"))
			{
				CreateEnemy(saveEnemy);
				saveEnemy = std::string();
			}
		}
	}
}

EnemyManager::~EnemyManager()
{
	for (Enemy * ItemPtr : m_ItemPtrArr)
	{
		if (ItemPtr != nullptr)
		{
			delete ItemPtr;
			ItemPtr = nullptr;
		}
	}

	for (FlipDirectionSensor * sensorPtr : m_FlipSensorPtrArr)
	{
		if (sensorPtr != nullptr)
		{
			delete sensorPtr;
			sensorPtr = nullptr;
		}
	}

	//m_ItemPtrArr.clear();
}

Enemy * EnemyManager::Add(const DOUBLE2 Center, const double width, const double height, const double velocity, EnemyType type)
{
 	bool isCreated = false;
	int insertedIdx = 0;
	int test = m_ItemPtrArr.size();
	for (size_t i = 0; i < m_ItemPtrArr.size(); i++)
	{
		if (m_ItemPtrArr[i] == nullptr)
		{
			m_ItemPtrArr[i] = Create(Center, width, height, velocity, type);
			isCreated = true;
			insertedIdx = i;
			break;
		}
	}
	if (!isCreated)
	{
		m_ItemPtrArr.push_back(Create(Center, width, height, velocity, type));
		insertedIdx = m_ItemPtrArr.size() - 1;
	}
	return m_ItemPtrArr[insertedIdx];
}

Enemy * EnemyManager::Create(const DOUBLE2 Center, const double width, const double height, const double velocity, EnemyType type)
{
	if (type == EnemyType::GOOMBA)
	{
		return new Goomba(Center, width, height, velocity);
	}

	if (type == EnemyType::MUSHROOM)
	{
		return new EnemyMushroom(Center, width, height, velocity);
	}

	if (type == EnemyType::KOOPA)
	{
		return new Koopa(Center, width, height, velocity);
	}

	if (type == EnemyType::SHELL)
	{
		return new EnemyShell(Center, width, height, velocity);
	}

	if (type == EnemyType::STAR)
	{
		return new EnemyStar(Center, width, height, velocity);
	}
	return nullptr;
}

bool EnemyManager::Remove(Enemy * itemPtr)
{
	for (size_t i = 0; i < m_ItemPtrArr.size(); i++)
	{
		if (m_ItemPtrArr[i] == itemPtr)
		{
			if (m_ItemPtrArr[i] != nullptr)
			{
				delete m_ItemPtrArr[i];
				m_ItemPtrArr[i] = nullptr;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;

}

void EnemyManager::Tick(double deltaTime, Level* levelPtr, HUD* HUDPtr)
{
	if (m_EnemyQueued)
	{
		Add(m_QueueCenter, m_QueueWidth, m_QueueHeight, m_QueueVelocity, m_QueueType);
		m_EnemyQueued = false;
	}
	for (Enemy * ItemPtr : m_ItemPtrArr)
	{
		if (ItemPtr != nullptr)
		{
			ItemPtr->Tick(deltaTime, this, levelPtr, HUDPtr);
			if (ItemPtr->IsDead())
			{
				Remove(ItemPtr);
				int test = 0;
			}
			int test = 0;
 		}
		int test = 0;
	}
	int test = 0;
}

void EnemyManager::Paint()
{
	for (Enemy * ItemPtr : m_ItemPtrArr)
	{
		if (ItemPtr != nullptr)
		{
			ItemPtr->Paint();
		}
	}
}

void EnemyManager::IsHit(PhysicsActor* actOtherPtr)
{
	for (Enemy * ItemPtr : m_ItemPtrArr)
	{
		if (ItemPtr != nullptr)
		{
			if (ItemPtr->IsHit(actOtherPtr))
			{
				ItemPtr->SetDead();
			}
		}
	}
}

/*bool IsOverlapping(PhysicsActor* actOtherPtr)
{

}*/

std::string EnemyManager::Getvalue(const std::string & nameRef, const std::string & enemyRef)
{
	std::string strSearch = nameRef + "=\"";
	std::size_t index = enemyRef.find(strSearch);
	if (index == -1) return std::string("not_found");
	std::string value = enemyRef.substr(index + strSearch.size(), enemyRef.size() - (index - 1));
	index = value.find("\"");
	value = value.substr(0, index);
	return value;
}

DOUBLE2 EnemyManager::StringToDOUBLE2(const std::string & DOUBLE2Ref)
{
	std::stringstream stringStream = std::stringstream(DOUBLE2Ref);
	std::string xString, yString;
	std::getline(stringStream, xString, ',');
	std::getline(stringStream, yString, ',');
	int xInt = stoi(xString);
	int yInt = stoi(yString);
	DOUBLE2 resultDOUBLE2 = DOUBLE2(xInt, yInt);
	return resultDOUBLE2;
}

void EnemyManager::CreateEnemy(const std::string & enemyRef)
{
	std::string enemyType = enemyRef.substr(1, enemyRef.size() - 1);
	std::size_t index = enemyType.find(' ');
	enemyType = enemyType.substr(0, index);
	if (enemyType == "Goomba") CreateGoomba(enemyRef);
	else if (enemyType == "FlipSensor") CreateFlipSensor(enemyRef);
	else if (enemyType == "Koopa") CreateKoopa(enemyRef);
}

void EnemyManager::CreateGoomba(const std::string & shapeRef)
{
	DOUBLE2 center = StringToDOUBLE2(Getvalue("Center", shapeRef));
	double width = std::stod(Getvalue("Width", shapeRef));
	double heigth = std::stod(Getvalue("Height", shapeRef));
	double velocity = std::stod(Getvalue("Velocity", shapeRef));
	m_ItemPtrArr.push_back(new Goomba(center, width, heigth, velocity));
}

void EnemyManager::CreateKoopa(const std::string & shapeRef)
{
	DOUBLE2 center = StringToDOUBLE2(Getvalue("Center", shapeRef));
	double width = std::stod(Getvalue("Width", shapeRef));
	double heigth = std::stod(Getvalue("Height", shapeRef));
	double velocity = std::stod(Getvalue("Velocity", shapeRef));
	m_ItemPtrArr.push_back(new Koopa(center, width, heigth, velocity));
}

void EnemyManager::CreateFlipSensor(const std::string & shapeRef)
{
	DOUBLE2 center = StringToDOUBLE2(Getvalue("Center", shapeRef));
	m_FlipSensorPtrArr.push_back(new FlipDirectionSensor(center));
}

void EnemyManager::QueueEnemy(const DOUBLE2 Center, const double width, const double height, const double velocity, EnemyType type)
{
	m_QueueCenter = Center;
	m_QueueWidth = width;
	m_QueueHeight = height;
	m_QueueVelocity = velocity;
	m_QueueType = type;
	m_EnemyQueued = true;
}