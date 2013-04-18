//
//  RandomWaypointPathFinder.h
//  DynamicPathFinding
//
//  Created by Cédric Famin on 15/04/13.
//
//

#ifndef DynamicPathFinding_RandomWaypointPathFinder_h
#define DynamicPathFinding_RandomWaypointPathFinder_h

#include "IPathFinder.h"
#include "VectorFieldHistogramPathFinder.h"
#include "../algo.hpp"

struct RandomWaypointAdvancedProperty
{
    unsigned int dist;
    float walkable;
};

struct RandomWaypointNodeComparator
{
    inline bool operator()(Node<RandomWaypointAdvancedProperty>* p1, Node<RandomWaypointAdvancedProperty>* p2)
    {
        return (p1->GetData().dist + p1->GetDepth()) > (p2->GetData().dist + p2->GetDepth());
    }
};

class RandomWaypointPathFinder : public APathFinder<RandomWaypointAdvancedProperty, RandomWaypointNodeComparator>
{
    typedef APathFinder<RandomWaypointAdvancedProperty, RandomWaypointNodeComparator>::node_type node_type;
    static unsigned int const GRID_RESOLUTION = 25;
public:
    RandomWaypointPathFinder();
    ~RandomWaypointPathFinder();
    
    virtual void Init(Map const * map);
    virtual DirectionList ComputePath();
    
protected:
    virtual unsigned int EvalNode(node_type* node) const;
    virtual void DrawDebug(sf::RenderWindow& app) const;
    
private:
    // partie specialie
    unsigned int _maxDepth;
    unsigned int _nbNode;
    int _goalX;
    int _goalY;
    Map const * _map;
    unsigned int _currentGridResolution;
    
    void CreateEdge(Map const * map, node_type * nodeToConnect);
    void CreateWaypoint();
    
    std::list<node_type*> _waypoints;
    
    std::list<node_type*> _nodeList;
    // Creation du graph grid based
    node_type * CreateGraph(Map const * map, unsigned int gridResolution);
    
    // VFH
    VectorFieldHistogramPathFinder _vfh;
};

#endif
