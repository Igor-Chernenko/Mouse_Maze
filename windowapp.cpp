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
    const unsigned algorithm_delay = 5; //ms
    const float margin = 5.f;

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
    
    //create and store font
    sf::Font font;
    if (!font.loadFromFile("ArialMdm.ttf")) {
        throw std::runtime_error("Could not load font");
    }
    sf::Text text;
    text.setFont(font);
    text.setString("Left Click to Draw Maze --or-- Right Click to Set Starting Point");
    text.setCharacterSize(20);        // pixels
    text.setFillColor(sf::Color::Black);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition(
        margin, 
        window.getSize().y - bounds.height - bounds.top - margin
    );
    

    //create an image that stores pixels of canvas in RAM
    canvas.create(100, 75, sf::Color::White);

    //send to GPU
    sf::Texture texture;
    texture.loadFromImage(canvas);

    //display
    sf::Sprite sprite(texture);

    sprite.setScale(scale, scale);
    //zoom in to ensure pixels are visual
    window.draw(text);

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
                            text.setString("Start Point set! -- Left Click to Draw Maze --or-- Right Click to Set End Point(Place Cheese)");
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
                            text.setString("End Point set! -- Left Click to Draw Maze --or-- Right Click to Free the Mouse");
                            canvas.setPixel(x, y, sf::Color::Red);
                            texture.update(canvas);
                            end = {x, y};
                            state++;
                        
                        } else if (state == 2){
                            text.setString("Sniffing out the Cheese!");
                            window.clear();
                            window.draw(sprite);
                            window.draw(text);
                            window.display();
                            sf::Image canvas_save= canvas;
                            std::vector<std::vector<Vertex>> grid = image_to_grid(canvas, canvasWidth, canvasHeight);
                             std::vector<Vertex*> shortest_path = {};
                            try{
                                 shortest_path = run_algorithm(start, end, canvas, grid, texture, window, sprite, algorithm_delay, text);
                            }catch (std::exception& e){
                                std::cout<< "error occured: "<<e.what()<<std::endl;
                                continue;
                            };
                            window.clear();
                            canvas = canvas_save;
                            texture.update(canvas);
                            window.draw(sprite);
                            text.setString("Sniffing out the Cheese!");
                            window.draw(text);
                            window.display();
                            if(shortest_path.size()>0){
                                text.setString("found the cheese!");
                            }else{
                                text.setString("could not find the cheese");
                            }
                            for(int i=0; i<shortest_path.size(); i++){
                                Vertex* current_vertex = shortest_path[i];
                                canvas.setPixel(current_vertex->get_x(), current_vertex->get_y(), sf::Color::Green);
                            }
                            texture.update(canvas);
                            state+=1;

                        } else if (state == 3){
                            //clear start and end to 0
                            canvas.setPixel(start.x, start.y, sf::Color::White);
                            canvas.setPixel(end.x, end.y, sf::Color::White);
                            texture.update(canvas);
                            text.setString("Left Click to Draw Maze --or-- Right Click to Set Starting Point");
                            state = 0;

                        }
                    }
                
            }
            window.clear();
            window.draw(sprite);
            window.draw(text);
            window.display();
        }
    }
}