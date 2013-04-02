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

GridBasedPathFinder::node_type * GridBasedPathFinder::CreateGraph(Map const * map, int x,  int y, int dist, bool ignoreCollision)
{
    if (map->GetBounded() != boost::tribool::true_value)
    {
        if (x <= 0) x = map->GetX() - 1;
        if (y <= 0) x = map->GetY() - 1;
        if (x >= map->GetX()) x = 1;
        if (y >= map->GetY()) y = 1;
    }
    
    if ((x >= map->GetX() || y >= map->GetY() || x < 0 || y < 0))
    {
        return NULL;
    }
    
    if (this->_edgeMap.find(x) != this->_edgeMap.end() && this->_edgeMap[x].find(y) != this->_edgeMap[x].end())
    {
        return this->_edgeMap[x][y];
    }
    
    
    unsigned int computedDist = 0;
    {
        int goalX = map->GetGoalX() / Map::_MAP_SCALE;
        int goalY = map->GetGoalY() / Map::_MAP_SCALE;
        computedDist = abs(goalX - x) + abs(goalY - y);
        if (map->GetBounded() != boost::tribool::true_value)
        {
            computedDist = std::min(computedDist, x + map->GetX() - goalX + y + map->GetY() - goalY);
        }
        computedDist *= 1;
    }
    
    node_type* edge = new node_type();
    
    this->_edgeMap[x][y] = edge;
    edge->GetData().dist = computedDist;
    edge->GetData().walkable = !map->GetMap()[x + y * map->GetX()] || ignoreCollision;
    this->_maxDepth = std::max(edge->GetData().dist, this->_maxDepth);
    
    edge->X() = x;
    edge->Y() = y;
    this->_nbNode++;
    
    std::pair<int, int> moves[4] = {
        std::make_pair( 1, 0),
        std::make_pair(-1, 0),
        std::make_pair( 0, 1),
        std::make_pair( 0,-1)
    };
    
    for (std::pair<int, int> & move : moves)
    {
        node_type * n = CreateGraph(map, x + move.first, y + move.second, dist + 1);
        if (n) {
            edge->AddNode(n);
            //edge->GetData().dist += !n->GetData().walkable;
        }
    }
    
    return edge;
}

