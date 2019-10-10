//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Portal.h"
#include "Mario.h"
#include "Level.h"
#include "Enums.h"
#include "Enemy.h"
#include "PortalParticleManager.h"
#include "PortalGunParticleManager.h"
#define GAME_ENGINE (GameEngine::GetSingleton())

Portal::Portal()
{
	m_ActBluePortal = new PhysicsActor(-500, -500, 0, BodyType::KINEMATIC);
	m_ActBluePortal->AddBoxFixture(m_Length, 10, 0, 0);
	m_ActBluePortal->SetSensor(true);
	m_ActBluePortal->SetUserData(int(PortalType::BLUE));
	m_ActBluePortal->AddContactListener(this);
	m_ActOrangePortal = new PhysicsActor(-500, -500, 0, BodyType::KINEMATIC);
	m_ActOrangePortal->AddBoxFixture(m_Length, 10, 0, 0);
	m_ActOrangePortal->SetSensor(true);
	m_ActOrangePortal->SetUserData(int(PortalType::ORANGE));
	m_ActOrangePortal->AddContactListener(this);
	m_BmpCrosshairRed = new Bitmap(String("Resources/portalcrosshairRed.png"));
	m_BmpCrosshairGreen = new Bitmap(String("Resources/portalcrosshairGreen.png"));
	m_BmpGlowPtr = new Bitmap(String("Resources/portalglow.png"));
	m_ActCanSpawnArrPtr.push_back(new PhysicsActor({ -500,-500 }, 0, BodyType::KINEMATIC));
	m_ActCanSpawnArrPtr[0]->AddBoxFixture(0.2, 0.2, 0, 0);
	m_ActCanSpawnArrPtr[0]->SetSensor(true);
	m_ActCanSpawnArrPtr.push_back(new PhysicsActor({ -500,-500 }, 0, BodyType::KINEMATIC));
	m_ActCanSpawnArrPtr[1]->AddBoxFixture(0.2, 0.2, 0, 0);
	m_ActCanSpawnArrPtr[1]->SetSensor(true);
	m_ActCanSpawnArrPtr.push_back(new PhysicsActor({ -500,-500 }, 0, BodyType::KINEMATIC));
	m_ActCanSpawnArrPtr[2]->AddBoxFixture(0.2, 0.2, 0, 0);
	m_ActCanSpawnArrPtr[2]->SetSensor(true);
	m_ActCanSpawnArrPtr.push_back(new PhysicsActor({ -500,-500 }, 0, BodyType::KINEMATIC));
	m_ActCanSpawnArrPtr[3]->AddBoxFixture(0.2, 0.2, 0, 0);
	m_ActCanSpawnArrPtr[3]->SetSensor(true);
	m_BmpBluePortalAnimation = new Bitmap(String("Resources/PortalAnimationBlue.png"));
	m_BmpOrangePortalAnimation = new Bitmap(String("Resources/PortalAnimationOrange.png"));
	m_PortalParticleBluePtr = new PortalParticleManager(m_ActBluePortal->GetPosition(), m_NormalBluePortal);
	m_PortalParticleOrangePtr = new PortalParticleManager(m_ActOrangePortal->GetPosition(), m_NormalOrangePortal);
	m_PortalGunParticleManagerPtr = new PortalGunParticleManager(DOUBLE2(0, 0), DOUBLE2(1000, 0));
	m_SndBluePortalPtr = new FmodSound();
	m_SndBluePortalPtr->CreateStream(String("Resources/Sounds/portal1open.ogg"));
	m_SndOrangePortalPtr = new FmodSound();
	m_SndOrangePortalPtr->CreateStream(String("Resources/Sounds/portal2open.ogg"));
}

Portal::~Portal()
{
	delete m_ActBluePortal;
	m_ActBluePortal = nullptr;
	delete m_ActOrangePortal;
	m_ActOrangePortal = nullptr;
	delete m_BmpCrosshairRed;
	m_BmpCrosshairRed = nullptr;
	delete m_BmpCrosshairGreen;
	m_BmpCrosshairGreen = nullptr;
	delete m_BmpGlowPtr;
	m_BmpGlowPtr = nullptr;
	for (PhysicsActor * actPtr : m_ActCanSpawnArrPtr) delete actPtr;
	m_ActCanSpawnArrPtr.clear();
	delete m_BmpBluePortalAnimation;
	m_BmpBluePortalAnimation = nullptr;
	delete m_BmpOrangePortalAnimation;
	m_BmpOrangePortalAnimation = nullptr;
	delete m_PortalParticleBluePtr;
	m_PortalParticleBluePtr = nullptr;
	delete m_PortalParticleOrangePtr;
	m_PortalParticleOrangePtr = nullptr;
	delete m_PortalGunParticleManagerPtr;
	m_PortalGunParticleManagerPtr = nullptr;
	delete m_SndBluePortalPtr;
	m_SndBluePortalPtr = nullptr;
	delete m_SndOrangePortalPtr;
	m_SndOrangePortalPtr = nullptr;
}

void Portal::Tick(const double deltaTime, Mario* marioPtr, Level* levelPtr)
{
	DOUBLE2 marioPos = marioPtr->getPos();
	MATRIX3X2 matView = GAME_ENGINE->GetViewMatrix();
	DOUBLE2 mousePos = matView.Inverse().TransformPoint(GAME_ENGINE->GetMousePosition());
	DOUBLE2 intersection = { 0,0 }, normal = { 0,0 };
	double fraction = 999; //any value larger as one
	m_HasIntersection = false;
	DOUBLE2 projectedPoint = mousePos - marioPos;
	projectedPoint.normalize();
	projectedPoint *= m_MaxLength;
	if (levelPtr->Raycast(marioPos, (projectedPoint) + marioPos, intersection, normal, fraction))
	{
		m_LastIntersection = intersection;
		m_LastNormal = normal;
		m_HasIntersection = true;
	}

	//portal animation
	m_AnimTime += deltaTime;
	if (m_AnimTime >= 1.0 / m_NrFramesPerSec)
	{
		++m_AnimFrame;
		if (m_AnimFrame > m_NrOfFrames) m_AnimFrame = 0;
		m_AnimTime -= m_AnimTime / m_NrFramesPerSec;
	}

	//portalgun particle animation
	if (GAME_ENGINE->IsMouseButtonPressed(VK_LBUTTON) || GAME_ENGINE->IsMouseButtonPressed(VK_RBUTTON))
	{
		if (m_HasIntersection)
		{
			m_PortalGunParticleManagerPtr->ActivateNewAnimation(marioPos, m_LastIntersection, (m_LastIntersection - marioPos).Length());
		}
		else m_PortalGunParticleManagerPtr->ActivateNewAnimation(marioPos, mousePos, m_MaxLength);
	}

	//spawn portal
	m_CanSpawn = CanSpawnPortal(levelPtr);
	if (m_CanSpawn && m_LastNormal == normal)
	{
		if (GAME_ENGINE->IsMouseButtonPressed(VK_LBUTTON))
		{
			m_ActBluePortal->SetPosition(m_LastIntersection);
			m_NormalBluePortal = m_LastNormal;
			m_SndBluePortalPtr->Play();
		}
		else if (GAME_ENGINE->IsMouseButtonPressed(VK_RBUTTON))
		{
			m_ActOrangePortal->SetPosition(m_LastIntersection);
			m_NormalOrangePortal = m_LastNormal;
			m_SndOrangePortalPtr->Play();
		}
	}
	m_PortalParticleBluePtr->Tick(deltaTime, m_ActBluePortal->GetPosition(), m_NormalBluePortal);
	m_PortalParticleOrangePtr->Tick(deltaTime, m_ActOrangePortal->GetPosition(), m_NormalOrangePortal);
	m_PortalGunParticleManagerPtr->Tick(deltaTime);

	if (!m_HasIntersection)
	{
		DOUBLE2 directionVector = mousePos - marioPtr->getPos();
		directionVector.normalize();
	}
}
	MATRIX3X2 matView = GAME_ENGINE->GetViewMatrix();

void Portal::Paint(Mario* marioPtr)
{
	PhysicsActor* mario = marioPtr->GetMario();
	DOUBLE2 mousePos = matView.Inverse().TransformPoint(GAME_ENGINE->GetMousePosition());
	DrawPortal(m_ActBluePortal, m_NormalBluePortal, m_BmpGlowPtr, m_BmpBluePortalAnimation);
	DrawPortal(m_ActOrangePortal, m_NormalOrangePortal, m_BmpGlowPtr, m_BmpOrangePortalAnimation);
	COLOR origClr = GAME_ENGINE->GetColor();
	if (m_CanSpawn) GAME_ENGINE->SetColor(COLOR(0, 255, 0));
	else GAME_ENGINE->SetColor(COLOR(255, 0, 0));

	if (m_HasIntersection)
	{
		DrawDottedLine(marioPtr, DOUBLE2(m_LastIntersection - marioPtr->getPos()).Length());
		if (m_CanSpawn)	DrawCrosshair(m_BmpCrosshairGreen);
		else DrawCrosshair(m_BmpCrosshairRed);
	}
	else DrawDottedLine(marioPtr, m_MaxLength);

	GAME_ENGINE->SetColor(m_ClrBlue);
	m_PortalParticleBluePtr->Paint();
	GAME_ENGINE->SetColor(m_ClrOrange);
	m_PortalParticleOrangePtr->Paint();
	GAME_ENGINE->SetColor(origClr);
	m_PortalGunParticleManagerPtr->Paint();
}

void Portal::DrawDottedLine(Mario* marioPtr, double length)
{
	DOUBLE2 marioPos = marioPtr->getPos();
	MATRIX3X2 matView = GAME_ENGINE->GetViewMatrix();
	DOUBLE2 mousePos = matView.Inverse().TransformPoint(GAME_ENGINE->GetMousePosition());
	DOUBLE2 directionVector = mousePos - marioPos;
	directionVector.normalize();
	for (double i = m_SpaceBetweenDots/2; i + m_DottedLineOffset  < length; i += m_SpaceBetweenDots)
	{
		GAME_ENGINE->FillEllipse(DOUBLE2(marioPos + (i + m_DottedLineOffset) * directionVector), m_DottedLineRadius, m_DottedLineRadius);
	}
	m_DottedLineOffset += m_DottedLineOffsetIncrements;
	if (m_DottedLineOffset >= m_SpaceBetweenDots)
	{
		m_DottedLineOffset = 0;
	}
}

bool Portal::CanSpawnPortal(DOUBLE2 point, Level* levelPtr)
{
	return false;
}

void Portal::DrawCrosshair(Bitmap* bitmap)
{
	if (m_LastNormal.y < 0)
	{
		GAME_ENGINE->DrawBitmap(bitmap, m_LastIntersection - DOUBLE2(bitmap->GetWidth()/2, bitmap->GetHeight()));
	}
	else
	{
		MATRIX3X2 matWorldOrig = GAME_ENGINE->GetWorldMatrix();
		MATRIX3X2 matCenter, matRotate, matTranslate, matWorld;
		DOUBLE2 bitmapTopLeft, bitmapCenter;
		if (m_LastNormal.y > 0)
		{
			bitmapTopLeft = DOUBLE2(m_LastIntersection - DOUBLE2(bitmap->GetWidth() / 2, 0));
			bitmapCenter = bitmapTopLeft + DOUBLE2(bitmap->GetWidth()/2, bitmap->GetHeight()/2);
			matCenter.SetAsTranslate(-bitmapCenter);
			matRotate.SetAsRotate(M_PI);
			matTranslate.SetAsTranslate(bitmapCenter);
			matWorld = matCenter * matRotate * matTranslate;
			GAME_ENGINE->SetWorldMatrix(matWorld);
			GAME_ENGINE->DrawBitmap(bitmap, bitmapTopLeft);
		}
		else if (m_LastNormal.x < 0)
		{
			bitmapTopLeft = DOUBLE2(m_LastIntersection - DOUBLE2(bitmap->GetWidth(), bitmap->GetHeight() / 2));
			bitmapCenter = bitmapTopLeft + DOUBLE2(bitmap->GetWidth() / 2, bitmap->GetHeight() / 2);
			matCenter.SetAsTranslate(-bitmapCenter);
			matRotate.SetAsRotate(-M_PI/2);
			matTranslate.SetAsTranslate(bitmapCenter);
			matWorld = matCenter * matRotate * matTranslate;
			GAME_ENGINE->SetWorldMatrix(matWorld);
			GAME_ENGINE->DrawBitmap(bitmap, bitmapTopLeft);
		}
		else if (m_LastNormal.x > 0)
		{
			bitmapTopLeft = DOUBLE2(m_LastIntersection + DOUBLE2(0, -bitmap->GetHeight() / 2));
			bitmapCenter = bitmapTopLeft + DOUBLE2(bitmap->GetWidth() / 2, bitmap->GetHeight() / 2);
			matCenter.SetAsTranslate(-bitmapCenter);
			matRotate.SetAsRotate(M_PI / 2);
			matTranslate.SetAsTranslate(bitmapCenter);
			matWorld = matCenter * matRotate * matTranslate;
			GAME_ENGINE->SetWorldMatrix(matWorld);
			GAME_ENGINE->DrawBitmap(bitmap, bitmapTopLeft);
		}
		GAME_ENGINE->SetWorldMatrix(matWorldOrig);
	}
}

void Portal::DrawPortal(PhysicsActor* actPortalPtr, const DOUBLE2 normal, Bitmap* bitmap, Bitmap* animationBitmap)
{
	RECT2 animationRect = { 0,double(m_AnimFrame*m_AnimationHeight),double(animationBitmap->GetWidth()),double((m_AnimFrame + 1)*m_AnimationHeight) };
	DOUBLE2 position = actPortalPtr->GetPosition();
	if (normal.y < 0)
	{
		GAME_ENGINE->DrawBitmap(bitmap, position - DOUBLE2(bitmap->GetWidth() / 2, bitmap->GetHeight()));
		GAME_ENGINE->DrawBitmap(animationBitmap, position - DOUBLE2(animationBitmap->GetWidth() / 2, m_AnimationHeight), animationRect);
		actPortalPtr->SetAngle(0);
	}
	else
	{
		MATRIX3X2 matWorldOrig = GAME_ENGINE->GetWorldMatrix();
		MATRIX3X2 matCenter, matRotate, matTranslate, matWorld;
		DOUBLE2 bitmapTopLeft, bitmapCenter;
		if (normal.y > 0)
		{
			bitmapTopLeft = DOUBLE2(position - DOUBLE2(bitmap->GetWidth() / 2, 0));
			bitmapCenter = bitmapTopLeft + DOUBLE2(bitmap->GetWidth() / 2, bitmap->GetHeight() / 2);
			matCenter.SetAsTranslate(-bitmapCenter);
			matRotate.SetAsRotate(M_PI);
			matTranslate.SetAsTranslate(bitmapCenter);
			matWorld = matCenter * matRotate * matTranslate;
			GAME_ENGINE->SetWorldMatrix(matWorld);
			GAME_ENGINE->DrawBitmap(bitmap, bitmapTopLeft);
			bitmapTopLeft.y += bitmap->GetHeight() - m_AnimationHeight;
			GAME_ENGINE->DrawBitmap(animationBitmap, bitmapTopLeft, animationRect);
			actPortalPtr->SetAngle(M_PI);
		}
		else if (normal.x < 0)
		{
			bitmapTopLeft = DOUBLE2(position - DOUBLE2(bitmap->GetHeight(), bitmap->GetWidth() / 2));//intentionally flipped since bitmap will we turned around
			bitmapCenter = position;
			matCenter.SetAsTranslate(-bitmapCenter);
			matRotate.SetAsRotate(-M_PI / 2);
			matTranslate.SetAsTranslate(bitmapCenter);
			matWorld = matCenter * matRotate * matTranslate;
			GAME_ENGINE->SetWorldMatrix(matWorld);
			GAME_ENGINE->DrawBitmap(bitmap, bitmapTopLeft);
			bitmapTopLeft.y += bitmap->GetHeight() - m_AnimationHeight;
			GAME_ENGINE->DrawBitmap(animationBitmap, bitmapTopLeft, animationRect);
			actPortalPtr->SetAngle(-M_PI / 2);

		}
		else if (normal.x > 0)
		{
			bitmapTopLeft = DOUBLE2(position - DOUBLE2(0, bitmap->GetWidth() / 2));
			bitmapCenter = position + DOUBLE2(bitmap->GetWidth() / 2, bitmap->GetHeight());
			matCenter.SetAsTranslate(-bitmapCenter);
			matRotate.SetAsRotate(M_PI / 2);
			matTranslate.SetAsTranslate(position - DOUBLE2(bitmap->GetHeight(),0));
			matWorld = matCenter * matRotate * matTranslate;
			GAME_ENGINE->SetWorldMatrix(matWorld);
			GAME_ENGINE->DrawBitmap(bitmap, bitmapTopLeft);
			bitmapTopLeft.y += bitmap->GetHeight() - m_AnimationHeight;
			GAME_ENGINE->DrawBitmap(animationBitmap, bitmapTopLeft, animationRect);
			actPortalPtr->SetAngle(M_PI / 2);
		}
		GAME_ENGINE->SetWorldMatrix(matWorldOrig);
	}
}

bool Portal::CanSpawnPortal(Level* levelPtr)
{
	double width2 = m_BmpGlowPtr->GetWidth() / 2;
	double height = m_BmpGlowPtr->GetWidth();

	m_ActCanSpawnArrPtr[0]->SetPosition(m_LastNormal.y * width2 + m_LastIntersection.x, m_LastNormal.x * width2 + m_LastIntersection.y);
	m_ActCanSpawnArrPtr[1]->SetPosition(-m_LastNormal.y * width2 + m_LastIntersection.x, -m_LastNormal.x * width2 + m_LastIntersection.y);
	if (levelPtr->IsOnGround(m_ActCanSpawnArrPtr[0]) && levelPtr->IsOnGround(m_ActCanSpawnArrPtr[1]))
	{
		if (m_ActBluePortal->IsOverlapping(m_ActCanSpawnArrPtr[0]) || m_ActOrangePortal->IsOverlapping(m_ActCanSpawnArrPtr[0])) return false;
		if (m_ActBluePortal->IsOverlapping(m_ActCanSpawnArrPtr[1]) || m_ActOrangePortal->IsOverlapping(m_ActCanSpawnArrPtr[1])) return false;
		return true;
		
		//following code is unnecessary since being inside the svg does not trigger overlapping
		/*m_ActCanSpawnArrPtr[2]->SetPosition(m_ActCanSpawnArrPtr[0]->GetPosition() + m_LastNormal);
		m_ActCanSpawnArrPtr[3]->SetPosition(m_ActCanSpawnArrPtr[1]->GetPosition() + m_LastNormal);
		if (!levelPtr->IsOnGround(m_ActCanSpawnArrPtr[0]) && !levelPtr->IsOnGround(m_ActCanSpawnArrPtr[1]))
		{
			return true;
		}*/
	}
	return false;
}

bool Portal::IsOverlapping(PhysicsActor* actOtherPtr)
{
	if (m_ActBluePortal->IsOverlapping(actOtherPtr) || m_ActOrangePortal->IsOverlapping(actOtherPtr))return true;
	else return false;
}

void Portal::BeginContact(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr)
{
	if (actOtherPtr->GetUserData() == int(ActorType::MARIO) || actOtherPtr->GetUserData() == int(ActorType::BIGMARIO))
	{
		if (m_ReadyForNextTeleport && m_ActBluePortal->GetPosition().x >= 0 && m_ActOrangePortal->GetPosition().x >= 0)
		{
			m_ReadyForNextTeleport = false;
			Mario* marioPtr = (Mario*)actOtherPtr->GetUserPointer();
			if (actThisPtr->GetUserData() == int(PortalType::BLUE))
			{
				//teleport to orange
				DOUBLE2 velocity = marioPtr->GetMario()->GetLinearVelocity();
				CalculateVelocity(velocity, m_NormalBluePortal, m_NormalOrangePortal);
				marioPtr->Teleport(m_ActOrangePortal->GetPosition() + m_NormalOrangePortal, velocity);
			}
			else
			{
				//teleport to blue
				DOUBLE2 velocity = marioPtr->GetMario()->GetLinearVelocity();
				CalculateVelocity(velocity, m_NormalOrangePortal, m_NormalBluePortal);
				marioPtr->Teleport(m_ActBluePortal->GetPosition() + m_NormalBluePortal, velocity);
			}
		}
	}
	else if (actOtherPtr->GetUserData() == int(ActorType::ENEMY))
	{
		Enemy* enemyPtr = (Enemy*)actOtherPtr->GetUserPointer();

		if (enemyPtr->GetReadyForTeleport() && m_ActBluePortal->GetPosition().x >= 0 && m_ActOrangePortal->GetPosition().x >= 0)
		{
			enemyPtr->SetReadyForTeleport(false);
			if (actThisPtr->GetUserData() == int(PortalType::BLUE))
			{
				//teleport to orange
				DOUBLE2 velocity = enemyPtr->GetActor()->GetLinearVelocity();
				CalculateVelocity(velocity, m_NormalBluePortal, m_NormalOrangePortal);
				enemyPtr->Teleport(m_ActOrangePortal->GetPosition() + m_NormalOrangePortal, velocity);
			}
			else
			{
				//teleport to blue
				DOUBLE2 velocity = enemyPtr->GetActor()->GetLinearVelocity();
				CalculateVelocity(velocity, m_NormalOrangePortal, m_NormalBluePortal);
				enemyPtr->Teleport(m_ActBluePortal->GetPosition() + m_NormalBluePortal, velocity);
			}
		}
	}
}

void Portal::CalculateVelocity(DOUBLE2 &velocity, const DOUBLE2 &normalStart, const DOUBLE2 &normalEnd)
{
	if (normalStart.x == 0 && normalEnd.x != 0)
	{
		double x = velocity.x;
		velocity.x = velocity.y/4;
		if (normalEnd.y == 0) velocity.y = 0;
		else velocity.y = x;
	}
	else if (normalStart.y == -1 && normalEnd.y == -1 && velocity.y < 25)
	{
		double x = velocity.x;
		velocity.x = velocity.y;
		velocity.y = x;
	}
	if (normalEnd.x == 0)
	{
		velocity.y = abs(velocity.y) * normalEnd.y;
		if (velocity.y > 600)
		{
			velocity.y = 600;
		}
	}
	else if (normalEnd.y == 0)
	{
		velocity.x = abs(velocity.x) * normalEnd.x;

	}
	else return;
}

void Portal::EndContact(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr)
{
	if (actOtherPtr->GetUserData() == int(ActorType::MARIO) || actOtherPtr->GetUserData() == int(ActorType::BIGMARIO))
	{
		++m_EndContactcount;
		if (m_EndContactcount >= 2)
		{
			m_ReadyForNextTeleport = true;
			m_EndContactcount = 0;
		}
	}
	else if (actOtherPtr->GetUserData() == int(ActorType::ENEMY))
	{
		Enemy* enemyPtr = (Enemy*)actOtherPtr->GetUserPointer();
		enemyPtr->IncreaseEndContactcount();
		if (enemyPtr->GetEndContactcount() >= 2)
		{
			enemyPtr->SetReadyForTeleport(true);
			enemyPtr->SetEndContactCount(0);
		}
	}
}