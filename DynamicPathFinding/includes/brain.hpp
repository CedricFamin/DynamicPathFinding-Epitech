#ifndef __M_AI_DYNAMICPATHFINDING_BRAIN_HPP__
#define __M_AI_DYNAMICPATHFINDING_BRAIN_HPP__

// C Includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// DynamicPathFinding Includes
#include "algo.hpp"

using namespace M_AI_DynamicPathFinding;
namespace M_AI_DynamicPathFinding
{
    class Brain : public Algo
    {
    public:
        /// ComputePath
        void ComputePath(void);
        
    public:
        /// default ctor
        Brain(Map* map);
        
        /// default dtor
        ~Brain(void);
    };
}

#endif /* !__M_AI_DYNAMICPATHFINDING_BRAIN_HPP__ */
