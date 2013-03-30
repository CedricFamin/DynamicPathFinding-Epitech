#include "../includes/wall.hpp"

// Default ctor ----------------------------------------------------------
Wall::Wall()
: Collidable()
{
}

// Square ctor -----------------------------------------------------------
Wall::Wall(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY)
: Collidable(minX, minY, maxX, maxY)
{
}

// default dtor ----------------------------------------------------------
Wall::~Wall()
{
}
