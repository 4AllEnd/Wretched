#include <Wretched/Graphics/Storage.h>
#include <cassert>

namespace Wretched {
  ///BufferObject struct
  GraphicsBuffer::~GraphicsBuffer() {
    glDeleteFramebuffers(1, &FrameBufferObject) , glDeleteTextures(FrameTextures.size(), &FrameTextures[0]) , glDeleteTextures(1, &depth_stencil_texture);
  };

  void GraphicsBuffer::resize(int width, int height) {
    if (currentwidth != width || currentheight != height) {
      for (auto &Texture : FrameTextures) {
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, internalformat, GL_FLOAT, NULL);
      }
      glBindTexture(GL_TEXTURE_2D, depth_stencil_texture);
      glTexImage2D(GL_TEXTURE_2D, 0, dformat, width, height, 0, dinternalformat, GL_FLOAT, NULL);
      currentwidth = width , currentheight = height;
    }
  }

  void GraphicsBuffer::write() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FrameBufferObject);
  }

  void GraphicsBuffer::read(unsigned int offset) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FrameBufferObject);
    glBindTextures(offset, FrameTextures.size(), &FrameTextures[0]);
    glActiveTexture(GL_TEXTURE0 + FrameTextures.size() + offset);
    glBindTexture(GL_TEXTURE_2D, depth_stencil_texture);
  }

  void GraphicsBuffer::setreadbuffer(int channel) {//Texture Layer
    glReadBuffer(GL_COLOR_ATTACHMENT0 + channel);
  }

  void GraphicsBuffer::begindraw(int width, int height) {
    resize(width, height);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FrameBufferObject);
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

  GraphicsBuffer::GraphicsBuffer(unsigned int colorbufferformat, unsigned int colorbufferinternalformat, unsigned int colorbufferssize,
                                 unsigned int depthbufferformat, unsigned int depthbufferinternalformat) : format(colorbufferformat), internalformat(colorbufferinternalformat),
                                                                                                           dformat(depthbufferformat), dinternalformat(depthbufferinternalformat) {
    FrameTextures.resize(colorbufferssize);
    std::vector<unsigned int> Attachments;
    glGenFramebuffers(1, &FrameBufferObject) , glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObject);
    glGenTextures(FrameTextures.size(), &FrameTextures[0]);
    for (auto Texture = 0; Texture != FrameTextures.size(); ++Texture) {
      glBindTexture(GL_TEXTURE_2D, FrameTextures[Texture]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D(GL_TEXTURE_2D, 0, format, 1, 1, 0, internalformat, GL_FLOAT, NULL);//makeparams//was originally GL_RGBA//possibly 32F for more precision and no alpha
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + Texture + TextureLayer::BEGIN, GL_TEXTURE_2D, FrameTextures[Texture], 0);
      Attachments.push_back(GL_COLOR_ATTACHMENT0 + Texture + TextureLayer::BEGIN);
    }
    glGenTextures(1, &depth_stencil_texture);
    glBindTexture(GL_TEXTURE_2D, depth_stencil_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, dformat, 1, 1, 0, dinternalformat, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_texture, 0);
    glDrawBuffers((GLsizei)Attachments.size(), Attachments.data());
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (Status) {
      case GL_FRAMEBUFFER_UNDEFINED: std::cout << "GL_FRAMEBUFFER_UNDEFINED" << std::endl;
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
        break;
      case GL_FRAMEBUFFER_UNSUPPORTED: std::cout << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
        break;
    }
    //assert(Status == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  GraphicsBuffer::GraphicsBuffer() : format(GL_RGBA32F), internalformat(GL_RGBA), dformat(GL_DEPTH_COMPONENT32F), dinternalformat(GL_DEPTH_COMPONENT) {
    std::vector<unsigned int> Attachments;
    glGenFramebuffers(1, &FrameBufferObject) , glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObject);
    FrameTextures.resize(6);
    glGenTextures(FrameTextures.size(), &FrameTextures[0]);
    for (auto Texture = 0; Texture != FrameTextures.size(); ++Texture) {
      glBindTexture(GL_TEXTURE_2D, FrameTextures[Texture]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D(GL_TEXTURE_2D, 0, format, 1, 1, 0, internalformat, GL_FLOAT, NULL);//makeparams//was originally GL_RGBA//possibly 32F for more precision and no alpha
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + Texture + TextureLayer::BEGIN, GL_TEXTURE_2D, FrameTextures[Texture], 0);
      Attachments.push_back(GL_COLOR_ATTACHMENT0 + Texture + TextureLayer::BEGIN);
    }
    glGenTextures(1, &depth_stencil_texture);
    glBindTexture(GL_TEXTURE_2D, depth_stencil_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) , glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, dformat, 1, 1, 0, dinternalformat, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_texture, 0);
    glDrawBuffers((GLsizei)Attachments.size(), Attachments.data());
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (Status) {
      case GL_FRAMEBUFFER_UNDEFINED: std::cout << "GL_FRAMEBUFFER_UNDEFINED" << std::endl;
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
        break;
      case GL_FRAMEBUFFER_UNSUPPORTED: std::cout << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
        break;
    }
    //assert(Status == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  BufferObject::BufferObject() {
    Type = GL_ARRAY_BUFFER;
    glGenBuffers(1, &BO);
  }

  BufferObject::BufferObject(unsigned int type) {
    Type = type;
    glGenBuffers(1, &BO);
  }

  BufferObject::~BufferObject() {
    glDeleteBuffers(1, &BO);
  }

  void BufferObject::bind() {
    glBindBuffer(Type, BO);
  }

  void BufferObject::data(unsigned int size, const void *data, unsigned int usage) {
    glBufferData(Type, size, data, usage);
  }

  void BufferObject::subdata(unsigned int offset, unsigned int size, const void *data) {
    glBufferSubData(Type, offset, size, data);
  }

  ///VertexArray struct
  VertexArray::VertexArray() {
    glGenVertexArrays(1, &VA);
  }

  void VertexArray::bind() {
    glBindVertexArray(VA);
  }

  VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &VA);
  }

  ///Texture2D struct
  Texture2D::Texture2D() {
    glGenTextures(1, &ID);
  }

  Texture2D::~Texture2D() {
    glDeleteTextures(1, &ID);
  }

  void Texture2D::bind() {
    glBindTexture(GL_TEXTURE_2D, ID);
  }

  void Texture2D::store(unsigned int level, unsigned int internalformat, unsigned int width, unsigned int height, unsigned int border, unsigned int format, unsigned int datatype, const void *pixels) {
    glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, border, format, datatype, pixels);
  }

  GLuint Texture2D::getID() {
    return ID;
  };

}

