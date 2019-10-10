//-----------------------------------------------------------------
// Game Engine Object
// C++ Header - Mari0_Stroeckx_Stijn.h - version v2_18a March 2016 
// Copyright Kevin Hoefman, 2006 - 2011 
// Copyright DAE Programming Team, 2012 - 2016 
// http://www.digitalartsandentertainment.be/ 
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include "Resource.h"	
#include "AbstractGame.h"

class Level;
class Mario;
class Camera;
class EnemyManager;
class HUD;
class ParseSession;

//-----------------------------------------------------------------
// Mari0_Stroeckx_Stijn Class																
//-----------------------------------------------------------------
class Mari0_Stroeckx_Stijn : public AbstractGame
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	Mari0_Stroeckx_Stijn();

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Mari0_Stroeckx_Stijn();

	// C++11 make the class non-copyable
	Mari0_Stroeckx_Stijn(const Mari0_Stroeckx_Stijn&) = delete;
	Mari0_Stroeckx_Stijn& operator=(const Mari0_Stroeckx_Stijn&) = delete;

	//---------------------------
	// General Methods
	//---------------------------

	virtual void GameInitialize(GameSettings &gameSettingsRef);
	virtual void GameStart();
	virtual void GameEnd();
	virtual void GameTick(double deltaTime);
	virtual void GamePaint();

	// -------------------------
	// Public Member functions
	// -------------------------

private:
	// -------------------------
	// Private Member functions
	// -------------------------
	void TogglePhysicsRendering();

	// -------------------------
	// Private Datamembers
	// -------------------------
	Level* m_LevelPtr = nullptr;
	Mario* m_MarioPtr = nullptr;
	bool m_IsPhysicsRenderingOn = false;
	Camera* m_CameraPtr = nullptr;

	double m_ElapsedTime;
	double m_Frames;
	EnemyManager* m_EnemyManagerPtr = nullptr;
	HUD* m_HUDPtr = nullptr;
	ParseSession* m_SessionPtr = nullptr;
	DOUBLE2 m_MarioSpawnPos{ 50,50 };
};
