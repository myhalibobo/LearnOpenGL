//
//  main.cpp
//  LeanOpenGL
//
//  Created by 刘波 on 2021/3/26.
//  Copyright © 2021 刘波. All rights reserved.
//

#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "ShaderGenVAO.h"
#include "TextureReader.h"
#include <time.h>
#include <sys/timeb.h>
#include "model.h"
float scale = 1;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLFWwindow* initOpenGL(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    return window;
}

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
    // 1. Position = known
    // 2. Calculate cameraDirection
    glm::vec3 zaxis = glm::normalize(position - target);
    // 3. Get positive right axis vector
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
    // 4. Calculate camera up vector
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);

    // Create translation and rotation matrix
    // In glm we access elements as mat[col][row] due to column-major layout
    glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
    translation[3][0] = -position.x; // Third column, first row
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xaxis.x; // First column, first row
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x; // First column, second row
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x; // First column, third row
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z;

    // Return lookAt matrix as combination of translation and rotation matrix
    return rotation * translation; // Remember to read from right to left (first translation then rotation)
}

int main()
{
    //初始化OpenGl
    GLFWwindow *window = initOpenGL();
    if (!window){
        return -1;
    }
    //初始化Shader
    Shader texShader("res/shader/texture.vs","res/shader/texture.fs");
//    unsigned int VAO = genTextrueVAO();
    unsigned int VAO = genCubeVAO();
    texShader.use();
    
    unsigned int tex1 = getTexture("res/pic/container.jpg");
    unsigned int tex2 = getTexture("res/pic/awesomeface.png");
    glUniform1i(glGetUniformLocation(texShader.ID, "tex1"), 0);
    glUniform1i(glGetUniformLocation(texShader.ID, "tex2"), 1);
    
//    Shader ourShader("res/shader/modelLoad.vs", "res/shader/modelLoad.fs");
//    Model ourModel("res/nanosuit/nanosuit.obj");
    
    glEnable(GL_DEPTH_TEST);
    //设置多边形模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        texShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);

        glm::vec3 cameraPos = glm::vec3(5*cos((float)glfwGetTime()),0,5*sin((float)glfwGetTime()));
        glm::vec3 lookPos   = glm::vec3(0,0,0);
        glm::vec3 worldUp   = glm::vec3(0,1,0);
        glm::vec3 f         = glm::normalize(cameraPos-lookPos);
        glm::vec3 s         = glm::normalize(glm::cross(worldUp,f));
        glm::vec3 u         = glm::normalize(glm::cross(f,s));
        
        glm::mat4 view = glm::mat4(1.0f);
        //x
        view[0][0] = s.x;
        view[1][0] = s.y;
        view[2][0] = s.z;
        //y
        view[0][1] = u.x;
        view[1][1] = u.y;
        view[2][1] = u.z;
        //z
        view[0][2] = f.x;
        view[1][2] = f.y;
        view[2][2] = f.z;
        
        glm::mat4 translation = glm::mat4(1.0f);
        translation[3][0] = -cameraPos.x;
        translation[3][1] = -cameraPos.y;
        translation[3][2] = -cameraPos.z;
        
        view = view * translation;
        //view = calculate_lookAt_matrix(cameraPos,lookPos,glm::vec3(0,1,0));
        for(int i=1;i<sizeof(cubePositions);i++){
            glm::mat4 model = glm::mat4(1.0);
            model = glm::translate(model, cubePositions[i]);
//            model = glm::rotate(model, glm::radians((float)glfwGetTime()*50), glm::vec3(1.0f, 1.0f, 1.0f));
            
            
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);
            texShader.setMat4("model", model);
            texShader.setMat4("view", view);
            texShader.setMat4("project", projection);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES,0,36);
        }

//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_UP))
        scale+=0.5;
    if(glfwGetKey(window, GLFW_KEY_DOWN))
        scale-=0.05;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
