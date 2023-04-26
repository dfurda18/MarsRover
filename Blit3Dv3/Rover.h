#pragma once

#include <vector>
#include <map>
#include "TileMap.h"
#include "MarsRover.h"
#include "MapNode.h"

/**
* A class that represents a Rover program.
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-11
*/
class Rover
{
private:
	/**
	* The Mars map
	*/
	TileMap* map;
	/**
	* The map to leave the breadcrums
	*/
	TileMap* breadcrumsMap;
	/**
	* The Rover
	*/
	MarsRover* rover;
	/**
	* The path taken to find the ending point
	*/
	std::vector<unsigned int> path;
	/**
	* The path taken recursively
	*/
	std::vector<unsigned int> recursivePath;
	/**
	* The open list
	*/
	std::vector<MapNode*> open;
	/**
	* The closed list
	*/
	std::vector<MapNode*> closed;
	/**
	* The current path position in the recursion
	*/
	unsigned int stackSize;
	/**
	* We¿hether the rover gound the goal or not
	*/
	bool foundIt = false;
	/**
	* Finds the map's ending point from a specific current point without taking in consideration partitions
	* @return True if the end point was found and false otherwise.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	bool SearchRecursively();
	/**
	* Gets the next direction given the direction counter based on the rover's location and a final location
	* @param directionCounter The direction counter to get.
	* @param roversDiration The current Rover's direction
	* @param finalLocation The final location.
	* @return The next direction to try.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	unsigned char GetNextDirection(unsigned char directionCounter, unsigned char roversDirection, unsigned int finalLocation);
	/**
	* Moves the Rover torwards a specified location
	* @param location The location to move the rover to
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	void MoveRoverTo(unsigned int location);
	/**
	* Decides if the rover can turn
	* @return True if there's at least one space where the rover can turn
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-27
	*/
	bool CanTurn();
	/**
	* Turns the rover torwards the next viable direction to move
	* @param previousDirection The previousDirection the roved was facing
	* @param finalDestination The direction the rover is aiming to get
	* @return True if the direction was found
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-27
	*/
	unsigned char GetNextViableDirection(unsigned char previousDirection, unsigned int finalDestination);
public:
	/**
	* Constructor method.
	* @return A new Instance of the Rover class
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	Rover();
	/**
	* Rover Deconstructor method.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	void DeInit();
	/**
	* Loads the map with a given vector map
	* @param map The vector map.
	*  @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-19
	*/
	void LoadMap(std::vector<char> map);
	/**
	* Sets the Rover's starting location
	* @param newLocation The new location
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	void SetRoverLocation(unsigned int newLocation);
	/**
	* Returns the rover
	* @return The rover
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	MarsRover* GetRover();
	/**
	* Returns the map's width
	* @return The map's width
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	unsigned int GetMapWidth();
	/**
	* Returns the map's height
	* @return The map's height
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	unsigned int GetMapHeight();
	/**
	* Returns the path taken to search the goal
	* @return The path taken to search the goal
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	std::vector<unsigned int> GetPath();
	/**
	* Returns whether or not it found the goal
	* @return True if the rover found the goal
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	bool FoundIt();
	/**
	* Finds the map's ending point from a specific current point
	* @return True if the end point was found and false otherwise.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	bool FindEndingPoint();
	/**
	* Returns the state of the map in a String.
	* @return The map in a string: X non-traversable tile, . breadcrum, S Starting point, E Ending point, R Rover
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	std::string PathToString();
	/**
	* Finds the node with lowest F, returns it and takes it out of the vector
	* @param list The list to search
	* @return The reference to the lowest F node int he list
	* @version 1.0
	* @since 2022-10-13
	*/
	MapNode* PopLowestF(std::vector<MapNode*> &list);

	/**
	* Builds the path from the end node
	* @author Dario Urdapilleta
	* @since 2023-04-13
	*/
	void ConstructPath();

	/**
	* Returns the location from a position torwards a direction.
	* @param position The current position
	* @param direction The direction
	* @return the location from a position torwards a direction	
	* @author Dario Urdapilleta
	* @since 2023-04-14
	*/
	int GetNextPosition(int position, int direction);

	/**
	* Return the cost for coing from node a to node b
	* @param a the original noce
	* @param b the new node
	* @return the cost for coing from node a to node b
	* @author Dario Urdapilleta
	* @since 2023-04-14
	*/
	int Cost(MapNode* a, MapNode* b);

	/**
	* Searches in the list if the list has a node with the same position
	* @param list The node list
	* @param position The node's position
	* @return True if the list ocntains the node
	* @author Dario Urdapilleta
	* @since 2023-04-14
	*/
	bool HasNode(std::vector<MapNode*> list, int position);

	/**
	* Removes a node from a list that has the specified position as the node
	* @param list The list
	* @param node The specified position
	* @author Dario Urdapilleta
	* @since 2023-04-14
	*/
	void Remove(std::vector<MapNode*> &list, MapNode* node);
};