//
//  NavigationMeshPathFinder.cpp
//  DynamicPathFinding
//
//  Created by Cédric Famin on 03/04/13.
//
//

#include <queue.h>

#include "../../includes/PathFinder/NavigationMeshPathFinder.h"

NavigationMeshPathFinder::NavigationMeshPathFinder()
{

}

NavigationMeshPathFinder::~NavigationMeshPathFinder()
{
    
}

bool IsWall(Map const * map, int x, int y)
{
    if (x == -1 || y == -1)
        return true;
    
    bool wall = map->GetMap()[x + y * map->GetX()];
    wall |= x == map->GetX() || y == map->GetY();
    return wall;
}

enum Corner
{
    NO_CORNER = 0,
    TOP = 1,
    BOT = 2,
    LEFT = 4,
    RIGHT = 8
};

bool operator<(Vertice const & v1, Vertice const & v2)
{
    if (v1.x < v2.x)
        return true;
    if (v1.x == v2.x && v1.y < v2.y)
        return true;
    return false;
}

Delaunay NavigationMeshPathFinder::GetAllVertices(Map const * map)
{
    Delaunay vertices;
    this->_mapY = map->GetY();
    this->_mapX = map->GetX();
    this->_marshingSquare = new char[map->GetX() * map->GetY()];
    
    for (int currentY = 0; currentY <= this->_mapY; ++currentY)
    {
        for (int currentX = 0; currentX < this->_mapX; ++currentX)
        {
            bool wall = IsWall(map, currentX, currentY);
            
            char corner = 0;
            if (!wall)
            {
                if (IsWall(map, currentX - 1, currentY))
                    corner += LEFT;
                if (IsWall(map, currentX + 1, currentY))
                    corner += RIGHT;
                if (IsWall(map, currentX, currentY - 1))
                    corner += TOP;
                if (IsWall(map, currentX, currentY + 1))
                    corner += BOT;
                
                if (corner == 0)
                {
                    if (IsWall(map, currentX - 1, currentY - 1))
                        corner += LEFT + TOP;
                    if (IsWall(map, currentX + 1, currentY + 1))
                        corner += RIGHT + BOT;
                    if (IsWall(map, currentX + 1, currentY - 1))
                        corner += RIGHT + TOP;
                    if (IsWall(map, currentX - 1, currentY + 1))
                        corner += LEFT + BOT;
                }
            }
            if (corner == BOT || corner == TOP || corner == RIGHT || corner == LEFT)
                corner = 0;
            this->_marshingSquare[currentX + currentY * map->GetX()] = corner;
            if (corner)
            {
                Vertice vertice;
                vertice.x = currentX;
                vertice.y = currentY;
                this->_vertices.push_back(vertice);
                
                Point pt(currentX, currentY);
                vertices.insert(pt);
            }
        }
    }
    return vertices;
}

void NavigationMeshPathFinder::Init(Map const * map)
{
    // Pre process the map in order to create the graph
    
    // 1 - Get All vertices ordered by X and Y
    _triangles = this->GetAllVertices(map);
    
    // 2 - Create mesh using Delaunay Triangulation
    
}

unsigned int NavigationMeshPathFinder::EvalNode(node_type* node) const
{
    return 0;
}

void NavigationMeshPathFinder::DrawDebug(sf::RenderWindow& app) const
{
    for (Vertice const & vertice : this->_vertices)
    {
        unsigned int x = vertice.x * Map::_MAP_SCALE + Map::_MAP_SCALE * 0.5;
        unsigned int y = vertice.y * Map::_MAP_SCALE + Map::_MAP_SCALE * 0.5;
        sf::Shape point = sf::Shape::Circle(x, y, Map::_MAP_SCALE * 0.5, sf::Color(255, 0, 0));
        
        app.Draw(point);
    }
    
    for (unsigned int y = 0; y < this->_mapY; ++y)
    {
        for (unsigned int x = 0; x < this->_mapX; ++x)
        {
            unsigned int index = x + y * this->_mapY;
            unsigned int xPos = x * Map::_MAP_SCALE;
            unsigned int yPos = y * Map::_MAP_SCALE;
            char ccase = this->_marshingSquare[index];
            if (ccase == NO_CORNER) continue;
            
            sf::Color color(0, 0, 0);
            
            color.r += ((ccase & BOT) > 0) * 155;
            color.g += ((ccase & TOP) > 0) * 155;
            color.b += ((ccase & RIGHT) > 0) * 155;
            
            
            color.b += ((ccase & LEFT) > 0) * 75;
            color.g += ((ccase & LEFT) > 0) * 75;
            
            sf::Shape point = sf::Shape::Rectangle(xPos, yPos, xPos + Map::_MAP_SCALE, yPos + Map::_MAP_SCALE, color);
            
            app.Draw(point);
        }
    }
    
    sf::Color colors[6] = {
        sf::Color(200, 0, 0),
        sf::Color(200, 200, 0),
        sf::Color(200, 0, 200),
        sf::Color(200, 120, 120),
        sf::Color(0, 200, 0),
        sf::Color(0, 0, 200),
    };
    auto face = this->_triangles.all_faces_begin();
    unsigned int j = 0;
    for (; face != this->_triangles.all_faces_end(); ++face)
    {
        j = (j + 1);
        sf::Shape shape = sf::Shape();
        for (int i = 0; i <= 2; ++i)
        {
            auto vertex = face->vertex(i);
            
            shape.AddPoint(sf::Vector2f(vertex->point().x() * Map::_MAP_SCALE, vertex->point().y() * Map::_MAP_SCALE), colors[j% 6]);
            
            app.Draw(shape);
        }
        if (j == 50)
            break;
    }
    

}
