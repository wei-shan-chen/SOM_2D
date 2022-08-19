#ifndef ITEM_H
#define ITEM_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "Vertex.h"

class Item{
public:

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::vector<unsigned int> VBO_array;

    Item( std::vector<Vertex> const& vertices, const std::vector<unsigned int>& indices = {})
        :VAO(0), VBO(0), EBO(0)
    {
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),vertices.data(),GL_DYNAMIC_DRAW );

        if( !indices.empty() ){
            glGenBuffers(1,&EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),indices.data(),GL_STATIC_DRAW);
        }

        //attribute
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        // normal attribute
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        // glEnableVertexAttribArray(1);
    }

    ~Item() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        if (EBO != 0) {
            glDeleteBuffers(1, &EBO);
        }
    }

    void renewVBO(const std::vector<Vertex>& vertices){
        std::cout << "Before Del VBO: " << VBO << std::endl;
        // glDeleteBuffers(1, &VBO);
        std::cout << "After Del VBO: " << VBO << std::endl;


        glBindVertexArray(VAO);
        // glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
        
        // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // std::cout << glGetError() << std::endl;
        // std::cout << "sw" << std::endl;
        // glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(Vertex), vertices.data());
    }

    void createVBOArray() {
        glGenBuffers(VBO_array.size(), VBO_array.data());
    }
};



#endif
