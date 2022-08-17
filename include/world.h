#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <vector>

#include "Vertex.h"

struct World {
    std::vector<Vertex> square;
    std::vector<Vertex> line;
    std::vector<unsigned int> squ_indices;
    std::vector<Vertex> tri;
};

void create_world();
void destroy_world();

extern struct World world;

#endif