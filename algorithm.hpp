#pragma once
#include <SFML/Graphics.hpp>
#include "point.hpp"
#include <vector>
#include "windowapp.hpp"
#include <string>


class Vertex{
    public:
        Vertex(Point location);
        
        void set_parent(Vertex* parent);
        void add_neighbour(Vertex* new_neighbour);
        void change_color(sf::Color new_color);

        sf::Color get_color();
        std::string get_color_string();
        int get_x();
        int get_y();
        std::vector<Vertex*> get_neighbours();

    private:
        Point location;
        Vertex* parent;
        sf::Color color;
        std::vector<Vertex*> neighbours;
};
std::vector<std::vector<Vertex>> image_to_grid(sf::Image image, int canvasWidth, int canvasHeight);
sf::Image run_algorithm(
    Point start,
    Point end,
    sf::Image& canvas,
    std::vector<std::vector<Vertex>> vertex_grid,
    sf::Texture& texture,
    sf::RenderWindow& window,
    sf::Sprite& sprite,
    unsigned algorithm_delay
    );
