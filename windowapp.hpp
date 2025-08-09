#pragma once
#include "point.hpp"
#include <SFML/Graphics.hpp>


class WindowApp{
    public:
        WindowApp();
        
        void change_color(int x, int y, sf::Color new_color);
        void run_algorith(Point start, Point end);

    private:
        sf::Image canvas;

};