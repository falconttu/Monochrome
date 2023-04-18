#include "stdafx.h"
#include "MyGame.h"

CMyGame::CMyGame(void) :
	m_player(400, 540, 0, 0, 0)
	// to initialise more sprites here use a comma-separated list
{
	// TODO: add initialisation here
	
}

CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	Uint32 t = GetTime();
	
	Healthbar0.Update(t);
	Healthbar1.Update(t);
	Healthbar2.Update(t);
	Healthbar3.Update(t);
	Healthbar4.Update(t);
	Healthbar5.Update(t);
	Healthbar6.Update(t);
	Healthbar7.Update(t);
	Healthbar8.Update(t);
	Healthbar9.Update(t);
	Healthbar10.Update(t);

	// Gravity
	if (m_state == AIRBORNE)
		m_player.Accelerate(0, -50);

	// Run and Stand
	if (m_state == STANDING || m_state == RUNNING)
	{
		m_player.SetVelocity(0, 0);
		if (IsKeyDown(SDLK_a) || IsKeyDown(SDLK_LEFT))
		{
			m_player.Accelerate(-300, 0);
			if (m_state != RUNNING || m_side != LEFT)
				m_player.SetAnimation("run_left");
			m_state = RUNNING;
			m_side = LEFT;
		}
		else if (IsKeyDown(SDLK_d) || IsKeyDown(SDLK_RIGHT))
		{
			m_player.Accelerate(300, 0);
			if (m_state != RUNNING || m_side != RIGHT)
				m_player.SetAnimation("run_right");
			m_state = RUNNING;
			m_side = RIGHT;
		}
		else
		{
			if (m_state == RUNNING)
				m_player.SetImage(m_side == LEFT ? "stand_left" : "stand_right");
			m_state = STANDING;
		}
	}

	// Jumping
	if ((IsKeyDown(SDLK_w) || IsKeyDown(SDLK_UP)) && (m_state == STANDING || m_state == RUNNING))
	{
		m_player.Accelerate(0, 800);
		if (IsKeyDown(SDLK_a) || IsKeyDown(SDLK_LEFT))
		{
			if (m_state == AIRBORNE)
			{
				m_player.Accelerate(150, 0);
			}
			else
			{
				m_player.Accelerate(-220, 0);
			}
		}
		else if (IsKeyDown(SDLK_d) || IsKeyDown(SDLK_RIGHT))
		{
			if (m_state == AIRBORNE)
			{
				m_player.Accelerate(-150, 0);
			}
			else
			{
				m_player.Accelerate(220, 0);
			}
		}
		m_state = AIRBORNE;
		m_player.SetImage(m_side == LEFT ? "jump_left" : "jump_right");
	}

	// Pre-Update Position
	CVector v0 = m_player.GetPos();

	// Updates
	for (CSprite* pSprite : m_sprites)
	{
		pSprite->Update(t);
	}
	m_player.Update(t);

	HealthBarControl();

	// Collisions
	bool bTouchingPlatform = false;
	int h = m_player.GetHeight() / 2 - 1;
	int w = m_player.GetWidth() / 2 - 1;
	for (CSprite* pSprite : m_sprites)
	{
		if (m_player.HitTest(pSprite, 0))
		{
			// platforms and walls
			if ((string)pSprite->GetProperty("tag") == "platform")
			{
				if (v0.m_y >= pSprite->GetTop() + h)	//Player stands on top of the platform
				{
					bTouchingPlatform = true;
					m_player.SetVelocity(0, 0);
					m_player.SetY(pSprite->GetTop() + h);
				}
				else if (v0.m_y <= pSprite->GetBottom() - h)	// Barricades the player from going through
				{
					bTouchingPlatform = true;
					m_player.SetY(pSprite->GetBottom() - h - 2);
					m_player.Accelerate(10, 0);
				}
				else if (v0.m_x >= pSprite->GetRight() + w)
				{
					bTouchingPlatform = true;
					m_player.SetX(pSprite->GetRight() + w + 2);
					m_player.Accelerate(10, 0);
				}
				else if (v0.m_x <= pSprite->GetLeft() - w)
				{
					bTouchingPlatform = true;
					m_player.SetX(pSprite->GetLeft() - w - 2);
					m_player.Accelerate(-10, 0);
				}
			}
		}
	}

	//gamewon logic
	for (CSprite* pSprite : m_sprites)
	{
		if (m_player.HitTest(pSprite, 0))
		{
			if ((string)pSprite->GetProperty("tag") == "gate")
			{
				if (keys_collected == 3)
				{
					gamewon = true;
					GameOver();
				}
			}
		}
	}

	// processing of airborne condition
	if (m_state == AIRBORNE && bTouchingPlatform)
	{
		// just landed
		m_state = STANDING;
		m_player.SetImage(m_side == LEFT ? "stand_left" : "stand_right");
	}
	if (m_state != AIRBORNE && !bTouchingPlatform)
	{
		// just taken off
		m_state = AIRBORNE;
		m_player.SetImage(m_side == LEFT ? "jump_left" : "jump_right");
	}

	//Key collector
	for (CSprite* pKeys : m_sprites)
	{
		if (m_player.HitTest(pKeys, 0))
		{
			if ((string)pKeys->GetProperty("tag") == "key")
			{
				pKeys->Delete();
				keys_collected++;

			}
		}
	}
	m_sprites.delete_if(deleted);

	//Player Deaths
	for (CSprite* pSprite : m_sprites)
	{
		if (m_player.HitTest(pSprite, 0))
		{
			if ((string)pSprite->GetProperty("tag") == "enemy")	//Hitting enemy
			{
				m_player.SetHealth(0);
				GameOver();
			}
			else if ((string)pSprite->GetProperty("tag") == "hazard")	//Hitting hazard
			{
				m_player.SetHealth(0);
				gamewon = false;
				GameOver();
			}
			else if (v0.m_y >= pSprite->GetTop() + h)
			{
				if (m_player.GetYVelocity() > 400)
				{
					m_player.SetHealth(0);
					gamewon = false;
					GameOver();
				}
			}
		}
	}

	//Changing enemey direction
	for (CSprite* pEnemy : m_sprites)
		if ((string)pEnemy->GetProperty("tag") == "enemy")
			for (CSprite* pCollider : m_sprites)
			{
				if ((string)pCollider->GetProperty("tag") == "lcollider" && pEnemy->HitTest(pCollider, 0))
				{
					// Collision response code – left collider
					pEnemy->SetVelocity(100, 0);
					pEnemy->SetOmega(3.82 * 100);
				}
				else
					if ((string)pCollider->GetProperty("tag") == "rcollider" && pEnemy->HitTest(pCollider, 0))
					{
						// Collision response code – right collider
						pEnemy->SetVelocity(-100, 0);
						pEnemy->SetOmega(-3.82 * 100);
					}
			}
}

void CMyGame::OnDraw(CGraphics* g)
{
	for (CSprite* pSprite : m_sprites)
	{
		if ((string)pSprite->GetProperty("tag") != "rcollider" && (string)pSprite->GetProperty("tag") != "lcollider")
		{
			pSprite->Draw(g);
		}
	}
	m_player.Draw(g);


	// Drawing The Health Bars

	if (health == 100) Healthbar10.Draw(g);
	if (health == 90) Healthbar9.Draw(g);
	if (health == 80) Healthbar8.Draw(g);
	if (health == 70) Healthbar7.Draw(g);
	if (health == 60) Healthbar6.Draw(g);
	if (health == 50) Healthbar5.Draw(g);
	if (health == 40) Healthbar4.Draw(g);
	if (health == 30) Healthbar3.Draw(g);
	if (health == 20) Healthbar2.Draw(g);
	if (health == 10) Healthbar1.Draw(g);
	if (health == 0) Healthbar0.Draw(g);

	//Game over texts
	if (IsGameOverMode())
	{
		if (gamewon)
		{
			*g << font(46) << color(CColor::LightGreen()) << vcenter << center << " GAME WON ";
		}
		else
		{
			*g << font(46) << color(CColor::LightRed()) << vcenter << center << " GAME OVER ";
		}
	}

	//Keys collected counter
	if (!IsGameOverMode())
	{
		*g << font(24) << color(CColor::White()) << xy(5, 580) << "Keys Collected:";
		*g << font(24) << color(CColor::LightBlue()) << xy(175, 580) << keys_collected;
	}
}

void CMyGame::HealthBarControl()
{
	health = m_player.GetHealth();

	// Setting Up all the Health Bar Sprites
	if (health == 100)
	{
		Healthbar10.LoadImageW("HealthBar.bmp", CColor::White());
		Healthbar10.SetImage("HealthBar.bmp");
		Healthbar10.SetPosition(100, 530);
		return;
	}

	if (health == 90)
	{
		Healthbar9.LoadImageW("90Health.bmp", CColor::White());
		Healthbar9.SetImage("90Health.bmp");
		Healthbar9.SetPosition(100, 530);
		return;
	}

	if (health == 80)
	{
		Healthbar8.LoadImageW("80Health.bmp", CColor::White());
		Healthbar8.SetImage("80Health.bmp");
		Healthbar8.SetPosition(100, 530);
		return;
	}

	if (health == 70)
	{
		Healthbar7.LoadImageW("70Health.bmp", CColor::White());
		Healthbar7.SetImage("70Health.bmp");
		Healthbar7.SetPosition(100, 530);
		return;
	}

	if (health == 60)
	{
		Healthbar6.LoadImageW("60Health.bmp", CColor::White());
		Healthbar6.SetImage("60Health.bmp");
		Healthbar6.SetPosition(100, 530);
		return;
	}

	if (health == 50)
	{
		Healthbar5.LoadImageW("50Health.bmp", CColor::White());
		Healthbar5.SetImage("50Health.bmp");
		Healthbar5.SetPosition(100, 530);
		return;
	}

	if (health == 40)
	{
		Healthbar4.LoadImageW("40Health.bmp", CColor::White());
		Healthbar4.SetImage("40Health.bmp");
		Healthbar4.SetPosition(100, 530);
		return;
	}

	if (health == 30)
	{
		Healthbar3.LoadImageW("30Health.bmp", CColor::White());
		Healthbar3.SetImage("30Health.bmp");
		Healthbar3.SetPosition(100, 530);
		return;
	}

	if (health == 20)
	{
		Healthbar2.LoadImageW("20Health.bmp", CColor::White());
		Healthbar2.SetImage("20Health.bmp");
		Healthbar2.SetPosition(100, 530);
		return;
	}

	if (health == 10)
	{
		Healthbar1.LoadImageW("10Health.bmp", CColor::White());
		Healthbar1.SetImage("10Health.bmp");
		Healthbar1.SetPosition(100, 530);
		return;
	}

	if (health == 0)
	{
		Healthbar0.LoadImageW("0Health.bmp", CColor::White());
		Healthbar0.SetImage("0Health.bmp");
		Healthbar0.SetPosition(100, 530);
		return;
	}

	Healthbar0.IsDeleted();
	Healthbar1.IsDeleted();
	Healthbar2.IsDeleted();
	Healthbar3.IsDeleted();
	Healthbar4.IsDeleted();
	Healthbar5.IsDeleted();
	Healthbar6.IsDeleted();
	Healthbar7.IsDeleted();
	Healthbar8.IsDeleted();
	Healthbar9.IsDeleted();
	Healthbar10.IsDeleted();

};

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
	// Loading and Setting the Health Bar
	Healthbar5.LoadImageW("HealthBar.bmp", CColor::White());
	Healthbar5.SetImage("HealthBar.bmp");

	m_player.LoadImage("player.png", "stand_right", 11, 6, 0, 0, CColor::White());
	m_player.LoadImage("player.png", "stand_left", 11, 6, 0, 1, CColor::White());
	m_player.AddImage("player.png", "run_right", 11, 6, 0, 0, 10, 0, CColor::White());
	m_player.AddImage("player.png", "run_left", 11, 6, 0, 1, 10, 1, CColor::White());
	m_player.LoadImage("player.png", "jump_right", 11, 6, 3, 2, CColor::White());
	m_player.LoadImage("player.png", "jump_left", 11, 6, 3, 3, CColor::White());
	m_player.LoadImage("player.png", "crouch_right", 11, 6, 2, 4, CColor::White());
	m_player.LoadImage("player.png", "crouch_left", 11, 6, 2, 5, CColor::White());
	m_player.LoadImage("player.png", "hang", 11, 6, 10, 2, CColor::White());
	m_player.AddImage("player.png", "climb", 11, 6, 9, 2, 10, 2, CColor::White());


}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{
	StartGame();	// exits the menu mode and starts the game mode
}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{
	// Clean up first
	for (CSprite* pSprite : m_sprites)
		delete pSprite;
	m_sprites.clear();
	m_sprites.delete_all();

	CSprite* pSprite;

	switch (nLevel)
	{
	case 1:// build Level 1 sprites

		// spawn the player
		m_player.SetPos(20, 205);
		m_player.SetImage("stand_right");

		// spawn all other sprites here ...

		// platforms
		pSprite = new CSpriteRect(400, 10, 800, 20, CColor::Black(), CColor::White(), GetTime());	// Base Floor
		pSprite->SetProperty("tag", "platform");
		m_sprites.push_back(pSprite);

		pSprite = new CSpriteRect(-10, 300, 10, 600, CColor::Black(), CColor::White(), GetTime());	// Left Barrier
		pSprite->SetProperty("tag", "platform");
		m_sprites.push_back(pSprite);

		pSprite = new CSpriteRect(810, 300, 10, 600, CColor::Black(), CColor::White(), GetTime());	// Right Barrier
		pSprite->SetProperty("tag", "platform");
		m_sprites.push_back(pSprite);

		pSprite = new CSpriteRect(115, 155, 230, 20, CColor::Black(), CColor::White(), GetTime());	// 1st Elevation
		pSprite->SetProperty("tag", "platform");
		m_sprites.push_back(pSprite);

		pSprite = new CSpriteRect(590, 180, 420, 20, CColor::Black(), CColor::White(), GetTime());	//2nd Elevation
		pSprite->SetProperty("tag", "platform");
		m_sprites.push_back(pSprite);

		pSprite = new CSpriteRect(240 - 10, 320, 240, 20, CColor::Black(), CColor::White(), GetTime());	//3rd Elevation
		pSprite->SetProperty("tag", "platform");
		m_sprites.push_back(pSprite);

		pSprite = new CSpriteRect(110 - 10, 370, 20, 120, CColor::Black(), CColor::White(), GetTime());	//3rd Elevation lefr barrier
		pSprite->SetProperty("tag", "platform");
		m_sprites.push_back(pSprite);

		pSprite = new CSpriteRect(450, 405, 200, 20, CColor::Black(), CColor::White(), GetTime());	//4th Elevation
		pSprite->SetProperty("tag", "platform");
		m_sprites.push_back(pSprite);

		pSprite = new CSpriteRect(675, 340, 250, 20, CColor::Black(), CColor::White(), GetTime());// Final Elevation
		pSprite->SetProperty("tag", "platform");
		m_sprites.push_back(pSprite);

		// Enemies
		pSprite = new CSprite(20, 30, "skull20.png", CColor::White(), GetTime());	//1st Enemy
		pSprite->SetProperty("tag", "enemy");
		pSprite->SetVelocity(100, 0);
		pSprite->SetOmega(3.82 * 100);
		m_sprites.push_back(pSprite);

		pSprite = new CSprite(455, 425, "skull20.png", CColor::White(), GetTime());	//2nd Enemy
		pSprite->SetProperty("tag", "enemy");
		pSprite->SetVelocity(100, 0);
		pSprite->SetOmega(3.82 * 100);
		m_sprites.push_back(pSprite);

		// Hazard
		pSprite = new CSprite(420, 205, "spikes.png", CColor::Black(), GetTime());	//1st Hazard
		pSprite->SetProperty("tag", "hazard");
		m_sprites.push_back(pSprite);

		pSprite = new CSprite(760, 205, "spikes.png", CColor::Black(), GetTime());	//2nd Hazard
		pSprite->SetProperty("tag", "hazard");
		m_sprites.push_back(pSprite);

		// Colliders
		pSprite = new CSpriteRect(-10, 30, 20, 20, CColor::Red(), GetTime());	//Base left collider
		pSprite->SetProperty("tag", "lcollider");
		m_sprites.push_back(pSprite);

		pSprite = new CSpriteRect(810, 30, 20, 20, CColor::Red(), GetTime());	//Base right collider
		pSprite->SetProperty("tag", "rcollider");
		m_sprites.push_back(pSprite);

		pSprite = new CSpriteRect(340, 425, 20, 20, CColor::Red(), GetTime());	//4th Elevation left collider
		pSprite->SetProperty("tag", "lcollider");
		m_sprites.push_back(pSprite);

		pSprite = new CSpriteRect(560, 425, 20, 20, CColor::Red(), GetTime());	//4th Elevation right collider
		pSprite->SetProperty("tag", "rcollider");
		m_sprites.push_back(pSprite);

		//Keys
		pSprite = new CSprite(760, 40, "key.png", CColor::Black(), GetTime());	//1st Key
		pSprite->SetProperty("tag", "key");
		m_sprites.push_back(pSprite);

		pSprite = new CSprite(20, 85, "key.png", CColor::Black(), GetTime());	//2nd Key
		pSprite->SetProperty("tag", "key");
		m_sprites.push_back(pSprite);

		pSprite = new CSprite(20, 450, "key.png", CColor::Black(), GetTime());	//3rd Key
		pSprite->SetProperty("tag", "key");
		m_sprites.push_back(pSprite);

		//Gate
		pSprite = new CSprite(745, 380, "gate.png", CColor::Black(), GetTime());	//Gate
		pSprite->SetProperty("tag", "gate");
		m_sprites.push_back(pSprite);

		keys_collected = 0;

		break;

	case 2:// Level 2 (EMPTY)

		break;
	}

	// any initialisation common to all levels
	m_player.SetHealth(100);
	m_player.SetImage("stand_right");
	m_player.SetVelocity(0, 0);
	m_state = STANDING;
	m_side = RIGHT;
}

// called when the game is over
void CMyGame::OnGameOver()
{
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x, Uint16 y, Sint16 relx, Sint16 rely, bool bLeft, bool bRight, bool bMiddle)
{
}

void CMyGame::OnLButtonDown(Uint16 x, Uint16 y)
{
}

void CMyGame::OnLButtonUp(Uint16 x, Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x, Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x, Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x, Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x, Uint16 y)
{
}
