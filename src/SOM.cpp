#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <iostream>

#include "SOM.h"

glm::fvec2 **lattice;
glm::fvec2 **dataset;

int go = 0;
bool tmp = true;
bool is_som_finished = false;
const int map_width = 10;
const int map_height = 10;
const int max_iter = 20000;
int iter = 0;
double learning_rate = 0.1;
double radius = 5;
double n_learning_rate = 0.1;
double neighbor = 5;

glm::fvec2 **createMap(int map_width, int map_height);
glm::fvec2 **createInputDataset(int width, int height);
const glm::fvec2 getInput(glm::fvec2 **dataset, int map_width, int map_height);
void destroy(glm::fvec2 **arr, int width, int height);
void updateNode(glm::fvec2** lattice, glm::fvec2 nowInput,glm::ivec2 bmuId, glm::ivec2 nodeId, double radius, double learning_rate);
double computerate(int iter, double fun);
double computeradius(int iter, double fun);
double computeScale(double sigma, double dist);
bool isInNeighborhood(double squaredDist, double radius);

void SOM_Create()
{
    srand(time(NULL));

    // 1. Create lattice
    lattice = createMap(map_width, map_height);
    // 2. Create input dataset
    dataset = createInputDataset(map_width, map_height);
}

void SOM_IterateOnce()
{
    // 1. Get one input from the dataset
    // 2. Find BMU
    // 3. Update BMU and the neighbors
    n_learning_rate = computerate(iter, learning_rate);
    neighbor = computeradius(iter, radius);

    const glm::fvec2 nowInput = getInput(dataset, map_width, map_height);
    double minDist = -1.0;
    glm::ivec2 bmu;
    // compute winner point
    for (int i = 0; i < map_width; i++)
    {
        for (int j = 0; j < map_height; j++)
        {
            double tmp = 0.0;
            tmp = pow(lattice[i][j].x - nowInput.x, 2) + pow(lattice[i][j].y - nowInput.y, 2);

            if (minDist < 0.0)
            {
                minDist = tmp;
                bmu.x = 0;
                bmu.y = 0;
            }
            else
            {
                if (minDist > tmp)
                {
                    minDist = tmp;
                    bmu.x = i;
                    bmu.y = j;
                }
            }
        }
    }
    // std::cout<< "bmu : "<< bmu.x << ","<< bmu.y <<std::endl;
    // renew winner point and neighnorhood

    for (int i = 0; i < map_width; i++)
    {
        for (int j = 0; j < map_height; j++)
        {
            glm::ivec2 node = glm::ivec2(i, j);
            glm::ivec2 diff = node - bmu;
            double squaredDist = static_cast<double>(diff.x * diff.x + diff.y * diff.y);
            // std::cout << "Dist " <<squaredDist<<std::endl;
            // std::cout << i<<", " << j << std::endl;
            if (isInNeighborhood(squaredDist, neighbor))
            {
                double n_radius = computeScale(neighbor, squaredDist);
                updateNode(lattice, nowInput, bmu, node, n_radius, n_learning_rate);
                
            }
        }
    }
    iter++; 
    is_som_finished = (iter > max_iter);
}

void SOM_Destroy()
{
    // 1. Destroy lattice
    destroy(lattice, map_width, map_height);
    // 2. Destroy input dataset
    destroy(dataset, map_width, map_height);
}

glm::fvec2 **createInputDataset(int map_width, int map_height)
{
    glm::fvec2 **dataset = (glm::fvec2 **)malloc(sizeof(glm::fvec2 *) * map_width);
    for (int i = 0; i < map_width; i++)
    {
        dataset[i] = (glm::fvec2 *)malloc(sizeof(glm::fvec2) * map_height);
    }
    double width = 1.0 / static_cast<double>(map_width);
    double height = 1.0 / static_cast<double>(map_height);
    for (int i = 0; i < map_width; i++)
    {
        for (int j = 0; j < map_height; j++)
        {
            double i0 = ((double)i) * width - 0.5f;
            double j0 = ((double)j) * height - 0.5f;
            //  std::cout << "1.0   "<<i0 << ", " << j0<<std::endl;
            dataset[i][j] = {i0, j0};
        }
    }
    return dataset;
}
glm::fvec2 **createMap(int map_width, int map_height)
{

    glm::fvec2 **lattice = (glm::fvec2 **)malloc(sizeof(glm::fvec2 *) * map_width);
    for (int i = 0; i < map_width; i++)
    {
        lattice[i] = (glm::fvec2 *)malloc(sizeof(glm::fvec2) * map_height);
    }
    for (int i = 0; i < map_width; i++)
    {
        for (int j = 0; j < map_height; j++)
        {
            double i0 = (float)(rand() / (RAND_MAX + 1.0))*0.9 - 0.5;
            double j0 = (float)(rand() / (RAND_MAX + 1.0))*0.9 - 0.5;
            // std::cout << i0 << ", " << j0<<std::endl;
            lattice[i][j] = {i0, j0};
        }
    }
    return lattice;
}
void destroy(glm::fvec2 **arr, int width, int height)
{
    for (int i = 0; i < width; i++)
    {
        free(arr[i]);
    }
}

double computeradius(int iter, double fun)
{
    double lamda = ((double)(max_iter)) / log(fun);
    double sigma = fun * exp(-1 * ((double)iter) / lamda);
    return sigma;
}
double computerate(int iter, double fun)
{
    double sigma = fun * exp(-1 * ((double)iter) / ((double)(max_iter)) );
    return sigma;
}

const glm::fvec2 getInput(glm::fvec2 **dataset, int map_width, int map_height)
{
    int i = rand() % map_width;
    int j = rand() % map_height;
    // std::cout << "i,j : " << i << ", " << j << std::endl;
    return dataset[i][j];
}

bool isInNeighborhood(double squaredDist, double radius)
{
    if (squaredDist <= (radius * radius))
    {
        // std::cout << "dist : " << squaredDist << " radius : " << radius*radius << std::endl;
        return true;
    }
    return false;
}

double computeScale(double sigma, double dist)
{
    double theta = exp((-1 * dist) / (2 * pow(sigma, 2)));
    
    return theta;
}

void updateNode(glm::fvec2** lattice, glm::fvec2 nowInput,glm::ivec2 bmuId, glm::ivec2 nodeId, double radius, double learning_rate){
    lattice[nodeId.x][nodeId.y].x = lattice[nodeId.x][nodeId.y].x + radius * learning_rate*(nowInput.x-lattice[nodeId.x][nodeId.y].x);
    lattice[nodeId.x][nodeId.y].y = lattice[nodeId.x][nodeId.y].y + radius * learning_rate*(nowInput.y-lattice[nodeId.x][nodeId.y].y);
    
}