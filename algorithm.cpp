#include "algorithm.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <deque>
#include <set>
#include <vector>

        
Vertex::Vertex(Point location): location(location){};

void Vertex::set_parent(Vertex* parent){
    this->parent = parent; 
}

void Vertex::add_neighbour(Vertex* new_neighbour){
    neighbours.push_back(new_neighbour);
}

void Vertex::change_color(sf::Color new_color){
    this->color = new_color;
}
sf::Color Vertex::get_color(){
    return this->color;
}
std::string Vertex::get_color_string(){
    sf::Color color = Vertex::get_color();
    if(color == sf::Color::White){
        return "White";
    } else{
        return "Black";
    }
}
int Vertex::get_x(){
    return this->location.x;
}
int Vertex::get_y(){
    return this->location.y;
}

std::vector<Vertex*> Vertex::get_neighbours(){
    return this->neighbours;
}


std::vector<std::vector<Vertex>> image_to_grid(sf::Image image, int canvasWidth, int canvasHeight){
    
    std::vector<std::vector<Vertex>> pixel_vertex_matrix;

    for(int y = 0; y< canvasHeight; y++){
        std::vector<Vertex> row = {};
        for(int x = 0; x< canvasWidth; x++){
            sf::Color current_pixel_color = image.getPixel(x, y);
            Point current_pixel_location = {x,y}; 
            Vertex new_vertex = {current_pixel_location};
            new_vertex.change_color(current_pixel_color);
            row.push_back(new_vertex);
            //std::cout<< row.size()<< std::endl;
        }
        pixel_vertex_matrix.push_back(row);
    }
    for(int y = 0; y< pixel_vertex_matrix.size(); y++){
        for(int x = 0; x< pixel_vertex_matrix[y].size(); x++){
            Vertex* current_vertex = &pixel_vertex_matrix[y][x];
            //check neighbours
            if(x != 0 && pixel_vertex_matrix[y][x-1].get_color() != sf::Color::Black){
                (*current_vertex).add_neighbour(&pixel_vertex_matrix[y][x-1]);
            }
            if(x < canvasWidth-1 && pixel_vertex_matrix[y][x+1].get_color() != sf::Color::Black){
                current_vertex->add_neighbour(&pixel_vertex_matrix[y][x+1]);
            }
            if(y < canvasHeight-1 && pixel_vertex_matrix[y+1][x].get_color() != sf::Color::Black){
                current_vertex->add_neighbour(&pixel_vertex_matrix[y+1][x]);
            }
            if(y !=0 && pixel_vertex_matrix[y-1][x].get_color() != sf::Color::Black){
                current_vertex->add_neighbour(&pixel_vertex_matrix[y-1][x]);
            };
        }
    }
    return pixel_vertex_matrix;
}   

sf::Image run_algorithm(Point start, Point end, sf::Image canvas, std::vector<std::vector<Vertex>> vertex_grid, sf::Texture texture){
    Vertex* start_vertex = &vertex_grid[start.y][start.x];
    std::vector<Vertex*> start_neighbours = start_vertex->get_neighbours();
    if(start_neighbours.size() <= 0){
        throw std::runtime_error("Nowhere to go from start");
    }
    std::set<Vertex*> visited = {};
    std::deque<Vertex*> q = {};
    q.push_back(start_vertex);
    while(q.size() > 0){
        Vertex* v = q.front();
        if(v->get_x() == end.x && v->get_y() == end.y){
            return canvas;
        }
        std::vector<Vertex*> neighbours = v->get_neighbours();
        for(int i =0; i<neighbours.size();i++){
            auto it = visited.find(neighbours[i]);
            if(it == visited.end() && neighbours[i]->get_color() != sf::Color::Black){
                q.push_back(neighbours[i]);
                visited.insert(neighbours[i]);
            }
        }
        if(!(v->get_x() == start.x && v->get_y() == start.y)){
            canvas.setPixel(v->get_x(), v->get_y(), sf::Color::Yellow);
            texture.update(canvas);
            sf::sleep(sf::milliseconds(100));
        }

        q.pop_front();    

    }
    return canvas;

}