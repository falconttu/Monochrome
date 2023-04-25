#include "stdafx.h"
#include "MyGame.h"

bool isWhite = false;

CMyGame::CMyGame(void) :
	background(400,300,0,0,0), player(400, 540, 0, 0, 0), ball(20, 20, 16, 16, CColor::White(), 0)
	// to initialise more sprites here use a comma-separated list
{
	// TODO: add initialisation here

	// Initialising Integer which will control the Level picked via the game menu
	MenuGameLV = 1;
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
	Uint32 dt = GetDeltaTime();

	// Menu 1 and 2 Update
	MenuLevel1.Update(t);
	MenuLevel2.Update(t);


	// Healthbar
	// try make this an array :)
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
	{
		player.Accelerate(0, -50);
	}
	ball.Accelerate(0, -5);

	PlayerController();

	// Pre-Update Position
	CVector v0 = player.GetPos();
	
	// Collisions for physically based objects
	for (CSprite* platform : platforms)
	{
		if (!isWhite)
		{
			if ((string)platform->GetProperty("tag") == "white")
			{
				int h = ball.GetHeight() / 2 - 1;
				CVector v = ball.GetVelocity() * dt / 1000;
				CVector dist = platform->GetCenter() - ball.GetPos();
				float X = (platform->GetWidth() / 2);
				float Y = (platform->GetHeight() / 2);
				CVector n = CVector(sin(platform->GetRotation()), cos(platform->GetRotation()));
				if (Dot(v, n) < 0)
				{
					// Perpendicular component (oncoming)
					float vy = Dot(v, n);		// velocity component
					CVector d = dist + (Y + h) * n;	// distance vector between edges
					float dy = Dot(d, n);		// perpendicular space between
					float f1 = dy / vy;

					// Parallel component (breadth control)
					float vx = Cross(v, n);		// velocity component
					float tx = Cross(dist, n);		// distance between centres
					float f2 = (tx - vx * f1) / (X + h);
					if (-f1 >= 0 && -f1 <= 1 && -f2 >= -1 && -f2 <= 1)	//testing
					{
						ball.SetVelocity(Reflect(ball.GetVelocity() * 0.65, n));
					}
				}
			}
		}

		if (isWhite)
		{
			if ((string)platform->GetProperty("tag") == "black")
			{
				int h = ball.GetHeight() / 2 - 1;
				CVector v = ball.GetVelocity() * dt / 1000;
				CVector dist = platform->GetCenter() - ball.GetPos();
				float X = (platform->GetWidth() / 2);
				float Y = (platform->GetHeight() / 2);
				CVector n = CVector(sin(platform->GetRotation()), cos(platform->GetRotation()));
				if (Dot(v, n) < 0)
				{
					// Perpendicular component (oncoming)
					float vy = Dot(v, n);		// velocity component
					CVector d = dist + (Y + h) * n;	// distance vector between edges
					float dy = Dot(d, n);		// perpendicular space between
					float f1 = dy / vy;

					// Parallel component (breadth control)
					float vx = Cross(v, n);		// velocity component
					float tx = Cross(dist, n);		// distance between centres
					float f2 = (tx - vx * f1) / (X + h);
					if (-f1 >= 0 && -f1 <= 1 && -f2 >= -1 && -f2 <= 1)	//testing
					{
						ball.SetVelocity(Reflect(ball.GetVelocity() * 0.65, n));
					}
				}
			}
		}
	}
	for (CSprite* platform : platforms)
	{
		if (!isWhite)
		{
			if ((string)platform->GetProperty("tag") == "white")
			{
				int h = ball.GetHeight() / 2 - 1;
				CVector n = CVector(sin(platform->GetRotation()), cos(platform->GetRotation()));
				if (ball.HitTest(platform, 0))
				{
					if (ball.GetVelocity().m_x >= platform->GetRight() + h)
					{
						ball.SetVelocity(Reflect(ball.GetVelocity() * 0.65, n));
					}
					else if (ball.GetVelocity().m_x <= platform->GetLeft() - h)
					{
						ball.SetVelocity(Reflect(ball.GetVelocity() * 0.65, n));
					}
				}
			}
		}

		if ((string)platform->GetProperty("tag") == "wall")
		{
			int h = ball.GetHeight() / 2 - 1;
			CVector n = CVector(sin(platform->GetRotation()), cos(platform->GetRotation()));
			if (ball.HitTest(platform, 0))
			{
				if (ball.GetVelocity().m_x >= platform->GetRight() + h)
				{
					ball.SetVelocity(Reflect(ball.GetVelocity() * 0.65, n));
				}
				else if (ball.GetVelocity().m_x <= platform->GetLeft() - h)
				{
					ball.SetVelocity(Reflect(ball.GetVelocity() * 0.65, n));
				}
			}
		}
	}
	
	// Updates
	for (CSprite* platform : platforms)
	{
		platform->Update(t);
	}
	player.Update(t);
	ball.Update(t);

	// Updating the HealthBar Control Function
	HealthBarControl();

	// Updating the Menu Control Function
	MenuControl();

	// Collisions for player
	bool bTouchingPlatform = false;
	int h = player.GetHeight() / 2 - 1;
	int w = player.GetWidth() / 2 - 1;
	for (CSprite* platform : platforms)
	{
		if (player.HitTest(platform, 0))
		{
			if ((string)platform->GetProperty("tag") == "wall")
			{
				if (v0.m_x >= platform->GetRight() + w)
				{
					bTouchingPlatform = true;
					player.SetX(platform->GetRight() + w + 2);
					player.Accelerate(10, 0);
				}
				else if (v0.m_x <= platform->GetLeft() - w)
				{
					bTouchingPlatform = true;
					player.SetX(platform->GetLeft() - w - 2);
					player.Accelerate(-10, 0);
				}
			}
			
			if (isWhite)
			{
				if ((string)platform->GetProperty("tag") == "black")
				{
					// platforms and walls
					if (v0.m_y >= platform->GetTop() + h)	//Player stands on top of the platform
					{
						bTouchingPlatform = true;
						player.SetVelocity(0, 0);
						player.SetY(platform->GetTop() + h);
					}
					else if (v0.m_y <= platform->GetBottom() - h)	// Barricades the player from going through
					{
						bTouchingPlatform = true;
						player.SetY(platform->GetBottom() - h - 2);
						player.Accelerate(10, 0);
					}
					else if (v0.m_x >= platform->GetRight() + w)
					{
						bTouchingPlatform = true;
						player.SetX(platform->GetRight() + w + 2);
						player.Accelerate(10, 0);
					}
					else if (v0.m_x <= platform->GetLeft() - w)
					{
						bTouchingPlatform = true;
						player.SetX(platform->GetLeft() - w - 2);
						player.Accelerate(-10, 0);
					}
				}
			}

			if (!isWhite)
			{
				if ((string)platform->GetProperty("tag") == "white")
				{
					// platforms and walls
					if (v0.m_y >= platform->GetTop() + h)	//Player stands on top of the platform
					{
						bTouchingPlatform = true;
						player.SetVelocity(0, 0);
						player.SetY(platform->GetTop() + h);
					}
					else if (v0.m_y <= platform->GetBottom() - h)	// Barricades the player from going through
					{
						bTouchingPlatform = true;
						player.SetY(platform->GetBottom() - h - 2);
						player.Accelerate(10, 0);
					}
					else if (v0.m_x >= platform->GetRight() + w)
					{
						bTouchingPlatform = true;
						player.SetX(platform->GetRight() + w + 2);
						player.Accelerate(10, 0);
					}
					else if (v0.m_x <= platform->GetLeft() - w)
					{
						bTouchingPlatform = true;
						player.SetX(platform->GetLeft() - w - 2);
						player.Accelerate(-10, 0);
					}
				}
			}
		}
	}

	// processing of airborne condition
	if (m_state == AIRBORNE && bTouchingPlatform)
	{
		// just landed
		m_state = STANDING;
		player.SetImage(m_side == LEFT ? "stand_left" : "stand_right");
	}
	if (m_state != AIRBORNE && !bTouchingPlatform)
	{
		// just taken off
		m_state = AIRBORNE;
		player.SetImage(m_side == LEFT ? "jump_left" : "jump_right");
	}

	// Procssing background
	if (isWhite == false) {
		background.SetImage("black_back.png");
		back_colour = false;
		ball.SetColorKey(CColor::White());
	}

	if (isWhite == true) {
		background.SetImage("white_back.png");
		back_colour = true;
		ball.SetColorKey(CColor::Black());
	}
}

void CMyGame::PlayerController()
{
	// Run and Stand
	if (m_state == STANDING || m_state == RUNNING)
	{
		player.SetVelocity(0, 0);
		if (IsKeyDown(SDLK_a) || IsKeyDown(SDLK_LEFT))
		{
			player.Accelerate(-300, 0);
			if (m_state != RUNNING || m_side != LEFT)
				player.SetAnimation("run_left");
			m_state = RUNNING;
			m_side = LEFT;
		}
		else if (IsKeyDown(SDLK_d) || IsKeyDown(SDLK_RIGHT))
		{
			player.Accelerate(300, 0);
			if (m_state != RUNNING || m_side != RIGHT)
				player.SetAnimation("run_right");
			m_state = RUNNING;
			m_side = RIGHT;
		}
		else
		{
			if (m_state == RUNNING)
				player.SetImage(m_side == LEFT ? "stand_left" : "stand_right");
			m_state = STANDING;
		}
	}

	// Jumping
	if ((IsKeyDown(SDLK_w) || IsKeyDown(SDLK_UP)) && (m_state == STANDING || m_state == RUNNING))
	{
		player.Accelerate(0, 800);
		if (IsKeyDown(SDLK_a) || IsKeyDown(SDLK_LEFT))
		{
			if (m_state == AIRBORNE)
			{
				player.Accelerate(150, 0);
			}
			else
			{
				player.Accelerate(-220, 0);
			}
		}
		else if (IsKeyDown(SDLK_d) || IsKeyDown(SDLK_RIGHT))
		{
			if (m_state == AIRBORNE)
			{
				player.Accelerate(-150, 0);
			}
			else
			{
				player.Accelerate(220, 0);
			}
		}
		m_state = AIRBORNE;
		player.SetImage(m_side == LEFT ? "jump_left" : "jump_right");
	}
}

void CMyGame::HealthBarControl()
{
	health = player.GetHealth();

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

}

void CMyGame::MenuControl()
{
	if (IsKeyDown(SDLK_UP) || IsKeyDown(SDLK_w))
	{
		MenuGameLV = 1;
	}

	if (IsKeyDown(SDLK_DOWN) || IsKeyDown(SDLK_s))
	{
		MenuGameLV = 2;
	}

	if (MenuGameLV == 1)
	{
		MenuLevel1.LoadImage("MonochromeMenuLV1.bmp");
		MenuLevel1.SetImage("MonochromeMenuLV1.bmp");
		MenuLevel1.SetPosition(400, 300);
	}
	
    if (MenuGameLV == 2)
	{
		MenuLevel2.LoadImage("MonochromeMenuLV2.bmp");
		MenuLevel2.SetImage("MonochromeMenuLV2.bmp");
		MenuLevel2.SetPosition(400, 300);
	}

	MenuLevel1.IsDeleted();
	MenuLevel2.IsDeleted();
	// Entering the Game if on level 1

	
}

void CMyGame::OnDraw(CGraphics* g)
{
	// Drawing The Background
	background.Draw(g);
	for (CSprite* p : platforms)
	{
		p->Draw(g);
	}
	player.Draw(g);
	ball.Draw(g);

	// Drawing the Menu Level 1
	if (IsMenuMode() && MenuGameLV == 1)
	{
		MenuLevel1.Draw(g);
		return;
	}

	if (IsMenuMode() && MenuGameLV == 2)
	{
		MenuLevel2.Draw(g);
		return;
	}

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
		*g << font(24) << color(CColor::Blue()) << xy(5, 580) << "Keys Collected:";
		*g << font(24) << color(CColor::LightBlue()) << xy(175, 580) << keys_collected;
	}
}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
	// Loading and Setting the Health Bar
	Healthbar5.LoadImageW("HealthBar.bmp", CColor::White());
	Healthbar5.SetImage("HealthBar.bmp");

	background.LoadImage("black_back.png");
	background.LoadImage("white_back.png");

	player.LoadImage("player.png", "stand_right", 11, 6, 0, 0, CColor::White());
	player.LoadImage("player.png", "stand_left", 11, 6, 0, 1, CColor::White());
	player.AddImage("player.png", "run_right", 11, 6, 0, 0, 10, 0, CColor::White());
	player.AddImage("player.png", "run_left", 11, 6, 0, 1, 10, 1, CColor::White());
	player.LoadImage("player.png", "jump_right", 11, 6, 3, 2, CColor::White());
	player.LoadImage("player.png", "jump_left", 11, 6, 3, 3, CColor::White());
	player.LoadImage("player.png", "crouch_right", 11, 6, 2, 4, CColor::White());
	player.LoadImage("player.png", "crouch_left", 11, 6, 2, 5, CColor::White());
	player.LoadImage("player.png", "hang", 11, 6, 10, 2, CColor::White());
	player.AddImage("player.png", "climb", 11, 6, 9, 2, 10, 2, CColor::White());

	
}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{
	MenuControl();	// exits the menu mode and starts the game mode

	// Menu Music
	if (IsMenuMode())
	{
		//MenuTheme.Play("BGM.wav", -1);

	}

	else
	{
		MenuTheme.Pause();
	}
}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{}

CSprite* platform;

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{
	// Clean up first
	for (CSprite* platforms : platforms)
	{
		delete platforms;
	}
	platforms.clear();
	platforms.delete_all();

	switch (nLevel)
	{
	case 1:// build Level 1 sprites

		// Loading the Background
		background.SetImage("white_back.png");

		// spawn the player
		player.SetPos(20, 205);
		player.SetImage("stand_right");

		// platforms
		platform = new CSpriteRect(400, 10, 800, 20, CColor::White(), CColor::White(), GetTime());	// Floor
		platform->SetProperty("tag", "white");
		platforms.push_back(platform);
		
		platform = new CSpriteRect(-10, 300, 10, 600, CColor::Black(), CColor::White(), GetTime());	// Left Wall
		platform->SetProperty("tag", "wall");
		platforms.push_back(platform);	

		platform = new CSpriteRect(810, 300, 10, 600, CColor::Black(), CColor::White(), GetTime());	// Right Wall
		platform->SetProperty("tag", "wall");
		platforms.push_back(platform);

		platform = new CSpriteRect(150, 100, 600, 20, CColor::Black(), CColor::Black(), GetTime());
		platform->SetProperty("tag", "black");
		platforms.push_back(platform); 


		ball.SetPosition(400, 300);

		break;

	case 2:// Level 2 (EMPTY)

		break;
	}

	// any initialisation common to all levels
	player.SetHealth(100);
	player.SetImage("stand_right");
	player.SetVelocity(0, 0);
	m_state = STANDING;
	m_side = RIGHT;
}

// called when the game is over
void CMyGame::OnGameOver()
{}

// one time termination code
void CMyGame::OnTerminate()
{}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	// Pause Game
	if (sym == SDLK_F2)
		NewGame();

	// Menu Level Selection
	if (IsMenuMode())
	{
		if (IsKeyDown(SDLK_SPACE))
		{
			StartGame();
		}
	}

	// color invert
	if (sym == SDLK_n) {
		isWhite = true;
		cout << "isWhite = " << isWhite << endl;
	}

	if (sym == SDLK_m) {
		isWhite = false;
		cout << "isWhite = " << isWhite << endl;
	}
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x, Uint16 y, Sint16 relx, Sint16 rely, bool bLeft, bool bRight, bool bMiddle)
{}

void CMyGame::OnLButtonDown(Uint16 x, Uint16 y)
{}

void CMyGame::OnLButtonUp(Uint16 x, Uint16 y)
{}

void CMyGame::OnRButtonDown(Uint16 x, Uint16 y)
{}

void CMyGame::OnRButtonUp(Uint16 x, Uint16 y)
{}

void CMyGame::OnMButtonDown(Uint16 x, Uint16 y)
{}

void CMyGame::OnMButtonUp(Uint16 x, Uint16 y)
{}