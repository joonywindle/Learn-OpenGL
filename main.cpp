#include <iostream>
#include <cmath>
#include "SOIL.h"
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void do_movement();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
GLfloat lastX = WIDTH/2;
GLfloat lastY = HEIGHT/2;
Camera camera(glm::vec3(0.0f,0.0f,3.0f));
bool keys[1024];
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scroll_callback);
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // Define the viewport dimensions
    int fwidth, fheight;

    glfwGetFramebufferSize(window, &fwidth, &fheight);
    glViewport(0, 0, fwidth, fheight);
    Shader ourShader("texture.vs","texture.frag");
    GLfloat vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
           0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                             -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
               0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                             -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                                 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,


                                     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                                         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                                             -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                                                 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                                                     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                                                         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
               0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                              0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
              0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                           -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                               -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                                         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                                              0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                                                  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                                                      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    GLint indeces[] {
      0,1,3,
      1,2,3
    };
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
    GLuint VAO,VBO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
//    glGenBuffers(1,&EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
  //  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
  //  glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);


    GLuint texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;
    unsigned char* image = SOIL_load_image("container.jpg",&width,&height,0,SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);
    // Game loop
    GLfloat pos = 0.5f;
        while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        do_movement();
        // Swap the screen buffers
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, texture);
        ourShader.Use();
       glm::mat4 trans;
        glm::mat4 projection;
        glm::mat4 view;
        GLint modelLogic = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLogic = glGetUniformLocation(ourShader.Program, "view");
        GLint projLogic = glGetUniformLocation(ourShader.Program, "projection");
        //view = glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
        GLfloat radius = 10.0f;
        GLfloat camX = sin(glfwGetTime()) * radius;
        GLfloat camY = cos(glfwGetTime()) * radius;
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom),(float)WIDTH/(float)HEIGHT, 0.1f,100.0f);
        glUniformMatrix4fv(viewLogic,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projLogic,1,GL_FALSE,glm::value_ptr(projection));


        glBindVertexArray(VAO);
        for(GLint i = 0; i < 10; i++){
          glm::mat4 model;
          model = glm::translate(model,cubePositions[i]);
          GLfloat angle = 20.0f * i;
          if(i%3 == 0)
            angle = glfwGetTime() * 25.0f;

          model = glm::rotate(model,glm::radians(angle),glm::vec3(1.0f,0.3f,0.5f));

          glUniformMatrix4fv(modelLogic,1,GL_FALSE,glm::value_ptr(model));
          glDrawArrays(GL_TRIANGLES,0, 36);
        }
                //trans = glm::rotate(trans,(GLfloat)glfwGetTime() * glm::radians(180.0f),glm::vec3(0.0,0.0,1.0));
//      trans = glm::scale(trans,glm::vec3(0.5,0.5,0.5));
      //  GLuint transformLogic = glGetUniformLocation(ourShader.Program, "transform");
       //glBindVertexArray(VAO);
        glBindVertexArray(0);
        glfwSwapBuffers(window);

    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
//    glDeleteBuffers(2,&EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void do_movement()
{
    if(keys[GLFW_KEY_W])
      camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
      camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
      camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
      camera.ProcessKeyboard(RIGHT, deltaTime);

}

void scroll_callback(GLFWwindow* window, double xoffset,double yoffset){
  camera.ProcessMouseScroll(yoffset);
 }

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
 if(firstMouse){
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;
  camera.ProcessMouseMovement(xoffset, yoffset);
  }

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

    // When a user presses the escape key, we set the WindowShouldClose property to true,
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    if(action == GLFW_PRESS)
      keys[key] = true;
    else if(action == GLFW_RELEASE)
      keys[key] = false;
   }

