#include <Wretched/Graphics/Shader.h>

namespace Wretched {
  //need to clean
  struct Shader {
    Shader(GLenum shader_type) {
      shader = glCreateShader(shader_type);
    }
  private:
    GLuint shader = 0;
  };
  Program::Program(): PID(glCreateProgram()) {}

  Program::~Program() {
    glDeleteProgram(PID);
  }

  void LoadShader(unsigned int &shader, const char *filepath) {
    std::string script(readscript(filepath));
    const char *shaderscript = script.c_str();
    glShaderSource(shader, 1, &shaderscript, NULL);
    glCompileShader(shader);
    int LogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &LogLength);
    std::cout << "...Loading File Shader " << fpath::getfilename(filepath).c_str() << std::endl;
    if (LogLength > 1) {
      std::vector<char> ErrorMessage(LogLength);
      glGetShaderInfoLog(shader, LogLength, NULL, &ErrorMessage[0]);
      fprintf(stdout, "%s\n", &ErrorMessage[0]);
    }
  }

  ShaderProgram::ShaderProgram(const char *vertex_filepath, const char *fragment_filepath) {
    CreateProgram(CreateVertexShader(vertex_filepath), CreateFragmentShader(fragment_filepath));
  }

  //load a vertex shader and return its _name
  //it does not attach to the shader object
  GLuint ShaderProgram::CreateVertexShader(const char *file_path) {
    GLuint VSID(glCreateShader(GL_VERTEX_SHADER));
    LoadShader(VSID, file_path);
    return VSID;
  };

  //load a fragment shader and return its _name
  //it does not attach to the shader object
  GLuint ShaderProgram::CreateFragmentShader(const char *file_path) {
    GLuint FSID(glCreateShader(GL_FRAGMENT_SHADER));
    LoadShader(FSID, file_path);
    return FSID;
  };

  GLuint ShaderProgram::CreateProgram(GLuint VertexShader, GLuint FragmentShader) {
    if (glIsProgram(PID))glDeleteProgram(PID);
    GLuint ProgID(glCreateProgram());
    glAttachShader(ProgID, VertexShader);
    glAttachShader(ProgID, FragmentShader);
    glLinkProgram(ProgID);
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    PID = ProgID;
    return PID;
  };

  void ShaderProgram::definefragoutput(int a, const char *b) {
    glBindFragDataLocation(PID, a, b);
  }

  void ShaderProgram::set() {
    glLinkProgram(PID);
  }

  void ShaderProgram::use() {
    glUseProgram(PID);
  }

  GLint ShaderProgram::GetUniform(const char *a) {
    return glGetUniformLocation(PID, a);
  };

  ComputeProgram::ComputeProgram(const char *filepath) {
    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    LoadShader(shader, filepath);
    glAttachShader(PID, shader);
    glLinkProgram(PID);
    glDeleteShader(shader);
  }

  void ComputeProgram::dispatch(const unsigned int &x, const unsigned int &y, const unsigned int &z) {
    glDispatchCompute(x, y, z);
  }

  void ComputeProgram::use() {
    glUseProgram(PID);
  }
}

