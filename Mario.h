#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
class Level;
class EnemyManager;
class HUD;
class Portal;
class EndGameFlag;
#include "Enums.h"

class Mario
{
public:
	Mario(const DOUBLE2 & centerRef);
	virtual ~Mario();
	Mario(const Mario&) = delete;
	Mario& operator=(const Mario&) = delete;

	void Tick(double deltaTime, Level * levelPtr, EnemyManager* enemyManagerPtr, HUD* HUDPtr);
	void HandleControls(Level * levelPtr, EnemyManager* enemyManagerPtr);
	void CalculateAnimations(double deltaTime);
	void Paint(Level * levelPtr);
	void Died(HUD* HUDPtr);
	DOUBLE2 getPos();
	void SetFlashing(bool flag);
	bool GetFlashing();
	PhysicsActor* GetMario();
	void SetSensor(bool flag);
	void Teleport(DOUBLE2 position, DOUBLE2 velocity);
	void Bounce();
	void SetEndReached(bool flag);
	void SetSpecialBigMario(bool flag);

private:
	enum class ActionState
	{
		JUMPING,
		WAITING,
		MOVING,
		LEFTOFFLAG,
		RIGHTOFFLAG,
		DIEING,
	};

	void DrawBitmap(Bitmap * bitmapPtr);

	PhysicsActor* m_ActPtr = nullptr;
	PhysicsActor* m_ActFeetPtr = nullptr;
	const int m_HeightActor = 17;
	const int m_WidthActor = 13;
	const int m_BitmapOffset = 1;
	int m_BitmapHeightOffset = 0;
	double m_HorSpeed = 0;
	const double m_Velocity = 100;
	const double m_JumpSpeed = 400;
	ActionState m_ActionState = ActionState::WAITING;
	Bitmap * m_BmpSpriteSheetPtr = nullptr;
	Bitmap * m_BmpMarioSpriteSheetPtr = nullptr;
	Bitmap * m_BmpBigMarioSpriteSheetPtr = nullptr;
	const int m_ClipHeightSmall = 20;
	const int m_ClipHeightBig = 36;
	int m_ClipHeight = 20;
	const int m_ClipWidth = 20;
	const int m_NrOfFrames = 3;
	const int m_NrFramesPerSec = 4;
	double m_AnimTime = 0.0;
	int m_AnimFrame = 0;
	FmodSound* m_SndJumpPtr = nullptr;
	std::vector<DOUBLE2> m_ActVector;
	ActorType m_LastState;
	bool m_Flickering = false;
	double m_FlickeringTime = 0;
	int m_FlickeringValue = 0;
	const int m_FlickeringDuration = 3;
	bool m_IsDieing = false;
	double m_deadtimer = 0;
	double m_MouseAngle = 0;
	bool m_Facingleft = false;
	int m_BitmapRow = 0;
	bool m_Flashing = false;
	double m_FlashingDuration = 25;
	double m_FlashingTime = 0;
	int m_FlashingValue = 0;
	bool m_ShouldPaint = true;
	bool m_ShouldPaintFlash = false;
	Bitmap * m_BmpFlashSpriteSheetPtr = nullptr;
	Bitmap * m_BmpMarioFlashSpriteSheetPtr = nullptr;
	Bitmap * m_BmpBigMarioFlashSpriteSheetPtr = nullptr;
	Portal* m_PortalPtr = nullptr;
	bool m_IsSensor = false;
	DOUBLE2 m_TeleportPosition;
	bool m_ShouldTeleport = false;
	DOUBLE2 m_TeleportVelocity;
	double m_VelocityDecay = 1.5;
	double m_VelocityDecayMultiplier = 2;
	bool m_ShouldBounce = false;
	FmodSound* m_SndTeleportPtr{};
	FmodSound* m_SndDeathPtr{};
	Bitmap* m_BmpFlagPtr{};
	DOUBLE2 m_FlagPos{ 3160,42 };
	double m_FlagVelocity = 150;
	double m_FlagMaxHeigth = 166;
	DOUBLE2 m_FlagHoldLeftPosition{ 3168,176 };
	DOUBLE2 m_FlagHoldRightPosition{ 3184,176 };
	double m_FlagHoldRightTimer = 0;
	DOUBLE2 m_EndDoorCenter{ 3272,192 };
	bool m_EndReached = false;
	EndGameFlag* m_EndGameFlagPtr{};
	FmodSound* m_SndBackgroundPtr{};
	FmodSound* m_SndEndPtr{};
	FmodSound* m_SndFlashingPtr{};
	bool m_SpecialBigMario = false;
	Bitmap * m_BmpSpecialBigMarioSpriteSheetPtr = nullptr;
	double m_FireBallWidth = 8;
	double m_FireballHeigth = 8;
	double m_fireballVelocity = 200;
	FmodSound* m_SndFireBall{};
	double m_ExplosionCooldown = 0.5;
	double m_LastExplosionTimer = 0;
	DOUBLE2 m_ExplosionCenter = { 3270,90 };
	int m_ExplosionOffset = 100;
	Bitmap* m_BmpEndFlagPtr{};
	DOUBLE2 m_EndFlagPosition = { 3264, 112 };
	bool m_ShouldDrawEndFlag = false;
	FmodSound* m_SndBackGroundFastPtr{};
	double m_SndBackGroundFastLength = 25.5;
};