/*
 * =====================================================================================
 *
 *       Filename:  Shader.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/03/17 20:26:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader{
  public :
    GLuint Program;

    Shader(const GLchar* vertexPath, const GLchar* fragmentPath){
      std::string vertexCode;
      std::string fragmentCode;
      std::ifstream vShaderFile;
      std::ifstream fShaderFile;

      vShaderFile.exceptions(std::ifstream::badbit);
      fShaderFile.exceptions(std::ifstream::badbit);
      try{
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
      }
      catch(std::ifstream::failure e){
        std::cout << "Error::Shader::File_Not_Successfully_Read" << std::endl;
      }
      const GLchar* vShaderCode = vertexCode.c_str();
      const GLchar* fShaderCode = fragmentCode.c_str();

      GLuint vertex,fragment;
      GLint success;
      GLchar infoLog[512];

      vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex,1,&vShaderCode,NULL);
      glCompileShader(vertex);

      glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
      if(!success){
        glGetShaderInfoLog(vertex,512,NULL,infoLog);
        std::cout << "Error::Shader::Vertex::Compilation_Failed\n" << infoLog << std::endl;

      }
      fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment,1,&fShaderCode,NULL);
      glCompileShader(fragment);

      glGetShaderiv(fragment,GL_COMPILE_STATUS, &success);
      if(!success){
        glGetShaderInfoLog(fragment,512,NULL,infoLog);
        std::cout << "Error::Shader::Fragment::Compilation_Failed\n" << infoLog << std::endl;
      }

      this->Program = glCreateProgram();
      glAttachShader(this->Program, vertex);
      glAttachShader(this->Program, fragment);
      glLinkProgram(this->Program);

      glGetProgramiv(this->Program, GL_LINK_STATUS, &success);

      if(!success){
        glGetProgramInfoLog(this->Program,512,NULL,infoLog);
        std::cout << "Error::Shader::Program::Linking_Failed\n" << infoLog << std::endl;
      }

      glDeleteShader(vertex);
      glDeleteShader(fragment);

    }

    void Use(){
      glUseProgram(this->Program);
    }
};

#endif
