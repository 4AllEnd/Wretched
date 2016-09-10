#pragma once
#include <bitset>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <pugixml.hpp>
#include <IL/il.h>
#include <IL/ilu.h>
#include <limits>
#include <Wretched/Math/Rect.h>
#include <Wretched/Math/BitManipulation.h>
#include <Wretched/System/Entity.h>
#include <Wretched/System/Event.h>
#include <Wretched/Graphics/GLError.h>
#include <Wretched/Graphics/Structures.h>
#include <Wretched/Algorithm/Tree.h>

namespace Wretched {
  namespace Graphics {
    struct InterfaceEvent;
    struct InterfaceSubscriber;
    struct Interface;
    struct Element;
    typedef std::shared_ptr<Element> Elementptr;
    typedef Math::Rect<float> BRect;
    typedef glm::vec4 Color;
    struct GL4Interface;
    struct GL4Element;

    struct Input {
      Input();
      Input(int _x, int _y, bool l, bool r, bool ctrl, bool shft, bool alt, int key);
      int key;
      float x, y;
      bool LB, RB, control, shift, alt, mod;
      void setposition(float x, float y);
    };

    struct InterfaceSubscriber : Subscriber {
      InterfaceSubscriber(Element &_reciever) : reciever(&_reciever) {}
      Element *reciever = 0;
      virtual void acknowledge(InterfaceEvent &msgr, std::string a);
    };

    struct InterfaceEvent: Event {
      InterfaceEvent(Element &_sender): sender(&_sender) {}
      Element *sender = 0;
      virtual void notify(std::string a);
    };

    struct Element : Node<Element> {
      enum Att {
        Disabled,
        Draggable,
        Grabbable,
        Resizeable,
        Scaling
      };
      Interface *master = 0;
      Entity* owner = 0;
      bool incurrent, inprevious;
      int state = 0, zorder = 0;
      bool isActive = false, isInteracting = false, usingimage = false;
      BRect bounds = {0.f,0.f,.2f,.2f};
      std::unordered_map<std::string, std::string> attribute;
      Color color, color2;
      std::string text, path, type;
      InterfaceEvent messenger = InterfaceEvent(*this);
      InterfaceSubscriber inbox = InterfaceSubscriber(*this);
    };

    struct Interface {
      enum Inputflags : char {
        Mouse = 1 << 0,
        Keyboard = 1 << 1
      };

      std::vector<Element*> ElementList;//list of Elements such as buttons and windows; it is regularly sorted and used for drawing.
      std::chrono::high_resolution_clock::time_point t_past;
      double delta, holddur = 0;//time since last update
      Input currentinput, previousinput;//input markers	
      int width = 0, height = 0;//pixel width and height of viewport
      bool mousein = false;
      virtual void updateinput(int x, int y, bool LMB, bool RMB, int key, bool control, bool shift, bool alt, bool mod, int flags);
      virtual void update();//update Interface components
      virtual void setviewport(int width, int height);//set pixel width and height of viewport
      Element* GetChild(Element &, int uiIndex);
      virtual int LoadXML(std::string Filename, Entity::Manager &);
      //virtual int SaveXML(std::string Filename);
      virtual void render();
      ///probably need to do input filtering in interface
      virtual Element* createelement(Entity &entity) =0;
      virtual Element* createelement(Entity &entity, Element &element) =0;
      virtual void setposition(Element &, float &, float &);
      Element *Focus = 0;
      virtual void focus(Element &);
      virtual void unfocus(Element &element);
      virtual void move(Element &);
      virtual void move(float &, float);
      virtual bool updatestate(Element *), updateaction(Element *);
      virtual void resize(Element &);

      /*virtual void hide(Element&);
      virtual void show(Element&);
      virtual bool displaytext(Element&);
      virtual void drag(Element&);
      virtual void makeactive(Element&);
      virtual bool isvisible(Element&);
      */
      //commands requiring tangibility
      virtual void useimage(Element &, std::string Filename) {};

      virtual void draw(Element &) =0;

      virtual void settocolor(const Element &, const Color &) {};

      //virtual void drawtext(Element&);
      virtual bool ownsinput() {
        return Focus != 0;
      }
    };

    struct GL4Element : public Element {
      GL::Texture2D texture;
      std::shared_ptr<GL::Texture2D> cursorimage;
    };

    struct GL4Interface : public Interface {
      GL4Interface();
      GL::VertexArray VA;
      GL::BufferObject VBO;
      virtual void draw(Element &);
      //virtual void drawtext(Element&);
      virtual void settocolor(const Element &element, const Color &);
      virtual void useimage(Element &, std::string Filename);
    };

    bool ischild(Element &child, Element &parent);
    bool isparent(Element &parent, Element &child);
  }
}

