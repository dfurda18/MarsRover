#include "Vehicle.h"

/**
* See Vehicle.h for method definitions
*/

Vehicle::Vehicle()
{
	this->location = 0;				// Set the default locaiton to 0;
}

Vehicle::~Vehicle()
{
	// No need to do anything
}

void Vehicle::SetLocation(unsigned int newLocation)
{
	this->location = newLocation;	// Set the location to the newLocation
}

