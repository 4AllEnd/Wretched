#include <Wretched/Graphics/GLError.h>

namespace Wretched {
  bool GLCheckError() {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
      printf("ERROR %s\n\n", gluErrorString(err));
      system("PAUSE");
      exit(1);
      return 1;
    }
    return 0;
  }
}

