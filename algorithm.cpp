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

Vertex* Vertex::get_parent(){
    return this->parent;
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
            //check neighbours---------------
            //check left
            if(x != 0 && pixel_vertex_matrix[y][x-1].get_color() != sf::Color::Black){
                (*current_vertex).add_neighbour(&pixel_vertex_matrix[y][x-1]);
            }
            //check right
            if(x < canvasWidth-1 && pixel_vertex_matrix[y][x+1].get_color() != sf::Color::Black){
                current_vertex->add_neighbour(&pixel_vertex_matrix[y][x+1]);
            }
            //check bottom
            if(y < canvasHeight-1 && pixel_vertex_matrix[y+1][x].get_color() != sf::Color::Black){
                current_vertex->add_neighbour(&pixel_vertex_matrix[y+1][x]);
            }
            //check top
            if(y !=0 && pixel_vertex_matrix[y-1][x].get_color() != sf::Color::Black){
                current_vertex->add_neighbour(&pixel_vertex_matrix[y-1][x]);
            };
            //check bottom right
            if(x < canvasWidth-1 && y< canvasHeight-1 && 
                pixel_vertex_matrix[y+1][x].get_color() != sf::Color::Black &&
                pixel_vertex_matrix[y][x+1].get_color() != sf::Color::Black
            ){
                current_vertex->add_neighbour(&pixel_vertex_matrix[y+1][x+1]);
            }
            //check top right
            if(x < canvasWidth-1 && y>0 &&
                 pixel_vertex_matrix[y-1][x].get_color() != sf::Color::Black&&
                 pixel_vertex_matrix[y][x+1].get_color() != sf::Color::Black
                ){
                current_vertex->add_neighbour(&pixel_vertex_matrix[y-1][x+1]);
            }
            //check top left
            if(y !=0 && x!=0 && 
                pixel_vertex_matrix[y-1][x].get_color() != sf::Color::Black&&
                pixel_vertex_matrix[y][x-1].get_color() != sf::Color::Black
            ){
                current_vertex->add_neighbour(&pixel_vertex_matrix[y-1][x-1]);
            };
            //check bottom left
            if(y < canvasHeight-1 && x>=1 && 
                pixel_vertex_matrix[y+1][x].get_color() != sf::Color::Black&&
                pixel_vertex_matrix[y][x-1].get_color() != sf::Color::Black
            ){
                current_vertex->add_neighbour(&pixel_vertex_matrix[y+1][x-1]);
            };
        
        }
    }
    return pixel_vertex_matrix;
}   

std::vector<Vertex*> run_algorithm(
    Point start,
     Point end,
     sf::Image& canvas,
     std::vector<std::vector<Vertex>>& vertex_grid,
     sf::Texture& texture,
     sf::RenderWindow& window,
     sf::Sprite& sprite,
     unsigned algorithm_delay,
     sf::Text text
    ){
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
            texture.update(canvas);
            window.clear();
            window.draw(sprite);
            window.display();
            return get_path(v, start);
        }
        std::vector<Vertex*> neighbours = v->get_neighbours();
        for(int i =0; i<neighbours.size();i++){
            auto it = visited.find(neighbours[i]);
            if(it == visited.end() && neighbours[i]->get_color() != sf::Color::Black){
                q.push_back(neighbours[i]);
                neighbours[i]->set_parent(v);
                visited.insert(neighbours[i]);
            }
        }
        if(!(v->get_x() == start.x && v->get_y() == start.y)){
            canvas.setPixel(v->get_x(), v->get_y(), sf::Color::Yellow);
            texture.update(canvas);
            window.clear();
            window.draw(sprite);
            window.draw(text);
            window.display();
            sf::sleep(sf::milliseconds(algorithm_delay));
        }

        q.pop_front();    

    }
    texture.update(canvas);
    window.clear();
    window.draw(sprite);
    window.display();
    std::vector<Vertex*> no_end_found = {};
    return no_end_found;
}

std::vector<Vertex*> get_path(Vertex* end_vertex, Point start){
    std::vector<Vertex*> path_list = {};
    bool end_found = false;
    path_list.push_back(end_vertex);
    while(!end_found){
        Vertex* current_vector = path_list.front();
        path_list.insert(path_list.begin(), current_vector->get_parent());
        if(current_vector->get_x() == start.x && current_vector->get_y() == start.y){
            end_found = true;
        }
    }
    return path_list;
}