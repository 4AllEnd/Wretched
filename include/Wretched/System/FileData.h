#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace Wretched {
  struct File {
    std::string filename = 0;
  };

  std::string readscript(std::string filepath);

  namespace fpath {
    std::string getfilename(std::string filepath);
  }

  void normalizefp(std::string &g);
  void LoadXML(std::string filepath);
}

