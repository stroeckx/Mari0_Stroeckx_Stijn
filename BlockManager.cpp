//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "BlockManager.h"
#include "BlockWall.h"
#include "BlockMushroom.h"
#include "BlockSingleCoin.h"
#include "BlockStar.h"
//#define GAME_ENGINE (GameEngine::GetSingleton())

BlockManager::BlockManager(EnemyManager* enemyManager)
{
	m_EnemyManager = enemyManager;

	m_ItemPtrArr = std::vector<Block *>(0);
	m_ItemPtrArr.push_back(nullptr);
	m_ItemPtrArr.push_back(nullptr);

	std::ifstream ifileStream;
	ifileStream.open("Resources/Blocks.txt");
	if (ifileStream.fail() == true)
	{
		std::cout << "could not open the file Resources/Blocks.txt";
		return;
	}

	std::string readLine;
	std::string saveBlock;
	while (ifileStream.eof() == false)
	{
		std::getline(ifileStream, readLine);
		saveBlock += readLine;
		if (readLine.size() >= 2)
		{
			if (readLine.substr(int(readLine.length()) - 2, 2) == std::string("/>"))
			{
				CreateBlock(saveBlock);
				saveBlock = std::string();
			}
		}
	}
}

BlockManager::~BlockManager()
{
	for (Block * ItemPtr : m_ItemPtrArr)
	{
		if (ItemPtr != nullptr)
		{
			delete ItemPtr;
			ItemPtr = nullptr;
		}
	}
}

void BlockManager::Tick(double deltaTime, EnemyManager* enemyManager, HUD* HUDPtr)
{
	for (Block * ItemPtr : m_ItemPtrArr)
	{
		if (ItemPtr != nullptr)
		{
			EnemyType type = ItemPtr->SpawnEnemy();
			if (type != EnemyType::EMPTY)
			{
				enemyManager->Add(ItemPtr->GetPosition() + DOUBLE2(0, -ItemPtr->GetHeight() + 1), ItemPtr->GetWidth(), ItemPtr->GetHeight(), 50, type);
			}
			ItemPtr->Tick(deltaTime, HUDPtr);
			if (ItemPtr->IsDestroyed())
			{
				Remove(ItemPtr);
			}
		}
	}
}

void BlockManager::Paint()
{
	for (Block * ItemPtr : m_ItemPtrArr)
	{
		if (ItemPtr != nullptr)
		{
			ItemPtr->Paint();
		}
	}
}

bool BlockManager::IsOverlapping(PhysicsActor* actOtherPtr)
{
	for (Block * ItemPtr : m_ItemPtrArr)
	{
		if (ItemPtr != nullptr)
		{
			if (ItemPtr->IsOverlapping(actOtherPtr))
			{
				return true;
			}
		}
	}
	return false;
}

void BlockManager::SpawnEnemy()
{
	for (Block * ItemPtr : m_ItemPtrArr)
	{
		if (ItemPtr != nullptr)
		{
			if (ItemPtr->SpawnEnemy() == EnemyType::MUSHROOM)
			{
				m_EnemyManager->Add(ItemPtr->GetPosition() + DOUBLE2(0, -ItemPtr->GetHeight() + 1), ItemPtr->GetWidth(), ItemPtr->GetHeight(), 50, EnemyType::MUSHROOM);
			}
			else if (ItemPtr->SpawnEnemy() == EnemyType::STAR)
			{
				m_EnemyManager->Add(ItemPtr->GetPosition() + DOUBLE2(0, -ItemPtr->GetHeight() + 1), ItemPtr->GetWidth(), ItemPtr->GetHeight(), 50, EnemyType::STAR);
			}
		}
	}
}

bool BlockManager::Remove(Block * itemPtr)
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

std::string BlockManager::Getvalue(const std::string & nameRef, const std::string & blockRef)
{
	std::string strSearch = nameRef + "=\"";
	std::size_t index = blockRef.find(strSearch);
	if (index == -1) return std::string("not_found");
	std::string value = blockRef.substr(index + strSearch.size(), blockRef.size() - (index - 1));
	index = value.find("\"");
	value = value.substr(0, index);
	return value;
}

void BlockManager::CreateBlock(const std::string & blockRef)
{
	std::string blockType = blockRef.substr(1, blockRef.size() - 1);
	std::size_t index = blockType.find(' ');
	blockType = blockType.substr(0, index);
	if (blockType == "Wall") CreateWall(blockRef);
	else if (blockType == "SingleCoin") CreateSingleCoin(blockRef);
	else if (blockType == "Mushroom") CreateMushroom(blockRef);
	else if (blockType == "Star") CreateStar(blockRef);
	else if (blockType == "InfiniteCoin") CreateInfiniteCoin(blockRef);
}

void BlockManager::CreateWall(const std::string & shapeRef)
{
	DOUBLE2 center = StringToDOUBLE2(Getvalue("Center", shapeRef));
	double width = std::stod(Getvalue("Width", shapeRef));
	double heigth = std::stod(Getvalue("Height", shapeRef));
	m_ItemPtrArr.push_back(new BlockWall(center, width, heigth, m_EnemyManager));
}

void BlockManager::CreateSingleCoin(const std::string & shapeRef)
{
	DOUBLE2 center = StringToDOUBLE2(Getvalue("Center", shapeRef));
	double width = std::stod(Getvalue("Width", shapeRef));
	double heigth = std::stod(Getvalue("Height", shapeRef));
	m_ItemPtrArr.push_back(new BlockSingleCoin(center, width, heigth, m_EnemyManager));
}

void BlockManager::CreateMushroom(const std::string & shapeRef)
{
	DOUBLE2 center = StringToDOUBLE2(Getvalue("Center", shapeRef));
	double width = std::stod(Getvalue("Width", shapeRef));
	double heigth = std::stod(Getvalue("Height", shapeRef));
	m_ItemPtrArr.push_back(new BlockMushroom(center, width, heigth, m_EnemyManager));
}

void BlockManager::CreateStar(const std::string & shapeRef)
{
	DOUBLE2 center = StringToDOUBLE2(Getvalue("Center", shapeRef));
	double width = std::stod(Getvalue("Width", shapeRef));
	double heigth = std::stod(Getvalue("Height", shapeRef));
	m_ItemPtrArr.push_back(new BlockStar(center, width, heigth, m_EnemyManager));
}

void BlockManager::CreateInfiniteCoin(const std::string & shapeRef)
{
	DOUBLE2 center = StringToDOUBLE2(Getvalue("Center", shapeRef));
	double width = std::stod(Getvalue("Width", shapeRef));
	double heigth = std::stod(Getvalue("Height", shapeRef));
	m_ItemPtrArr.push_back(new BlockSingleCoin(center, width, heigth, m_EnemyManager, 10));
}

DOUBLE2 BlockManager::StringToDOUBLE2(const std::string & DOUBLE2Ref)
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

bool BlockManager::Raycast(DOUBLE2 point1, DOUBLE2 point2, DOUBLE2 &intersectionRef, DOUBLE2 &normalRef, double &fractionRef)
{
	DOUBLE2 intersectReturn = intersectionRef, normalReturn = normalRef;
	double fractionReturn = fractionRef;
	bool returnFlag = false;

	for (Block * ItemPtr : m_ItemPtrArr)
	{
		if (ItemPtr != nullptr)
		{
			if(ItemPtr->Raycast(point1, point2, intersectionRef, normalRef, fractionRef))
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
		}
	}
	return returnFlag;
}