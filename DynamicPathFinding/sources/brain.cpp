#include "../includes/brain.hpp"

#include "../includes/PathFinder/GridBasedPathFinder.h"
#include "../includes/PathFinder/NavigationMeshPathFinder.h"
#include "../includes/PathFinder/VectorFieldHistogramPathFinder.h"
#include "../includes/PathFinder/RandomWaypointPathFinder.h"


// ComputePath -----------------------------------------------------------
void Brain::ComputePath()
{
    unsigned int avatarPositionX = 0;
    unsigned int avatarPositionY = 0;
    
    this->_pathFinder->ClearPathAndDestination();
    //this->_pathFinder->SetRoot(Algo::GetMap().GetAvatarX(), Algo::GetMap().GetAvatarY());
    IPathFinder::DirectionList dir = this->_pathFinder->ComputePath();
    // Hack bidon car je suis pas sur d'avoir le droit de passer des methode en public
    if (dir.size() == 2 && !Algo::CheckMovement(dir.front()))
    {
        dir.push_back(dir.front());
        dir.pop_front();
    }
    Algo::Move(dir);
    return ;
    // GetMap() to get a const reference to the map
    // You are encourage to create filters from this map but maybe not here ,,,
    Algo::GetMap();
    
    // Move(Avatar::Direction) to set a new move the avatar
    for (int i = 0; i < 10; ++i)
    {
        // CheckMovement to check if next movement will by applied (not blocked)
        int rnd = (rand() % 4 + 1);
        switch (rnd)
        {
            case 1 :
                if (Algo::CheckMovement(Avatar::DOWN))
                    Move(Avatar::DOWN);
                break;
            case 2 :
                if (Algo::CheckMovement(Avatar::UP))
                    Move(Avatar::UP);
                break;
            case 3 :
                if (Algo::CheckMovement(Avatar::LEFT))
                    Move(Avatar::LEFT);
                break;
            case 4 :
                if (Algo::CheckMovement(Avatar::RIGHT))
                    Move(Avatar::RIGHT);
                break;
        };
    }
    
    // Move (list) to define a sequence of moves
    // Move(Avatar::Direction) to set a new move the avatar
    std::list<Avatar::Direction> movList;
    for (int i = 0; i < 10; ++i)
    {
        // CheckMovement to check if next movement will by applied (not blocked)
        int rnd = (rand() % 4 + 1);
        switch (rnd)
        {
            case 1 :
                if (Algo::CheckMovement(Avatar::DOWN))
                    movList.push_back(Avatar::DOWN);
                break;
            case 2 :
                if (Algo::CheckMovement(Avatar::UP))
                    movList.push_back(Avatar::UP);
                break;
            case 3 :
                if (Algo::CheckMovement(Avatar::LEFT))
                    movList.push_back(Avatar::LEFT);
                break;
            case 4 :
                if (Algo::CheckMovement(Avatar::RIGHT))
                    movList.push_back(Avatar::RIGHT);
                break;
        };
    }
    Algo::Move(movList);
}

const void Brain::DrawDebug(sf::RenderWindow& app)
{
    this->_pathFinder->DrawDebug(app);
}


Brain::Brain(Map* map)
: Algo(map)
, _pathFinder(new RandomWaypointPathFinder())
{
    this->_pathFinder->Init(map);
}

// DTOR ------------------------------------------------------------------
Brain::~Brain()
{
    this->_pathFinder->ClearPathAndDestination();
    delete this->_pathFinder;
}
