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

int main(int argc, const char * argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    return 0;
}
