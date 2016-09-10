#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>

namespace Wretched {
  namespace Graphics {
    namespace GL {
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

        GraphicsBuffer(unsigned int colorbufferinternalformat, unsigned int colorbufferformat, unsigned int colorbufferssize, unsigned int depthbufferinternalformat, unsigned int depthbufferformat);
        GraphicsBuffer();
        ~GraphicsBuffer();
        //resize Framebuffer textures to fit window width if width or height 
        void resize(int width, int height);
        void write();
        void read(unsigned int offset = 0);
        void setreadbuffer(int TextureType);
        void begindraw(int width, int height);
        void enddraw();
        void beginillumination();
        void endillumination();
        void finish();
        void displaychannel(const int channel, const int xoffset, const int yoffset, const float scale);
        void displaydepthchannel(int xoffset, int yoffset, float scale);
        GLuint frame_buffer_object, depth_texture;
        unsigned int format, internalformat, dformat, dinternalformat;
        std::vector<GLuint> frame_textures;
      protected:
        int currentwidth, currentheight;
      };

      struct BufferObject {
        enum {
          Static=GL_STATIC_DRAW,
          Dynamic=GL_DYNAMIC_DRAW,
          Stream=GL_STREAM_DRAW
        };

        BufferObject();
        BufferObject(unsigned int);
        ~BufferObject();

        enum Type {
          Array = GL_ARRAY_BUFFER
        };

        GLuint _name;
        unsigned int type;
        void bind();
        void bindtoindex(int index);
        void bindtoindex();
        void data(unsigned int size, const void *data, unsigned int usage);
        void subdata(unsigned int offset, unsigned int size, const void *data);
        GLuint name();
      };

      struct VertexArray {
        VertexArray();
        void bind();
        void release();
        ~VertexArray();
        GLuint VA;
      };

      struct Texture2D {
        Texture2D();
        ~Texture2D();
        void bind();
        static void release();
        void store(unsigned int level, unsigned int internalformat, unsigned int width, unsigned int height, unsigned int border, unsigned int format, unsigned int datatype, const void *pixels);
        GLuint getID();
      protected:
        GLuint ID;
      };
    }
  }
}

