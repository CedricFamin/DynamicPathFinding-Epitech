#include "../includes/main.hpp"

// Main loop
int main(int ac, char **av)
{
    // Welcome message
    std::cout << "*************************" << std::endl << "Welcome to the DynamicPathFinding Project Software !" << std::endl;
    
    // Check for command line argument
    if (ac != 2)
    {
        std::cout << "/!\\ Error : missing argument(s)" << std::endl << "Usage : ./" << av[0] << " <map_file>" << std::endl;
        return 0;
    }
    
    // Create the labyrinth map from the given file
    std::string fileName = av[1];
    Map* labyrinth = new Map(fileName);
    
    // Create the SFML window
    std::stringstream tmpStr;
    tmpStr << "Dynamic Path Finding :: " << labyrinth->GetMapName();
    sf::RenderWindow app(sf::VideoMode(960, 640, 32), tmpStr.str(), sf::Style::Titlebar | sf::Style::Close);
    sf::Vector2f center(labyrinth->GetX() * Map::_MAP_SCALE / 2, labyrinth->GetY() * Map::_MAP_SCALE / 2);
    sf::Vector2f halfsize(labyrinth->GetX() * Map::_MAP_SCALE /2, labyrinth->GetY() * Map::_MAP_SCALE / 2);
    sf::View view(center, halfsize);
    float zoom = 1.0f;
    view.Zoom(1.0f);
    app.SetView(view);
    std::cout << "Init brain" << std::endl;
    Algo* algo = new Brain(labyrinth);
    
    bool turbo = false;
    bool pause = false;
    // While SFML application is on...
    while (app.IsOpened())
    {
        // Watch for event
        sf::Event event;
        while (app.GetEvent(event))
        {
            switch (event.Type)
            {
                    // Close if cross button is pressed
                case sf::Event::Closed :
                    app.Close();
                    break;
                case sf::Event::KeyPressed :
                    switch (event.Key.Code)
                {
                    case sf::Key::Escape :
                        app.Close();
                        break;
                    case sf::Key::Up :
                        view.Move(0, -VIEW_SPEED / zoom);
                        break;
                    case sf::Key::Down :
                        view.Move(0, VIEW_SPEED / zoom);
                        break;
                    case sf::Key::Right :
                        view.Move(VIEW_SPEED / zoom, 0);
                        break;
                    case sf::Key::Left :
                        view.Move(-VIEW_SPEED / zoom, 0);
                        break;
                    case sf::Key::Add:
                        zoom *= 1.2f;
                        view.Zoom(1.2f);
                        break;
                    case sf::Key::Equal:
                        zoom *= 0.8f;
                        view.Zoom(0.8f);
                        break;
                    case sf::Key::LShift:
                    case sf::Key::RShift:
                        turbo = true;
                        break;
                    case sf::Key::Comma:
                        pause = !pause;
                        break;
                    default:
                        break;
                }
                case sf::Event::KeyReleased :
                    switch (event.Key.Code)
                {
                    case sf::Key::LShift:
                    case sf::Key::RShift:
                        turbo = false;
                        break;
                    default:
                        break;
                }
                default :
                    break;
            }
        }
        
        // Clear window
        app.Clear();
        if (!pause)
        {
            algo->Update();
        }
        
        algo->DrawDebug(app);
        
        // Draw the map
        labyrinth->Draw(app);
        
 
        // Display Actual buffer
        app.Display();
    }
    
    delete algo;
    delete labyrinth;
    
    // Exit normally
    return 0;
}
