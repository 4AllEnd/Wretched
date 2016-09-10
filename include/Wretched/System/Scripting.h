//need to relate to Entities
//entity given container component instead of script probably
#include <string>
namespace Wretched {
  namespace Scripting {

    struct Script {
      std::string filename;
      //reload object
      //check extension
      virtual void reload(const char* filename);
      bool changed = false;
    };
  }
}
