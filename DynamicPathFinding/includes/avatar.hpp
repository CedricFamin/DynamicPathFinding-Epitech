#ifndef __M_AI_DYNAMICPATHFINDING_AVATAR_HPP__
#define __M_AI_DYNAMICPATHFINDING_AVATAR_HPP__

// C Includes
#include <stddef.h>

// SFML Includes
#include <SFML/Graphics.hpp>

// DynamicPathFinding Includes
#include "collidable.hpp"

using namespace M_AI_DynamicPathFinding;
namespace M_AI_DynamicPathFinding
{
    class Avatar : public Collidable
    {
    public:
        static const unsigned int _AVATAR_SIZE = 100; //< Avatar size in the rendered map TODO : AVATAR_SIZE must be equal to Map::MAP_SCALE
        
        enum Direction
        {
            UP,
            DOWN,
            LEFT,
            RIGHT
        };
        
    private:
        sf::Vector2f _position; //< avatar's position
        
    public:
        /// sf::Vector2f GetPosition(void);
        /// @return position on the rendered map given by the orign vector
        sf::Vector2f GetPosition(void);
        
        /// bool Move(enum Avatar::Direction);
        /// it move the avatar in one of the four directions ; up, down, left, right.
        /// @param direction as defined by the Avatar::Direction class enum.
        /// @return true if moving operation has succeed, else false and the avatar didn't move.
        /// \sa Avatar::Direction
        bool Move(Avatar::Direction dir);
        
        /// bool Move(sf::Vector2f& vec);
        /// it move the avatar using a 2D vector.
        /// @param direction as defined by a sf::Vector2f reference
        /// @return true if moving operation has succeed, else false and the avatar didn't move.
        /// \sa sf::Vector2f
        bool Move(sf::Vector2f& vec);
        
    public:
        // default ctor
        Avatar(sf::Vector2f& position);
        
        // default dtor
        ~Avatar(void);
    };
}

#endif /* !__M_AI_DYNAMICPATHFINDING_WALL_HPP__ */
