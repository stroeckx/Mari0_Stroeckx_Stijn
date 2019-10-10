//-----------------------------------------------------------------
// Game Engine Object
// C++ Source - Mari0_Stroeckx_Stijn.cpp - version v2_18a March 2016  
// Copyright Kevin Hoefman, 2006 - 2011 
// Copyright DAE Programming Team, 2012 - 2016 
// http://www.digitalartsandentertainment.be/ 
//-----------------------------------------------------------------

// this include must be the first include line of every cpp file (due to using precompiled header)
#include "stdafx.h"		

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Mari0_Stroeckx_Stijn.h"																				
#include "Level.h"
#include "Mario.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "HUD.h"
#include "ParseSession.h"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//-----------------------------------------------------------------
// Mari0_Stroeckx_Stijn methods																				
//-----------------------------------------------------------------

Mari0_Stroeckx_Stijn::Mari0_Stroeckx_Stijn()
{
	// nothing to create
}

Mari0_Stroeckx_Stijn::~Mari0_Stroeckx_Stijn()
{
	// nothing to destroy
}

void Mari0_Stroeckx_Stijn::GameInitialize(GameSettings &gameSettingsRef)
{
	gameSettingsRef.SetWindowTitle(String("Mari0_Stroeckx_Stijn - Stroeckx Stijn - 1DAE06"));
	gameSettingsRef.SetWindowWidth(800);
	gameSettingsRef.SetWindowHeight(450);
	gameSettingsRef.EnableConsole(true);
	gameSettingsRef.EnableAntiAliasing(false);
}

void Mari0_Stroeckx_Stijn::GameStart()
{
	//GAME_ENGINE->SetBitmapInterpolationModeNearestNeighbor();
	m_LevelPtr = new Level(m_EnemyManagerPtr);
	m_MarioPtr = new Mario({ m_MarioSpawnPos });
	m_CameraPtr = new Camera(GAME_ENGINE->GetWidth(), GAME_ENGINE->GetHeight(), m_MarioPtr, m_LevelPtr);
	m_EnemyManagerPtr = new EnemyManager();
	m_HUDPtr = new HUD();
	m_SessionPtr = new ParseSession();
	m_SessionPtr->SaveSessionInfoStart(m_HUDPtr);
	//m_SessionPtr->GetLastSessionData();
}

void Mari0_Stroeckx_Stijn::GameEnd()
{
	m_SessionPtr->SaveSessionInfoEnd(m_HUDPtr);
	delete m_LevelPtr;
	m_LevelPtr = nullptr;
	delete m_MarioPtr;
	m_MarioPtr = nullptr;
	delete m_CameraPtr;
	m_CameraPtr = nullptr;
	delete m_EnemyManagerPtr;
	m_EnemyManagerPtr = nullptr;
	delete m_HUDPtr;
	m_HUDPtr = nullptr;
	delete m_SessionPtr;
	m_SessionPtr = nullptr;
}

void Mari0_Stroeckx_Stijn::GameTick(double deltaTime)
{
	if (GAME_ENGINE->IsKeyboardKeyPressed('R'))
	{
		delete m_EnemyManagerPtr;
		m_EnemyManagerPtr = nullptr;
		m_EnemyManagerPtr = new EnemyManager();
	}
	//m_ElapsedTime += deltaTime;
	//m_Frames++;
	TogglePhysicsRendering();
	m_LevelPtr->Tick(deltaTime, m_EnemyManagerPtr, m_HUDPtr);
	m_EnemyManagerPtr->Tick(deltaTime, m_LevelPtr, m_HUDPtr, m_MarioPtr);
	m_MarioPtr->Tick(deltaTime, m_LevelPtr, m_EnemyManagerPtr, m_HUDPtr);
	m_HUDPtr->Tick(deltaTime);

}

void Mari0_Stroeckx_Stijn::GamePaint()
{
	MATRIX3X2 matIdentity, matView = m_CameraPtr->getViewMatrix(m_LevelPtr, m_MarioPtr);
	matIdentity.SetAsIdentity();
	GAME_ENGINE->SetViewMatrix(matView);

	m_LevelPtr->PaintBackground();
	m_MarioPtr->Paint(m_LevelPtr);
	m_EnemyManagerPtr->Paint();
	m_HUDPtr->Paint(m_SessionPtr);
	//m_CameraPtr->Paint(m_LevelPtr, m_MarioPtr);
	//GAME_ENGINE->DrawString(String(m_Frames / m_ElapsedTime), 150, 150);
}

void Mari0_Stroeckx_Stijn::TogglePhysicsRendering()
{
	if (GAME_ENGINE->IsKeyboardKeyPressed('P'))
	{
		m_IsPhysicsRenderingOn = !m_IsPhysicsRenderingOn;
		GAME_ENGINE->EnablePhysicsDebugRendering(m_IsPhysicsRenderingOn);
	}
}