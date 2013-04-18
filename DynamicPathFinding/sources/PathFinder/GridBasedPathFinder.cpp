//
//  GridBasedPathFinder.cpp
//  DynamicPathFinding
//
//  Created by Cédric Famin on 01/04/13.
//
//

#include "../../includes/PathFinder/GridBasedPathFinder.h"

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
    this->_edgeMap.Init(map->GetX(), map->GetY());
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

GridBasedPathFinder::node_type * GridBasedPathFinder::CreateGraph(Map const * map, int parX,  int parY, int dist, bool ignoreCollision)
{
    struct Params
    {
        Params(int px, int py, node_type * _parent = 0)
        {
            x = px;
            y = py;
            parent = _parent;
        }
        int x;
        int y;
        node_type * parent;
        
    };
    
    std::list<Params> stackParam;
    stackParam.push_back(Params(parX, parY));
    
    while (stackParam.size())
    {
        int x = stackParam.front().x;
        int y = stackParam.front().y;
        Params params = stackParam.front();
        stackParam.pop_front();
        
        if (map->GetBounded() != boost::tribool::true_value)
        {
            if (x <= 0) x = map->GetX() - 1;
            if (y <= 0) x = map->GetY() - 1;
            if (x >= map->GetX()) x = 1;
            if (y >= map->GetY()) y = 1;
        }
        
        if ((x >= map->GetX() || y >= map->GetY() || x < 0 || y < 0))
        {
            continue;
        }
        
        if (this->_edgeMap[y][x] != 0)
        {
            if (params.parent)
                params.parent->AddNode(this->_edgeMap[y][x]);
            continue;
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
        if (params.parent)
            params.parent->AddNode(edge);
        this->_edgeMap[y][x] = edge;
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
            Params param(x + move.first, y + move.second, edge);
            stackParam.push_back(param);
        }
        ignoreCollision = false;
    }
    
    return NULL;
}

GridBasedPathFinder::DirectionList GridBasedPathFinder::ComputePath()
{
    
    _debugPath.clear();
    
    DirectionList directions;
    OpenList openList;
    ClosedList closedList;
    node_type* currentNode = 0;
    
    unsigned int avatarX = this->_avatar->GetPosition().x / Map::_MAP_SCALE;
    unsigned int avatarY = this->_avatar->GetPosition().y / Map::_MAP_SCALE;
    this->_root = this->_edgeMap[avatarY][avatarX];
    if (!this->_root)
        return directions;
    
    openList.push(this->_root);
    this->_root->Open(true);
    
    while (!openList.empty())
    {
        currentNode = openList.top();
        openList.pop();
        currentNode->Close(true);
        currentNode->Open(false);
        closedList.insert(currentNode);
        
        if (this->EvalNode(currentNode) == currentNode->GetDepth())
            break;
        
        for (node_type * node : currentNode->Edges())
        {
            if (!node->GetData().walkable && !node->Closed())
            {
                node->Close(true);
                closedList.insert(node);
            }
            
            if (!node->Closed() && !node->Opened())
            {
                node->SetParent(currentNode);
                node->SetDepth(currentNode->GetDepth() + 1);
                openList.push(node);
                node->Open(true);
            }
            
            if (node->Closed() && node->GetDepth() > currentNode->GetDepth() + 1)
            {
                node->Close(false);
                node->Open(true);
                node->SetParent(currentNode);
                closedList.erase(node);
                openList.push(node);
            }
        }
        
        currentNode = 0;
    }
    if (currentNode)
    {
        currentNode->Open(false);
        currentNode->Close(false);
        std::cout << "Current Node: Dist = " << this->EvalNode(currentNode) << std::endl
        << "Position : (" << currentNode->X() << ", " << currentNode->Y() << ")" << std::endl
        << "OpenList: " << openList.size() << " ClosedList: " << closedList.size() << std::endl;
    }
    
    while (currentNode && currentNode->GetParent())
    {
        Direction dir;
        node_type* parent = currentNode->GetParent();
        _debugPath.push_back(currentNode);
        if (currentNode->X() > parent->X())
            dir = Avatar::RIGHT;
        if (currentNode->X() < parent->X())
            dir = Avatar::LEFT;
        if (currentNode->Y() > parent->Y())
            dir = Avatar::DOWN;
        if (currentNode->Y() < parent->Y())
            dir = Avatar::UP;
        
        directions.push_front(dir);
        
        currentNode = parent;
        if (directions.size() > closedList.size() + openList.size())
        {
            std::cout << "Erreur durant le path finding" << std::endl;
            break;
        }
    }
    
    for (node_type * node : closedList)
    {
        node->Reinit();
    }
    
    while (openList.size() > 0)
    {
        openList.top()->Reinit();
        openList.pop();
    }
    
    return directions;
}

