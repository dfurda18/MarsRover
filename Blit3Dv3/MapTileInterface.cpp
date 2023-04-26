#include "MapTileInterface.h"

/**
* See MapTileInterface.h for method definitions
*/

#define _TILE_SIZE 32				// Tile size 32px
#define _SPRITE_SHEET_WIDTH 64		// Spritesheet width 64px
#define _SPRITE_SHEET_HEIGHT 64		// Spritesheet height 64px
#define MAX_DISPLAY_SIZE 32			// Max display size in the interface
#define ANIMATION_TIME_SLICE 0.15f	// How fast the animation goes

MapTileInterface::MapTileInterface(Blit3D* blit3D)
{
	this->blit3D = blit3D;																				// Set the the pointer to Blit3D
	this->maxDisplaySize = MAX_DISPLAY_SIZE;															// Set the mas display size
	this->tileSize = _TILE_SIZE;																		// Set the mas tile size
	std::random_device randomDevice;																	// Create a low efficiency, but highly random results, random device
	rng.seed(randomDevice());																			// Seed the random with a random value
	for (yCounter = 0; yCounter < _SPRITE_SHEET_HEIGHT / this->tileSize; ++yCounter)					// Loop through the sprite sheet Y tiles
	{
		for (xCounter = 0; xCounter < _SPRITE_SHEET_WIDTH / this->tileSize; ++xCounter)					// Loop through the sprite sheet X tiles
		{
			tileList.push_back(blit3D->MakeSprite(xCounter * _TILE_SIZE, yCounter * _TILE_SIZE, _TILE_SIZE, _TILE_SIZE, "Media\\terrain.png"));	// Add the sprite to the sprite list
		}
	}
	tileList.push_back(blit3D->MakeSprite(0, 0, _TILE_SIZE, _TILE_SIZE, "Media\\rover.png"));			// Add the rover sprite to the sprite list
	tileList.push_back(blit3D->MakeSprite(0, 0, _TILE_SIZE, _TILE_SIZE, "Media\\goal.png"));			// Add the goal sprite to the spritelist
	this->editTileNum = 0;																				// Set the default value for the edit tile number
	this->isAnimating = false;																			// Don't animate in the begining
	this->animationProgress = 0;																		// Set a default value to the animation progress.
}
MapTileInterface::~MapTileInterface()
{
	// No need to delete anything
}
bool MapTileInterface::LoadRandomMap(unsigned int width, unsigned int height, unsigned char totalTiles, float probability)
{
	this->width = width;																		// Set the map's width
	this->height = height;																		// Set the map's height
	this->map.clear();																			// Clears the map vector
	for (unsigned int mapCounter = 0; mapCounter < this->height * this->width; ++mapCounter)	// loop on the map
	{
		map.push_back(this->GetTileNumberFromRandom(totalTiles, probability));					// Add the new random tile to the map vector
	}
	this->SetRandomSartAndEnd();																// Sets a random start and end locations
	return true;																				// Return as success
}
bool MapTileInterface::LoadEmptyMap(unsigned int width, unsigned int height)
{
	this->map.clear();																			// Clears the map vector
	for (unsigned int mapCounter = 0; mapCounter < this->height * this->width; ++mapCounter)	// loop on themap
	{
		map.push_back(0);																		// Add 0
	}
	this->SetRandomSartAndEnd();																// Sets a random start and end locations
	return true;																				// Return as success
}
bool MapTileInterface::LoadMap(unsigned int width, unsigned int height, std::string filename)
{
	std::string fileText;																	// Declare the file string
	std::ifstream MapFile(filename);														// Read the file
	if (MapFile)																			// Check if the file was successfuly found and is readable
	{
		try {
			this->map.clear();																// Clears the map vector
			char tile;																// Declare outside the loop
			std::getline(MapFile, fileText);												// Read the next line
			for (unsigned int mapCounter = 0; mapCounter < width * height; ++mapCounter)	// Loop on the map
			{
				tile = stoi(ReadWord(fileText, ' '));										// Get the foreground number
				this->map.push_back(tile);													// Add the tile
				if (tile == -1)																// Check if the tile is the starting tile
				{
					this->start = mapCounter;												// Set the start tile
				}
				if (tile == -2)																// Check if the tile is the ending tile
				{
					this->end = mapCounter;													// Set the end tile
				}
			}
		}
		catch (const std::invalid_argument& exception)										// Handle if the string is not a number
		{
			std::cerr << "Invalid argument: " << exception.what() << std::endl;				// Log the exception
			return false;																	// Return with an error
		}
		catch (const std::exception& exception)												// Handle if there's a general problem
		{
			std::cerr << "Undefined error: " << exception.what() << std::endl;				// Log the exception
			return false;																	// Return with an error
		}
	}
	else
	{
		std::cout << "ERROR! Reading the file.";											// Log that there was an error reading the file
		return false;																		// Return with an error
	}
	return true;																			// Return as success
}

bool MapTileInterface::SaveMap(std::string filename)
{
	int tempInt;
	std::ofstream mapFile(filename, std::ios::trunc);																// Read the file in the Streamming Assets folder
	bool isSaved = false;
	if (!mapFile.fail())																			// Check if the file has writing premissions
	{
		for (unsigned int mapCounter = 0; mapCounter < this->width * this->height; ++mapCounter)	// Loop on the map
		{
			tempInt = map[mapCounter];
			mapFile << std::to_string(tempInt);														// Write Map element into file
			if (mapCounter + 1 < this->map.size())													// Check if there are still more tiles to write in that tile
			{
				mapFile << " ";																		// Place an empy space if there's more to write
			}
		}
		mapFile.close();																			// Close the file stream
		isSaved = true;																				// Set the return vlaue to successful
	}
	else
	{
		std::cout << "ERROR! Writing the file.";													// Log the error
		isSaved = false;																			// Set the return vlaue to unsuccessful
	}
	return isSaved;																					// Return whether it was saved or not

}

std::string MapTileInterface::ReadWord(std::string& phrase, char separator) {
	std::string word = "";											// Declare where to store the string
	if (phrase.length() == 0) {										// Check if the phrase is empty
		return "0";													// The file is over, return empty tiles agter that
	}
	while (phrase.length() != 0 && phrase.front() != separator)		// Loop through each text before finding the separator or until the phrase is empty
	{
		word = word + phrase.front();								// Add the parsed character
		phrase.erase(0, 1);											// Delete the character read
	}
phrase.erase(0, 1);													// Delete the separator
return word;														// Return the word
}
int MapTileInterface::GetTileNumberFromRandom(unsigned char totalTiles, float clearTileProbabilty)
{
	std::uniform_int_distribution<unsigned int> distBackground(0, totalTiles + (totalTiles * 2 * clearTileProbabilty));		// Distribution for creating a random map
	unsigned int randomNumber = distBackground(rng);																		// Get a random number
	if (randomNumber < totalTiles)																							// If the random number is un walkable
	{
		randomNumber++;																										// Increase the random number
	}
	else
	{
		randomNumber = 0;																									// Make the tile traversable
	}
	return randomNumber;																									// Return the random number
}

void MapTileInterface::SetRandomSartAndEnd()
{
	std::uniform_int_distribution<unsigned int> randomTile(0, this->map.size() - 1);		// Distribution for creating a random tile location
	unsigned int start = randomTile(rng);													// Get a random location in the map
	unsigned int end;																		// Declare the end bariable
	do
	{
		end = randomTile(rng);																// Get a random location in the map
	} while (end == start);																	// Loop until start and end are different
	this->end = end;																		// Make the Location object from the array index
	this->start = start;																	// Make the Location object from the array index
	this->map[start] = -1;																	// Set the start tile
	this->map[end] = -2;																	// Set the end tile
	this->previousStep = start;																// Set a defaultValue to the previous step
}
std::vector<char> MapTileInterface::GetMap()
{
	return this->map;									// Return the map
}
unsigned int MapTileInterface::GetStartLocation()
{
	return this->start;									// Return the start location
}
unsigned int MapTileInterface::GetWidth()
{
	return this->width;									// Return the map's width
}
unsigned int MapTileInterface::GetHeight()
{
	return this->height;								// Return the map's height
}
char MapTileInterface::GetTile(unsigned int location)
{
	return this->map[location];							// Return the tile in the specified location
}
void MapTileInterface::SetTile(unsigned int location)
{
	if (editTileNum == this->tileList.size() - 2)		// Check if the editing tile is the rover sprite
	{
		this->map[this->start] = 0;						// Set the previous rover location traversable
		this->map[location] = -1;						// Set the specified location to the rover
		this->start = location;							// Set the start location to the specified location
	}
	else if (editTileNum == this->tileList.size() - 1)	// Check if the editing tile is the goal
	{
		this->map[this->end] = 0;						// Set the previous goal location traversable
		this->map[location] = -2;						// Set the specified location to the goal
		this->end = location;							// Set the end location to the specified location
	}
	else
	{
		this->map[location] = editTileNum;				// Set the specified location to the edit tile number
	}
}
void MapTileInterface::SetEditTile(unsigned char number)
{
	this->editTileNum = number;							// Set the edit tile number to the specified number
}
Sprite* MapTileInterface::GetTileList(unsigned int tileNumber)
{
	return this->tileList[tileNumber];					// Return the tileNumber-th sprite of the list
}
void MapTileInterface::SetPath(std::vector<unsigned int> path)
{
	this->emptyPath();																	// Clear the object's path
	for (unsigned int pathCounter = 0; pathCounter < path.size(); pathCounter++)		// loop through the path
	{
		this->animationPath.push(path[pathCounter]);									// Copy each element of the path to the object's path
	}

}
void MapTileInterface::emptyPath()
{
	std::queue<unsigned int> empty;								// Create an empty queue
	std::swap(this->animationPath, empty);						// Swap it with the current animation path so it is empty
}
void MapTileInterface::Draw()
{
	if (this->isAnimating)																			// Check if it is animating
	{
		if (this->previousStep % this->width >= this->maxDisplaySize / 2)								// Check if the rover is not centered on the X axis
		{
			this->offsetX = (this->previousStep % this->width) - (this->maxDisplaySize / 2);			// Set the offsetX to the rovers X coordinate - map half display width
			if (this->offsetX > this->width - this->maxDisplaySize)										// Check if the rover is close to the edge
			{
				this->offsetX = this->width - this->maxDisplaySize;										// Cap the offset
			}
		}
		else
		{
			this->offsetX = 0;																		// Set the offset to 0
		}
		if (std::floor(this->previousStep / this->height) >= this->maxDisplaySize / 2)					// Check if the rover is not centered on the Y axis
		{
			this->offsetY = std::floor(this->previousStep / this->height) - (this->maxDisplaySize / 2);	// Set the offsetY to the rovers Y coordinate - map half display width 
			if (this->offsetY > this->height - this->maxDisplaySize)									// Check if the rover is close to the edge
			{
				this->offsetY = this->height - this->maxDisplaySize;									// Cap the offset
			}
		}
		else
		{
			this->offsetY = 0;																		// Set the offset to 0
		}
		
	}

	endOffsetX = this->width;											// Set the X end offset with the maximum value
	endOffsetY = this->height;											// Set the Y end offset with the maximum value

	if (this->offsetX + this->maxDisplaySize < this->width)				// Check if the offset X plus the max display sisze is less than the width
	{
		this->endOffsetX = this->offsetX + this->maxDisplaySize;		// Change the end offset to the offset X plus the mad display size
	}
	if (this->offsetY + this->maxDisplaySize < this->height)			// Check if the offset Y plus the max display sisze is less than the width
	{
		this->endOffsetY = this->offsetY + this->maxDisplaySize;		// Change the end offset to the offset Y plus the mad display size
	}
	
	for (yCounter = this->offsetY; yCounter < this->endOffsetY; ++yCounter)				// Loop through the Y axis
	{
		for (xCounter = this->offsetX; xCounter < this->endOffsetX; ++xCounter)			// loop through the X axis
		{
			if (this->map[(yCounter * this->width) + xCounter] < 0)							// If the tile is either the rover or the goal
			{
																							// Display the traversable tile in the X,Y location
				this->tileList[0]->Blit((xCounter - this->offsetX) * this->tileSize + this->tileSize / 2, blit3D->screenHeight - ((yCounter - this->offsetY) * this->tileSize + this->tileSize / 2));
				if (this->map[(yCounter * this->width) + xCounter] == -1)					// If the tile is the rover
				{
					if (!this->isAnimating)													// Make sure the rover is not animating
					{
																							// Display the rover on top of the previously displayed tile
						this->blitRover((xCounter - this->offsetX) * this->tileSize + this->tileSize / 2, blit3D->screenHeight - ((yCounter - this->offsetY) * this->tileSize + this->tileSize / 2));
					}
				}
				else
				{
																							// Otherwise, display the goal on top of the previously displayed tile
					this->blitGoal((xCounter - this->offsetX) * this->tileSize + this->tileSize / 2, blit3D->screenHeight - ((yCounter - this->offsetY) * this->tileSize + this->tileSize / 2));
				}
			}
			else
			{
																							// Display the corresponfing tile in the X,Y location
				this->tileList[this->map[(yCounter * this->width) + xCounter]]->Blit((xCounter - this->offsetX) * this->tileSize + this->tileSize / 2, blit3D->screenHeight - ((yCounter - this->offsetY) * this->tileSize + this->tileSize / 2));
			}
			
		}
	}
	if (this->isAnimating)																	// Handle when it is animating
	{
		{
			roverX = this->previousStep % this->width;										// get the X coordinate of the prevous step
			roverY = std::floor(this->previousStep / width);								// Get the Y coordinate of the previous step
			if (this->roverDirection == 0)													// If it is faceing North
			{
				roverY -= this->animationProgress / ANIMATION_TIME_SLICE;					// Reduce the Y coordinate
			}
			else if (this->roverDirection == 90)											// If facing west
			{
				roverX -= this->animationProgress / ANIMATION_TIME_SLICE;					// Reduce thr X coosdinate
			}
			else if (this->roverDirection == 180)											// If facing south
			{
				roverY += this->animationProgress / ANIMATION_TIME_SLICE;					// Increase the Y coordinate
			}
			else
			{
				roverX += this->animationProgress / ANIMATION_TIME_SLICE;					// Increase the C coordinate if facing east
			}

			this->tileList[this->tileList.size() - 2]->angle = this->roverDirection;		// Set the sprite angle
																							// Draw the rover
			this->blitRover((roverX - this->offsetX) * this->tileSize + this->tileSize / 2, blit3D->screenHeight - ((roverY - this->offsetY) * this->tileSize + this->tileSize / 2));
		}
		
	}

}
void MapTileInterface::Update(float elapsedTime)
{
	if (this->isAnimating)													// Check if it is anumating
	{
		this->animationProgress += elapsedTime;								// Set the animation progress to the elapsed time
		if (this->animationProgress > ANIMATION_TIME_SLICE)					// Check if it's time to move to the next step
		{
			if (this->animationPath.size() <= 1)							// Check if we have finished
			{
				this->isAnimating = false;									// Stop the animation
			} 
			else
			{
				this->TakeAStep();											// Take a rover step
			}
			
		}
	}
	else
	{
		if (this->scrollRight && this->offsetX < this->width - this->maxDisplaySize)	// Check bounds before changing
		{
			this->offsetX++;															// Move the map to the right
		}
		if (this->scrollLeft && this->offsetX > 0)										// Check bounds before changing
		{
			this->offsetX--;															// move the map to the left
		}
		if (this->scrollDown && this->offsetY < this->height - this->maxDisplaySize)	// Check bounds before changing
		{
			this->offsetY++;															// Move the map down
		}
		if (this->scrollUp && this->offsetY > 0)										// Check bounds before changing
		{
			this->offsetY--;															// move the map up
		}
	}
	
}
void MapTileInterface::blitRover(unsigned int xCoordinate, unsigned int yCoordinate)
{
	this->tileList[this->tileList.size() - 2]->Blit(xCoordinate, yCoordinate);			// Paint the rover
}
void MapTileInterface::blitGoal(unsigned int xCoordinate, unsigned int yCoordinate)
{
	this->tileList[this->tileList.size() - 1]->Blit(xCoordinate, yCoordinate);			// Paint the goal
}

void MapTileInterface::BeginAnimation()
{
	this->isAnimating = true;															// Start the animation
	this->TakeAStep();																	// Take a rover step
	this->battery = 10000;																// Set the initial battery
	this->days = 0;																		// set the initial days
}
void MapTileInterface::SetRoverDirection()
{
	if (this->animationPath.size() > 0)													// Check it the path has at least one element
	{
		if (this->previousStep + 1 == this->animationPath.front())						// Check if advancing from the previous to the right arrives tot he current
		{
			this->roverDirection = 270;													// Set the direction to the west
		}
		else if (this->previousStep - 1 == this->animationPath.front())					// Check if advancing from the previous to the left arrives tot he current
		{
			this->roverDirection = 90;													// Set the direction to the east
		}
		else if (this->previousStep + this->width == this->animationPath.front())		// Check if advancing from the previous downwards arrives tot he current
		{
			this->roverDirection = 180;													// Set the direction to the south
		}
		else {
			this->roverDirection = 0;													// Set the direction to the north
		}
	}
}

void MapTileInterface::TakeAStep()
{
	if (this->animationPath.size() > 1)								// Make sure there's at least two steps
	{
		this->animationProgress = 0.f;								// Reset the animation progress
		this->previousStep = this->animationPath.front();			// Set the previour step with the current step
		this->animationPath.pop();									// Eliminate the corrent step to move to the next step
		this->roverLocation = this->animationPath.front();			// Set the location to the next step
		this->SetRoverDirection();									// Calculate the rover's direction
		this->battery--;
		if (this->battery == 0)										// Check if the rover ran out of battery
		{
			this->battery = 10000;									// Reset the batter
			this->days++;											// Add a day count
		}
	}
}

