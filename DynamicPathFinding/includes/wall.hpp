#ifndef __M_AI_DYNAMICPATHFINDING_WALL_HPP__
#define __M_AI_DYNAMICPATHFINDING_WALL_HPP__

// C Includes
#include <stddef.h>

// SFML Includes
#include <SFML/Graphics.hpp>

// DynamicPathFinding Includes
#include "collidable.hpp"

using namespace M_AI_DynamicPathFinding;
namespace M_AI_DynamicPathFinding
{
    class Wall : public Collidable
    {
    public:
        // default ctor
        Wall(void);
        
        // square ctor
        Wall(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY);
        
        // default dtor
        ~Wall(void);
    };
}

#endif /* !__M_AI_DYNAMICPATHFINDING_WALL_HPP__ */
