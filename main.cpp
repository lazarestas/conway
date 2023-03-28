#include "logic.h"
#include "console.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <string>

void drawCells(sf::RenderWindow& window, Cell** cellarr)
{
    // Loop through each cell in the array
    for (int i = 0; i < FIELDMAXh; i++) {
        for (int j = 0; j < FIELDMAXw; j++) {
            // Check if the cell is alive
            if (cellarr[i][j].cstate==1) {
                // Create a rectangle shape
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                cell.setFillColor(sf::Color::Green);

                // Draw the rectangle to the window
                window.draw(cell);
            }
        }
    }
}
int main() {
    //there we set up cell array Cellarr
    Cell** Cellarr = CreateDefautArr();
    LoadFromFile(Cellarr);
    // Declare and load a font
    sf::Font font;
    if (!font.loadFromFile("D:\\Work\\pycharm\\cglcpp\\SF-Pro.ttf"))
    {
        return 123;
    }
    // start/stop iterator
    int gamestate = 1;

    // Create a text
    sf::Text text("e-restart esc-exit s-edit l-savetofile",font);
    //text is frametime measure
    sf::Text text1,text2;
    text.setCharacterSize(20);
    text1.setCharacterSize(20);
    text2.setCharacterSize(20);
    text.setFont(font);
    text1.setFont(font);
    text2.setFont(font);
    text.setFillColor(sf::Color::White);
    text1.setFillColor(sf::Color::Red);
    text2.setFillColor(sf::Color::Yellow);
    text.setPosition(FIELDMAXw*CELL_SIZE/2-150,FIELDMAXh*CELL_SIZE-20);
    text1.setPosition(FIELDMAXw*CELL_SIZE/2-150,FIELDMAXh*CELL_SIZE-40);
    text2.setPosition(FIELDMAXw*CELL_SIZE/2-100,FIELDMAXh*CELL_SIZE-40);
    sf::RenderWindow window(sf::VideoMode(FIELDMAXw*CELL_SIZE, FIELDMAXh*CELL_SIZE), "conway`s game of life",sf::Style::Close);

    window.setFramerateLimit(10);

    auto begin = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    auto end1 = std::chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    while (window.isOpen())
    {
        //clear window
        window.clear();
        // timer for frametime
        end = std::chrono::high_resolution_clock::now();
        elapsed = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
        text1.setString(to_string(elapsed));
        begin = std::chrono::high_resolution_clock::now();

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            {
                // left key is pressed: move our character
                LoadFromFile(Cellarr);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
            {
                if (gamestate==1) gamestate=0;
                else gamestate=1;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && gamestate == 0) {
                SetLive(event.mouseButton.y/CELL_SIZE,event.mouseButton.x/CELL_SIZE,Cellarr);
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && gamestate == 0) {
                SetDead(event.mouseButton.y/CELL_SIZE,event.mouseButton.x/CELL_SIZE,Cellarr);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L) {
                SaveToFile(Cellarr);
            }
            if (event.type == sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        // drawing stuff
        drawCells(window,Cellarr);
        window.draw(text);
        window.draw(text1);
        end1 = std::chrono::high_resolution_clock::now();
        elapsed = chrono::duration_cast<chrono::milliseconds>(end1 - begin).count();
        text2.setString(to_string(elapsed));
        window.draw(text2);
        window.display();

        if (gamestate) Stepcgl(Cellarr);

    }
    //free the allocated memory after competion
    for (int i=0;i<FIELDMAXh;i++){
        delete Cellarr[i];
    }
    delete Cellarr;
    //I want it to be menu controlled so let's write down menu
    return 0;
}
