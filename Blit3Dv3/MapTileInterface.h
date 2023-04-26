#pragma once

#include <vector>
#include <queue>
#include <Blit3D.h>

#include "TileMap.h"

/**
* A class that represents a Rover mapTile in the interface.
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-19
*/
class MapTileInterface
{
private:
	/*
	* Variables created for the loops
	*/
	unsigned int xCounter;
	unsigned int yCounter;
	float roverX;
	float roverY;
	/**
	* A pointer to Blit3D
	*/
	Blit3D* blit3D;
	/**
	* The Tile map
	* -2 Finish
	* -1 Start
	* 0 Clear tile
	* > 0 Obstacles
	* <= 0 traversable
	*/
	std::vector<char> map;
	/**
	* The Tile list
	*/
	std::vector<Sprite*> tileList;
	/**
	* The animation path the rover will take
	*/
	std::queue<unsigned int> animationPath;
	/**
	* The map's width
	*/
	unsigned int width = 0;
	/**
	* The map's height
	*/
	unsigned int height = 0;
	/**
	* The start position
	*/
	unsigned int start = 0;
	/**
	* The finish position
	*/
	unsigned int end = 0;
	/**
	* The end X offset when displaying the map
	*/
	unsigned int endOffsetX;
	/**
	* The end Y offset when displaying the map
	*/
	unsigned int endOffsetY;
	/**
	* The Rover's location
	*/
	unsigned int roverLocation;
	/**
	* The previous step in the animation
	*/
	unsigned int previousStep;
	/**
	* The Rover's direction
	*/
	float roverDirection;
	/**
	* The animation pfrogress
	*/
	float animationProgress;
	/**
	 * The Random device.
	*/
	std::mt19937 rng;
public:
	/**
	* Whether it is scrolling to the rigt
	*/
	bool scrollRight = false;
	/**
	* Whether it is scrolling to the left
	*/
	bool scrollLeft = false;
	/**
	* Whether it is scrolling to the top
	*/
	bool scrollUp = false;
	/**
	* Whether it is scrolling to the bottom
	*/
	bool scrollDown = false;
	/**
	* The X offset when displaying the map
	*/
	int offsetX = 0;
	/**
	* The Y offset when displaying the map
	*/
	int offsetY = 0;
	/**
	* The tile to use when editing
	*/
	unsigned int editTileNum;
	/**
	* Whether or not the rover is animating
	*/
	bool isAnimating;
	/**
	* The max display size in the screen
	*/
	unsigned int maxDisplaySize;
	/**
	* The tile size
	*/
	unsigned int tileSize;
	/**
	* Battery credits
	*/
	unsigned int battery;
	/**
	* Days elapsed
	*/
	unsigned int days;
	/**
	* The MapTileInterface constructor
	* @param blit3D A pointer to Blit3D
	* @return A new instance of the class MapTileInterface
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	MapTileInterface(Blit3D* blit3D);
	/**
	* The MapTileInterface de-constructor
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	~MapTileInterface();
	/**
	* Loads a random map.
	* @param width The map's width.
	* @param height The map's height;
	* @param totalTiles The amount of total tiles the map has.
	* @param probability The probability for a clear tile to appear.
	* @return True if the map was loaded.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	bool LoadRandomMap(unsigned int width, unsigned int height, unsigned char totalTiles, float probability);
	/**
	* Loads an empty map.
	* @param width The map's width.
	* @param height The map's height;
	* @return True if the map was loaded.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	bool LoadEmptyMap(unsigned int width, unsigned int height);
	/**
	* Loads a map definition from a file.
	* @param filename The file to read from.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	bool LoadMap(unsigned int width, unsigned int height, std::string filename);
	/**
	* Saves the map to a file.
	* @param filename The file to save to.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	bool SaveMap(std::string filename);
	/**
	* Parses a line from the file and fills the corresponging information into the TileMap
	* @param phrase The lien to parse.
	* @param separator The separator character.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	std::string ReadWord(std::string& phrase, char separator);
	/**
	* Gets a random tile number
	* @param totalTiles The amount of total tiles the map has.
	* @param probability The probability for a clear tile to appear.
	* @return The random tile number
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	int GetTileNumberFromRandom(unsigned char totalTiles, float clearTileProbabilty);
	/**
	* Sets random start and end locations
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	void SetRandomSartAndEnd();
	/**
	* Gets the vector map.
	* @return The vector map.
	*  @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	std::vector<char> GetMap();
	/**
	* Gets the starting location.
	* @return The starting location in the vector map.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	unsigned int GetStartLocation();
	/**
	* Returns the map's width
	* @return The map's width
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	unsigned int GetWidth();
	/**
	* Returns the map's height
	* @return The map's height
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	unsigned int GetHeight();
	/**
	* Returns the map tile at the specified location.
	* @param location The tile's location
	* @return The map's tile at the specified location.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	char GetTile(unsigned int location);
	/**
	* Sets the tile to the selected tile number
	* @param location the location in the map 
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	void SetTile(unsigned int location);
	/**
	* Sets the edit tile from the spritesheet location
	* @param number The number of the tile in the sprite sheet.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	void SetEditTile(unsigned char number);
	/**
	* Returns the sprite in the tileList given a index number.
	* @param tileNumber The tile Sprite index to return
	* @return The tile Sprite in the index location.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	Sprite* GetTileList(unsigned int tileNumber);
	/**
	* Sets the path to animate.
	* @param path The path to animate
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-20
	*/
	void SetPath(std::vector<unsigned int> path);
	/**
	* Emptied the path
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-25
	*/
	void emptyPath();
	/**
	* Draws the Map
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	void Draw();
	/**
	* Updates the Map
	* @param elapsedTime The time that has passed since last update
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	void Update(float elapsedTime);
	/**
	* Paints the rover in the screen at the specified positions
	* @param xCoordinate The X coordinate
	* @param yCoordinate The Y coordinate
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	void blitRover(unsigned int xCoordinate, unsigned int yCoordinate);
	/**
	* Paints the goal in the screen at the specified positions
	* @param xCoordinate The X coordinate
	* @param yCoordinate The Y coordinate
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	void blitGoal(unsigned int xCoordinate, unsigned int yCoordinate);
	/**
	* Begind the animation.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-20
	*/
	void BeginAnimation();
	/**
	* Sets the rovers sprite direction based on the current location and the next one.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-20
	*/
	void SetRoverDirection();
	/**
	* Takes a rover's step on the animation.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-25
	*/
	void TakeAStep();
};