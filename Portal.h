#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
class Mario;
class Level;
class PortalParticleManager;
class PortalGunParticleManager;
#include "ContactListener.h"

class Portal : public ContactListener
{
public:
	Portal( );
	virtual ~Portal( );
	Portal( const Portal& ) = delete;
	Portal& operator=( const Portal& ) = delete;
	
	void Tick(const double deltaTime, Mario* marioPtr, Level* levelPtr);
	void Paint(Mario* marioPtr);
	void DrawDottedLine(Mario* marioPtr, double length);
	bool CanSpawnPortal(const DOUBLE2 point, Level* levelPtr);
	void DrawCrosshair(Bitmap* bitmap);
	void DrawPortal(PhysicsActor* actPortalPtr, const DOUBLE2 normal, Bitmap* bitmap, Bitmap* animationBitmap);
	bool CanSpawnPortal(Level* levelPtr);
	bool IsOverlapping(PhysicsActor* actOtherPtr);
	void CalculateVelocity(DOUBLE2 &velocity, const DOUBLE2 &normalStart, const DOUBLE2 &normalEnd);
	void BeginContact(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr);
	void EndContact(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr);
	void MakeSensors();

private: 
	const double m_Length = 32;
	PhysicsActor* m_ActBluePortal = nullptr;
	bool m_IsBlueActive = false;
	PhysicsActor* m_ActOrangePortal = nullptr;
	bool m_IsOrangeActive = false;
	bool m_HasIntersection = false;
	const double m_MaxLength = 350;
	DOUBLE2 m_LastIntersection = { -500,-500 };
	DOUBLE2 m_LastNormal = { 1,0 };
	Bitmap* m_BmpCrosshairRed{};
	Bitmap* m_BmpCrosshairGreen{};
	Bitmap* m_BmpGlowPtr{};
	DOUBLE2 m_NormalBluePortal = { 1,0 };
	DOUBLE2 m_NormalOrangePortal = { 1,0 };
	double m_DottedLineOffset = 0;
	double m_DottedLineOffsetIncrements = 0.5;
	double m_SpaceBetweenDots = 20;
	std::vector<PhysicsActor*> m_ActCanSpawnArrPtr;
	bool m_CanSpawn = false;
	bool m_ReadyForNextTeleport = true;
	int m_EndContactcount = 0;
	double m_DottedLineRadius = 0.6;
	Bitmap* m_BmpBluePortalAnimation{};
	Bitmap* m_BmpOrangePortalAnimation{};
	const int m_NrOfFrames = 5;
	const int m_NrFramesPerSec = 3;
	double m_AnimTime = 0.0;
	int m_AnimFrame = 0;
	const int m_AnimationHeight = 5;
	PortalParticleManager* m_PortalParticleBluePtr{};
	PortalParticleManager* m_PortalParticleOrangePtr{};
	COLOR m_ClrBlue = { 0,141,255 };
	COLOR m_ClrOrange = { 255,114,0 };
	PortalGunParticleManager* m_PortalGunParticleManagerPtr{};
	FmodSound* m_SndBluePortalPtr{};
	FmodSound* m_SndOrangePortalPtr{};
};
