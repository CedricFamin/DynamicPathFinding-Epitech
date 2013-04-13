//
//  NavigationMeshPathFinder.h
//  DynamicPathFinding
//
//  Created by Cédric Famin on 03/04/13.
//
//

#ifndef __DynamicPathFinding__NavigationMeshPathFinder__
#define __DynamicPathFinding__NavigationMeshPathFinder__

#include <vector>
#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include "IPathFinder.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef K::Point_2 Point;

struct Vertice
{
    unsigned int x;
    unsigned int y;
};

struct NavigationMeshAdvancedProperty
{
    std::vector<Vertice> vertices;
    bool walkable;
};

struct NavigationMeshNodeComparator
{
    inline bool operator()(Node<NavigationMeshAdvancedProperty>* p1, Node<NavigationMeshAdvancedProperty>* p2)
    {
        return false;
    }
};

class NavigationMeshPathFinder : public APathFinder<NavigationMeshAdvancedProperty, NavigationMeshNodeComparator>
{
    typedef APathFinder<NavigationMeshAdvancedProperty, NavigationMeshNodeComparator>::node_type node_type;
    
public:
    NavigationMeshPathFinder();
    virtual ~NavigationMeshPathFinder();
    
    virtual void Init(Map const * map);
    
protected:
    virtual unsigned int EvalNode(node_type* node) const;
    virtual void DrawDebug(sf::RenderWindow& app) const;
    
private:
    
    Delaunay GetAllVertices(Map const * map);
    
    Delaunay _triangles;
    
    // partie specialie
    unsigned int _maxDepth;
    unsigned int _nbNode;
    unsigned int _mapX;
    unsigned int _mapY;
    
    std::list<Vertice> _vertices;
    char* _marshingSquare;
};

#endif /* defined(__DynamicPathFinding__NavigationPathPathFinder__) */
