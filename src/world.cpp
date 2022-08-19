#include "world.h"


struct World world = {};

void create_world() {
    
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            world.dataset_square.push_back(Vertex{ { (float)i, (float)j, 0.0f}});
            world.dataset_square.push_back(Vertex{ { (float)i+1.0f, (float)j+0.0f, 0.0f}});
            world.dataset_square.push_back(Vertex{ { (float)i+1.0f, (float)j+1.0f, 0.0f}});

            world.dataset_square.push_back(Vertex{ { (float)i+1.0f, (float)j+1.0f, 0.0f}});
            world.dataset_square.push_back(Vertex{ { (float)i+0.0f, (float)j+1.0f, 0.0f}});
            world.dataset_square.push_back(Vertex{ { (float)i+0.0f, (float)j+0.0f, 0.0f}});
        }
    }
    for(int i = 0; i < map_width-1; i++){
        for(int j = 0; j < map_height-1; j++){
            world.dataset_square_four_edges.push_back(Vertex{ (float)i, (float)j, 0.0f});
            world.dataset_square_four_edges.push_back(Vertex{ (float)i+1.0f, (float)j+0.0f, 0.0f});

            world.dataset_square_four_edges.push_back(Vertex{ (float)i+1.0f, (float)j+0.0f, 0.0f});
            world.dataset_square_four_edges.push_back(Vertex{ (float)i+1.0f, (float)j+1.0f, 0.0f});

            world.dataset_square_four_edges.push_back(Vertex{ (float)i+1.0f, (float)j+1.0f, 0.0f});
            world.dataset_square_four_edges.push_back(Vertex{ (float)i+0.0f, (float)j+1.0f, 0.0f});

            world.dataset_square_four_edges.push_back(Vertex{ (float)i+0.0f, (float)j+1.0f, 0.0f});
            world.dataset_square_four_edges.push_back(Vertex{ (float)i, (float)j, 0.0f});
        }
    }

    for(int i = 0; i < map_width-1; i++){
        for(int j = 0; j < map_height-1; j++){
            
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j].x, lattice[i][j].y, 0.0f});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j].x, lattice[i+1][j].y, 0.0f});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j].x, lattice[i+1][j].y, 0.0f});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j+1].x, lattice[i+1][j+1].y, 0.0f});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j+1].x, lattice[i+1][j+1].y, 0.0f});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j+1].x, lattice[i][j+1].y, 0.0f});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j+1].x, lattice[i][j+1].y, 0.0f});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j].x, lattice[i][j].y, 0.0f});

        }
    }

    world.square = {
            Vertex{ { 0.0f, 0.0f, 0.0f}},
            Vertex{ { 1.0f, 0.0f, 0.0f}},
            Vertex{ { 1.0f, 1.0f, 0.0f}},
            Vertex{ { 0.0f, 1.0f, 0.0f}},
    };
    world.line = {
            Vertex{ { 0.0f, 0.0f, 0.0}},
            Vertex{ { 0.1f, 0.0f, 0.0f}},
            Vertex{ { 0.1f, 0.005f, 0.0f}},
            Vertex{ { 0.0f, 0.005f, 0.0f}},
    };
//     world.line = {
//             Vertex{ { 0.05f, 0.0f, 0.0}},
//             Vertex{ { 1.0f, 0.0f, 0.0f}},
//             Vertex{ { 1.0f, 0.05f, 0.0f}},
//             Vertex{ { 0.05f, 0.05f, 0.0f}},
//     };
    world.squ_indices = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };
    world.tri= {
		Vertex{ { 0.5f, -0.5f, 0.0f}},
        Vertex{ {-0.5f, -0.5f, 0.0f}},
        Vertex{ { 0.0f,  0.5f, 0.0f}}
	};

}
void renew_world(){
    world.lattice_square_four_edges.clear(); 
    for(int i = 0; i < map_width-1; i++){
        for(int j = 0; j < map_height-1; j++){
            
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j].x, lattice[i][j].y, 0.0f});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j].x, lattice[i+1][j].y, 0.0f});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j].x, lattice[i+1][j].y, 0.0f});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j+1].x, lattice[i+1][j+1].y, 0.0f});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j+1].x, lattice[i+1][j+1].y, 0.0f});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j+1].x, lattice[i][j+1].y, 0.0f});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j+1].x, lattice[i][j+1].y, 0.0f});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j].x, lattice[i][j].y, 0.0f});

        }
    }
    // std::cout << lattice[0][0].x << ", " <<lattice[0][0].y<<std::endl;
}
void destroy_world() {

}