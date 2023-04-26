#include "Rover.h"
#include <cmath>
#include <queue>
#include <bitset>

/**
* See Rover.h for method definitions
*/

#define _PRIORITY_NUMBER 3
#define _CARDINAL_TOTAL 4

Rover::Rover()
{
	this->map = new TileMap();													// Create the Tile Map
	this->breadcrumsMap = new TileMap();										// Create the breadcrums (BC) map
	this->rover = new MarsRover(0);												// Create the Rover
	this->stackSize = 0;														// reset the stack size
}
void Rover::DeInit()
{
	this->map->DeInit();
	delete(this->map);															// Delete the map
	delete(this->breadcrumsMap);												// Delete the breadcrums map
	delete(this->rover);														// Delete the rover	
	
	this->closed.clear();
	this->open.clear();
	
	this->path.clear();
}
void Rover::LoadMap(std::vector<char> map)
{
	this->foundIt = false;														// Resets the finding variable
	this->map->LoadMap(map);													// Loads the map
	this->path.clear();															// Clear the path
	this->recursivePath.clear();												// Clear the recursive path
	this->path.push_back(this->map->GetStartPoint());							// Add the starting point
	this->recursivePath.push_back(this->map->GetStartPoint());					// Add the starting point to the recursive path
	this->breadcrumsMap->ClearMap();											// Clear the breadcrums map
	this->stackSize = 0;														// reset the stack size
}
void Rover::SetRoverLocation(unsigned int newLocation)
{
	this->rover->SetLocation(newLocation);										// Set the rover's location
}
MarsRover* Rover::GetRover()
{
	return this->rover;															// return the rover
}
unsigned int Rover::GetMapWidth()
{
	return this->map->GetWidth();												// return the map's width
}
unsigned int Rover::GetMapHeight()
{
	return this->map->GetHeight();												// Return the map's height
}
std::vector<unsigned int> Rover::GetPath()
{
	return this->path;															// Return thr path
}
bool Rover::FoundIt()
{
	return this->foundIt;														// Return true if the rover has been found
}
bool Rover::FindEndingPoint()
{
	this->path.clear();
	this->open.clear();
	this->closed.clear();
	MapNode* n;
	MapNode* neighbor;
	float newg;
	int nextPosition;
	this->foundIt = false;																				// Set the initial value of not found
	// Find the optimum path with A*
	// Set the start node and add it to Open
	this->map->map[this->map->start]->g = 0;
	this->map->map[this->map->start]->h = this->map->DistanceToGoal(this->map->start);
	this->map->map[this->map->start]->f = this->map->map[this->map->start]->g + this->map->map[this->map->start]->h;
	this->map->map[this->map->start]->parent = NULL;
	this->open.push_back(this->map->map[this->map->start]);

	while (!this->open.empty())
	{
		n = this->PopLowestF(this->open);
		if (this->map->map[this->map->end]->position == n->position)
		{
			this->ConstructPath();
			this->foundIt = true;
			return this->foundIt;
		}
		for (int directionCoutner = 0; directionCoutner < 4; directionCoutner++)
		{
			nextPosition = this->GetNextPosition(n->position, directionCoutner);
			if (nextPosition >= 0 && nextPosition < this->map->map.size())
			{
				neighbor = this->map->map[nextPosition];
				neighbor->position = nextPosition;
				newg = n->g + this->Cost(n, neighbor);
			}

			// Dont do anything if the neightbor is out of bounds, an obstacle or if it's cheaper arriving to the neighbor
			if (nextPosition <= -1 || neighbor->w == 1|| ((this->HasNode(this->closed, nextPosition) || this->HasNode(this->open, nextPosition)) && neighbor->g <= newg) )
			{
				// SKIP
			}
			else
			{
				neighbor->parent = n;
				neighbor->g = newg;
				neighbor->h = this->map->DistanceToGoal(neighbor->position);
				neighbor->f = neighbor->g + neighbor->h;
				if (this->HasNode(this->closed, neighbor->position))
				{
					this->Remove(this->closed, neighbor);
				}
				if (!this->HasNode(this->open, neighbor->position))
				{
					this->open.push_back(neighbor);
				}
			}
		}
		this->closed.push_back(n);
	}

	
	return this->foundIt;																				// Return the rover found the goal
}
void Rover::ConstructPath()
{
	MapNode* node = this->map->map[this->map->end];
	this->path.clear();
	this->path.push_back(node->position);
	while (node->parent != NULL)
	{
		node = node->parent;
		this->path.push_back(node->position);
		std::rotate(this->path.rbegin(), this->path.rbegin() + 1, this->path.rend());
	}
	
}
std::string Rover::PathToString()
{
	std::string pathTotString = "Path:";													// Create a string to return the path
	for (unsigned int mapCounter = 0; mapCounter < this->map->GetSize(); mapCounter++)		// Loop through the maps size
	{
		if (mapCounter % this->map->GetWidth() == 0)										// Check if it's the beginning of a line
		{
			pathTotString += '\n';															// Add a line breack
		}
		if (this->map->GetStartPoint() == mapCounter)										// Check if it's the start pont
		{
			pathTotString += 'S';															// Add an S
		}
		else if (this->map->GetEndPoint() == mapCounter)									// Check if its the end point
		{
			pathTotString += 'E';															// Add an E
		}
		else if (this->rover->location == mapCounter)										// Check if it's the rover's location
		{
			pathTotString += 'R';															// Add an R
		}
		else if (this->map->GetValue(mapCounter) > 0)										// Check if it's a blocking tile
		{
			pathTotString += 'X';															// Add an X
		}
		else if (this->breadcrumsMap->GetValue(mapCounter) > 0)								// Check if there's a breadcrum
		{
			pathTotString += '.';															// add a .
		}
		else
		{
			pathTotString += ' ';															// Otherwise, add a space
		}
	}
	return pathTotString;																	// Return the path
}
bool Rover::SearchRecursively()
{
	this->stackSize++;																		// Increase the stack counter 3000 is a safe place to stop
	if (this->stackSize > 2000)																// Check if the stack is about to fill
	{
		this->stackSize = 0;
		return false;
	}
	if (this->rover->location == this->map->GetEndPoint())														// Check if the rover arrived to the goal
	{
		return true;																							// Set the found variable to true
	}
	this->breadcrumsMap->SetValueOnLocation(this->rover->location, 1);											// Place a breacrum
	if (this->GetNextViableDirection(this->rover->GetDirection(), this->map->GetEndPoint()))					// Check if there's an available location to move
	{
		this->MoveRoverTo(this->map->GetFrontLocation(this->rover->location, this->rover->GetDirection()));		// Move the rover forward
		return this->SearchRecursively();																		// Call recursively
	}
	else
	{
		if (!this->recursivePath.empty())																		// Make sure the recursive path is not empty
		{
			this->recursivePath.pop_back();																		// Take out the last element of the recursive path
			if (!this->recursivePath.empty())																	// Make sure the recursive path is not empty again
			{

				this->rover->SetLocation(this->recursivePath.back());											// Set the rover's location to the previous location
				this->path.push_back(this->recursivePath.back());												// Add the location to the path
			}
			this->stackSize--;																					// Clear one stack call
		}
		return false;																							// Return false
	}
}
unsigned char Rover::GetNextDirection(unsigned char directionCounter, unsigned char roversDirection, unsigned int finalLocation)
{
	unsigned char first, next;																										// Declare the first and next variables
	float direction, nextDirection, slope;																							// Declare the direction, next direction and slope variables
	float deltaX = (float)(finalLocation % this->map->GetWidth()) - (float)(this->rover->location % this->map->GetWidth());			// Declare the change between the rover's location and final location in the X axis 
	float deltaY = std::floor(finalLocation / this->map->GetWidth()) - std::floor(this->rover->location / this->map->GetWidth());	// Declare the change between the rover's location and final location in the Y axis
	if (deltaX == 0)											// Check if the change in X is 0
	{
		deltaX = 0.0001;										// make it a small amount
	}
	if (deltaY == 0)											// Check if the change in Y is 0
	{
		deltaY = 0.0001;										// make it small amount
	}
	slope = deltaY / deltaX;									// Calculate the slope
	
	if (std::abs(slope) <= 1)									// check if the absolute value of the slope is less or equal to 1
	{
		if (deltaX <= 0)										// Check if the change in X is negative
		{
			first = 3;											// better moving west
			if (deltaY <= 0)									// Check if the target is to the south
			{
				direction = -1;									// Make the direction counter clockwise
			} else
			{
				direction = 1;									// Make the direction clockwise
			}
		}
		else
		{
			first = 1;											// better moving east
			if (deltaY <= 0)									// Check if the target is to the south
			{
				direction = 1;									// Make the direction clockwise
			}
			else
			{
				direction = -1;									// Make the direction counter clockwise
			}
		}
		
	}
	else
	{
		if (deltaY <= 0)										// Check if the change in Y is less or equal to 0
		{
			first = 0;											// better moving south
			if (deltaX <= 0)									// Check if the target is to the south
			{
				direction = -1;									// Make the direction counter clockwise
			}
			else
			{
				direction = 1;									// Make the direction clockwise
			}
		}
		else
		{
			first = 2;											// better moving North
			if (deltaX <= 0)									// Check if the target is to the south
			{
				direction = 1;									// Make the direction clockwise
			}
			else
			{
				direction = -1;									// Make the direction counter clockwise
			}
		}
	}
	nextDirection = first + (directionCounter * direction);		// Get the next direction by adding to the first the direction counter
	
	if (nextDirection < 0)										// check the next direction is less than 0
	{
		nextDirection += _CARDINAL_TOTAL;						// Add 4 to it
	}
	next = (unsigned char)nextDirection % _CARDINAL_TOTAL;		// Get the module 4 in case it is creater than 3
	return next;												// Return the next direction
}
void Rover::MoveRoverTo(unsigned int location)
{
	this->rover->SetLocation(location);								// Set the rover's location to the specified location
	this->path.push_back(location);									// Add the location to the path
	this->recursivePath.push_back(location);						// Add the location to the Recursive path
}
bool Rover::CanTurn()
{
	return (this->map->GetValue(this->rover->location + 1) <= 0 && this->breadcrumsMap->GetValue(this->rover->location + 1) <= 0)														// It can move east
		|| (this->map->GetValue(this->rover->location - 1) <= 0 && this->breadcrumsMap->GetValue(this->rover->location - 1) <= 0)														// It can move west
		|| (this->map->GetValue(this->rover->location + this->map->GetWidth()) <= 0 && this->breadcrumsMap->GetValue(this->rover->location + this->breadcrumsMap->GetWidth()) <= 0)		// It can move south
		|| (this->map->GetValue(this->rover->location - this->map->GetWidth()) <= 0 && this->breadcrumsMap->GetValue(this->rover->location - this->breadcrumsMap->GetWidth()) <= 0);	// It can move north
}
unsigned char Rover::GetNextViableDirection(unsigned char previousDirection, unsigned int finalDestination)
{
	bool isFound = false;																										// Declare the return value
	for (unsigned char directionCounter = 0; directionCounter < _CARDINAL_TOTAL && !isFound; directionCounter++)				// Loop through the directions
	{
		this->rover->TurnToDirection(this->GetNextDirection(directionCounter, previousDirection, finalDestination));	// Get the next diretion and turn the rover that way
		if (!isFound																											// Check if the direction hasn't been found
			&& this->breadcrumsMap->GetFrontLocationValue(this->rover->location, this->rover->GetDirection()) != 1				// Make sure that in fron of the rover there's no breadcrums
			&& this->map->GetFrontLocationValue(this->rover->location, this->rover->GetDirection()) <= 0)						// Make sure there's no blocks ahead
		{
			isFound = true;																										// Call recursively
		}
	}
	return isFound;																												// Return True if the directionw as found
}

MapNode* Rover::PopLowestF(std::vector<MapNode*> &list)
{
	MapNode* lowest = list[0];
	int lowestPosition = -1;
	for (int counter = 0; counter < list.size(); counter++)
	{
		if (list[counter]->f < lowest->f)
		{
			lowest = list[counter];
			lowestPosition = counter;
		}
	}
	this->Remove(list, lowest);
	return lowest;
}

int Rover::GetNextPosition(int position, int direction)
{
	int newPosition = -1;
	switch (direction)
	{
	case 0: // North
		newPosition = position - this->map->width;
		if (newPosition < 0)
		{
			newPosition = -1;
		}
		break;
	case 1: // East
		if (position % this->map->width == this->map->width - 1)
		{
			newPosition = -1;
		}
		else
		{
			newPosition = position + 1;
		}
		break;
	case 2: // South
		newPosition = position + this->map->width;
		if (newPosition >= this->map->width * this->map->height)
		{
			newPosition = -1;
		}
		break;
	case 3: // West
		if (position % this->map->width == 0)
		{
			newPosition = -1;
		}
		else
		{
			newPosition = position - 1;
		}
		break;
	default:
		break;
	}

	return newPosition;
}

int Rover::Cost(MapNode* a, MapNode* b)
{
	return 1;
}

bool Rover::HasNode(std::vector<MapNode*> list, int position)
{
	for (MapNode* currentNode : list)
	{
		if (currentNode->position == position)
		{
			return true;
		}
	}
	return false;
}

void Rover::Remove(std::vector<MapNode*> &list, MapNode* node)
{
	std::vector<MapNode*>::iterator iter = list.begin();
	for(iter; iter < list.end(); iter++)
	{
		if ((*iter)->position == node->position)
		{
			list.erase(iter);
			break;
		}
	}
}
