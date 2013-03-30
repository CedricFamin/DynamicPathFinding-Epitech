#ifndef __M_AI_DYNAMICPATHFINDING_COLLIDABLE_HPP__
#define __M_AI_DYNAMICPATHFINDING_COLLIDABLE_HPP__

// C Includes
#include <stddef.h>

// C++ Includes
#include <iostream>

// SFML Includes
#include <SFML/Graphics.hpp>

// DynamicPathFinding Includes
//#include "map.hpp"

namespace M_AI_DynamicPathFinding {}
using namespace M_AI_DynamicPathFinding;
namespace M_AI_DynamicPathFinding
{
    class Map;
    /// Represent a collidable object
    class Collidable
    {
        protected :
        bool _block;		//< Is this object blocks avatars
        
        bool _destroyable;		//< Is this object destroyable
        
        unsigned int _minX;		//< object minimal X coordinate
        
        unsigned int _minY;		//< object minimal Y coordinate
        
        unsigned int _maxX;		//< object maximal X coordinate
        
        unsigned int _maxY;		//< object maximal Y coordinate
        
        bool _ready;		//< Is this object is ready to be created
        
        sf::Shape* _shape;		//< SFML shape object
        
    private:
        /// Create or modify the current shape
        bool CreateShape(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY);
        
    public:
        /// Get mininal coordinate in X
        const unsigned int GetMinX(void) const;
        
        /// Get mininal coordinate in Y
        const unsigned int GetMinY(void) const;
        
        /// Get maximal coordinate in X
        const unsigned int GetMaxX(void) const;
        
        /// Get maximal coordinate in Y
        const unsigned int GetMaxY(void) const;
        
        /// return a const pointer to the SFML shape
        const sf::Shape* GetShape(void) const;
        
        /// Is the object's ready ?
        bool IsReady(void);
        
        /// Set the object color filter (on basic white shape)
        void SetColor(sf::Color& color);
        
    public:
        /// default ctor
        Collidable(void);
        
        /// square ctor
        Collidable(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY);
        
        /// default dtor
        ~Collidable(void);
    };
}

#endif /* !__M_AI_DYNAMICPATHFINDING_COLLIDABLE_HPP__ */
