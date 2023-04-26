#pragma once

/**
* A mapNode
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-11
*/
class MapNode {
public:
	/**
	* The map G
	*/
	float g;
	/**
	* The map H
	*/
	float h;
	/**
	* The map F
	*/
	float f;
	/**
	* The node wight
	*/
	int w;
	/**
	* The node location
	*/
	unsigned int position;
	/**
	* 
	*/
	/**
	* The node's parent
	*/
	MapNode* parent;

	/**
	* Creates a new node
	* @author Dario Urdapilleta
	* @since 2023-04-13
	*/
	MapNode() {
		this->g = -1.f;
		this->h = -1.f;
		this->f = -1.f;
		this->w = 1.f;
		this->parent = NULL;
	}
};