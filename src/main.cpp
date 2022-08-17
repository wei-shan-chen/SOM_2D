#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "matrixStack.h"
#include "shader.h"
#include "item.h"
#include "world.h"
#include "SOM.h"

#include <iostream>
#include <algorithm>
#include <array>
#include <cstdlib> 
#include <ctime> 
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

using namespace std;

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SOM", NULL, NULL);
    if(window == NULL){
        std::cout << "failed to crerate GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<< "fialed to initialize GLAD" << std::endl;
        return -1;
    }
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    srand( time(NULL) );
    Shader ourShader("shader/vShader.vs", "shader/fShader.fs");

    create_world();
    Item square(world.square,world.squ_indices);
    Item line(world.line,world.squ_indices);

    glEnable(GL_DEPTH_TEST);

    SOM_Create();
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("SOM");

        if(ImGui::Button("Start")) {
            go = 1;
        }
        if(ImGui::Button("Stop")) {
            tmp = !tmp;
        }
        ImGui::Text("iter : %d",iter);

        ImGui::End();
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

		MatrixStack model;
        // MatrixStack view;
        // view.Save(glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        // ourShader.setMat4("view", view.Top());
        if(!is_som_finished && go == 1 && tmp == true) {
            SOM_IterateOnce();
        }
        // lattice
        for(int i = 0; i < map_width; i++){
            for(int j = 0; j < map_height; j++){ 
                //draw input data point
                model.Push();
                model.Save(glm::translate(model.Top(), glm::vec3(lattice[i][j].x,lattice[i][j].y,0.0f)));
                model.Save(glm::scale(model.Top(),glm::vec3(0.01, 0.01, 1.0f)));
                ourShader.setMat4("model", model.Top());
                ourShader.setVec3("color", glm::vec3(0.0,1.0,0.0));
                glBindVertexArray(square.VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                model.Pop();
                //draw input data line
                // ----
                if(i+1 < map_width){
                    float dist = sqrt(pow(lattice[i][j].x - lattice[i+1][j].x,2) + pow(lattice[i][j].y - lattice[i+1][j].y,2));
                    model.Push();
                    model.Save(glm::translate(model.Top(), glm::vec3(lattice[i][j].x,lattice[i][j].y,0.0f)));
                    model.Save(glm::rotate(model.Top(), atan2((lattice[i+1][j].y - lattice[i][j].y),(lattice[i+1][j].x - lattice[i][j].x)),glm::vec3(0.0f, 0.0f, 1.0f)));
                    model.Save(glm::scale(model.Top(),glm::vec3((dist/0.1), 1.0f, 1.0f)));
                    ourShader.setMat4("model", model.Top());
                    ourShader.setVec3("color", glm::vec3(1.0,1.0,1.0));
                    glBindVertexArray(line.VAO);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    model.Pop();
                }
                // |||
                if(j+1 < map_height){
                    float dist = sqrt(pow(lattice[i][j].x - lattice[i][j+1].x,2) + pow(lattice[i][j].y - lattice[i][j+1].y,2));
                    model.Push();
                    model.Save(glm::translate(model.Top(), glm::vec3(lattice[i][j].x,lattice[i][j].y,0.0f)));
                    model.Save(glm::rotate(model.Top(), atan2(lattice[i][j+1].y - lattice[i][j].y ,lattice[i][j+1].x - lattice[i][j].x),glm::vec3(0.0f, 0.0f, 1.0f)));
                    model.Save(glm::scale(model.Top(),glm::vec3((dist/0.1), 0.1, 1.0f)));
                    ourShader.setMat4("model", model.Top());
                    ourShader.setVec3("color", glm::vec3(1.0,0.0,1.0));
                    glBindVertexArray(line.VAO);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    model.Pop();
                }
                
            }
        }
        


        
        // input data
        for(int i = 0; i < map_width; i++){
            for(int j = 0; j < map_height; j++){ 
                //draw input data point
                model.Push();
                model.Save(glm::translate(model.Top(), glm::vec3(dataset[i][j].x,dataset[i][j].y,0.0f)));
                model.Save(glm::scale(model.Top(),glm::vec3(0.01, 0.01, 1.0f)));
                ourShader.setMat4("model", model.Top());
                ourShader.setVec3("color", glm::vec3(1.0,0.0,0.0));
                glBindVertexArray(square.VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                model.Pop();
                //draw input data line
                // ----
                if(i+1 < map_width){
                    model.Push();
                    model.Save(glm::translate(model.Top(), glm::vec3(dataset[i][j].x,dataset[i][j].y+(0.1*0.05),0.0f)));
                    // model.Save(glm::scale(model.Top(),glm::vec3(0.1, 0.1, 1.0f)));
                    ourShader.setMat4("model", model.Top());
                    ourShader.setVec3("color", glm::vec3(0.5,0.0,0.0));
                    glBindVertexArray(line.VAO);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    model.Pop();
                }
                //|||
                if(j+1 < map_height){
                    model.Push();
                    model.Save(glm::translate(model.Top(), glm::vec3(dataset[i][j].x+(0.1*0.05),dataset[i][j].y,0.0f)));
                    model.Save(glm::rotate(model.Top(), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
                    // model.Save(glm::scale(model.Top(),glm::vec3(0.1, 0.1, 1.0f)));
                    ourShader.setMat4("model", model.Top());
                    ourShader.setVec3("color", glm::vec3(0.5,0.0,0.0));
                    glBindVertexArray(line.VAO);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    model.Pop();
                }
                
            }
        }
        // std::cout<< "end"<<std::endl;
        ImGui::ShowDemoWindow();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    SOM_Destroy();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
        if(go == 0)
            go = 1;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


