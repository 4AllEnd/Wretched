#include <Wretched/System/FileData.h>
#include <pugixml.hpp>

namespace Wretched {
  std::string readscript(std::string filepath) {
    std::ifstream fdata(filepath);
    if (fdata.fail()) {
      return "";
    }
    std::string script((std::istreambuf_iterator<char>(fdata)), (std::istreambuf_iterator<char>()));
    return script;
  };

  namespace fpath {
    std::string getfilename(std::string a) {
      return a.substr(a.find_last_of("\\/") + 1);
    };
  }

  void normalizefp(std::string &g) {
    while (g.find("\\") != -1)g.replace(g.find("\\"), 1, "/");
  }

  void LoadXML(std::string filepath) {
#ifdef HEADER_PUGIXML_HPP
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filepath.c_str());
#elif __XML_XMLREADER_H__
#endif
  };

}