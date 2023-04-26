#include "TileMap.h"

/**
* See TileMap.h for method definitions
*/

TileMap::TileMap()
{
}
void TileMap::DeInit()
{
	std::vector<MapNode*>::iterator iter = this->map.begin();
	for (iter; iter < this->map.end(); iter++)
	{
		if (*iter != NULL) delete *iter;
	}
}
bool TileMap::LoadMap(std::vector<char> map)
{
	MapNode* tempNode;
	char tile;																		// Ceclare the tile variable
	for (unsigned int mapCounter = 0; mapCounter < this->width * this->height; ++mapCounter)				// Loop on the map
	{
		tempNode = new MapNode();
		
		if(mapCounter < map.size())													// Check if the counter is still within bounds
		{
			tile = map[mapCounter];													// Get the foreground number
			
			
			
			if (tile > 0)															// Check if the tile has obstacles
			{
				tempNode->w = 1;														// Add an obstacle
			}
			else
			{
				tempNode->w = 0;														// Add the tile to the map
			}
			if (tile == -1)															// Check if the tile is the starting tile
			{
				this->start = mapCounter;											// Set the start tile
			}
			if (tile == -2)															// Check if the tile is the ending tile
			{
				this->end = mapCounter;												// Set the end tile
			}
		}
		else
		{
			tempNode->w = 0;															// Add the tile to the map
		}
		tempNode->position = mapCounter;
		this->map.push_back(tempNode);
	}
	return true;																	// Return as success
}
bool TileMap::ClearMap()
{
	this->map.clear();																// Clear the map
	return true;																	// Return true
}
unsigned int TileMap::GetStartPoint()
{
	return this->start;																// Return the start point
}
unsigned int TileMap::GetEndPoint()
{
	return this->end;																// Return the end point
}
unsigned int TileMap::GetWidth()
{
	return this->width;																// Return the map's width
}
unsigned int TileMap::GetHeight()
{
	return this->height;															// Returnt he map's height
}
unsigned int TileMap::GetSize()
{
	return this->map.size();														// Return the map's size
}
unsigned char TileMap::GetValue(unsigned int position)
{
	unsigned int value;																// Declare the value variable
	if (position < 0 || position >= this->map.size())								// Check is the position is outside bounds
	{
		value = 1;																	// Set it to 1
	}
	else
	{
		value = this->map[position]->w;												// Set it to the map's value in the position
	}
	return value;																	// Return the value
}
void TileMap::SetValueOnLocation(unsigned int location, unsigned char value)
{
	this->map[location]->w = value;													// Return the value in the specified location
}
unsigned char TileMap::GetFrontLocationValue(unsigned int location, unsigned char direction)
{
	unsigned int forntalLocation = this->GetFrontLocation(location, direction);		// Decalre the forntal location and set it with the corresponging value
	unsigned char value = 1;														// Declare value and set it to 1
	if (forntalLocation < 0 || forntalLocation >= this->map.size())					// Check if the forntal location is out of bounds
	{
		value = 1;																	// Set the value to 1
	}
	else
	{
		value = this->map[forntalLocation]->w;											// Set the value to the value in the map's location
	}
	return value;																	// Returnt he value
}
unsigned int TileMap::GetFrontLocation(unsigned int location, unsigned char direction)
{
	unsigned int forntalLocation = -1;												// Declare frontalLoaction and set it to the max number
	if (direction == 1 && location % this->width != this->width - 1)				// Check if it is facing east amd it's not at the end of the row
	{
		forntalLocation = location + 1;												// the frontal location is the next element
	}
	if (direction == 3 && location % this->width != 0)								// Check if it is facing west and it's not ad the beginning of the row
	{
		forntalLocation = location - 1;												// The forntal location is the previous location
	}
	if (direction == 0 && location >= this->width)									// Check if it is facing north and not in the first row
	{
		forntalLocation = location - this->width;									// The frontal location is one width above
	}
	if (direction == 2 && location <= this->width * (this->height - 1))				// Check is it's facing south and not in the last row
	{
		forntalLocation = location + this->width;									// The frontal location is with more
	}
	return forntalLocation;															// Return the forntal location
}
unsigned char TileMap::GetBackLocationValue(unsigned int location, unsigned char direction)
{
	unsigned int backLocation = this->GetBackLocation(location, direction);			// Declare the bacl location and set it accordingly
	unsigned char value = 1;														// Declare value and set it to 1
	if (backLocation < 0 || backLocation >= this->map.size())						// Check if the back location is out of bounds
	{
		value = 1;																	// Set the value to 1
	}
	else
	{
		value = this->map[backLocation]->w;											// Set the value to the value in the map's location
	}
	return value;																	// Return the value
}
unsigned int TileMap::GetBackLocation(unsigned int location, unsigned char direction)
{
	unsigned int backLocation = -1;													// Declare backLoaction and set it to the max number
	if (direction == 1 && location % this->width != 0)								// Check if it is facing east amd it's not at the end of the row
	{
		backLocation = location - 1;												// the location is the one before
	}
	if (direction == 3 && location % this->width != this->width - 1)				// Check if it is facing west and it's not ad the beginning of the row
	{
		backLocation = location + 1;												// The location ist he one after
	}
	if (direction == 0 && location <= this->width * (this->height - 1))				// Check if it is facing north and not in the first row
	{
		backLocation = location + this->width;										// The location is the one with after
	}
	if (direction == 2 && location >= this->width)									// Check is it's facing south and not in the last row
	{
		backLocation = location - this->width;										// The location is the one with before
	}
	return backLocation;															// Return the location
}

float TileMap::DistanceToGoal(int position)
{
	float positionX = position % this->width;
	float positionY = position / this->width;
	float destinationX = this->end % this->width;
	float destinationY = this->end / this->width;
	float xDistance = destinationX - positionX;
	float yDistance = destinationY - positionY;

	return sqrt(xDistance * xDistance + yDistance * yDistance);
}