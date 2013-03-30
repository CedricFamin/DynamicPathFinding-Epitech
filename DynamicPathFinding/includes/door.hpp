#ifndef __M_AI_DYNAMICPATHFINDING_DOOR_HPP__
#define __M_AI_DYNAMICPATHFINDING_DOOR_HPP__

// C Includes
#include <stddef.h>

// SFML Includes
#include <SFML/Graphics.hpp>

// DynamicPathFinding Includes
#include "collidable.hpp"

using namespace M_AI_DynamicPathFinding;
namespace M_AI_DynamicPathFinding
{
    class Door : public Collidable
    {
    private:
        bool _doorState;
        
    public:
        // default ctor
        Door(void);
        
        // square ctor
        Door(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY, bool state);
        
        // default dtor
        ~Door(void);
    };
}

#endif /* !__M_AI_DYNAMICPATHFINDING_DOOR_HPP__ */
