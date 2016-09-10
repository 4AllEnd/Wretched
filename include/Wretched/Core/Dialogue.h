#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>

namespace Wretched {
  namespace Dialogue {
    typedef int DialogueID;
    struct DialogSystem {
      void select();//select choice
      void load();
      void loadframe();
    };

    struct Token:public std::string {
      Token(const std::string &_data, const short &_type): std::string(_data), type(_type) {}
      enum {
        none,
        string,
        token,
        evaltoken
      };
      short type = none;
    };

    struct DialogueMap {
      std::unordered_map<std::string, std::string> values;
      std::unordered_map<std::string, std::string> commands;
    };

    struct Dialogue :public std::vector<Token> {
      void substitutetoken(std::string token, std::string text) {
        for (auto &each : *this) {
          if (each.type == Token::token && each == token) {
            each = Token(text, each.type);
          }
        }
      };
    };

    Dialogue tokenize(const char *text, const char delim = '$') {
      std::istringstream stream(text);
      std::string tk;
      Dialogue token;
      bool nottok = true;
      while (std::getline(stream, tk, delim)) {
        if (tk.size()) {
          if (nottok)token.push_back(Token(tk, Token::string));
          else token.push_back(Token(tk, Token::token));
        }
        else if (!nottok) token.push_back(Token(std::string() + delim, Token::string));
        nottok = !nottok;
      }
      return token;
    }

    struct Option {
      Dialogue dialogue;
      DialogueID DialogueRef = -1;//reference to dialogue node
    };

    struct DialogueNode {
      DialogueID id;
      Dialogue dialogue;
      std::vector<std::string> audio;
      std::vector<Option> options;
    };
  }
}

