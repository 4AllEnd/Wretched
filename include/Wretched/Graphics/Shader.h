#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <Wretched/System/FileData.h>
#include <Wretched/Graphics/GLError.h>
#include <Wretched/System/Entity.h>

namespace Wretched {
  struct Program {
    GLuint PID = 0, VERTEXS = 0, TESSCONTROLS = 0, TESSEVALS = 0, GEOMETRYS = 0, FRAGS = 0, COMPS = 0;
    Program();
    ~Program();
  };

  struct ShaderProgram : public Program {
    ShaderProgram();
    ShaderProgram(const char *, const char *);
    //load a vertex shader and return its _name
    //it does not attach to the shader object
    GLuint CreateVertexShader(const char *vertex_file_path);
    //load a fragment shader and return its _name
    //it does not attach to the shader object
    GLuint CreateFragmentShader(const char *fragfpath);
    GLuint CreateProgram(GLuint VSShaderProgram, GLuint FSShaderProgram);

    void definefragoutput(int a, const char *b);
    void set();
    void use();
    GLint GetUniform(const char *a);
  };

  struct ComputeProgram : public Program {
    ComputeProgram();
    ComputeProgram(const char *);
    void dispatch(const unsigned int &, const unsigned int &, const unsigned int &);
    void use();
  };
}

