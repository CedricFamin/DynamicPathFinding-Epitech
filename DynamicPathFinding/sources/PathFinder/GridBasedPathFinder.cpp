//
//  GridBasedPathFinder.cpp
//  DynamicPathFinding
//
//  Created by Cédric Famin on 01/04/13.
//
//

#include "../../includes/PathFind/GridBasedPathFinder.h"

GridBasedPathFinder::GridBasedPathFinder()
{
    
}

GridBasedPathFinder::~GridBasedPathFinder()
{
    
}

void GridBasedPathFinder::Init(Map const * map)
{
    unsigned int goalX = map->GetGoalX();
    unsigned int goalY = map->GetGoalY();
    
    this->_maxDepth = 0;
    // Creation du graph.
    // On part de l'avatar vers le goal
    CreateGraph(map, goalX / Map::_MAP_SCALE, goalY / Map::_MAP_SCALE, 0, true);
    
    this->_avatar = map->GetAvatar();
    std::cout << "Node created: " << this->_nbNode << std::endl;
}

unsigned int GridBasedPathFinder::EvalNode(node_type* node) const
{
    if (!node->GetData().walkable)
        return (unsigned int)-1;
    return node->GetData().dist + node->GetDepth();
}



void GridBasedPathFinder::DrawDebug(sf::RenderWindow& app) const
{
    /*
     std::for_each(this->_edgeMap.begin(), this->_edgeMap.end(),
     [=, &app](std::pair<int, std::map<unsigned int, node_type*>> const & nodes)
     {
     std::for_each(nodes.second.begin(), nodes.second.end(),
     [=, &app](std::pair<int, node_type *> item)
     {
     node_type * node = item.second;
     sf::Shape shape;
     unsigned int minX = node->X() * Map::_MAP_SCALE;
     unsigned int maxX = node->X() * Map::_MAP_SCALE + Map::_MAP_SCALE;
     unsigned int minY = node->Y() * Map::_MAP_SCALE;
     unsigned int maxY = node->Y() * Map::_MAP_SCALE + Map::_MAP_SCALE;
     unsigned int color = (float)node->GetData().dist / (float)this->_maxDepth * 200;
     
     if (!node->GetData().walkable)
     color = 255;
     shape.AddPoint(sf::Vector2f(minX, minY), sf::Color(color, color, color));
     shape.AddPoint(sf::Vector2f(minX, maxY), sf::Color(color, color, color));
     shape.AddPoint(sf::Vector2f(maxX, maxY), sf::Color(color, color, color));
     shape.AddPoint(sf::Vector2f(maxX, minY), sf::Color(color, color, color));
     
     app.Draw(shape);
     });
     });
     */
    
    for (node_type * node : _debugPath)
    {
        sf::Shape shape;
        unsigned int minX = node->X() * Map::_MAP_SCALE;
        unsigned int maxX = node->X() * Map::_MAP_SCALE + Map::_MAP_SCALE;
        unsigned int minY = node->Y() * Map::_MAP_SCALE;
        unsigned int maxY = node->Y() * Map::_MAP_SCALE + Map::_MAP_SCALE;
        unsigned int color = 255 - (float)node->GetData().dist / (float)this->_maxDepth * 255;
        
        shape.AddPoint(sf::Vector2f(minX, minY), sf::Color(color, 0, 0));
        shape.AddPoint(sf::Vector2f(minX, maxY), sf::Color(color, 0, 0));
        shape.AddPoint(sf::Vector2f(maxX, maxY), sf::Color(color, 0, 0));
        shape.AddPoint(sf::Vector2f(maxX, minY), sf::Color(color, 0, 0));
        
        app.Draw(shape);
    }
}

GridBasedPathFinder::node_type * GridBasedPathFinder::CreateGraph(Map const * map, unsigned int x, unsigned int y, int dist, bool ignoreCollision)
{
    if (x >= map->GetX() || y >= map->GetY())
    {
        return NULL;
    }
    
    if (this->_edgeMap.find(x) != this->_edgeMap.end() && this->_edgeMap[x].find(y) != this->_edgeMap[x].end())
    {
        return this->_edgeMap[x][y];
    }
    
    
    
    int goalX = map->GetGoalX() / Map::_MAP_SCALE;
    int goalY = map->GetGoalY() / Map::_MAP_SCALE;
    
    node_type* edge = new node_type();
    
    this->_edgeMap[x][y] = edge;
    edge->GetData().dist = abs(goalX - (int)x) + abs(goalY - (int)y);
    edge->GetData().walkable = !map->GetMap()[x + y * map->GetX()] || ignoreCollision;
    this->_maxDepth = std::max(edge->GetData().dist, this->_maxDepth);
    
    edge->X() = x;
    edge->Y() = y;
    this->_nbNode++;
    
    node_type * n1 = CreateGraph(map, x + 1, y, dist + 1);
    if (n1) { edge->AddNode(n1); }
    node_type * n2 = CreateGraph(map, x - 1, y, dist + 1);
    if (n2) { edge->AddNode(n2); }
    node_type * n3 = CreateGraph(map, x, y + 1, dist + 1);
    if (n3) { edge->AddNode(n3); }
    node_type * n4 = CreateGraph(map, x, y - 1, dist + 1);
    if (n4) { edge->AddNode(n4); }
    
    return edge;
}

