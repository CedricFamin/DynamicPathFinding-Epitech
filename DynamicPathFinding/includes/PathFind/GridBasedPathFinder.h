//
//  GridBasedPathFinder.h
//  DynamicPathFinding
//
//  Created by Cédric Famin on 01/04/13.
//
//

#ifndef DynamicPathFinding_GridBasedPathFinder_h
#define DynamicPathFinding_GridBasedPathFinder_h

#include "IPathFinder.h"

struct GridBasedAdvancedProperty
{
    unsigned int dist;
    float walkable;
};

struct GridBasedNodeComparator
{
    inline bool operator()(Node<GridBasedAdvancedProperty>* p1, Node<GridBasedAdvancedProperty>* p2)
    {
        return (p1->GetData().dist + p1->GetDepth()) > (p2->GetData().dist + p2->GetDepth());
    }
};

class GridBasedPathFinder : public APathFinder<GridBasedAdvancedProperty, GridBasedNodeComparator>
{
    typedef APathFinder<GridBasedAdvancedProperty, GridBasedNodeComparator>::node_type node_type;
    
public:
    GridBasedPathFinder();
    ~GridBasedPathFinder();
    
    virtual void Init(Map const * map);
    
protected:
    virtual unsigned int EvalNode(node_type* node) const;
    virtual void DrawDebug(sf::RenderWindow& app) const;

private:
    // partie specialie
    unsigned int _maxDepth;
    unsigned int _nbNode;
    // Creation du graph grid based
    node_type * CreateGraph(Map const * map, unsigned int x, unsigned int y, int dist, bool ignoreCollision = false);
};

#endif
