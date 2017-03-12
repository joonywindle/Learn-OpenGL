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
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
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
    Shader ourShader("lighting.vs","lighting.frag");
    Shader lampShader("lamp.vs","lamp.frag");
    GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    GLuint VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLfloat*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);


    GLuint lightVAO;
    glGenVertexArrays(1,&lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    // Game loop
        while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        // Swap the screen buffers
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        do_movement();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ourShader.Use();
        GLint objectColLoc = glGetUniformLocation(ourShader.Program, "objectColor");
        GLint lightColLoc = glGetUniformLocation(ourShader.Program, "lightColor");
        GLint lightPosLoc = glGetUniformLocation(ourShader.Program, "lightPos");
        GLint viewPosLoc = glGetUniformLocation(ourShader.Program, "viewPos");
        
        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z); 
        glUniform3f(objectColLoc, 1.0f, 0.5f, 0.31f);
        glUniform3f(lightColLoc, 1.0f, 1.0f, 1.0f);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom),(float)WIDTH/(float)HEIGHT, 0.1f,100.0f);
        GLint modelLogic = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLogic = glGetUniformLocation(ourShader.Program, "view");
        GLint projLogic = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(viewLogic,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projLogic,1,GL_FALSE,glm::value_ptr(projection));


        glBindVertexArray(VAO);
          glm::mat4 model;
          glUniformMatrix4fv(modelLogic,1,GL_FALSE,glm::value_ptr(model));
          glDrawArrays(GL_TRIANGLES,0, 36);
          glBindVertexArray(0);
        lampShader.Use();
        modelLogic = glGetUniformLocation(lampShader.Program, "model");
        viewLogic = glGetUniformLocation(lampShader.Program, "view");
        projLogic = glGetUniformLocation(lampShader.Program, "projection");
          model = glm::translate(model,lightPos);
          model = glm::scale(model, glm::vec3(0.2f));


        glUniformMatrix4fv(viewLogic,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projLogic,1,GL_FALSE,glm::value_ptr(projection));
          glUniformMatrix4fv(modelLogic,1,GL_FALSE,glm::value_ptr(model));
          glBindVertexArray(lightVAO);
          glDrawArrays(GL_TRIANGLES,0, 36);
        glBindVertexArray(0);
        glfwSwapBuffers(window);

    }
    glDeleteVertexArrays(1,&VAO);
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

