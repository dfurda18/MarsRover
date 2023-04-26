/*
	Rover Application
	Made by Dario Urdapilleta
	2022-10-28
	Version 1.0
*/

//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>
#include "Blit3D.h"
#include "Rover.h"
#include "MapTileInterface.h"

//GLOBAL DATA
Blit3D* blit3D = NULL;				// The Blit3D Object
AngelcodeFont* afont = NULL;		// The font
Rover *roverApplication;			// The Rover application
MapTileInterface *map;				// The map interface
Sprite* tileMap = NULL;				// The tile sprite sheet


enum class GameMode { PICKING, EDITING, SHOWING };	// The enum for the game states
GameMode gameMode;									// The game state

//mouse cursor
float cursor_x = 0;		// The mouse x position
float cursor_y = 0;		// The mouse y position
int x_tile = 0;			// The tile x position the mouse is on
int y_tile = 0;			// The tile y position the mouse is on

float elapsedTime = 0;			// The elapsed between each updates
float timeSlice = 1.0f / 30.0f; // Update rate

std::string coords;				// The string with the coordinates so it's not declare on each draw

/**
* Method that prepares the inforamtion for the application to run
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-28
*/
void Init()
{
	gameMode = GameMode::EDITING;																		// Set the initial Game state
	roverApplication = new Rover();																		// Create the rover application object
	map = new MapTileInterface(blit3D);																	// Create the map
	map->LoadRandomMap(roverApplication->GetMapWidth(), roverApplication->GetMapHeight(), 3, 1.0f);		// Load a random map
	
	afont = blit3D->MakeAngelcodeFontFromBinary32("Media\\Oswald_72.bin");								// the Angelcode font
	tileMap = blit3D->MakeSprite(0, 0, 64, 64, "Media\\terrain.png");									// load the tilemap sprite
}
/**
* Method that frees the memory stored
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-28
*/
void DeInit(void)
{
	roverApplication->DeInit();
	if (roverApplication) {
		delete(roverApplication);		// Delete the rover application
	}
	if (map) delete(map);								// Delete the map
}
/**
* Method that updates each frame
* @param seconds the time passed in seconds between the last update.
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-28
*/
void Update(double seconds)
{
	if (seconds > 0.15) seconds = 0.15;			// Cap the update rate for faster processors
	elapsedTime += (float)seconds;				// Ass the time to the elapsed time
	while (elapsedTime >= timeSlice)			// Loop until the elapsed time is smaller than that frame rate
	{
		elapsedTime -= timeSlice;				// reduse the elapsed time
		map->Update(timeSlice);					// Update the map
		if (map->isAnimating)					// Check if the map is in animating state
		{
			gameMode = GameMode::SHOWING;		// Change the state to showing
		}
	}	
}
/**
* Method that draws in the screen
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-28
*/
void Draw()
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);					//clear colour: r,g,b,a 	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// wipe the drawing surface clear

	switch (gameMode)										// Draw differently depending on the game state
	{
	case GameMode::EDITING:									// Editing state
		map->Draw();										// Draw the map

		if (map->editTileNum != -1)										// Check if the editing tile is set
		{
			map->GetTileList(map->editTileNum)->Blit(1050, 1010);		// Draw the current editing tile
		}
		else
		{
			afont->BlitText(1025, 900, "Empty");						// Draw the text empty
		}

		coords = "X:" + std::to_string((int)x_tile);					// Prepare the X coordinate text
		afont->BlitText(1025, 980, coords);								// Show the text
		coords = "Y:" + std::to_string((int)y_tile);					// Prepare the Y coordinate text
		afont->BlitText(1025, 900, coords);								// Show the text

		afont->BlitText(1025, 820, "Press SPACE to pick a tile to draw.");		// Show the instructions
		afont->BlitText(1025, 740, "Press R to load a random map.");			// Show the instructions
		afont->BlitText(1025, 660, "Press E to load an empty map.");			// Show the instructions
		afont->BlitText(1025, 580, "Press L to load a saved map.");				// Show the instructions
		afont->BlitText(1025, 500, "Press S to save the map.");					// Show the instructions
		afont->BlitText(1025, 420, "Press ENTER to find the goal.");			// Show the instructions
		break;
	case GameMode::SHOWING:														// Game state showing
		map->Draw();															// Draw the map
		afont->BlitText(1025, 820, "Finding path...");							// Show the text that the rover is finding the path
		afont->BlitText(1025, 740, "Battery: " + std::to_string(map->battery));	// Show the remaining battery for the day
		afont->BlitText(1025, 660, "Days: " + std::to_string(map->days));		// Show the Elapsed days
		if (!map->isAnimating)													// Check if the map stopped animating
		{
			if (roverApplication->FoundIt())									// Check if the rover found the goal
			{
				afont->BlitText(1025, 580, "The Rover found the goal.");		// Show the message that it was found
			}
			else
			{
				afont->BlitText(1025, 580, "The Rover didn't find the goal.");	// Show the message that it wasnt found
			}
			
			afont->BlitText(1025, 500, "Press enter to continue.");				// Show the instruction to continue
		}
		break;

	case GameMode::PICKING:														// Game state Picking
		tileMap->Blit(532, blit3D->screenHeight - 232);							// draw the tilemap
		map->blitRover(516, blit3D->screenHeight - 280);						// Draw the rover tile
		map->blitGoal(548, blit3D->screenHeight - 280);							// Draw the goal tile

		coords = "X:" + std::to_string(x_tile);									// Prepare the X coordinate text
		afont->BlitText(500, 480, coords);										// Show the text
		coords = "Y:" + std::to_string(y_tile);									// Prepare the Y coordinate text
		afont->BlitText(500, 400, coords);										// Show the text

		afont->BlitText(10, 80, "Pick a tile to draw with.");					// Show the isntructions
		break;
	}
}
/**
* Method that handles the input
* @param key The key pressed
* @param scancode The code of the key scanned
* @param action The action taken
* @param mods Unknown and unused variable
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-28
*/
void DoInput(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)		// Escape Key
	{
		blit3D->Quit();										//start the shutdown sequence
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)	// Space Key
	{
		switch (gameMode)									// Handle differently depending on the game state
		{
		case GameMode::EDITING:								// Editing mode
			gameMode = GameMode::PICKING;					// Switch mode to Picking
			break;

		case GameMode::PICKING:								// Picking mode
			gameMode = GameMode::EDITING;					// Switch mode to editing
			break;
		}
	}

	if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)						// Enter key
	{
		switch (gameMode)														// Handle differently depending on the game state
		{
		case GameMode::EDITING:													// Editing mode
			roverApplication->LoadMap(map->GetMap());							// Load the map on the interface to the rover application
			roverApplication->SetRoverLocation(map->GetStartLocation());		// Set the start location on the application
			{
				boolean fountIt = roverApplication->FindEndingPoint();			// Find the goal
				std::cout << roverApplication->PathToString() << std::endl;		// Show the map on console
				if (fountIt)													// Check if the goal was found
				{
					std::cout << "Found it!!" << std::endl;						// Print on console that the goal was found
				}
				else
				{
					std::cout << "Didnt find it!!" << std::endl;				// Print on console that the goal wasnt found
				}
			}
			map->SetPath(roverApplication->GetPath());							// Set the path from the application to the interface
			map->BeginAnimation();												// Start the animation
			gameMode = GameMode::SHOWING;										// Switch to showing mode
			break;
		case GameMode::SHOWING:													// Showing mode
			gameMode = GameMode::EDITING;										// Switch to editing mode
			break;
		}
	}

	if (key == GLFW_KEY_R && action == GLFW_RELEASE)															// R key
	{
		switch (gameMode)																						// Handle differently depending on the game state
		{
		case GameMode::EDITING:																					// Editing mode
			map->LoadRandomMap(roverApplication->GetMapWidth(), roverApplication->GetMapHeight(), 3, 1.0f);		// Load a random map
			break;
		}
		
	}
	if (key == GLFW_KEY_E && action == GLFW_RELEASE)															// E Key
	{
		switch (gameMode)																						// Handle differently depending on the game state
		{
		case GameMode::EDITING:																					// Editing mode
			map->LoadEmptyMap(roverApplication->GetMapWidth(), roverApplication->GetMapHeight());				// Load an empty map
			break;
		}
	}

	if (key == GLFW_KEY_L && action == GLFW_RELEASE)															// L Key
	{
		switch (gameMode)																						// Handle differently depending on the game state
		{
		case GameMode::EDITING:																					// Editing mode
			map->LoadMap(roverApplication->GetMapWidth(), roverApplication->GetMapHeight(), "mapfile.dat");		// Load map from file
			break;
		}
	}

	if (key == GLFW_KEY_S && action == GLFW_RELEASE)			// S key
	{
		switch (gameMode)										// Handle differently depending on the game state
		{
		case GameMode::EDITING:									// Editing mode			
			map->SaveMap("mapfile.dat");						// Save map into file
			break;
		}
	}

	if (gameMode == GameMode::EDITING)							// Editing mode
	{
		if (key == GLFW_KEY_UP && action == GLFW_PRESS)			// Press Up Key 
		{
			map->scrollUp = true;								// Move the map up
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)		// Press Down key
		{
			map->scrollDown = true;								// Move the map down 
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)		// Press Left Key
		{
			map->scrollLeft = true;								// Move the map left
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)		// Press right key
		{
			map->scrollRight = true;							// Move the map to the right
		}
		if (key == GLFW_KEY_UP && action == GLFW_RELEASE)		// Release up key
		{
			map->scrollUp = false;								// Stop moving up
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)		// Release down key
		{
			map->scrollDown = false;							// Stop moving down
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)		// Release left key
		{
			map->scrollLeft = false;							// Stop moving left
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)	// Release right key
		{
			map->scrollRight = false;							// Stop moving right
		}
	}
}
/**
* Method that handles the mous cursor position
* @param x the mouse position in the x axis
* @param y The mouse position in the y axis
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-28
*/
void DoCursor(double x, double y)
{
	cursor_x = blit3D->screenWidth / blit3D->trueScreenWidth * (float)x;					// Convert the position from screen to Blit metrics
	cursor_y = blit3D->screenHeight / blit3D->trueScreenHeight * (float)y;					// Convert the position from screen to Blit metrics
	switch (gameMode)																		// Handle differently for each state
	{
	case GameMode::EDITING:																	// Editing mode
		x_tile = (int)(cursor_x / map->tileSize);														// Get the tile X position
		y_tile = (int)(cursor_y / map->tileSize);														// Get the tile Y position
		x_tile += map->offsetX;																// Update the tile X position with the map offset
		y_tile += map->offsetY;																// Update the tile Y position with the map offset
		if (cursor_x >= map->maxDisplaySize * map->tileSize || cursor_y >= map->maxDisplaySize * map->tileSize) {	// Check if the cursor is out of bounds
			x_tile = -1;																	// Set the tile to -1
			y_tile = -1;																	// Set the tile to -1
		}
		break;
	case GameMode::PICKING:																	// Picking mode
		x_tile = (int)((cursor_x - 500) / map->tileSize);									// Get the tile X location
		y_tile = (int)((cursor_y - 200) / map->tileSize);									// Get the tile Y location
		if (cursor_x >= 564  || cursor_x < 500 || cursor_y >= 296 || cursor_y < 200) {		// If the tile cursor is out of bounds
			x_tile = -1;																	// Set the tile to -1
			y_tile = -1;																	// Set the tile to -1
		}
		break;
	}
}
/**
* Method that handles the mouse clicks
* @param button The mouse button pressed
* @param action The action taken
* @param mods Unknown and unused variable
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-28
*/
void DoMouseButton(int button, int action, int mods)
{
	switch (gameMode)																// Handle differently depending on the mode								
	{
	case GameMode::EDITING:															// Editing mode
		if ((button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)				// Left mouse button
			|| (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)) 		// Right mouse button	
		{
			
			if (x_tile != -1 && y_tile != -1) {										// if it's out of the image, set it to -1
				map->SetTile(x_tile + y_tile * map->GetWidth());					// Set the map's tile
			}
		}
		break;

	case GameMode::PICKING:															// Picking mode
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)				// Left button
		{
			if (x_tile == -1 || y_tile == -1) {										// if it's out of the image, set it to -1
				map->SetEditTile(0);												// Set the tile to the traversable tile
			}
			else {
				map->SetEditTile(x_tile + y_tile * 2);								// Set the tile to the corresponding sprite
				gameMode = GameMode::EDITING;										// Change to editing mode
			}
		}
		break;
	}
}
/**
* Method that is called at the beginning of the application
* @param argc Not used
* @param argv Not used
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-28
*/
int main(int argc, char *argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//set X to the memory allocation number in order to force a break on the allocation:
	//useful for debugging memory leaks, as long as your memory allocations are deterministic.
	//_crtBreakAlloc = 378951;

	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN_1080P, 640, 400);		// Instantiate the Blit3D object

	blit3D->SetInit(Init);						// Set Init
	blit3D->SetDeInit(DeInit);					// Set DeInit
	blit3D->SetUpdate(Update);					// Set Update
	blit3D->SetDraw(Draw);						// Set Draw
	blit3D->SetDoInput(DoInput);				// Set DoInput
	blit3D->SetDoCursor(DoCursor);				// Set DoCursor
	blit3D->SetDoMouseButton(DoMouseButton);	// Set DoMouseButton

	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);		// Run Blit3D
	if (blit3D) delete blit3D;							// Delete Blit3D
}