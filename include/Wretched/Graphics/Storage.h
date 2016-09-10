#pragma once
#include <GL/glew.h>
#include <vector>
#include <iostream>

namespace Wretched {
  class GraphicsBuffer {
  public:
    //enumeration for Framebuffer Textures
    enum TextureLayer {
      BEGIN = 0,
      DIFFUSE = BEGIN,
      NORMAL,
      TANGENT,
      SPECULAR,
      AMBIENT,
      END,
      NUMOFTEXTURES = END - BEGIN
    };

    GraphicsBuffer(unsigned int colorbufferformat, unsigned int colorbufferinternalformat, unsigned int colorbufferssize, unsigned int 
      bufferformat, unsigned int depthbufferinternalformat);
    GraphicsBuffer();
    ~GraphicsBuffer();
    //resize Framebuffer textures to fit window width if width or height 
    void resize(int width, int height);
    void write();
    void read(unsigned int offset = 0);
    void setreadbuffer(int TextureType);
    void begindraw(int width, int height);
    void displaychannel(const int channel, const int xoffset, const int yoffset, const float scale);
    void displaydepthchannel(int xoffset, int yoffset, float scale);
    GLuint FrameBufferObject, depth_stencil_texture;
    unsigned int format, internalformat, dformat, dinternalformat;
    std::vector<GLuint> FrameTextures;
  protected:
    int currentwidth, currentheight;
  };

  struct BufferObject {
    BufferObject();
    //enter GL type
    BufferObject(unsigned int type);
    ~BufferObject();
    GLuint BO;
    GLenum Type;
    void bind();
    void data(unsigned int, const void *, unsigned int);
    void subdata(unsigned int, unsigned int, const void *);
  };

  struct VertexArray {
    VertexArray();
    void bind();
    ~VertexArray();
    GLuint VA;
  };

  struct Texture2D {
    Texture2D();
    ~Texture2D();
    void bind();
    void store(unsigned int level, unsigned int internalformat, unsigned int width, unsigned int height, unsigned int border, unsigned int format, unsigned int datatype, const void *pixels);
    GLuint getID();
  protected:
    GLuint ID;
  };

  struct SSBO {
    SSBO();
    ~SSBO();
  };

  struct UBO {
    UBO();
  };
}

