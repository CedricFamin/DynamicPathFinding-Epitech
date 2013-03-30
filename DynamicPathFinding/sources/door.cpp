#include "../includes/door.hpp"

// Default ctor ----------------------------------------------------------
Door::Door()
: Collidable()
{
}

// Square ctor -----------------------------------------------------------
Door::Door(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY, bool state)
: Collidable(minX, minY, maxX, maxY)
{
    _doorState = state;
    if (_doorState)
    {
        sf::Color col(255, 0, 0);
        SetColor(col);
    }
    else
    {
        sf::Color col(100, 0, 0);
        SetColor(col);
    }
}

// default dtor ----------------------------------------------------------
Door::~Door()
{
}
