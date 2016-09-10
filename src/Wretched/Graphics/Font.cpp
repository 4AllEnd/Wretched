#include <Wretched/Graphics/Font.h>

namespace Wretched {
  std::string getenv(char *constant) {
    //#ifdef _WIN32
    //		size_t i;
    //		char b[500];
    //		getenv_s(&i, 0, 0, constant);
    //		getenv_s(&i, b, i, constant);
    //		std::string a(b, i);
    //		return a;

    ////#else
    return std::getenv(constant);
    //#endif
  };
}

FT_Error FTR(FTC_FaceID faceID, FT_Library lib, FT_Pointer reqData, FT_Face *face) {
#ifdef _WIN32
  std::string fontdirect = Wretched::getenv("windir") + "\\Fonts\\";
#else
  std::string fontdirect = Wretched::getenv("windir") + "/Fonts/";
#endif
  int ret = FT_New_Face(lib, (fontdirect + (char *)faceID).c_str(), 0, face);
  if (ret) {
    ret = FT_New_Face(lib, (char *)faceID, 0, face);
  }
  if (ret)printf("error %d for face\n", ret);
  return ret;
}

namespace Wretched {
  FontLibrary::FontLibrary() : shader(0), sheight(0), swidth(0) {
#ifdef _WIN32
    fontdir = Wretched::getenv("windir") + "\\Fonts\\";
    std::cout << fontdir.c_str() << std::endl;
#endif
    int er;
    er |= FT_Init_FreeType(&ft);
    er |= FTC_Manager_New(ft, 256, 1024, 5000000, FTR, 0, &cache);//FtcFaceRequester
    er |= FTC_CMapCache_New(cache, &chcache);

    glGenVertexArrays(1, &VA);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VA);
    GLuint attribute_coord(0);
    glVertexAttribFormat(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(attribute_coord, 0);
    glEnableVertexAttribArray(attribute_coord);
    glBindVertexBuffer(0, VBO, 0, 4 * sizeof(int));
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(int), 0, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
  }

  FontLibrary::~FontLibrary() {
    FTC_Manager_Done(cache);
    FT_Done_FreeType(ft);
    glDeleteVertexArrays(1, &VA);
    glDeleteBuffers(1, &VBO);
  }

  void FontLibrary::setviewport(const int &width, const int &height) {
    swidth = width , sheight = height;
  }

  void FontLibrary::setshader(const unsigned int &a) {
    shader = a;
  }

  Font::Font(FontLibrary &lib, const char *fontfile) {
    parent = &lib;
    FTC_SBitCache_New(parent->cache, &bcache);
    scale.face_id = (FTC_FaceID)fontfile;
    scale.pixel = 1;
    scale.x_res = 0;
    scale.y_res = 0;
    scale.height = 0;
    scale.width = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, 0);

  };

  Font::~Font() {
    glDeleteTextures(1, &texture);
  }

  void Font::render(const float &x, const float &y, const float unit, const bool &inpixels, const char *txt, ...) {
    float xn = x, yn = y;
    va_list b;
    va_start(b,txt);
    const int g = vsnprintf(0, 0, txt, b);
    char *text = (char*)malloc(g + 1);
    vsnprintf(&text[0], g, txt, b);
    va_end(b);
    text[g] = '\0';
    glUseProgram(parent->shader);//useshader
    //set uniforms for shader
    glUniform4f(0, color.r, color.g, color.b, color.a);
    glBindVertexArray(parent->VA);//bind vertex array and vertex object
    glBindBuffer(GL_ARRAY_BUFFER, parent->VBO);
    glActiveTexture(GL_TEXTURE0);//bind texture
    glBindTexture(GL_TEXTURE_2D, texture);
    float sx = 1.f / parent->swidth, sy = 1.f / parent->sheight;
    if (inpixels) {
      scale.width = (FT_UInt)(unit) , scale.height = (FT_UInt)(unit);
    }
    else {
      scale.width = (FT_UInt)(unit * parent->swidth) , scale.height = (FT_UInt)(unit * parent->sheight);
    }
    FT_Size size;
    FTC_SBit ftb;
    FTC_Node node;
    for (auto *p = text; *p; ++p) {
      switch (*p) {//check for special characters
        case '\n': {
          FTC_Manager_LookupSize(parent->cache, &scale, &size);
          yn -= sy * float(size->metrics.height >> 6);
          xn = x;
          continue;
        }
        case '\t': {
          FTC_Manager_LookupSize(parent->cache, &scale, &size);
          xn += sx * float(size->metrics.max_advance >> 6);
          continue;
        }
      }
      unsigned int index = FTC_CMapCache_Lookup(parent->chcache, scale.face_id, 0, *p);
      FTC_SBitCache_LookupScaler(bcache, &scale, FT_LOAD_RENDER, index, &ftb, &node);
      FTC_Node_Unref(node, parent->cache);
      float nw = sx * ftb->width;
      float nh = sy * -ftb->height;
      float x2 = xn + sx * ftb->left;
      float y2 = yn + sy * ftb->top;
      xn += sx * ftb->xadvance;
      yn += sy * ftb->yadvance;
      glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, ftb->width, ftb->height, 0, GL_RED, GL_UNSIGNED_BYTE, ftb->buffer);
      GLfloat box[4][4] = {
        {x2, y2, 0, 0},
        {x2, y2 + nh, 0, 1},
        {x2 + nw, y2, 1, 0},
        {x2 + nw, y2 + nh, 1, 1}
      };
      glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), &box[0][0]);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    free(text);
    glBindVertexArray(0);
  }
}

