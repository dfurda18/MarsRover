#pragma once

#include <bitset>
#include <random>
#include <fstream>
#include <string>
#include <cassert>
#include <iostream>

#include "MapNode.h"

#define _SIZE_X 40// Map X size
#define _SIZE_Y 40// Map Y size
#define _TOTAL_SIZE _SIZE_X * _SIZE_Y // Map's total size

/**
* A 2D map with different types of obstacles.
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-11
*/
class TileMap {	
public:
	/**
	* The map's width
	*/
	unsigned int width = _SIZE_X;
	/**
	* The map's height
	*/
	unsigned int height = _SIZE_Y;
	/**
	* The Tile map
	* -2 Finish
	* -1 Start
	* 0 Clear tile
	* > 0 Obstacles
	* <= 0 traversable
	*/
	std::vector<MapNode*> map;
	/**
	* The start position
	*/
	unsigned int start;
	/**
	* The finish position
	*/
	unsigned int end;
	/**
	* The Empty Map constructor
	* @return A new instance of the class TileMap
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	TileMap();
	/**
	* The Map de-constructor
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	void DeInit();
	/**
	* Loads a map definition from a file.
	* @param map The map to load as a vector.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	bool LoadMap(std::vector<char> map);
	/**
	* Clears the map
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	bool ClearMap();
	/**
	* Gets the start point Location (value -1)
	* @return The Location of the starting point
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	unsigned int GetStartPoint();
	/**
	* Gets the end point Location (value -1)
	* @return The Location of the ending point
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	unsigned int GetEndPoint();
	/**
	* Returns the map's width
	* @return The map's width
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	unsigned int GetWidth();
	/**
	* Returns the map's height
	* @return The map's height
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	unsigned int GetHeight();
	/**
	* Returns the map size
	* @return The map size.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	unsigned int GetSize();
	/**
	* Returns the value in the specified map position
	* @param position The position to check the value
	* @return The value in the specified position.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	unsigned char GetValue(unsigned int position);
	/**
	* Sets the map's tile to a specified value in the given location
	* @param location The location to set the tile
	* @param value The tile's value
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	void SetValueOnLocation(unsigned int location, unsigned char value);
	/**
	* Gets the value in the location in front of certain location and direction.
	* @return The location in front of the specified location.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	unsigned char GetFrontLocationValue(unsigned int location, unsigned char direction);
	/**
	* Gets the location in front of certain location and direction.
	* @return The location in front of the specified location.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	unsigned int GetFrontLocation(unsigned int location, unsigned char direction);
	/**
	* Gets the value in the location in the back of certain location and direction.
	* @return The location in the back of the specified location.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	unsigned char GetBackLocationValue(unsigned int location, unsigned char direction);
	/**
	* Gets the location in the back of certain location and direction.
	* @return The location in the back of the specified location.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	unsigned int GetBackLocation(unsigned int location, unsigned char direction);
	/**
	* Calculates the distance from the position to the end node
	* @param position The starting position
	* @return The distance between both positions
	* @author Dario Urdapilleta
	* @since 2023-04-13
	*/
	float DistanceToGoal(int position);
};
