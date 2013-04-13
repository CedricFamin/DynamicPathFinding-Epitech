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

#include "Node.h"

#include "../map.hpp"
#include "../avatar.hpp"

using namespace M_AI_DynamicPathFinding;

struct Position
{
    int x;
    int y;
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
template<typename Data, typename NodeComparator>
class APathFinder : public IPathFinder
{
public:
    typedef Data data_type;
    typedef Node<data_type> node_type;
    typedef std::priority_queue<node_type*, std::vector<node_type*>, NodeComparator> OpenList;
    typedef std::set<node_type*> ClosedList;
    typedef Avatar::Direction Direction;
    typedef std::list<Direction> DirectionList;
    typedef std::list<node_type*> NodeList;
    
    struct EdgeMap
    {
        void Init(unsigned int width, unsigned int height)
        {
            this->_width = width;
            this->_height = height;
            this->_container = new node_type*[width * height];
            for (unsigned int i = 0; i < width * height; ++i)
            {
                this->_container[i] = 0;
            }
        }
        
        node_type ** operator[](unsigned int y)
        {
            return _container + y * this->_width;
        }
    private:
        
        unsigned int _width;
        unsigned int _height;
        
        node_type **  _container;
    };
    
    APathFinder() { }
    virtual ~APathFinder() { }
    
    virtual DirectionList ComputePath()
    {
        
        _debugPath.clear();
        
        DirectionList directions;
        OpenList openList;
        ClosedList closedList;
        node_type* currentNode = 0;
        
        unsigned int avatarX = this->_avatar->GetPosition().x / Map::_MAP_SCALE;
        unsigned int avatarY = this->_avatar->GetPosition().y / Map::_MAP_SCALE;
        this->_root = this->_edgeMap[avatarY][avatarX];
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
    
    virtual void ClearPathAndDestination()
    {
        
    }
    
protected:
    virtual unsigned int EvalNode(node_type* node) const = 0;
    
    Position   _destination;
    node_type* _root;
    EdgeMap    _edgeMap;
    OpenList   _openList;
    ClosedList _closedList;
    Avatar * _avatar;
    NodeList _debugPath;
};

#endif
