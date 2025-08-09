#include "windowapp.hpp"
#include "algorithm.hpp"
#include "point.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>


void WindowApp::change_color(int x, int y, sf::Color new_color){
    canvas.setPixel(x, y, new_color);
}

WindowApp::WindowApp(){
    const int scale = 8;
    const int canvasWidth = 100;
    const int canvasHeight = 75;

    int state = 0;
    Point start =  {};
    Point end = {};
    /*
    state = 0 - no start, no end
    state = 1 - start set, no end
    state = 2 - start set, end set
    state = 3 - find path
    */

    sf::RenderWindow window(sf::VideoMode(canvasWidth* scale, canvasHeight* scale), "Maze Finder");
    
    //create an image that stores pixels of canvas in RAM
    canvas.create(100, 75, sf::Color::White);

    //send to GPU
    sf::Texture texture;
    texture.loadFromImage(canvas);

    //display
    sf::Sprite sprite(texture);

    sprite.setScale(scale, scale);
    //zoom in to ensure pixels are visual


    while (window.isOpen()){
        sf::Event event;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            int x = pos.x/scale;
            int y = pos.y/scale;
            //std::cout<<x<<" ,"<<y<<std::endl;
            if (x >= 0 && x < canvasWidth && y >= 0 && y < canvasHeight) {
                canvas.setPixel(x, y, sf::Color::Black);
                texture.update(canvas);
            }
        }
        while(window.pollEvent(event)){
            switch (event.type){
                //CASE: Window closed
                case sf::Event::Closed:
                    window.close();
                    break;
                
                case sf::Event::MouseButtonPressed:
                    if(event.mouseButton.button == sf::Mouse::Right){
                        if(state == 0){
                            //set start
                            int x = event.mouseButton.x/scale;
                            int y = event.mouseButton.y/scale;

                            canvas.setPixel(x, y, sf::Color::Red);
                            texture.update(canvas);
                            start = {x, y};
                            state++;
                        } else if (state == 1) {
                            //set end
                            int x = event.mouseButton.x/scale;
                            int y = event.mouseButton.y/scale;

                            canvas.setPixel(x, y, sf::Color::Red);
                            texture.update(canvas);
                            end = {x, y};
                            state++;
                        
                        } else if (state == 2){
                            std::vector<std::vector<Vertex>> grid = image_to_grid(canvas, canvasWidth, canvasHeight);
                            try{
                                canvas = run_algorithm(start, end, canvas, grid, texture);
                            }catch (std::exception& e){
                                std::cout<< "error occured: "<<e.what()<<std::endl;
                                continue;
                            }
                            texture.update(canvas);

                            state+=1;

                        } else if (state == 3){
                            //clear start and end to 0
                            canvas.setPixel(start.x, start.y, sf::Color::White);
                            canvas.setPixel(end.x, end.y, sf::Color::White);
                            texture.update(canvas);
                            state = 0;

                        }
                    }
                
            }
            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
}