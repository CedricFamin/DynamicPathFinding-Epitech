//
//  VectorFieldHistogramPathFinder.cpp
//  DynamicPathFinding
//
//  Created by Cédric Famin on 13/04/13.
//
//

#include "../../includes/PathFinder/VectorFieldHistogramPathFinder.h"

VectorFieldHistogramPathFinder::VectorFieldHistogramPathFinder()
: _width(0)
, _height(0)
, _avatar(0)
{
}

VectorFieldHistogramPathFinder::~VectorFieldHistogramPathFinder()
{
}

void VectorFieldHistogramPathFinder::Init(Map const * map)
{
    _width = map->GetX();
    _height = map->GetY();
    _avatar = map->GetAvatar();
    _goalX = map->GetGoalX() / Map::_MAP_SCALE;
    _goalY = map->GetGoalY() / Map::_MAP_SCALE;
    _map = map->GetMap();
    
    _alreadyTakingPath = new bool*[this->_width * this->_height];
    for (unsigned int i = 0; i < this->_width * this->_height; ++i)
        _alreadyTakingPath[i] = new bool[POLAR_RANGE];
}


void VectorFieldHistogramPathFinder::SetGoal(unsigned int goalX, unsigned int goalY)
{
    _goalX = goalX;
    _goalY = goalY;
    
    for (unsigned int i = 0; i < this->_width * this->_height; ++i)
    {
        for (unsigned int j = 0; j < POLAR_RANGE; ++j)
            _alreadyTakingPath[i][j] = false;
    }
}

VectorFieldHistogramPathFinder::DirectionList VectorFieldHistogramPathFinder::ComputePath()
{
    DirectionList dirs;
    
    // la technique est simple
    // On part de la map que l'on restreint (ex 20 case) que l'on appelle l'"Active window"
    // On transforme la grille en "polar histogram"
    // De la on choisi une direction parmis les "trous" dans l'histogram
    static float const PI = 3.14f;
    
    this->_visited.push_back(this->_avatar->GetPosition());
    unsigned int avatarX = this->_avatar->GetPosition().x / Map::_MAP_SCALE;
    unsigned int avatarY = this->_avatar->GetPosition().y / Map::_MAP_SCALE;
    int targetDirection = 0;
    {
        Position vectorPT = {_goalX - avatarX, _goalY - avatarY};
        if (abs(vectorPT.x) <= 1 && abs(vectorPT.y) <= 1)
            return dirs;
        
        double angle = atan2(vectorPT.y, vectorPT.x);
        targetDirection = 180.0 * angle / 3.14;
        targetDirection = targetDirection / POLAR_HISTOGRAM_STEP + (targetDirection % POLAR_HISTOGRAM_STEP > POLAR_HISTOGRAM_STEP / 2);
        
        if (targetDirection < 0)
            targetDirection += POLAR_RANGE;
    }
    
    for (unsigned int euleurAngle = 0; euleurAngle <= 360; euleurAngle += POLAR_HISTOGRAM_STEP)
    {
        unsigned int dist = 1;
        float xDecal = cosf(euleurAngle * PI / 180.0f);
        float yDecal = sinf(euleurAngle * PI / 180.0f);
        for (; dist < ACTIVE_WINDOW_RADIUS; dist++)
        {
            unsigned int x = xDecal * dist + avatarX;
            unsigned int y = yDecal * dist + avatarY;
            
            if (x > this->_width || y > this->_height)
                break;
            if (this->_map[x + y * this->_width])
                break;
        }
        _polarResult[euleurAngle/POLAR_HISTOGRAM_STEP] = ACTIVE_WINDOW_RADIUS - dist;
        
    }
    
    // Convert target direction in
    int direction = targetDirection;
    unsigned int takingIndex = avatarX + avatarY * this->_width;
    unsigned int distToGoal = abs(avatarX - this->_goalX) + abs(avatarY - this->_goalY);
    
    for (unsigned int i = 0; i <= POLAR_RANGE / 2; ++i)
    {
        direction = (targetDirection + i) % POLAR_RANGE;
        
        if (!this->_alreadyTakingPath[takingIndex][direction] && (!_polarResult[direction] || _polarResult[direction] > distToGoal))
            break;
        
        // On tourne a 360
        direction = targetDirection - i;
        if (direction < 0) direction += POLAR_RANGE;
        
        // On remet la direction dans la range [0; POLAR_RANGE]
        if (!this->_alreadyTakingPath[takingIndex][direction] && (!_polarResult[direction] || _polarResult[direction] > distToGoal))
            break;
    }
    
    // convert direction index into avatar direction
    this->_lastDirection = direction;
    this->_alreadyTakingPath[takingIndex][direction] = true;
    direction *= POLAR_HISTOGRAM_STEP;
    
    // monkey conversion pas belle
    double cosDir = cos(direction * PI / 180.0f), sinDir = sin(direction * PI / 180.0f);
    
    if (cosDir)
    {
        if (cosDir > 0)
            dirs.push_back(Avatar::RIGHT);
        else
            dirs.push_back(Avatar::LEFT);
    }
    
    if (sinDir)
    {
        if (sinDir > 0)
            dirs.push_back(Avatar::DOWN);
        else
            dirs.push_back(Avatar::UP);
    }
    return dirs;
}

void VectorFieldHistogramPathFinder::ClearPathAndDestination()
{
    
}

void VectorFieldHistogramPathFinder::DrawDebug(sf::RenderWindow& app) const
{
    static unsigned int const x = 1 * Map::_MAP_SCALE;
    static unsigned int const y = 10 * Map::_MAP_SCALE;
    static unsigned int const stepX = 500;
    static unsigned int const textSize = 250;
    static unsigned int const baseBarHeightMax = 1200;
    {
        sf::String angleTxt("Angle");
        angleTxt.SetPosition(x, y);
        angleTxt.SetSize(textSize);
        angleTxt.SetColor(sf::Color::White);
        app.Draw(angleTxt);
    }
    
    { // BORDER
        unsigned int corners[4][2] =
        {
            {x -   50, y +  300},
            {x +11000, y +  350},
            {x +10950, y - 1500},
            {x -    0, y - 1450}
        };
        
        for (unsigned int i = 0 ; i < 4; ++i)
        {
            sf::Shape bar = sf::Shape::Rectangle(corners[i][0], corners[i][1],
                                                 corners[(i+1)%4][0], corners[(i+1)%4][1],
                                                 sf::Color::Red);
            app.Draw(bar);
        }
    }
    
    {
        std::stringstream str;
        str << this->_lastDirection;
        sf::String angleTxt("Angle avec le but : " + str.str());
        angleTxt.SetPosition(x + 500, y + 700);
        angleTxt.SetSize(textSize);
        angleTxt.SetColor(sf::Color::White);
        app.Draw(angleTxt);
    }
    
    for (unsigned int i = 0; i < POLAR_RANGE; ++i)
    {
        unsigned int baseX = x + 700 + i * stepX;
        unsigned int baseY = y;
        {
            std::stringstream str;
            str << i * POLAR_HISTOGRAM_STEP;
            sf::String angleTxt(str.str());
            
            angleTxt.SetSize(textSize);
            angleTxt.SetColor(sf::Color::White);
            sf::FloatRect rect = angleTxt.GetRect();
            unsigned int xText = baseX + (500 - rect.GetWidth()) / 2;
            angleTxt.SetPosition(xText, baseY);
            
            app.Draw(angleTxt);
        }
        unsigned int height = baseBarHeightMax * this->_polarResult[i] / ACTIVE_WINDOW_RADIUS;
        {
            sf::Shape bar = sf::Shape::Rectangle(baseX+(stepX - 100) / 2, baseY - 100,
                                                 baseX + (stepX + 100) / 2, baseY - 100 - height,
                                                 sf::Color::White);
            app.Draw(bar);
        }
    }
}
