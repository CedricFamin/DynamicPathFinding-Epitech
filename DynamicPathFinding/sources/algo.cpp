#include "../includes/algo.hpp"

// CheckMovement ----------------------------------------------------------
bool Algo::CheckMovement(Avatar::Direction dir)
{
    switch (dir)
    {
        case Avatar::DOWN:
            if (_plannedPos[1] < _map->GetY() - 1)
                if (_map->GetMap()[_plannedPos[0] + (_plannedPos[1] + 1) * _map->GetX()] == 0)
                    return true;
                else
                    return false;
                else
                    return false;
            break;
        case Avatar::UP:
            if (_plannedPos[1] > 0)
                if (_map->GetMap()[_plannedPos[0] + (_plannedPos[1] - 1) * _map->GetX()] == 0)
                    return true;
                else
                    return false;
                else
                    return false;
            break;
        case Avatar::RIGHT:
            if (_plannedPos[0] < _map->GetX() - 1)
                if (_map->GetMap()[_plannedPos[0] + 1 + (_plannedPos[1]) * _map->GetX()] == 0)
                    return true;
                else
                    return false;
                else
                    return false;
            break;
        case Avatar::LEFT:
            if (_plannedPos[0] > 0)
                if (_map->GetMap()[_plannedPos[0] - 1 + (_plannedPos[1]) * _map->GetX()] == 0)
                    return true;
                else
                    return false;
                else
                    return false;
            break;
    };
}

// GetMap -----------------------------------------------------------------
const unsigned int& Algo::GetMap() const
{
    return *_map->GetMap();
}

// Move -------------------------------------------------------------------
void Algo::Move(Avatar::Direction dir)
{
    if (CheckMovement(dir))
    {
        switch (dir)
        {
            case Avatar::DOWN:
                _plannedPos[1] += 1;
                _moves.push_back(dir);
                break;
            case Avatar::UP:
                _plannedPos[1] -= 1;
                _moves.push_back(dir);
                break;
            case Avatar::RIGHT:
                _plannedPos[0] += 1;
                _moves.push_back(dir);
                break;
            case Avatar::LEFT:
                _plannedPos[0] -= 1;
                _moves.push_back(dir);
                break;
        };
    }
}

// Move (list) ------------------------------------------------------------
void Algo::Move(std::list<Avatar::Direction> movList)
{
    std::list<Avatar::Direction>::iterator it;
    for (it = movList.begin(); it != movList.end(); ++it)
        Move(*it);
}

// Update -----------------------------------------------------------------
void Algo::Update()
{
    if (_avatar == NULL) return;
    if (_moves.size() != 0)
    {
        sf::Vector2f vec;
        float dist = Map::_MAP_SCALE / 10;
        switch (_moves.front())
        {
            case Avatar::DOWN:
                vec = sf::Vector2f(0, dist);
                break;
            case Avatar::UP:
                vec = sf::Vector2f(0, -dist);
                break;
            case Avatar::RIGHT:
                vec = sf::Vector2f(dist, 0);
                break;
            case Avatar::LEFT:
                vec = sf::Vector2f(-dist, 0);
                break;
        };
        _avatar->Move(vec);
        if (++_count == 10)
        {
            _count = 0;
            _moves.pop_front();
        }
    }
    else
    {
        ComputePath();
    }
}

// CTOR -------------------------------------------------------------------
Algo::Algo(Map* labyrinth)
: _avatar(NULL),
_count(0),
_map(NULL)
{
    if (labyrinth != NULL)
    {
        _map = labyrinth;
        _avatar = _map->GetAvatar();
        _moves.clear();
        _plannedPos[0] = _avatar->GetPosition().x / Map::_MAP_SCALE;
        _plannedPos[1] = _avatar->GetPosition().y / Map::_MAP_SCALE;
    }
}

Algo::~Algo()
{
}
