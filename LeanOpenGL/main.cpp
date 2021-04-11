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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "ShaderGenVAO.h"
#include "TextureReader.h"
#include <time.h>
#include <sys/timeb.h>
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
long long getTimeStamp()
{
    timeb t;
    ftime(&t);
    return t.time * 1000 + t.millitm;
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
    
    glEnable(GL_DEPTH_TEST);
    //设置多边形模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        texShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);
        
        glm::mat4 model = glm::mat4(1.0f);
        
        model = glm::rotate(model, glm::radians((float)glfwGetTime()*50), glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);
        texShader.setMat4("model", model);
        texShader.setMat4("view", view);
        texShader.setMat4("project", projection);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,36);
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
