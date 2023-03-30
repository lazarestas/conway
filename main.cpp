#pragma clang diagnostic push
#include "include/logic.h"
#include "include/console.h"
#include "include/window.h"
#include <thread>
#include <string>


int main() {

    //there we set up cell array Cellarr
    Cell** Cellarr = CreateDefautArr();
    //loading array from file
    LoadFromFile(Cellarr);

    // Declare and load a font
    sf::Font font;
    if (!font.loadFromFile(R"(D:\Work\pycharm\cglcpp\include\SF-Pro.ttf)")) return 4;
    // start/stop iterator
    unsigned int gamestate = 1;

    // text1,2 is frametime measure
    sf::Text text1("", font, 20), text2("", font, 20);
    text1.setFont(font);
    text2.setFont(font);
    text1.setFillColor(sf::Color::Black);
    text2.setFillColor(sf::Color::Black);
    text1.setPosition(5, WINDOWh - 45);
    text2.setPosition(30, WINDOWh - 45);
    // window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(
            sf::VideoMode(
                    WINDOWw,
                    WINDOWh),
            "conway`s game of life",
            sf::Style::Default,
            settings);
    //icon
    sf::Image icon;
    if (!icon.loadFromFile(R"(D:\Work\pycharm\cglcpp\crap\astolfo.png)"))
        return 5;
    window.setIcon(200, 200, icon.getPixelsPtr()); //fucking hardcoded width/heigth because of stupid sfml dll differences
    window.setFramerateLimit(FRAMERATE);

    sf::Clock clock; // starts the clock
//    sf::Time elapsed1;
    sf::Time elapsed2,elapsedmax,elapsedmax2;
    //cell rectangle 4 drawcell
    sf::RectangleShape cellrect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cellrect.setFillColor(sf::Color::Green);
    // create threads and assign ranges to each thread
    while (window.isOpen()){
        // timer for frametime
        clock.restart();

        //clear window
        window.clear();

        sf::Event event{};

        while (window.pollEvent(event)){
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case (sf::Keyboard::S):
                        gamestate ^= 1;
                        break;
                    case (sf::Keyboard::E):
                        LoadFromFile(Cellarr);
                        break;
                    case (sf::Keyboard::L):
                        SaveToFile(Cellarr);
                        break;
                    case (sf::Keyboard::R):
                        elapsedmax = sf::microseconds(0);
                        elapsedmax2 = sf::microseconds(0);
                        break;
                    case (sf::Keyboard::Escape):
                        window.close();
                        break;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed && gamestate == 0){
                switch (event.mouseButton.button){
                    case (sf::Mouse::Left):
                        SetLive((event.mouseButton.y-FRAMETHIC)/CELL_SIZE,(event.mouseButton.x-FRAMETHIC)/CELL_SIZE,Cellarr);
                        break;
                    case (sf::Mouse::Right):
                        SetDead((event.mouseButton.y-FRAMETHIC)/CELL_SIZE,(event.mouseButton.x-FRAMETHIC)/CELL_SIZE,Cellarr);
                        break;
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // perf overlay
//        elapsed1 = clock.restart();
//        text1.setString(std::to_string(elapsed1.asMilliseconds()));
        //update the field if the game is not in edit mode
        if (gamestate) Stepcgl(Cellarr,window,elapsedmax);

        // drawing stuff
        drawCells(window,Cellarr,cellrect);
        drawUIbottom(window);

        // perf overlay
        window.draw(text1);
        window.draw(text2);

        //draw window
        window.display();

        // perf overlay
        text2.setString(std::to_string(elapsed2.asMicroseconds()));
        elapsed2 = clock.restart();
        if (elapsedmax2<elapsed2){
            elapsedmax2 = elapsed2;
            std::cout << "max frame time " << elapsedmax2.asMicroseconds() << std::endl;
        }

    }
    //free the allocated memory after competion
    for (int i=0;i<FIELDMAXh;i++){
        delete Cellarr[i];
    }
    delete Cellarr;
    //I want it to be menu controlled so let's write down menu
    return 0;
}

#pragma clang diagnostic pop