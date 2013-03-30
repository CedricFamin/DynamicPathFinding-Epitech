#include "../includes/map.hpp"

const std::string Map::_DEFAULT_MAP_PATH = "../map/";
const std::string Map::_MAP_DEFAULT_NAME = "Boring Clumsy Default Map Name :(";

// AddDynamicObject --------------------------------------------------------
bool Map::AddDynamicGeometry(Collidable* obj)
{
}

// AddDoor -----------------------------------------------------------------
void Map::AddDoor(std::string& line)
{
    if (StripArgument(line, 6).empty() && !StripArgument(line, 5).empty())
    {
        // Get Coordinates
        unsigned int minX, minY, maxX, maxY;
        ExtractObstacleCoord(line, &minX, &minY, &maxX, &maxY);
        // Get Door's state
        std::stringstream tmpStr;
        bool state;
        tmpStr << StripArgument(line, 5);
        tmpStr >> state;
        
        // Creating door object
        Door* newObj = new Door(minX, minY, maxX, maxY, state);
        if (newObj->IsReady())
        {
            if (!AddStaticGeometry(newObj))
                delete newObj;
        }
        else
        {
            std::cerr << "/!\\ Warning : Trying to add non ready object to collidable" << std::endl;
            delete newObj;
        }
    }
    else
    {
        std::cerr << "/!\\ Warning : a <door> keyword has incorrect number of arguments" << std::endl;
    }
}

// AddStaticGeometry -------------------------------------------------------
bool Map::AddStaticGeometry(Collidable* obj)
{
    if (CheckSquareElement(obj))
    {
        _staticGeometry.push_front(obj);
        return true;
    }
    return false;
}

// AddWall -----------------------------------------------------------------
void Map::AddWall(std::string& line)
{
    if (StripArgument(line, 5).empty() && !StripArgument(line, 4).empty())
    {
        // Get Coordinates
        unsigned int minX = 0, minY = 0, maxX = 0, maxY = 0;
        ExtractObstacleCoord(line, &minX, &minY, &maxX, &maxY);
        
        // Creating wall object
        Wall* newObj = new Wall(minX, minY, maxX, maxY);
        if (newObj->IsReady())
        {
            if (!AddStaticGeometry(newObj))
                delete newObj;
        }
        else
        {
            std::cerr << "/!\\ Warning : Trying to add non ready wall to static collidable" << std::endl;
            std::cout << "error coord " << minX << " " << minY << " " << maxX << " " << maxY << std::endl;
            
            delete newObj;
        }
    }
    else
    {
        std::cerr << "/!\\ Warning : a <wall> keyword has incorrect number of arguments" << std::endl;
    }
}

// CheckSquareElement ------------------------------------------------------
bool Map::CheckSquareElement(const Collidable* obj)
{
    if (obj->GetMinX() > obj->GetMaxX() || obj->GetMinY() > obj->GetMaxY())
    {
        std::cerr << "/!\\ Warning : <collidable> defined by [" << obj->GetMinX() << "," << obj->GetMinY() << "|" << obj->GetMaxX() << "," << obj->GetMaxY() << "] has incoherent arguments" << std::endl;
    }
    else if (obj->GetMinX() / _MAP_SCALE > _sizeX || obj->GetMinY() / _MAP_SCALE > _sizeY || obj->GetMaxX() / _MAP_SCALE > _sizeX || obj->GetMaxY() / _MAP_SCALE > _sizeY)
    {
        if (_sizeX == 0 || _sizeY == 0)
            std::cerr << "/!\\ Warning : Trying to add collidable object before <size> definition or with wrong <size> definition : object ignored !" << std::endl;
        else
            std::cerr << "/!\\ Warning : Trying to add collidable object with coordinates out of bound" << std::endl;
    }
    else
    {
        return true;
    }
    return false;
}

// ExtractObjectCoord ------------------------------------------------------
void Map::ExtractObstacleCoord(std::string& line, unsigned int* minX, unsigned int* minY, unsigned int* maxX, unsigned int* maxY)
{
    *minX = (atoi(StripArgument(line, 1).c_str()) - 1) * _MAP_SCALE;
    *minY = (atoi(StripArgument(line, 2).c_str()) - 1) * _MAP_SCALE;
    *maxX = (atoi(StripArgument(line, 3).c_str()) - 1) * _MAP_SCALE + _MAP_SCALE;
    *maxY = (atoi(StripArgument(line, 4).c_str()) - 1) * _MAP_SCALE + _MAP_SCALE;
}

// GenerateMap -------------------------------------------------------------
void Map::GenerateMap()
{
    _map = new unsigned int[_sizeX * _sizeY];
    memset(_map, 0, _sizeX * _sizeY * sizeof(unsigned int));
    std::list<Collidable*>::iterator it;
    for (it = _staticGeometry.begin(); it != _staticGeometry.end(); ++it)
    {
        for (unsigned int i = (*it)->GetMinY() / _MAP_SCALE; i < (*it)->GetMaxY() / _MAP_SCALE; ++i)
        {
            for (unsigned int j = (*it)->GetMinX() / _MAP_SCALE; j < (*it)->GetMaxX() / _MAP_SCALE; ++j)
            {
                _map[i * _sizeX + j] = _MAP_STATICBLOCK;
            }
        }
    }
    _map[_goalX / _MAP_SCALE + _goalY / _MAP_SCALE * _sizeX] = _MAP_GOAL;
}

// LoadFile ----------------------------------------------------------------
bool Map::LoadFile()
{
    std::ifstream inputFile;
    std::string line;
    std::string word;
    
    // Check if <_fileName> has been setted
    if (_fileName.empty())
    {
        std::cerr << "/!\\ Trying to open map file while no file name has been setted" << std::endl;
        return false;
    }
    else
    {
        // try to Open <_fileName>
        inputFile.open(this->_fileName.c_str());
        if (inputFile.is_open())
        {
            InitializeDictionnary();
            while (inputFile.good())
            {
                std::getline(inputFile, line);
                word = StripFirstWord(line);
                if (!word.empty())
                {
                    switch (ParseWord(_dictionnary, word.c_str()))
                    {
                        case Map::SIZE :
                            RegisterSize(line);
                            break;
                        case Map::BOUNDED :
                            RegisterBounded(line);
                            SetBorders();
                            break;
                        case Map::NAME :
                            RegisterName(line);
                            break;
                        case Map::WALL :
                            AddWall(line);
                            break;
                        case Map::DOOR :
                            AddDoor(line);
                            break;
                        case Map::AVATAR :
                            SetAvatar(line);
                            break;
                        case Map::GOAL :
                            SetGoal(line);
                            break;
                        case Map::COMMENT :
                            break;
                        case Map::UNKNOWN :
                        default :
                            std::cerr << "/!\\ Warning : Unknown or unhandled keyword !" << std::endl;
                            break;
                    }
                }
            }
        }
        else
        {
            std::cerr << "/!\\ Unable to open file [" << _fileName << "]" << std::endl;
            return false;
        }
    }
}

// InitializeDictionnary ---------------------------------------------------
void Map::InitializeDictionnary()
{
    if (_dictionnary == NULL)
    {
        _dictionnary = AddWord("size", Map::SIZE);
        AddWord("bounded", Map::BOUNDED, _dictionnary);
        AddWord("name", Map::NAME, _dictionnary);
        AddWord("wall", Map::WALL, _dictionnary);
        AddWord("door", Map::DOOR, _dictionnary);
        AddWord("avatar", Map::AVATAR, _dictionnary);
        AddWord("goal", Map::GOAL, _dictionnary);
        AddWord("#", Map::COMMENT, _dictionnary);
    }
}

// RegisterBounded ---------------------------------------------------------
void Map::RegisterBounded(std::string& line)
{
    if (_boundedByWalls != 2)
    {
        std::cerr << "/!\\ Warning : <bounded> keyword redefinition" << std::endl;
    }
    if (StripArgument(line, 2).empty() && !StripArgument(line, 1).empty())
    {
        bool value;
        std::stringstream tmpStr;
        tmpStr << StripArgument(line, 1);
        tmpStr >> value;
        _boundedByWalls = value;
    }
    else
    {
        std::cerr << "/!\\ Error : <bounded> keyword has no or wrong argument in map file" << std::endl;
    }
}

// RegisterName ------------------------------------------------------------
void Map::RegisterName(std::string& line)
{
    std::string first;
    
    first = StripArgument(line, 1);
    if (!first.empty())
    {
        _mapName = line.substr(line.find(first), std::string::npos);
    }
    else
    {
        std::cerr << "/!\\ Warning : <name> keyword found in map file but no name have been defined !" << std::endl;
        _mapName = Map::_MAP_DEFAULT_NAME;
    }
}

// Register size -----------------------------------------------------------
void Map::RegisterSize(std::string& line)
{
    if (StripArgument(line, 3).empty())
    {
        if (_sizeX != 0 || _sizeY != 0)
        {
            std::cerr << "/!\\ Warning : Map size redefinition in configuration file" << std::endl;
        }
        
        _sizeX = atoi(StripArgument(line, 1).c_str());
        _sizeY = atoi(StripArgument(line, 2).c_str());
        
        if (_sizeX == 0 || _sizeY == 0)
        {
            std::cerr << "/!\\ Error : Map Configuration file has an invalid map size" << std::endl;
        }
    }
    else
    {
        std::cerr << "/!\\ Error : <size> keyword in map file has bad arguments" << std::endl;
    }
}

// Draw --------------------------------------------------------------------
const void Map::Draw(sf::RenderWindow& app)
{
    //  std::cout << "static : " << _staticGeometry.size() << " | dynamic : " << _dynamicGeometry.size() << std::endl;
    
    app.Draw(_borders);
    app.Draw(_goal);
    std::list<Collidable*>::iterator it;
    for (it = _staticGeometry.begin(); it != _staticGeometry.end(); ++it)
        app.Draw(*(*it)->GetShape());
    for (it = _dynamicGeometry.begin(); it != _dynamicGeometry.end(); ++it)
        app.Draw(*(*it)->GetShape());
    app.Draw(*(_avatar->GetShape()));
}

// GetAvatar ---------------------------------------------------------------
Avatar* Map::GetAvatar()
{
    return _avatar;
}

// GetBounded --------------------------------------------------------------
const boost::tribool Map::GetBounded()
{
    return this->_boundedByWalls;
}

// GetMap ------------------------------------------------------------------
const unsigned int* Map::GetMap() const
{
    return _map;
}

// GetMapName --------------------------------------------------------------
const std::string Map::GetMapName()
{
    return _mapName;
}

// GetX --------------------------------------------------------------------
const unsigned int Map::GetX()
{
    return _sizeX;
}

// GetY --------------------------------------------------------------------
const unsigned int Map::GetY()
{
    return _sizeY;
}

// IsReady -----------------------------------------------------------------
const bool Map::IsReady()
{
    return this->_isReady;
}

// DisplayTerm -------------------------------------------------------------
void Map::TermDisplay()
{
    static bool blop = false;
    if (!blop)
    {
        for (unsigned int i = 0; i < _sizeY; ++i)
        {
            for (unsigned int j = 0; j < _sizeX; ++j)
                std::cout << _map[j + i * _sizeX] << " | ";
            std::cout << std::endl;
        }
        blop = true;
    }
}

bool Map::SetAvatar(std::string& line)
{
    unsigned int minX, minY, maxX, maxY = 0;
    ExtractObstacleCoord(line, &minX, &minY, &maxX, &maxY);
    
    sf::Vector2f vec(minX, minY);
    SetAvatar(vec);
}

// SetAvatar ---------------------------------------------------------------
bool Map::SetAvatar(sf::Vector2f position)
{
    if (_avatar == NULL)
    {
        _avatar = new Avatar(position);
        if (!CheckSquareElement(_avatar))
        {
            delete _avatar;
            _avatar = NULL;
            std::cerr << "/!\\ Warning Avatar is TP-dead into the void" << std::endl;
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        std::cout << "/!\\ Error Avatar already exists" << std::endl;
        return false;
    }
}

// SetBorder ---------------------------------------------------------------
void Map::SetBorders()
{
    sf::Color borderColor;
    if (_boundedByWalls == false)
    {
        borderColor += sf::Color(80, 80, 80);
    }
    else
    {
        borderColor += sf::Color::White;
    }
    _borders.AddPoint(0, 0, sf::Color::Blue, borderColor);
    _borders.AddPoint(_sizeX * _MAP_SCALE, 0, sf::Color::Blue, borderColor);
    _borders.AddPoint(_sizeX * _MAP_SCALE, _sizeY * _MAP_SCALE, sf::Color::Blue, borderColor);
    _borders.AddPoint(0, _sizeY * _MAP_SCALE, sf::Color::Blue, borderColor);
    _borders.SetOutlineWidth(100);
    _borders.EnableOutline(true);
    _borders.EnableFill(false);
}

// SetGoal -----------------------------------------------------------------
void Map::SetGoal(std::string& line)
{
    unsigned int minX, minY = 0;
    minX = atoi(StripArgument(line, 1).c_str()) * _MAP_SCALE;
    minY = atoi(StripArgument(line, 2).c_str()) * _MAP_SCALE;
    
    _goal.AddPoint(minX, minY, sf::Color::Red);
    _goal.AddPoint(minX, minY + _MAP_SCALE, sf::Color::Red);
    _goal.AddPoint(minX + _MAP_SCALE, minY + _MAP_SCALE, sf::Color::Red);
    _goal.AddPoint(minX + _MAP_SCALE, minY, sf::Color::Red);
    
    _goalX = minX;
    _goalY = minY;
}


unsigned int Map::GetGoalX()
{
    return _goalX;
}

unsigned int Map::GetGoalY()
{
    return _goalY;
}
// Default Ctor ------------------------------------------------------------
Map::Map()
: _avatar(NULL), 
_boundedByWalls(2),
_dictionnary(NULL),
_isReady(false),
_sizeX(0),
_sizeY(0)
{
    this->_path = Map::_DEFAULT_MAP_PATH;
}

// File Ctor ---------------------------------------------------------------
Map::Map(std::string& fileName)
: _avatar(NULL),
_boundedByWalls(2),
_dictionnary(NULL),
_isReady(false),
_sizeX(0),
_sizeY(0)
{
    if (fileName.empty())
    {
        std::cerr << "Given file name is empty !!!" << std::endl;
    }
    else
    {
        _fileName = fileName;
        std::cout << "Loading map file [" << this->_fileName << "]" << std::endl;
        LoadFile();
        if (_boundedByWalls == 2)
        {
            _boundedByWalls = true;
            SetBorders();
        }
        GenerateMap();
    }
}

// Default Dtor ------------------------------------------------------------
Map::~Map()
{
    if (_avatar != NULL)
    {
        delete _avatar;
        _avatar = NULL;
    }
    
    std::list<Collidable*>::iterator it;
    for (it = _staticGeometry.begin(); it != _staticGeometry.end(); ++it)
        delete (*it);
    _staticGeometry.clear();
    for (it = _dynamicGeometry.begin(); it != _dynamicGeometry.end(); ++it)
        delete (*it);
    _dynamicGeometry.clear();
}
