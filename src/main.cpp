#include <glad/glad.h>


#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SOM_2D", NULL, NULL);
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
    glDebugMessageCallback([](GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar *message,
            const void *userParam){
                std::cout << type << ", " << id << ", " << message << std::endl;
            }, nullptr); 
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
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

    SOM_Create();
    create_world();
    Item square(world.square,world.squ_indices);
    Item line(world.line,world.squ_indices);
    Item dataset_square(world.dataset_square);
    Item lattice_square_four_edges(world.lattice_square_four_edges);
    Item dataset_square_four_edges(world.dataset_square_four_edges);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("SOM_2D");
        ImGui::Text("iter : %d",iter);
        ImGui::Text("radius, %f", neighbor);
        ImGui::Text("learning_rate, %f", n_learning_rate);
        if(ImGui::Button("Start")) {
            go = 1;
        }
        if(ImGui::Button("Stop")) {
            tmp = !tmp;
        }

        ImGui::End();
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

		MatrixStack model;
        // MatrixStack view;
        if(!is_som_finished && go == 1 && tmp == true) {
            SOM_IterateOnce();
        }
        // lattice
        model.Push();
        model.Save(glm::translate(model.Top(), glm::vec3(0.0,0.0,0.0f)));
        model.Save(glm::scale(model.Top(),glm::vec3(1.0f, 1.0f, 1.0f)));
        ourShader.setMat4("model", model.Top());
        ourShader.setVec3("color", glm::vec3(1.0,0.0,0.0));
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glBindVertexArray(lattice_square_four_edges.VAO);
        glDrawArrays(GL_LINES, 0, world.lattice_square_four_edges.size());
        model.Pop();
        renew_world();
        lattice_square_four_edges.renewVBO(world.lattice_square_four_edges);

        // input data
        model.Push();
        model.Save(glm::translate(model.Top(), glm::vec3(-0.5,-0.5,0.0f)));
        model.Save(glm::scale(model.Top(),glm::vec3(1.0 / static_cast<double>(map_width), 1.0 / static_cast<double>(map_height), 1.0f)));
        ourShader.setMat4("model", model.Top());
        ourShader.setVec3("color", glm::vec3(1.0,1.0,0.0));
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glBindVertexArray(dataset_square_four_edges.VAO);
        glDrawArrays(GL_LINES, 0, world.dataset_square_four_edges.size());
        model.Pop();
        
        
       
        // std::cout<< "end"<<std::endl;
        // ImGui::ShowDemoWindow();
        
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


