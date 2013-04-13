//
//  VectorFieldHistogramPathFinder.h
//  DynamicPathFinding
//
//  Created by Cédric Famin on 13/04/13.
//
//

#ifndef DynamicPathFinding_VectorFieldHistogramPathFinder_h
#define DynamicPathFinding_VectorFieldHistogramPathFinder_h

#include "IPathFinder.h"

class VectorFieldHistogramPathFinder : public IPathFinder
{
    typedef Avatar::Direction Direction;
    typedef std::list<Direction> DirectionList;
    
    static unsigned int const ACTIVE_WINDOW_RADIUS = 5;
    static unsigned int const POLAR_HISTOGRAM_STEP = 45;
    static unsigned int const POLAR_RANGE = 360 / POLAR_HISTOGRAM_STEP;
public:
    
    VectorFieldHistogramPathFinder();
    virtual ~VectorFieldHistogramPathFinder();
    
    virtual void Init(Map const * map);
    virtual DirectionList ComputePath();
    virtual void ClearPathAndDestination();
    virtual void DrawDebug(sf::RenderWindow& app) const;

private:
    unsigned int _width;
    unsigned int _height;
    unsigned int _goalX;
    unsigned int _goalY;
    Avatar * _avatar;
    unsigned int const * _map;
    
    // Machine learning, on ne revient pas dans un endroit que l'on a deja visiter
private:
    bool **_alreadyTakingPath;
    
    unsigned int _polarResult[POLAR_RANGE];
};

#endif
