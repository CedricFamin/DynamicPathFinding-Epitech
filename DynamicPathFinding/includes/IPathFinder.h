//
//  IPathFinder.h
//  DynamicPathFinding
//
//  Created by Cédric Famin on 30/03/13.
//
//

#ifndef DynamicPathFinding_IPathFinder_h
#define DynamicPathFinding_IPathFinder_h

#include <list>
#include <map>
#include <set>
#include <algorithm>

#include "map.hpp"
#include "avatar.hpp"

using namespace M_AI_DynamicPathFinding;

struct Position
{
    unsigned int x;
    unsigned int y;
};

// Node for A* algorithm
// You can embed yout data (travel cost ...) in the data property
// and use this in the eval_node method of APathFinder
template<typename Data>
class Node
{
public:
    typedef Data data_type;
    
    Node() : _closed(false), _opened(false), _parent(0) {}
    ~Node() {}
    
    void AddNode(Node<data_type> * node) { _edges.push_back(node); }
    std::list<Node<data_type>*> const & Edges() const { return _edges; }
    
    data_type & GetData() { return _data; }
    
    
    unsigned int X() const { return _x; }
    unsigned int Y() const { return _y; }
    unsigned int & X() { return _x; }
    unsigned int & Y() { return _y; }
    
    bool Closed() const { return _closed; }
    void Close(bool value) { _closed = value; }
    
    bool Opened() const { return _opened; }
    void Open(bool value) { _opened = value; }
    
    void SetParent(Node<data_type>* parent) { _parent = parent; }
    Node<data_type>* GetParent() const { return _parent; }
    
    unsigned int GetDepth() const { return _depth; }
    void SetDepth(unsigned int parValue) { _depth = parValue; }
    
    void Reinit()
    {
        _closed = false;
        _opened = false;
        _depth = 0;
        _parent = 0;
    };
    
private:
    std::list<Node<data_type>*> _edges;
    Data _data;
    unsigned int _x;
    unsigned int _y;
    unsigned int _depth;
    bool _closed;
    bool _opened;
    
    Node<data_type>* _parent;
};


class IPathFinder
{
public:
    typedef Avatar::Direction Direction;
    typedef std::list<Direction> DirectionList;
    
    virtual ~IPathFinder() { }
    
    virtual void Init(Map const * map) = 0;
    virtual DirectionList ComputePath() = 0;
    virtual void ClearPathAndDestination() = 0;
    virtual void DrawDebug(sf::RenderWindow& app) const {}
};

// Provide a simple A* algorithm
// Create the graph and fill the EdgeMap in the init method
// Implement your evaluation func in eval_node
template<typename Data>
class APathFinder : public IPathFinder
{
public:
    
    typedef Data data_type;
    typedef Node<data_type> node_type;
    
    struct NodeComparator
    {
        inline bool operator()(node_type* p1, node_type* p2)
		{
			return (p1->GetData().dist + p1->GetDepth()) > (p2->GetData().dist + p2->GetDepth());
		}
    };
    
    typedef std::map<unsigned int, std::map<unsigned int, node_type*> > EdgeMap;
    typedef std::priority_queue<node_type*, std::vector<node_type*>, NodeComparator> OpenList;
    typedef std::set<node_type*> ClosedList;
    typedef Avatar::Direction Direction;
    typedef std::list<Direction> DirectionList;
    typedef std::list<node_type*> NodeList;
    
    
    APathFinder() { }
    virtual ~APathFinder() { }
    
    virtual void Init(Map const * map)
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
    
    virtual DirectionList ComputePath()
    {
        
        _debugPath.clear();
        
        DirectionList directions;
        OpenList openList;
        ClosedList closedList;
        node_type* currentNode = 0;
        
        unsigned int avatarX = this->_avatar->GetPosition().x / Map::_MAP_SCALE;
        unsigned int avatarY = this->_avatar->GetPosition().y / Map::_MAP_SCALE;
        this->_root = this->_edgeMap.find(avatarX)->second.find(avatarY)->second;
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
            std::cout << "Current Node: Dist = " << currentNode->GetData().dist << std::endl
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
    
    virtual void ClearPathAndDestination()
    {
        
    }
    
protected:
    virtual unsigned int EvalNode(node_type* node) const
    {
        if (!node->GetData().walkable)
            return (unsigned int)-1;
        return node->GetData().dist + node->GetDepth();
    }
    
    Position   _destination;
    node_type* _root;
    EdgeMap    _edgeMap;
    OpenList   _openList;
    ClosedList _closedList;
    Avatar * _avatar;
    NodeList _debugPath;
protected:
    // partie specialie
    unsigned int _maxDepth;
    unsigned int _nbNode;
    // Creation du graph grid based
    node_type * CreateGraph(Map const * map, unsigned int x, unsigned int y, int dist, bool ignoreCollision = false)
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
        
        Node<Data> * edge = new Node<Data>();
        
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
    
    virtual void DrawDebug(sf::RenderWindow& app) const
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
};

#endif
