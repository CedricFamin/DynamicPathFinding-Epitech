#include "../includes/avatar.hpp"

// GetPositon ------------------------------------------------------------
sf::Vector2f Avatar::GetPosition()
{
    return _position;
}

// Move (enum Avatar::Direction) -----------------------------------------
bool Avatar::Move(Avatar::Direction dir)
{
    sf::Vector2f vec;
    switch (dir)
    {
        case UP :
            vec = sf::Vector2f(0 , -Avatar::_AVATAR_SIZE);
            break;
        case DOWN :
            vec = sf::Vector2f(0, Avatar::_AVATAR_SIZE);
            break;
        case LEFT :
            vec = sf::Vector2f(-Avatar::_AVATAR_SIZE, 0);
            break;
        case RIGHT :
            vec = sf::Vector2f(Avatar::_AVATAR_SIZE, 0);
            break;
    }
    Move(vec);
    return true;
}

// Move (sf::Vector2f) ---------------------------------------------------
bool Avatar::Move(sf::Vector2f& vec)
{
    _position += vec;
    _minX += vec.x;
    _maxX += vec.x;
    _minY += vec.y;
    _maxY += vec.y;
    _shape->Move(vec);
    return true;
}

// Default ctor ----------------------------------------------------------
Avatar::Avatar(sf::Vector2f& position)
:  Collidable(position.x, position.y, position.x + _AVATAR_SIZE, position.y + _AVATAR_SIZE)
{
    _position = position;
    sf::Color col(sf::Color::Green);
    SetColor(col);
}

// default dtor ----------------------------------------------------------
Avatar::~Avatar()
{
}
