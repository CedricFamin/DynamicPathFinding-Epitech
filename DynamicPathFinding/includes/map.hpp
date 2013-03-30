#ifndef __M_AI_DYNAMICPATHFINDING_MAP_HPP__
#define __M_AI_DYNAMICPATHFINDING_MAP_HPP__

// Standard C Includes
#include <stddef.h>

// Standard C++ Includes
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <list>

// Boost Includes
#include <boost/logic/tribool.hpp>

// TBB Includes

// SFML Includes
#include <SFML/Graphics.hpp>

// DynamicPathFinding Includes
#include "avatar.hpp"
#include "collidable.hpp"
#include "dictionnary.hpp"
#include "door.hpp"
#include "wall.hpp"

using namespace M_AI_DynamicPathFinding;

namespace M_AI_DynamicPathFinding
{
    class Avatar;
    class Collidable;
    class Door;
    class Wall;
    /// This class represents a map for
    /// the ACSEL's DynamicPathFinding SuperCool Test Software
    class Map
    {
    private:
        static const std::string _DEFAULT_MAP_PATH; //< define the default map folder
        
        static const std::string _MAP_DEFAULT_NAME; //< default map name
        
    public:
        static const unsigned int _MAP_SCALE = 150; //< default unit size (in px)
        static const unsigned int _MAP_STATICBLOCK = 1 << 0;
        static const unsigned int _MAP_DYNAMICBLOCK = 1 << 1;
        static const unsigned int _MAP_GOAL = 1 << 2;
        
        /// Map file Dictionnary words ID
        enum DICTIONNARY_WORD
        {
            UNKNOWN,
            SIZE,
            BOUNDED,
            NAME,
            WALL,
            DOOR,
            AVATAR,
            COMMENT,
            GOAL
        };
        
    private:
        boost::tribool _boundedByWalls;	//< True if the world is bounded by wall
                                        //< Else when a character reach the end in X or Y
                                        //< He reappears on the other side (like a plannisphere)
        
        DictionnaryNode_t* _dictionnary;	//< dictionnary root node
        
        std::string _fileName;		//< string containing the file
                                    //< file extension must be included
        
        bool _isGenerated;			//< Does the SFML object have been created
        
        bool _isReady;			//< True is the map is correctly setted
                                //< and ready to be used
        
        std::string _mapName;		//< Name given to the map !
        
        std::string _path;			//< string containing the file path
        
        unsigned int _sizeX;		//< map size in X (units represents a square meter)
        
        unsigned int _sizeY;		//< map size in Y (units represents a square meter)
        
        sf::Shape _borders;
        
        std::list<Collidable*> _staticGeometry;	//< list of all obstacles and collisionable object that could not move
        
        std::list<Collidable*> _dynamicGeometry;	//< list of all obstables and collisionable object that have to be updated
        
        unsigned int* _map; //< map
        
        Avatar* _avatar; //< information about the avatar
        
        sf::Shape _goal;
        
        unsigned int _goalX;
        
        unsigned int _goalY;
        
    private:
        /// Add a door in this map
        void AddDoor(std::string& line);
        
        /// Add dynamic geometry
        bool AddDynamicGeometry(Collidable* obj);
        
        /// Add static geometry
        bool AddStaticGeometry(Collidable* obj);
        
        /// Add a wall in this map
        void AddWall(std::string& line);
        
        /// Check if object is correct coordinates
        /// and definition before static or dynamic object list insertion.
        bool CheckSquareElement(const Collidable* obj);
        
        /// Extract coordinates of a wall or door from a line
        void ExtractObstacleCoord(std::string& line, unsigned int* minX, unsigned int* minY, unsigned int* maxX, unsigned int* maxY);
        
        /// Generate the map table (bouh c'est laid !)
        void GenerateMap(void);
        
        /// Load the <_fileName> file and create map objects
        bool LoadFile(void);
        
        /// Initialize the dictionnary use to parse map files
        void InitializeDictionnary(void);
        
        /// Set bounded by wall value from file
        void RegisterBounded(std::string& line);
        
        /// Register the map name from file line
        void RegisterName(std::string& line);
        
        /// Set the map size from the configuration file line
        void RegisterSize(std::string& line);
        
        /// Set the border display (blocking by default)
        void SetBorders(void);
        
        /// Set the avatar's goal (destination)
        void SetGoal(std::string& line);
        
    protected:
        bool SetAvatar(std::string& line);
        
        /// Set Avatar Position
        bool SetAvatar(sf::Vector2f position);
        
    public:
        /// Draw the whole Map
        const void Draw(sf::RenderWindow& app);
        
        Avatar* GetAvatar(void);
        
        /// Return state of the boundedByWalls
        const boost::tribool GetBounded(void);
        
        unsigned int GetGoalX(void);
        
        unsigned int GetGoalY(void);
        
        ///
        const unsigned int* GetMap(void) const;
        
        // Return defined map name
        const std::string GetMapName(void);
        
        /// Get map's size in X
        const unsigned int GetX(void);
        
        /// Get map's size in Y
        const unsigned int GetY(void);
        
        /// Returns True if map is correctly setted
        const bool IsReady(void);
        
        /// Display the map on the cout
        void TermDisplay(void);
        
    public:
        /// Default Ctor
        Map(void);
        
        /// File Loading Ctor
        Map(std::string& fileName);
        
        /// Dtor
        ~Map(void);
    };
}

#endif /* __M_AI_DYNAMICPATHFINDING_MAP_HPP__ */
