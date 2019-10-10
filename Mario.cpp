//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Mario.h"
#include "Level.h"
#include "EnemyManager.h"
#include "HUD.h"
#include "Portal.h"
#include "EndGameFlag.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Mario::Mario(const DOUBLE2 & centerRef)
{
	m_ActPtr = new PhysicsActor({ centerRef }, 0, BodyType::DYNAMIC);
	m_ActVector.push_back(DOUBLE2(-m_WidthActor / 2, -m_HeightActor / 2));
	m_ActVector.push_back(DOUBLE2(m_WidthActor / 2, -m_HeightActor / 2));
	m_ActVector.push_back(DOUBLE2(m_WidthActor / 2, (m_HeightActor / 2) - 1));
	m_ActVector.push_back(DOUBLE2((m_WidthActor / 2) - 0.9, m_HeightActor / 2));
	m_ActVector.push_back(DOUBLE2(-(m_WidthActor / 2) + 0.9, m_HeightActor / 2));
	m_ActVector.push_back(DOUBLE2(-m_WidthActor / 2, (m_HeightActor / 2) - 1));

	m_ActPtr->AddPolygonFixture(m_ActVector, 0, 0);
	m_ActPtr->SetFixedRotation(true);
	m_ActPtr->SetUserData(int(ActorType::MARIO));
	m_ActPtr->SetBullet(true);

	/*m_CollisionFilterDefault = m_ActPtr->GetCollisionFilter();
	m_CollisionFilterDefault.categoryBits = ActorCollideType::MARIO;
	m_CollisionFilterDefault.maskBits = ActorCollideType::TERRAIN | ActorCollideType::ENEMY | ActorCollideType::SHELL | ActorCollideType::BOUNDARY;
	m_CollisionFilterFlickering.categoryBits = ActorCollideType::MARIO;
	m_CollisionFilterFlickering.maskBits = ActorCollideType::TERRAIN | ActorCollideType::BOUNDARY;*/

	m_ActFeetPtr = new PhysicsActor(m_ActPtr->GetPosition() + DOUBLE2(0,(m_HeightActor / 2) - 1 ), 0, BodyType::DYNAMIC);
	m_ActFeetPtr->AddBoxFixture(m_WidthActor - 4, 2, 0, 0);
	m_ActFeetPtr->SetFixedRotation(true);
	m_ActFeetPtr->SetSensor(true);
	m_ActFeetPtr->SetGravityScale(0);
	m_ActFeetPtr->SetUserData(int(ActorType::FEET));

	m_BmpMarioSpriteSheetPtr = new Bitmap(String("Resources/marioanimations.png"));
	m_BmpBigMarioSpriteSheetPtr = new Bitmap(String("Resources/bigmarioanimations.png"));
	m_BmpSpriteSheetPtr = m_BmpMarioSpriteSheetPtr;
	m_SndJumpPtr = new FmodSound();
	m_SndJumpPtr->CreateSound(String("Resources/Sounds/Jump.ogg"));
	m_BmpMarioFlashSpriteSheetPtr = new Bitmap(String("Resources/marioanimations1.png"));
	m_BmpBigMarioFlashSpriteSheetPtr = new Bitmap(String("Resources/bigmarioanimations1.png"));
	m_PortalPtr = new Portal();
	m_SndTeleportPtr = new FmodSound();
	m_SndTeleportPtr->CreateSound(String("Resources/Sounds/portalenter.ogg"));
	m_SndDeathPtr = new FmodSound();
	m_SndDeathPtr->CreateSound(String("Resources/Sounds/death.ogg"));
	m_BmpFlagPtr = new Bitmap(String("Resources/flag.png"));
	m_EndGameFlagPtr = new EndGameFlag(m_FlagPos);
	m_SndBackgroundPtr = new FmodSound();
	m_SndBackgroundPtr->CreateStream(String("Resources/Sounds/overworld.ogg"));
	m_SndEndPtr = new FmodSound();
	m_SndEndPtr->CreateStream(String("Resources/Sounds/levelend.ogg"));
	m_BmpSpecialBigMarioSpriteSheetPtr = new Bitmap(String("Resources/bigmarioanimations0.png"));
	m_SndFlashingPtr = new FmodSound();
	m_SndFlashingPtr->CreateStream(String("Resources/Sounds/starmusic_01.ogg"));
	m_SndFireBall = new FmodSound();
	m_SndFireBall->CreateSound(String("Resources/Sounds/fireball.ogg"));
	m_BmpEndFlagPtr = new Bitmap(String("Resources/castleflag.png"));
	m_SndBackGroundFastPtr = new FmodSound();
	m_SndBackGroundFastPtr->CreateStream(String("Resources/Sounds/overworld-fast.ogg"));
}

Mario::~Mario()
{
	delete m_ActPtr;
	m_ActPtr = nullptr;;
	//delete m_BmpSpriteSheetPtr;
	m_BmpSpriteSheetPtr = nullptr;
	delete m_SndJumpPtr;
	m_SndJumpPtr = nullptr;
	delete m_ActFeetPtr;
	m_ActFeetPtr = nullptr;
	delete m_BmpMarioSpriteSheetPtr;
	m_BmpMarioSpriteSheetPtr = nullptr;
	delete m_BmpBigMarioSpriteSheetPtr;
	m_BmpBigMarioSpriteSheetPtr = nullptr;
	m_BmpFlashSpriteSheetPtr = nullptr;
	delete m_BmpMarioFlashSpriteSheetPtr;
	m_BmpMarioFlashSpriteSheetPtr = nullptr;
	delete m_BmpBigMarioFlashSpriteSheetPtr;
	m_BmpBigMarioFlashSpriteSheetPtr = nullptr;
	delete m_PortalPtr;
	m_PortalPtr = nullptr;
	delete m_SndTeleportPtr;
	m_SndTeleportPtr = nullptr;
	delete m_SndDeathPtr;
	m_SndDeathPtr = nullptr;
	delete m_BmpFlagPtr;
	m_BmpFlagPtr = nullptr;
	delete m_EndGameFlagPtr;
	m_EndGameFlagPtr = nullptr;
	delete m_SndBackgroundPtr;
	m_SndBackgroundPtr = nullptr;
	delete m_SndEndPtr;
	m_SndEndPtr = nullptr;
	delete m_BmpSpecialBigMarioSpriteSheetPtr;
	m_BmpSpecialBigMarioSpriteSheetPtr = nullptr;
	delete m_SndFlashingPtr;
	m_SndFlashingPtr = nullptr;
	delete m_SndFireBall;
	m_SndFireBall = nullptr;
	delete m_BmpEndFlagPtr;
	m_BmpEndFlagPtr = nullptr;
	delete m_SndBackGroundFastPtr;
	m_SndBackGroundFastPtr = nullptr;
}

void Mario::Tick(double deltaTime, Level* levelPtr, EnemyManager* enemyManagerPtr, HUD* HUDPtr)
{
	//game end placeholder
	if (HUDPtr->GetLives() <= 0)
	{
		GAME_ENGINE->QuitGame();
	}

	//-----------------------------------------------------------------
	//check if end is reached, otherwise execute Tick()
	//-----------------------------------------------------------------
	if (m_EndReached)
	{
		//levelend animation
		if (m_FlagPos.y < m_FlagMaxHeigth)
		{
			m_FlagPos.y += m_FlagVelocity * deltaTime;
			m_ActPtr->SetPosition(m_FlagHoldLeftPosition);
			m_ActionState = ActionState::LEFTOFFLAG;
			m_Facingleft = false;
		}
		else if (m_FlagHoldRightTimer > 1)
		{
			if (m_ActPtr->GetPosition().x > m_EndDoorCenter.x)
			{
				m_ShouldDrawEndFlag = true;
				HUDPtr->AddScore(100, { -500,-500 });
				m_ShouldPaint = false;
				m_ActPtr->SetLinearVelocity({ 0,0 });
				if (!m_SndEndPtr->IsPlaying())
				{
					GAME_ENGINE->QuitGame();
				}

				if (m_LastExplosionTimer == 0)
				{
					DOUBLE2 offset;
					offset.x = (rand() % m_ExplosionOffset) - m_ExplosionOffset / 2;
					offset.y = (rand() % m_ExplosionOffset) - m_ExplosionOffset / 2;
					enemyManagerPtr->QueueEnemy(m_ExplosionCenter + offset, m_FireBallWidth, m_FireballHeigth, 0, EnemyType::AUTOEXPLODEFIREBALL);
				}
				m_LastExplosionTimer += deltaTime;
				if (m_LastExplosionTimer > m_ExplosionCooldown)
				{
					m_LastExplosionTimer = 0;
				}
			}
			else
			{	
				m_ActionState = ActionState::MOVING;
				CalculateAnimations(deltaTime);
				m_ActPtr->SetLinearVelocity(DOUBLE2(m_HorSpeed, m_ActPtr->GetLinearVelocity().y));
				m_Facingleft = false;
			}
		}
		else
		{
			m_ActPtr->SetPosition(m_FlagHoldRightPosition);
			m_ActionState = ActionState::RIGHTOFFLAG;
			m_Facingleft = true;
			m_FlagHoldRightTimer += deltaTime;
		}
	}
	else
	{
		//-----------------------------------------------------------------
		//execute Tick()
		//-----------------------------------------------------------------

		//check if mario died
		if (m_Flickering)
		{
			m_ActPtr->SetUserData(int(ActorType::MARIO));
		}
		if (m_ActPtr->GetUserData() == int(ActorType::DEAD) && m_LastState == ActorType::BIGMARIO)
		{
			int test = 0;
			m_SpecialBigMario = false;
			m_Flickering = true;
		}
		else if (m_ActPtr->GetPosition().y > GAME_ENGINE->GetHeight() || m_ActPtr->GetUserData() == int(ActorType::DEAD))
		{
			if (!(m_Flickering || m_Flashing) || (m_ActPtr->GetPosition().y > GAME_ENGINE->GetHeight()))
			{
				if (!m_IsDieing)
				{
					m_SndFlashingPtr->Stop();
					m_SndBackgroundPtr->Stop();
					m_SndDeathPtr->Play();
					m_SpecialBigMario = false;
					m_IsDieing = true;
					m_ActPtr->SetLinearVelocity({ 0, 0 });
					m_ActPtr->SetSensor(true);
					m_ActPtr->ApplyLinearImpulse({ 0,-500 });
					m_ActionState = ActionState::DIEING;
					m_Flashing = false;
				}
				else if (!m_SndDeathPtr->IsPlaying())
				{
					Died(HUDPtr);
					m_IsDieing = false;
				}
			}
		}
		else
		{
			//check if we bounced off an enemy
			if (m_ShouldBounce)
			{
				m_ShouldBounce = false;
				m_ActPtr->SetLinearVelocity(DOUBLE2(m_ActPtr->GetLinearVelocity().x, -250));
			}

			//velocity decay
			DOUBLE2 velocity = m_ActPtr->GetLinearVelocity();
			if (velocity.x != 0)
			{
				if (velocity.x > 0)
				{
					velocity.x -= m_VelocityDecay;
					if (velocity.x < 0) velocity.x = 0;
				}
				else
				{
					velocity.x += m_VelocityDecay;
					if (velocity.x > 0) velocity.x = 0;
				}
			}
			m_ActPtr->SetLinearVelocity(velocity);

			//teleport
			if (m_ShouldTeleport)
			{
				m_ActPtr->SetPosition(m_TeleportPosition);
				m_ShouldTeleport = false;
				m_ActPtr->ApplyLinearImpulse(m_TeleportVelocity*2.1);
			}

			//-----------------------------------------------------------------
			// Controls
			//-----------------------------------------------------------------
			HandleControls(levelPtr, enemyManagerPtr);

			//-----------------------------------------------------------------
			// actionStates
			//-----------------------------------------------------------------
			if (!levelPtr->IsOnGround(m_ActFeetPtr)) m_ActionState = ActionState::JUMPING;
			else if (m_ActPtr->GetLinearVelocity().y <= 1 && m_ActPtr->GetLinearVelocity().x != 0) m_ActionState = ActionState::MOVING;
			else m_ActionState = ActionState::WAITING;

			//-----------------------------------------------------------------
			// animation calculations
			//-----------------------------------------------------------------
			CalculateAnimations(deltaTime);

			//-----------------------------------------------------------------
			//portals
			//-----------------------------------------------------------------
			m_PortalPtr->Tick(deltaTime, this, levelPtr);

			//second actor for feet
			m_ActFeetPtr->SetPosition(m_ActPtr->GetPosition() + DOUBLE2(0, (m_HeightActor / 2) - 1));

			//-----------------------------------------------------------------
			//start music again if there is none
			//-----------------------------------------------------------------
			if (HUDPtr->GetTime() < m_SndBackGroundFastLength)
			{
				if (!m_SndBackGroundFastPtr->IsPlaying())
				{
					m_SndBackgroundPtr->Stop();
					m_SndFlashingPtr->Stop();
					m_SndBackGroundFastPtr->Play();
				}
				if (HUDPtr->GetTime() <= 0)
				{
					GAME_ENGINE->QuitGame();
				}
			}
			if (!m_SndBackgroundPtr->IsPlaying() && !m_SndFlashingPtr->IsPlaying() && !m_SndDeathPtr->IsPlaying() && !m_SndBackGroundFastPtr->IsPlaying())
			{
				m_SndBackgroundPtr->Play();
			}


			//set needed variables
			m_LastState = ActorType(m_ActPtr->GetUserData());
			m_ActPtr->SetUserPointer(this);
		}
	}
}
 
void Mario::Paint(Level * levelPtr)
{
	GAME_ENGINE->DrawBitmap(m_BmpFlagPtr, m_FlagPos);

	MATRIX3X2 matOrigWorld = GAME_ENGINE->GetWorldMatrix(), matWorld;

	//flips actor's bitmap when facing left
	if (m_Facingleft)
	{
		matOrigWorld = GAME_ENGINE->GetWorldMatrix();
		MATRIX3X2 matCenter, matScale, matTranslate;
		matCenter.SetAsTranslate(-m_ActPtr->GetPosition().x, -m_ActPtr->GetPosition().y);
		matScale.SetAsScale(-1, 1);
		matTranslate.SetAsTranslate(m_ActPtr->GetPosition());
		matWorld = matCenter * matScale * matTranslate;
		GAME_ENGINE->SetWorldMatrix(matWorld);
	} 

	//-----------------------------------------------------------------
	// animations
	//-----------------------------------------------------------------
	if (m_ShouldPaint)
	{
		DrawBitmap(m_BmpSpriteSheetPtr);
		if (m_SpecialBigMario)
		{
			DrawBitmap(m_BmpSpecialBigMarioSpriteSheetPtr);
		}
		if (m_ShouldPaintFlash)
		{
			DrawBitmap(m_BmpFlashSpriteSheetPtr);
		}
	}

	if (m_Facingleft)
	{
		GAME_ENGINE->SetWorldMatrix(matOrigWorld);
	}

	if (!m_EndReached)
	{
		m_PortalPtr->Paint(this);
	}

	if (m_ShouldDrawEndFlag)
	{
		GAME_ENGINE->DrawBitmap(m_BmpEndFlagPtr, m_EndFlagPosition);
	}
}

void Mario::HandleControls(Level * levelPtr, EnemyManager* enemyManagerPtr)
{
	//sprinting
	if (GAME_ENGINE->IsKeyboardKeyDown(VK_SHIFT)) m_HorSpeed = 750;
	else m_HorSpeed = m_Velocity;

	//jumping
	if (levelPtr->IsOnGround(m_ActFeetPtr) || m_ActPtr->GetLinearVelocity().y == 0)
	{
		//m_ActPtr->SetLinearVelocity({ 0,m_ActPtr->GetLinearVelocity().y });
		if (GAME_ENGINE->IsKeyboardKeyPressed(VK_UP) || GAME_ENGINE->IsKeyboardKeyPressed(VK_SPACE) || GAME_ENGINE->IsKeyboardKeyPressed('W') || GAME_ENGINE->IsKeyboardKeyPressed('Z'))
		{
			m_ActPtr->SetLinearVelocity({ m_ActPtr->GetLinearVelocity().x,-m_JumpSpeed });
			if (!m_SndJumpPtr->IsPlaying())	m_SndJumpPtr->Play();
		}
	}

	//moving left/right
	if (GAME_ENGINE->IsKeyboardKeyDown(VK_LEFT) || GAME_ENGINE->IsKeyboardKeyDown('A') || GAME_ENGINE->IsKeyboardKeyDown('Q'))
	{
		DOUBLE2 velocity = m_ActPtr->GetLinearVelocity();
		if (velocity.x > -m_HorSpeed)
		{
			velocity.x -= m_VelocityDecay * m_VelocityDecayMultiplier;
			if (velocity.x < -m_HorSpeed) velocity.x = -m_HorSpeed;
		}
		m_ActPtr->SetLinearVelocity({ velocity });
	}
	else if (GAME_ENGINE->IsKeyboardKeyDown(VK_RIGHT) || GAME_ENGINE->IsKeyboardKeyDown('D'))
	{
		DOUBLE2 velocity = m_ActPtr->GetLinearVelocity();
		if (velocity.x < m_HorSpeed)
		{
			velocity.x += m_VelocityDecay * m_VelocityDecayMultiplier;
			if (velocity.x > m_HorSpeed) velocity.x = m_HorSpeed;
		}
		m_ActPtr->SetLinearVelocity({ velocity });
	}

	//spawning fireballs
	if (GAME_ENGINE->IsKeyboardKeyPressed('E') && m_SpecialBigMario)
	{
		m_SndFireBall->Play();
		if (m_Facingleft)
		{
			enemyManagerPtr->QueueEnemy(m_ActPtr->GetPosition() - DOUBLE2(m_WidthActor*1.2,0), m_FireBallWidth, m_FireballHeigth, -m_fireballVelocity, EnemyType::FIREBALL);
		}
		else
		{
			enemyManagerPtr->QueueEnemy(m_ActPtr->GetPosition() + DOUBLE2(m_WidthActor*1.2, 0), m_FireBallWidth, m_FireballHeigth, m_fireballVelocity, EnemyType::FIREBALL);
		}
	}
}

void Mario::CalculateAnimations(double deltaTime)
{
	//-----------------------------------------------------------------
	// animation calculations
	//-----------------------------------------------------------------
	if (m_ActionState == ActionState::MOVING)
	{
		m_AnimTime += deltaTime;
		if (m_AnimTime >= 1.0 / m_NrFramesPerSec)
		{
			++m_AnimFrame;
			if (m_AnimFrame >= m_NrOfFrames) m_AnimFrame = 0;
			m_AnimTime -= m_AnimTime / m_NrFramesPerSec;
		}
	}

	m_ShouldPaint = true;
	if (m_Flickering)
	{
		m_FlickeringTime += deltaTime;
		m_FlickeringValue++;
		if (m_FlickeringTime > m_FlickeringDuration)
		{
			m_FlickeringValue = 0;
			m_FlickeringTime = 0;
			m_Flickering = false;
			//m_ActPtr->SetSensor(false);
			//m_ActPtr->SetGravityScale(1);
			m_ActPtr->SetUserData(int(ActorType::MARIO));

			//m_ActPtr->SetCollisionFilter(m_CollisionFilterDefault);
		}
		if (!(m_FlickeringValue % 8 > 3))
		{
			m_ShouldPaint = false;
		}
	}

	m_ShouldPaintFlash = false;
	if (m_Flashing)
	{
		m_FlashingTime += deltaTime;
		m_FlashingValue++;
		if (m_FlashingTime > m_FlashingDuration)
		{
			m_FlashingValue = 0;
			m_FlashingTime = 0;
			m_Flashing = false;
		}
		if (!(m_FlashingValue % 12 > 6))
		{
			m_ShouldPaintFlash = true;
		}
	}

	//-----------------------------------------------------------------
	//calculations to make the bitmap look towards the mouse
	//-----------------------------------------------------------------
	DOUBLE2 marioPos = m_ActPtr->GetPosition();
	DOUBLE2 mouseWorldPos = GAME_ENGINE->GetViewMatrix().Inverse().TransformPoint(GAME_ENGINE->GetMousePosition());
	double mouseDistance = DOUBLE2(mouseWorldPos - marioPos).Length();
	double distanceY = mouseWorldPos.y - marioPos.y;
	m_MouseAngle = 90 * distanceY / mouseDistance; // law of sines, result is in degrees, 0 = right of actor, - value = above actors center, + value = below actors center

	if (mouseWorldPos.x - marioPos.x < 0) m_Facingleft = true;
	else m_Facingleft = false;

	if (m_MouseAngle <= -52.5) m_BitmapRow = 0;
	else if (m_MouseAngle <= -15) m_BitmapRow = 1;
	else if (m_MouseAngle <= 0 || m_MouseAngle < 45) m_BitmapRow = 2;
	else if (m_MouseAngle < 90) m_BitmapRow = 3;

	//-----------------------------------------------------------------
	//set bitmaps, sizes and actor depending on state
	//-----------------------------------------------------------------
	switch (m_ActPtr->GetUserData())
	{
	case int(ActorType::MARIO) :
	case int(ActorType::DEAD) :
		m_ClipHeight = m_ClipHeightSmall;
		m_BmpSpriteSheetPtr = m_BmpMarioSpriteSheetPtr;
		m_BmpFlashSpriteSheetPtr = m_BmpMarioFlashSpriteSheetPtr;
		m_BitmapHeightOffset = 0;
		break;
	case int(ActorType::BIGMARIO) :
		m_ClipHeight = m_ClipHeightBig;
		m_BmpSpriteSheetPtr = m_BmpBigMarioSpriteSheetPtr;
		m_BmpFlashSpriteSheetPtr = m_BmpBigMarioFlashSpriteSheetPtr;
		m_BitmapHeightOffset = m_BitmapOffset * 8;
		break;
	default:
		break;
	}
}

DOUBLE2 Mario::getPos()
{
	return m_ActPtr->GetPosition();
}

void Mario::Died(HUD* HUDPtr)
{
		m_ActPtr->SetPosition(25, 50);
		m_ActPtr->SetUserData(int(ActorType::MARIO));
		m_ActPtr->SetLinearVelocity({ 0, 0 });
		m_ActPtr->SetSensor(false);
		HUDPtr->DecreaseLives();
}

void Mario::SetFlashing(bool flag)
{
	m_Flashing = flag;
	if (flag = true)
	{
		m_SndBackgroundPtr->Stop();
		m_SndFlashingPtr->Play();
	}
}

void Mario::DrawBitmap(Bitmap * bitmapPtr)
{
	DOUBLE2 pos = m_ActPtr->GetPosition();
	switch (m_ActionState)
	{
	case(ActionState::JUMPING) :
		GAME_ENGINE->DrawBitmap(bitmapPtr, pos.x - (m_ClipWidth / 2.0) - m_BitmapOffset, pos.y - (m_ClipHeight / 2.0) + m_BitmapOffset - m_BitmapHeightOffset, { m_ClipWidth * 5.0, double(m_BitmapRow * m_ClipHeight), double(m_ClipWidth) * 6, (m_BitmapRow + 1.0) * m_ClipHeight });
		break;
	case(ActionState::WAITING) :
		GAME_ENGINE->DrawBitmap(bitmapPtr, pos.x - (m_ClipWidth / 2.0) - m_BitmapOffset, pos.y - (m_ClipHeight / 2.0) + m_BitmapOffset - m_BitmapHeightOffset, { 0, double(m_BitmapRow * m_ClipHeight), double(m_ClipWidth),(m_BitmapRow + 1.0) * m_ClipHeight });
		break;
	case(ActionState::MOVING) :
		GAME_ENGINE->DrawBitmap(bitmapPtr, pos.x - (m_ClipWidth / 2.0) - m_BitmapOffset, pos.y - (m_ClipHeight / 2.0) + m_BitmapOffset - m_BitmapHeightOffset, { double(m_AnimFrame * m_ClipWidth) + m_ClipWidth, double(m_BitmapRow * m_ClipHeight), ((m_AnimFrame + 1.0) * m_ClipWidth) + m_ClipWidth ,(m_BitmapRow + 1.0) * m_ClipHeight });
		break;
	case(ActionState::LEFTOFFLAG) :
		GAME_ENGINE->DrawBitmap(bitmapPtr, pos.x - (m_ClipWidth / 2.0) - m_BitmapOffset, pos.y - (m_ClipHeight / 2.0) + m_BitmapOffset - m_BitmapHeightOffset, { m_ClipWidth * 8.0, double(m_BitmapRow * m_ClipHeight), double(m_ClipWidth) * 9, (m_BitmapRow + 1.0) * m_ClipHeight });
		break;
	case(ActionState::RIGHTOFFLAG) :
		GAME_ENGINE->DrawBitmap(bitmapPtr, pos.x - (m_ClipWidth / 2.0) - m_BitmapOffset, pos.y - (m_ClipHeight / 2.0) + m_BitmapOffset - m_BitmapHeightOffset, { m_ClipWidth * 8.0, double(m_BitmapRow * m_ClipHeight), double(m_ClipWidth) * 9, (m_BitmapRow + 1.0) * m_ClipHeight });
		break;
	case(ActionState::DIEING) :
		GAME_ENGINE->DrawBitmap(bitmapPtr, pos.x - (m_ClipWidth / 2.0) - m_BitmapOffset, pos.y - (m_ClipHeight / 2.0) + m_BitmapOffset - m_BitmapHeightOffset, { m_ClipWidth * 6.0, double(m_BitmapRow * m_ClipHeight), double(m_ClipWidth) * 7, (m_BitmapRow + 1.0) * m_ClipHeight });
		break;
	default:
		break;
	}
}

bool Mario::GetFlashing()
{
	return m_Flashing;
}

PhysicsActor* Mario::GetMario()
{
	if (m_ActPtr != nullptr)
	{
		return m_ActPtr;
	}
	else
	{
		return nullptr;
	}
}

void Mario::SetSensor(bool flag)
{
	m_IsSensor = flag;
}

void Mario::Teleport(DOUBLE2 position, DOUBLE2 velocity)
{
	m_TeleportPosition = position;
	m_ShouldTeleport = true;
	m_SndTeleportPtr->Play();
	m_TeleportVelocity = velocity;
}

void Mario::Bounce()
{
	m_ShouldBounce = true;
}

void Mario::SetEndReached(bool flag)
{
	m_EndReached = flag;
	if (flag = true)
	{
		m_SndBackgroundPtr->Stop();
		m_SndFlashingPtr->Stop();
		m_SndBackGroundFastPtr->Stop();
		if (!m_SndEndPtr->IsPlaying())
		{
			m_SndEndPtr->Play();
		}
	}
}

void Mario::SetSpecialBigMario(bool flag)
{
	m_SpecialBigMario = flag;
}