//////////////////////////////////////////////////////////////////////////////////////////////
//		__________ MAC'S EXTENDED DEBUG MODE (MEDM) for GFC2D __________					//
//			A modified version of 'game\main.cpp' injected with MEDM.						//
//		!!! MAKE SURE THIS FILE IS RENAMED TO main.cpp AND IN THE CORRECT DIRECTORY !!!		//
//			 There should be no compatibility changes when using this mod.					//
//		________________________________________________________________					//
//////////////////////////////////////////////////////////////////////////////////////////////
//		Mac, 2022

#include "stdafx.h"
#include "MyGame.h"

CMyGame game;
CGameApp app;

// Mac's Extended Debug Mode control panel
bool debugMode = true;	// Enable or disable MEDM. Some features of MEDM cannot be turned off.
string gameTitle = "MONOCHROME";	// Set the title of the game. (Cannot be overriden)
string medmVersion = "1.0"; // change the version number of MEDM that is reported. (Cannot be overriden)


int main(int argc, char* argv[])
{

	//app.DebugMemoryLeaks();

	if (debugMode == true) {
		app.OpenWindow(800, 600, (gameTitle + " - [Mac's Extended DEBUG MODE enabled] " + " - [MEDM Version: " + medmVersion + "]"));
	}
	else {
		app.OpenWindow(800, 600, gameTitle + " - [MEDM Version: " + medmVersion + "]");
	}

	//app.OpenFullScreen(800, 600, 24);
	//app.OpenConsole();

	if (debugMode == true) {
		app.OpenConsole();
		std::cout << "\n" << "Game title: " << gameTitle << "\n";
		std::cout << "MEDM Version: " << medmVersion << "\n";
		std::cout << "\nMac's Extended Debug mode is enabled.\nThis is NOT the default debug mode, but a modified version.";
		std::cout << "\n --Mac, 2022";
		std::cout << "\n\n ===[CONTROLS]===\nN = Bg White\nM = Bg Black\n\n";
	}

	app.SetClearColor(CColor::Black());
	app.Run(&game);
	return(0);
}
