#ifndef __M_AI_DYNAMICPATHFINDING_ALGO_HPP__
#define __M_AI_DYNAMICPATHFINDING_ALGO_HPP__

// C++ Includes
#include <list>

// Dynamic Path Finding Includes
#include "avatar.hpp"
#include "map.hpp"

using namespace M_AI_DynamicPathFinding;
namespace M_AI_DynamicPathFinding
{
    class Algo
    {
    private:
        Avatar* _avatar; //< Avatar the algorithm pilots.
        
        short int _count; //< Counter for animation.
        
        Map* _map; //< Map the algorithm is running.
        
        std::list<Avatar::Direction> _moves; //< Current movements list
        
        unsigned int _plannedPos[2]; //< position of the avatar at the end of the current movement list
        
    protected:
        /// Check if movement will be a success
        /// @return true if the given movement will succeed, elsewhere false
        bool CheckMovement(Avatar::Direction dir);
        
        /// Get a const reference to the map
        /// In this table :
        ///    - 0 is a empty case
        ///    - 1 is a wall
        const unsigned int& GetMap(void) const;
        
        /// bool Move(enum Avatar::Direction);
        /// Moves the avatar to a direction
        /// @param Given direction using the Avatar::Direction enum
        void Move(Avatar::Direction dir);
        
        /// Set a series of movements to be executed by the avatar
        /// @param a list on directions to be followed in order (from .begin() to .end())
        void Move(std::list<Avatar::Direction> movList);
        
    public:
        /// OverLoad to create Path
        virtual void ComputePath(void) = 0;
        
        /// Called by the main rendering loop (bouh c'est moche)
        /// Note that planning can be threaded inside the algorithm class
        ///  avoiding to mess with the rendering loop (MIMD world guys !)
        void Update(void);
        
    public:
        /// default ctor
        Algo(Map* labyrinth);
        
        /// default dtor
        virtual ~Algo(void);
        
        virtual const void DrawDebug(sf::RenderWindow& app) = 0;
    };
}

#endif /* !__M_AI_DYNAMICPATHFINDING_ALGO_HPP__ */
