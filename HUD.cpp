//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "HUD.h"
#include "ParseSession.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

HUD::HUD()
{
	m_BmpFontPtr = new Bitmap(String("Resources/Font.png"));
	m_TextSize = m_BmpFontPtr->GetHeight();
	m_BmpCoinHUDPtr = new Bitmap(String("Resources/CoinHUD.png"));
	m_FntScorePtr = new Font(String("Verdana"), 8);
	m_BmpLivesPtr = new Bitmap(String("Resources/Icon.gif"));
}

HUD::~HUD()
{
	delete m_BmpFontPtr;
	m_BmpFontPtr = nullptr;
	delete m_BmpCoinHUDPtr;
	m_BmpCoinHUDPtr = nullptr;
	delete m_FntScorePtr;
	m_FntScorePtr = nullptr;
	delete m_BmpLivesPtr;
	m_BmpLivesPtr = nullptr;
}

void HUD::Tick(double deltaTime)
{
	m_Time -= deltaTime;

	m_AnimTime += deltaTime;
	if (m_AnimTime >= 1.0 / m_NrFramesPerSec)
	{
		++m_AnimFrame;
		if (m_AnimFrame >= m_NrOfFrames) m_AnimFrame = 0;
		m_AnimTime -= m_AnimTime / m_NrFramesPerSec;
	}

	if (m_LastScoreAlpha != 0)
	{
		m_LastScoreAlpha -= 5;
	}
	m_LastScorePosOffset.y-=0.5;
}

void HUD::Paint(ParseSession* sessionPtr)
{
	//last added score
	Font* fntOriginal = GAME_ENGINE->GetFont();
	COLOR clrOriginal = GAME_ENGINE->GetColor();
	GAME_ENGINE->SetFont(m_FntScorePtr);
	GAME_ENGINE->SetColor(COLOR(255, 255, 255, m_LastScoreAlpha));
	GAME_ENGINE->DrawString(String(m_LastScore), m_LastScorePos + m_LastScorePosOffset);
	GAME_ENGINE->SetFont(fntOriginal);
	GAME_ENGINE->SetColor(clrOriginal);

	//store old viewmatrix and set identity as new viewmatrix
	MATRIX3X2 matViewOrig, matView;
	matView.SetAsIdentity();
	matViewOrig = GAME_ENGINE->GetViewMatrix();

	GAME_ENGINE->SetViewMatrix(matView);

	//-----------------------------------------------------------------
	//HUD
	//-----------------------------------------------------------------
	//score
	drawStringBitmap(String("mario"), { m_TextSize*4.0,double(m_TextSize) });
	String score = String(m_Score);
	if (score.Length()-1 < 5)
	{
		String temp = score;
		score = String("");
		for (int i = 0; i < 5 - (temp.Length()); i++) score += String("0");
		score = score + temp;
	}
	drawStringBitmap(String(score), { m_TextSize*4.0,m_TextSize*2.0 });
	//coins
	GAME_ENGINE->DrawBitmap(m_BmpCoinHUDPtr, { m_TextSize*12.0,double(m_TextSize) * 1.5 }, { m_AnimFrame * m_CoinWidth, 0, (m_AnimFrame + 1) * m_CoinWidth, double(m_BmpCoinHUDPtr->GetHeight())});
	String coins = String(m_NrOfCoins);

	if (coins.Length() < 2)
	{
		String temp = coins;
		coins = String(0);
		coins += temp;
	}
	drawStringBitmap(String("*") + coins, { m_TextSize*13.0,double(m_TextSize) * 1.5 });
	//lives
	GAME_ENGINE->DrawBitmap(m_BmpLivesPtr, m_TextSize*20.0, double(m_TextSize));
	drawStringBitmap(String("*") + String(m_Lives), { m_TextSize*22.0,double(m_TextSize) * 1.5 });
	//world and time
	drawStringBitmap(String("world"), { m_TextSize*29.0,double(m_TextSize) });
	drawStringBitmap(String("1-1"), { m_TextSize*30.0,m_TextSize*2.0 });
	drawStringBitmap(String("time"), { m_TextSize*42.0,double(m_TextSize) });
	drawStringBitmap(String(int(m_Time)), { m_TextSize*43.0,m_TextSize*2.0 });

	//Session info
	if (GAME_ENGINE->IsKeyboardKeyPressed('I'))	m_IsSessionDataOn = !m_IsSessionDataOn;
	if (m_IsSessionDataOn)
	{
		GAME_ENGINE->DrawString(String(sessionPtr->GetLastSessionData().c_str()), { 10,50 });
	}
	GAME_ENGINE->SetViewMatrix(matViewOrig);
}

void HUD::DecreaseLives()
{
	--m_Lives;
}

int HUD::GetLives()
{
	return m_Lives;
}

void HUD::drawStringBitmap(const String &text, const DOUBLE2 &position)
{
	DOUBLE2 pos = position;
	for (char character : std::string(text.C_str()))
	{
		GAME_ENGINE->DrawBitmap(m_BmpFontPtr, pos, GetCharRect(character));
		pos.x += m_TextSize;
	}
}
RECT2 HUD::GetCharRect(char character)
{
	int index = 0;
	if (character >= 'a' && character <= 'z') index = character - 'a' + 10;
	else if (character >= '0' && character <= '9') index = character - '0';
	else if (character == '-') index = 42;
	else if (character == '*') index = 45;
	else index = 0;
	return{ double(index * m_TextSize),0,double((index + 1)*m_TextSize),double(m_TextSize) };
}

void HUD::AddCoin()
{
	m_NrOfCoins++;
}

void HUD::AddScore(int score, DOUBLE2 position)
{
	m_Score += score;
	m_LastScore = score;
	m_LastScorePos = position;
	m_LastScoreAlpha = 255;
	m_LastScorePosOffset = DOUBLE2(0, 0);
}

double HUD::GetTime()
{
	return m_Time;
}