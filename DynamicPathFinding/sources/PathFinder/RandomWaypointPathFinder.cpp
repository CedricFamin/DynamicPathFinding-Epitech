//
//  RandomWaypointPathFinder.cpp
//  DynamicPathFinding
//
//  Created by Cédric Famin on 15/04/13.
//
//

#include <math.h>

#include "../../includes/PathFinder/RandomWaypointPathFinder.h"



RandomWaypointPathFinder::RandomWaypointPathFinder()
{
    
}

RandomWaypointPathFinder::~RandomWaypointPathFinder()
{
    
}

void RandomWaypointPathFinder::Init(Map const * map)
{
    unsigned int goalX = map->GetGoalX();
    unsigned int goalY = map->GetGoalY();
    
    this->_goalX = goalX / Map::_MAP_SCALE;
    this->_goalY = goalY / Map::_MAP_SCALE;
    this->_map = map;
    this->_maxDepth = 0;
    this->_nbNode = 0;
    // Creation du graph.
    // On part de l'avatar vers le goal
    this->_edgeMap.Init(map->GetX(), map->GetY());
    
    this->_currentGridResolution = GRID_RESOLUTION;
    
    CreateGraph(map, GRID_RESOLUTION);
    // Create a waypoint at the objectif
    this->_edgeMap[this->_goalY][this->_goalX] = new node_type();
    this->_edgeMap[this->_goalY][this->_goalX]->GetData().dist = 0;
    this->_edgeMap[this->_goalY][this->_goalX]->X() = this->_goalX;
    this->_edgeMap[this->_goalY][this->_goalX]->Y() = this->_goalY;
    this->CreateEdge(map, this->_edgeMap[this->_goalY][this->_goalX]);
    this->_nodeList.push_back(this->_edgeMap[this->_goalY][this->_goalX]);
    this->_nbNode++;
    //CreateEdge(map);
    this->_avatar = map->GetAvatar();
    std::cout << "Node created: " << this->_nbNode << std::endl;
    
    this->_vfh.Init(map);
}

unsigned int RandomWaypointPathFinder::EvalNode(node_type* node) const
{
    //if (!node->GetData().walkable)
    //    return (unsigned int)-1;
    return node->GetData().dist;
}



void RandomWaypointPathFinder::DrawDebug(sf::RenderWindow& app) const
{
    
    /*for (unsigned int y = 0; y < this->_edgeMap.height(); ++y)
    {
        for (unsigned int x = 0; x < this->_edgeMap.width(); ++x)
        {
            node_type * node = this->_edgeMap[y][x];
            if (!node)
                continue;
            
            unsigned int minX = node->X() * Map::_MAP_SCALE;
            unsigned int minY = node->Y() * Map::_MAP_SCALE;
            
            // dessine edge
            for (node_type const * subNode : node->Edges())
            {
                unsigned int maxX = subNode->X() * Map::_MAP_SCALE;
                unsigned int maxY = subNode->Y() * Map::_MAP_SCALE;
                sf::Shape shape = sf::Shape::Line(minX, minY, maxX, maxY, 50, sf::Color::Blue);
                
                app.Draw(shape);
            }
            
        }
    }*/
    node_type const * lastNode = 0;
    for (node_type const * node : this->_waypoints)
    {
        unsigned int minX = node->X() * Map::_MAP_SCALE;
        unsigned int minY = node->Y() * Map::_MAP_SCALE;
        unsigned int maxX = node->X() * Map::_MAP_SCALE + Map::_MAP_SCALE;
        unsigned int maxY = node->Y() * Map::_MAP_SCALE + Map::_MAP_SCALE;
        sf::Shape shape = sf::Shape::Rectangle(minX, minY, maxX, maxY, sf::Color::Green);
        
        app.Draw(shape);
        if (lastNode)
        {
            unsigned int maxX = lastNode->X() * Map::_MAP_SCALE;
            unsigned int maxY = lastNode->Y() * Map::_MAP_SCALE;
            sf::Shape shape = sf::Shape::Line(minX, minY, maxX, maxY, 50, sf::Color::Blue);
            
            app.Draw(shape);
        }
        lastNode = node;
    }
    this->_vfh.DrawDebug(app);
}

RandomWaypointPathFinder::node_type * RandomWaypointPathFinder::CreateGraph(Map const * map, unsigned int gridResolution)
{
    for (int y = gridResolution / 2; y < this->_edgeMap.height(); y += gridResolution)
    {
        for (int x = gridResolution / 2; x < this->_edgeMap.width(); x += gridResolution)
        {
            if (this->_edgeMap[y][x])
                continue;
            if (!map->GetMap()[x + y * map->GetX()])
            {
                node_type * node = new node_type;
                node->X() = x;
                node->Y() = y;
                node->GetData().dist = abs(x - this->_goalX) + abs(y - this->_goalY);
                if (this->_maxDepth < node->GetData().dist)
                    this->_maxDepth = node->GetData().dist;
                
                this->_edgeMap[y][x] = node;
                this->CreateEdge(map, node);
                this->_nodeList.push_back(node);
                this->_nbNode++;
            }
        }
    }
    
    return NULL;
}

void RandomWaypointPathFinder::CreateEdge(Map const * map, node_type * nodeToConnect)
{    
    static float const stepAngle = M_PI / (15.0f * M_PI);
    unsigned int xPos = nodeToConnect->X();
    unsigned int yPos = nodeToConnect->Y();
    
    for (float angle = 0; angle <= M_PI * 2; angle += stepAngle)
    {
        float xDecal = cosf(angle);
        float yDecal = sinf(angle);
        for (unsigned int dist = 1;; dist++)
        {
            unsigned int x = xDecal * dist + xPos;
            unsigned int y = yDecal * dist + yPos;
            
            if (x > map->GetX() || y >= map->GetY())
                break;
            if (map->GetMap()[x + y * map->GetX()])
                break;
            if (this->_edgeMap[y][x])
            {
                nodeToConnect->AddNode(this->_edgeMap[y][x]);
                this->_edgeMap[y][x]->AddNode(nodeToConnect);
            }
        }
        
        
    }
    
    return ;
}


RandomWaypointPathFinder::DirectionList RandomWaypointPathFinder::ComputePath()
{
    int avatarX = this->_avatar->GetPosition().x / Map::_MAP_SCALE;
    int avatarY = this->_avatar->GetPosition().y / Map::_MAP_SCALE;
    
    DirectionList directions;
    
    if (this->_waypoints.size() == 0)
    {
        // Create a waypont at the avatar pos
        node_type * node = new node_type;
        node->X() = this->_avatar->GetPosition().x / Map::_MAP_SCALE;
        node->Y() = this->_avatar->GetPosition().y / Map::_MAP_SCALE;
        if (this->_edgeMap[node->Y()][node->X()] == 0)
        {
            
            node->GetData().dist = abs(node->X() - this->_goalX) + abs(node->Y() - this->_goalY);
            if (this->_maxDepth < node->GetData().dist)
                this->_maxDepth = node->GetData().dist;
            
            this->_edgeMap[node->Y()][node->X()] = node;
            this->_nodeList.push_back(node);
            this->_nbNode++;
            CreateEdge(this->_map, node);
        }
        else
            delete node;
        
        CreateWaypoint();
        if (this->_waypoints.size() == 0 && this->_currentGridResolution)
        {
            this->_currentGridResolution >>= 1;
            this->CreateGraph(this->_map, this->_currentGridResolution);
            CreateEdge(this->_map, this->_edgeMap[this->_goalY][this->_goalX]);
            std::cout << "Create new waypoint : " << this->_nbNode << " resolution : " << this->_currentGridResolution << std::endl;
        }
        std::cout << "Nb Waypoint: " << this->_waypoints.size() << std::endl;
        return directions;
    }
    
    // go to waypoint
    if (this->_waypoints.size())
    {
        if (avatarX == this->_waypoints.front()->X() && avatarY == this->_waypoints.front()->Y())
        {
            this->_waypoints.pop_front();
            if (this->_waypoints.size())
                this->_vfh.SetGoal(this->_waypoints.front()->X(), this->_waypoints.front()->Y());
        }
        if (this->_waypoints.size())
        {
            
            directions = this->_vfh.ComputePath();
            if (directions.size() == 0)
            {
                this->_waypoints.pop_front();
                
                if (this->_waypoints.size())
                    this->_vfh.SetGoal(this->_waypoints.front()->X(), this->_waypoints.front()->Y());
            }
        }
    }
    return directions;
}

void RandomWaypointPathFinder::CreateWaypoint()
{
    OpenList openList;
    ClosedList closedList;
    node_type* currentNode = 0;
    
    unsigned int avatarX = this->_avatar->GetPosition().x / Map::_MAP_SCALE;
    unsigned int avatarY = this->_avatar->GetPosition().y / Map::_MAP_SCALE;
    this->_root = this->_edgeMap[avatarY][avatarX];
    if (!this->_root)
        return ;
    
    openList.push(this->_root);
    this->_root->Open(true);
    
    while (!openList.empty())
    {
        currentNode = openList.top();
        openList.pop();
        currentNode->Close(true);
        currentNode->Open(false);
        closedList.insert(currentNode);
        
        if (this->EvalNode(currentNode) == 0)
            break;
        
        for (node_type * node : currentNode->Edges())
        {
            
            if (!node->Closed() && !node->Opened())
            {
                node->SetParent(currentNode);
                node->SetDepth(currentNode->GetDepth() + 1);
                openList.push(node);
                node->Open(true);
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
    
    while (currentNode)
    {
        this->_waypoints.push_front(currentNode);
        currentNode = currentNode->GetParent();
        
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
    
}
