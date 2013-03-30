#include "../includes/collidable.hpp"

// CreateShape -----------------------------------------------------------------
// Check if shape is not already defined
//
bool Collidable::CreateShape(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY)
{
    _ready = false;
    // Check if shape is not already defined
    if (!(_minX == minX && _minY == minY && _maxX == maxX && _maxY == maxY))
    {
        if (minX > maxX || minY > maxY)
        {
            if (_shape != NULL)
            {
                std::cerr << "/!\\ Warning : Trying to modify a collidable object with invalid coordinates : request ignored" << std::endl;
            }
            else
            {
                std::cerr << "/!\\ Warning : Trying to create an collidable object with invalid coordinates : request ignored !" << std::endl;
            }
        }
        else
        {
            if (_shape != NULL)
            {
                delete _shape;
                _shape = NULL;
            }
            _minX = minX;
            _minY = minY;
            _maxX = maxX;
            _maxY = maxY;
            _shape = new sf::Shape();
            _shape->AddPoint(sf::Vector2f(minX, minY), sf::Color(255, 255, 255));
            _shape->AddPoint(sf::Vector2f(minX, maxY), sf::Color(255, 255, 255));
            _shape->AddPoint(sf::Vector2f(maxX, maxY), sf::Color(255, 255, 255));
            _shape->AddPoint(sf::Vector2f(maxX, minY), sf::Color(255, 255, 255));
            return true;
        }
    }
    return false;
}

// GetMinX ---------------------------------------------------------------------
const unsigned int Collidable::GetMinX() const
{
    return _minX;
}
// GetMinY ---------------------------------------------------------------------
const unsigned int Collidable::GetMinY() const
{
    return _minY;
}

// GetMaxX ---------------------------------------------------------------------
const unsigned int Collidable::GetMaxX() const
{
    return _maxX;
}

// GetMaxY ---------------------------------------------------------------------
const unsigned int Collidable::GetMaxY() const
{
    return _maxY;
}

// GetShape --------------------------------------------------------------------
const sf::Shape* Collidable::GetShape() const
{
    return _shape;
}

bool Collidable::IsReady()
{
    return _ready;
}

// SetColor --------------------------------------------------------------------
void Collidable::SetColor(sf::Color& color)
{
    if (_shape != NULL)
    {
        _shape->SetColor(color);
    }
}

// Default Ctor ----------------------------------------------------------------
Collidable::Collidable()
: _block(false),
_destroyable(false),
_ready(false),
_shape(NULL)
{
}

// Coordinate Ctor -------------------------------------------------------------
Collidable::Collidable(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY)
: _block(false),
_destroyable(false),
_ready(false),
_shape(NULL)
{
    if (minX > maxX || minY > maxY)
    {
        std::cerr << "/!\\ Warning : Tries to create a collidable object with invalid spatial definition" << std::endl;
    }
    else
    {
        _ready = CreateShape(minX, minY, maxX, maxY);
        sf::Color col(255, 255, 255);
        SetColor(col);
    }
}

// Dtor -------------------------------------------------------------------------
Collidable::~Collidable()
{
    if (_shape != NULL)
    {
        delete _shape;
        _shape = NULL;
    }
}
