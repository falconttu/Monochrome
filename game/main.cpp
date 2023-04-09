#include "stdafx.h"
#include "MyGame.h"
#include <string>
#include <iostream>
using namespace std;

CMyGame game;
CGameApp app;

bool DebugMode = true;

int main(int argc, char* argv[])
{
	//app.DebugMemoryLeaks();
	
	string gameName = "My game";
	
	if (DebugMode == false) 
	{
		app.OpenWindow(800, 600, gameName);
	}
	else
	{
		app.OpenWindow(800, 600, gameName + " [Debug Mode]");
		app.OpenConsole();
	}
	
	//app.OpenFullScreen(800, 600, 24);
	
	app.SetClearColor(CColor::Black());
	app.Run(&game);
	return(0);
}
