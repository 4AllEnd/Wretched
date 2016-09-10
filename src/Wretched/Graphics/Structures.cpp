#include <Wretched/Graphics/Structures.h>

///Texture2D struct

namespace Wretched {
  namespace Graphics {
    namespace GL {
      GraphicsBuffer::~GraphicsBuffer() {
        glDeleteFramebuffers((GLsizei)1, &frame_buffer_object), glDeleteTextures((GLsizei)frame_textures.size(), &frame_textures[0]), glDeleteTextures((GLsizei)1, &depth_texture);
      };

      void GraphicsBuffer::resize(int width, int height) {
        if (currentwidth != width || currentheight != height) {
          for (auto &texture : frame_textures) {
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_FLOAT, nullptr);
          }
          glBindTexture(GL_TEXTURE_2D, depth_texture);
          glTexImage2D(GL_TEXTURE_2D, 0, dinternalformat, width, height, 0, dformat, GL_FLOAT, nullptr);
          currentwidth = width , currentheight = height;
        }
      }

      void GraphicsBuffer::write() {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer_object);
      }

      void GraphicsBuffer::read(unsigned int offset) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, frame_buffer_object);
        glBindTextures((GLuint)offset, (GLsizei)frame_textures.size(), &frame_textures[0]);
        glActiveTexture((GLenum)(GL_TEXTURE0 + frame_textures.size() + offset));
        glBindTexture((GLenum)GL_TEXTURE_2D, depth_texture);
      }

      void GraphicsBuffer::setreadbuffer(int channel) {//Texture Layer
        glReadBuffer(GL_COLOR_ATTACHMENT0 + channel);
      }

      void GraphicsBuffer::begindraw(int width, int height) {
        resize(width, height);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer_object);
      }

      void GraphicsBuffer::displaychannel(const int channel, const int xoffset, const int yoffset, const float scale) {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + channel);
        glBlitFramebuffer(0, 0, currentwidth, currentheight,
                          xoffset, yoffset, GLint(xoffset + currentwidth * scale), GLint(yoffset + currentheight * scale), GL_COLOR_BUFFER_BIT, GL_NEAREST);
      }

      void GraphicsBuffer::displaydepthchannel(int xoffset, int yoffset, float scale) {
        glReadBuffer(GL_DEPTH_ATTACHMENT);
        glBlitFramebuffer(0, 0, currentwidth, currentheight,
                          xoffset, yoffset, GLint(xoffset + currentwidth * scale), GLint(yoffset + currentheight * scale), GL_COLOR_BUFFER_BIT, GL_NEAREST);
      }

      GraphicsBuffer::GraphicsBuffer(unsigned int colorbufferinternalformat, unsigned int colorbufferformat, unsigned int colorbufferssize, unsigned int depthbufferinternalformat,
        unsigned int depthbufferformat) : format(colorbufferformat), internalformat(colorbufferinternalformat), dformat(depthbufferformat), dinternalformat(depthbufferinternalformat) {
        frame_textures.resize(colorbufferssize);
        std::vector<unsigned int> Attachments;
        glGenFramebuffers(1, &frame_buffer_object) , glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_object);
        glGenTextures((GLsizei)frame_textures.size(), &frame_textures[0]);
        for (auto Texture = 0; Texture != frame_textures.size(); ++Texture) {
          glBindTexture(GL_TEXTURE_2D, frame_textures[Texture]);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
          glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
          glTexImage2D(GL_TEXTURE_2D, 0, internalformat, 1, 1, 0, format, GL_FLOAT, nullptr);//makeparams//was originally GL_RGBA//possibly 32F for more precision and no alpha
          glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + Texture + TextureLayer::BEGIN, GL_TEXTURE_2D, frame_textures[Texture], 0);
          Attachments.push_back(GL_COLOR_ATTACHMENT0 + Texture + TextureLayer::BEGIN);
        }
        glGenTextures(1, &depth_texture);
        glBindTexture(GL_TEXTURE_2D, depth_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, dinternalformat, 1, 1, 0, dformat, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);
        glDrawBuffers((GLsizei)Attachments.size(), Attachments.data());
        GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        switch (Status) {
          case GL_FRAMEBUFFER_UNDEFINED : std::cout << "GL_FRAMEBUFFER_UNDEFINED" << std::endl;
            break;
          case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT : std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
            break;
          case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT : std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
            break;
          case GL_FRAMEBUFFER_UNSUPPORTED : std::cout << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
            break;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
      }

      GraphicsBuffer::GraphicsBuffer() : GraphicsBuffer(GL_RGBA32F, GL_RGBA, 6, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT) {}

      BufferObject::BufferObject() {
        glGenBuffers(1, &_name);
        type = GL_ARRAY_BUFFER;
      }

      BufferObject::BufferObject(unsigned int type_) {
        glGenBuffers(1, &_name);
        type = type_;
      }

      BufferObject::~BufferObject() {
        glDeleteBuffers(1, &_name);
      }

      void BufferObject::bind() {
        glBindBuffer(type, _name);
      }


      void BufferObject::bindtoindex(int index) {
        glBindBufferBase(type, index, _name);
      }

      void BufferObject::data(unsigned int size, const void *data, unsigned int usage = Static) {
        glBufferData(type, size, data, usage);
      }

      void BufferObject::subdata(unsigned int offset, unsigned int size, const void *data) {
        glBufferSubData(type, offset, size, data);
      }

      GLuint BufferObject::name() {
        return _name;
      }

      VertexArray::VertexArray() {
        glGenVertexArrays(1, &VA);
      }

      VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &VA);
      }

      void VertexArray::bind() {
        glBindVertexArray(VA);
      }

      void VertexArray::release() {
        glBindVertexArray(0);
      }

      Texture2D::Texture2D() {
        glGenTextures(1, &ID);
      }

      Texture2D::~Texture2D() {
        glDeleteTextures(1, &ID);
      }

      void Texture2D::bind() {
        glBindTexture(GL_TEXTURE_2D, ID);
      }

      void Texture2D::release() {
        glBindTexture(GL_TEXTURE_2D, 0);
      }

      void Texture2D::store(unsigned int level, unsigned int internalformat, unsigned int width, unsigned int height, unsigned int border, unsigned int format, unsigned int datatype, const void *pixels) {
        glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, border, format, datatype, pixels);
      }

      GLuint Texture2D::getID() {
        return ID;
      };
    }
  }
}

