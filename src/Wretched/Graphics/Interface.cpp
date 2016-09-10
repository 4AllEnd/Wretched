#include <Wretched/Graphics/Interface.h>

#define OFFSCREENDEFAULT -1
#define GUI_MIN_FLOAT std::numeric_limits<float>::min()

bool mouseinbounds(Wretched::Graphics::Input &z, const float &x1_, const float &y1_, const float &x2_, const float &y2_) {
  return (x1_ <= z.x && y1_ <= z.y && z.x <= x2_ && z.y <= y2_);
}

bool activesort(Wretched::Graphics::Element *a, Wretched::Graphics::Element *b) {
  return (a->zorder < b->zorder);
}

void setdescendants(Wretched::Graphics::Element &element) {
  int num(element.zorder - element.numdescendants(element));
  for (auto each : element.children) {
    num += element.numdescendants(*each);
    each->zorder = num++;
    setdescendants(*each);
  }
}

void insertzorder(Wretched::Graphics::Interface &e, Wretched::Graphics::Element &element, const int index) {
  int n = element.numdescendants(*element.root);
  element.root->zorder = index + n;
  int nn = element.numdescendants(element);
  element.zorder = index + nn;
  if (nn != 0)setdescendants(element);
  int ind = n;
  for (auto &each : e.ElementList) {
    if ((each->root != element.root) && (each->zorder >= index) && (each == each->root)) {
      ind += 1 + element.numdescendants(*each);
      each->zorder = +ind;
      setdescendants(*each);
    }
  }
  setdescendants(*element.root);
}

#define ENTITY_LOGGING

namespace Wretched {
  namespace Graphics {
    bool ischild(Wretched::Graphics::Element &child, Wretched::Graphics::Element &parent) {
      for (Wretched::Graphics::Element *ptr = &child; ptr = ptr->parent;) {
        if (ptr == &parent)return true;
      }
      return false;
    };

    bool isparent(Wretched::Graphics::Element &parent, Wretched::Graphics::Element &child) {
      return ischild(child, parent);
    };

    Input::Input(int _x, int _y, bool l, bool r, bool ctrl, bool shft, bool alt, int key) : control(ctrl), shift(shft), alt(alt), key(0), x((float)_x), y((float)_y) {}

    Input::Input() : LB(false), RB(false), control(false), shift(false), alt(false), key(0), x(0), y(0) {}

    void Input::setposition(float x, float y) {
      this->x = x;
      this->y = y;
    }


    ///Interface*********************************
    GL4Interface::GL4Interface() {
      GLuint attribute_coord(0);
      VA.bind();
      glVertexAttribFormat(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0);
      glVertexAttribBinding(attribute_coord, 0);
      VBO.bind();
      glEnableVertexAttribArray(attribute_coord);
      VBO.data(16 * sizeof(float), 0, GL_DYNAMIC_DRAW);
      glBindVertexBuffer(0, VBO._name, 0, 4 * sizeof(float));
      VA.release();
    }

    void Interface::updateinput(int x = OFFSCREENDEFAULT, int y = OFFSCREENDEFAULT, bool LMB = false, bool RMB = false, int key = 0, bool control = false, bool shift = false, bool alt = false, bool mod = false, int flags = 0) {
      if (flags & Mouse) {
        previousinput.setposition(currentinput.x, currentinput.y);
        previousinput.LB = currentinput.LB;
        previousinput.RB = currentinput.RB;
        currentinput.x = std::min(std::max((float)x / width, 0.f), 1.f);
        currentinput.y = std::min(std::max((float)y / (height), 0.f), 1.f);
        currentinput.LB = LMB;
        currentinput.RB = RMB;
        mousein = mouseinbounds(currentinput, 0, 0, 1.f, 1.f);
      }
      if (flags & Keyboard) {
        previousinput.key = currentinput.key;
        previousinput.control = currentinput.control;
        previousinput.shift = currentinput.shift;
        previousinput.alt = currentinput.alt;
        previousinput.mod = currentinput.mod;
        currentinput.key = key;
        currentinput.control = control;
        currentinput.shift = shift;
        currentinput.alt = alt;
        currentinput.mod = mod;
      }
    };

    void Interface::setviewport(int w, int h) {
      width = w;
      height = h;
    }

    void Interface::update() {
      std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
      delta = std::chrono::duration<double>(now - t_past).count();
      t_past = now;
      for (auto &child:ElementList)updatestate(child);
      for (auto &child : ElementList)if (child)updateaction(child);
      auto uu = std::remove(ElementList.begin(), ElementList.end(), nullptr);
      if (uu != ElementList.end())ElementList.erase(uu, ElementList.end());
      std::sort(ElementList.begin(), ElementList.end(), activesort);
    };

    enum ElementState {
      incurrent = 1 << 0,
      inprevious = 1 << 1,
      currRinput = 1 << 2,
      prevRinput= 1 << 3,
      currLinput = 1 << 4,
      prevLinput = 1 << 5
    };

    bool Interface::updatestate(Element *element) {
      if (!element)return false;
      bool incurr = mouseinbounds(currentinput, element->bounds.min.x, element->bounds.min.y, element->bounds.max.x, element->bounds.max.y);
      bitassign(element->state, ((element->state & incurrent) << 1) | (int)incurr, inprevious | incurrent);
      element->inprevious = element->incurrent;
      element->incurrent = mouseinbounds(currentinput, element->bounds.min.x, element->bounds.min.y, element->bounds.max.x, element->bounds.max.y);
      bool entrance = ((element->state & (incurrent | inprevious)) == incurrent);
      bool exit = ((element->state & (incurrent | inprevious)) == inprevious);
      bool within = ((element->state & (incurrent | inprevious)) == (incurrent | inprevious));
      bool outside = ((element->state & (incurrent | inprevious)) == 0);
      bool lclick = (!previousinput.LB) && (currentinput.LB);
      bool lhold = (previousinput.LB) && (currentinput.LB);
      bool lrelease = (previousinput.LB) && (!currentinput.LB);
      bool rclick = (!previousinput.RB) && (currentinput.RB);
      bool rhold = (previousinput.RB) && (currentinput.RB);
      bool rrelease = (previousinput.RB) && (!currentinput.RB);
      bool click = rclick || lclick;
      if ((lhold || rhold) && !(lrelease || rrelease)) {
        holddur += delta;
      }
      else holddur = 0;
      if (Focus == element) {
        if (!element->incurrent && click)unfocus(*Focus);
        else if (element->incurrent) {
          for (auto &each : element->children) {
            if (each->incurrent && click) {
              focus(*each);
              break;
            }
          }
        }
      }
      else if (Focus) {
        if (Focus->parent == element->parent && !Focus->incurrent && element->incurrent && click)focus(*element);
      }
      else if (element->incurrent && click) {
        focus(*element);
      }
      return element->isActive;
    }

    bool Interface::updateaction(Element *element) {
      if (!element)return false;
      bool entrance = element->incurrent && !element->inprevious;
      bool exit = !element->incurrent && element->inprevious;
      bool within = element->incurrent && element->inprevious;
      bool outside = !element->incurrent && !element->inprevious;
      bool lclick = (!previousinput.LB) && (currentinput.LB);
      bool lhold = (previousinput.LB) && (currentinput.LB);
      bool lrelease = (previousinput.LB) && (!currentinput.LB);;
      bool rclick = (!previousinput.RB) && (currentinput.RB);
      bool rhold = (previousinput.RB) && (currentinput.RB);
      bool rrelease = (previousinput.RB) && (!currentinput.RB);
      bool click = lclick || rclick;
      if (!mousein)return false;
      bool action = false;//check for action occurrence
      if (element == Focus) {
        int washeld = false;
        if (lhold || rhold) {
          if (holddur != 0)washeld = true;
          holddur += delta;
        }
        else {
          holddur = 0;
          washeld = false;
        }
        if (lhold) {
          float dx = currentinput.x - previousinput.x;
          float dy = currentinput.y - previousinput.y;
          std::string olc = element->attribute["onlclick"];
          if (olc == "Grab") {
            if (dy != 0 || dx != 0) {
              if (element->parent) {
                move(*element->parent);
              }
              else move(*element);
              action = true;
            }
          }
          else if (olc == "Drag") {
            if (dy != 0 || dx != 0) {
              move(*element);
              action = true;
            }
          }
          else if (olc == "Resize") {
            resize(*element);
            action = true;
          }
          else if (element->attribute["onlclick"] == "Close") {
            unfocus(*element);
            //removeelement(*element->root);
            return true;
          }
        }
      }
      //on entry
      if (entrance) {
        //settocolor(element, { element->color.r, element->color.g, element->color.b, element->color.a });
      }
      else if (within) {
        //settocolor(element, { element->color.r, element->color.g, element->color.b, element->color.a });
      }
      //on leave
      else if (exit) {
        //settocolor(element, { element->color.r, element->color.g, element->color.b, element->color.a*.5f });
        //settocolor(element, { element->color.r - .1f, element->color.g - .1f, element->color.b - .2f, 1.f });
        //settocolor(element, { element->color.r, element->color.g, element->color.b, element->color.a });
        //setdefaultcolor({ color.r - .1f, color.g - .1f, color.b - .2f, color.a });
      }
      //if out of influence
      else { }

      return action;

      return false;
    }

    void Interface::render() {
      for (auto rit = ElementList.rbegin(); rit != ElementList.rend(); ++rit)draw(**rit);
    }

    void Interface::focus(Element &element) {
      Focus = &element;
      insertzorder(*this, element, 0);
      element.messenger.notify("gainedfocus");
    }

    void Interface::unfocus(Element &element) {
      Focus = 0;
      element.messenger.notify("lostfocus");
    }

    void Interface::setposition(Element &element, float &x, float &y) {}

    void scale(float &min, float &max, float &scale, bool changeright = true) {
      if (!changeright)min = scale * (max - min) + min;
      else max = scale * (min - max) + max;
    }

    void Interface::resize(Element &element) {
      float dx = currentinput.x - previousinput.x;
      float dy = currentinput.y - previousinput.y;
      float mx = (element.bounds.min.x + element.bounds.max.x) / 2.f;
      float my = (element.bounds.min.y + element.bounds.max.y) / 2.f;
      float xsize = element.bounds.max.x - element.bounds.min.x;
      float ysize = element.bounds.max.y - element.bounds.min.y;
      float scalex = 1 + dx / xsize;
      float scaley = 1 + dy / ysize;
      if ((mx + element.bounds.max.x) / 2.f < currentinput.x) {
        //scale(element.bounds.min.x, element.bounds.max.x, scalex, false);
        move(element.bounds.max.x, std::max(-element.bounds.max.x, dx));
      }
      else if ((mx + element.bounds.min.x) / 2.f > currentinput.x) {
        scale(element.bounds.min.x, element.bounds.max.x, scalex, true);
        //move(element.bounds.min.x, std::min(1 - element.bounds.min.x - GUI_MIN_FLOAT, dx));
      }
      if ((my + element.bounds.max.y) / 2.f < currentinput.y) {
        move(element.bounds.max.y, std::max(-element.bounds.max.y, dy));
      }
      else if ((my + element.bounds.min.y) / 2.f > currentinput.y) {
        move(element.bounds.min.y, std::min(1 - element.bounds.min.y, dy));
      }
      element.messenger.notify("resized");
    }

    void Interface::move(Element &element) {
      float dx = currentinput.x - previousinput.x;
      float dy = currentinput.y - previousinput.y;
      if (!dx && !dy)return;
      move(element.bounds.min.x, dx);
      move(element.bounds.max.x, dx);
      move(element.bounds.min.y, dy);
      move(element.bounds.max.y, dy);
      element.messenger.notify("moved");
    }

    enum Border {
      UPPER = 1,
      LOWER = 1 << 1,
      RIGHT = 1 << 2,
      LEFT = 1 << 3,
      ULCORNER=LEFT + UPPER,
      DLCORNER=LEFT + LOWER,
      URCORNER=LEFT + UPPER,
      LRCORNER=LEFT + LOWER,
      ALL=UPPER + LOWER + RIGHT + LEFT
    };

    void Interface::move(float &point, float dist) {
      point += dist;
    };

    void GL4Interface::draw(Element &element) {
      float square[4][4] = {
        {element.bounds.min.x, element.bounds.min.y, 0, 0},
        {element.bounds.max.x, element.bounds.min.y, 1, 0},
        {element.bounds.min.x, element.bounds.max.y, 0, 1},
        {element.bounds.max.x, element.bounds.max.y, 1, 1}
      };
      GL4Element *E = (GL4Element*)&element;
      E->texture.bind();
      VA.bind();
      VBO.bind();
      VBO.subdata(0, 16 * sizeof(float), &square[0][0]);
      glUniform1ui(3, element.owner->getid());
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    //GL4Element* GL4Interface::createelement(Entity &entity) {
    //  GL4Element *ptr = mgr->createcomponent<GL4Element>(entity);
    //  ptr->master = this;
    //  ptr->root = ptr;
    //  ptr->parent = NULL;
    //  ptr->zorder = 0;
    //  ptr->owner = &entity;
    //  for (auto &each : ElementList)++each->zorder;
    //  ElementList.push_back(ptr);
    //  return ptr;
    //}

    //GL4Element* GL4Interface::createelement(Entity &entity, Element &_parent) {
    //  GL4Element *ptr = createcomponent<GL4Element>(entity);
    //  ElementList.push_back(ptr);
    //  ptr->master = this;
    //  ptr->root = _parent.root;
    //  ptr->parent = &_parent;
    //  ptr->owner = &entity;
    //  _parent.children.push_back(ptr);
    //  insertzorder(*this, *ptr, ++_parent.zorder - _parent.children.size());//careful with size
    //  ptr->inbox.subscribe(_parent.messenger);
    //  _parent.inbox.subscribe(ptr->messenger);
    //  return ptr;
    //}

    void GL4Interface::useimage(Element &element, std::string filename) {
      element.usingimage = true;
      ILboolean isload = ilLoadImage(filename.c_str());
      GL4Element *E = (GL4Element*)&element;
      E->texture.bind();
      if (isload) {
        printf("loaded %s successfully!\n", filename.c_str());
        ilEnable(IL_ORIGIN_SET);
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
        glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_OFFX), ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
      }
      else {
        printf("Error couldn't load %s\n", filename.c_str());
        E->texture.store(0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_FLOAT, &E->color);
      }
      E->texture.release();
    };

    void GL4Interface::settocolor(const Element &element, const Color &a) {
      GL4Element &E = (GL4Element&)element;
      E.texture.bind();
      E.texture.store(0,GL_RGBA8, 1, 1, 0, GL_RGBA, GL_FLOAT, &a);
      E.texture.release();
    };

    void TraverseElement(Interface &_interface, Entity::Manager &emgr, Element &parent, pugi::xml_node &parent_node, pugi::xml_node &definition, void (*func)(Interface &, Entity::Manager &, Element &, pugi::xml_node &, pugi::xml_node &)) {
      for (auto &each : parent_node.children("Element")) {
        Entity &ent = *emgr.createentity();
        auto *ptr = _interface.createelement(ent, parent);
        func(_interface, emgr, *ptr, each, definition);
        TraverseElement(_interface, emgr, *ptr, each, definition, func);
        std::cout << "Element type: " << ptr->type.c_str() << " The daddy is: " << (ptr->parent ? ptr->parent->type + " Its order is: " + std::to_string(ptr->parent->zorder).c_str() : "") << " Pecking Order: " << ptr->zorder << std::endl;
      }
    }

    void perElement(Interface &_interface, Entity::Manager &emgr, Element &element, pugi::xml_node &a, pugi::xml_node &definition) {
      pugi::xml_node p;
      if (a.attribute("parent")) {
        p = definition.find_child_by_attribute("type", a.attribute("parent").value());
        for (auto &child : p.children("Element")) {//std::cout << child.path()<<"-"<<child.attribute("type").value()<< std::endl;
          a.append_copy(child);
        }
      }
      //inheritance hierarchy->explicit inheritance from element parent >explicit self defined color>implicit parent defined >implicit default
      //i.e. if(element.inheritscolor&&element->parent)
      element.path = a.path();
      element.type = a.attribute("type").value();
      auto *g = &a;
      if (a.child("color")) {
        std::stringstream nums(g->child_value("color"));
        std::string line;
        for (int i = 0; i < 4; ++i) {
          std::getline(nums, line, ',');
          try {
            element.color[i] = std::stof(line);
          }
          catch (const std::exception &e) {
            fprintf(stderr, e.what());
            element.color[i] = .1f;
          }
        }
      }
      if (a.child("size")) {
        std::stringstream nums(g->child_value("size"));
        std::string line;
        if (std::string(g->child("size").attribute("type").value()) == std::string("Relative") && element.parent) {
          try {
            std::getline(nums, line, ',');
            element.bounds.min.x = element.parent->bounds.min.x + std::stof(line) * (-element.parent->bounds.min.x + element.parent->bounds.max.x);
            std::getline(nums, line, ',');
            element.bounds.min.y = element.parent->bounds.min.y + std::stof(line) * (-element.parent->bounds.min.y + element.parent->bounds.max.y);
            std::getline(nums, line, ',');
            element.bounds.max.x = element.parent->bounds.min.x + std::stof(line) * (-element.parent->bounds.min.x + element.parent->bounds.max.x);
            std::getline(nums, line, ',');
            element.bounds.max.y = element.parent->bounds.min.y + std::stof(line) * (-element.parent->bounds.min.y + element.parent->bounds.max.y);
          }
          catch (const std::exception &e) {
            fprintf(stderr, e.what());
            element.bounds = {.0f,.0f,.0f,.0f};
          }
        }
        else {
          try {
            std::getline(nums, line, ',');
            element.bounds.min.x = std::stof(line);
            std::getline(nums, line, ',');
            element.bounds.min.y = std::stof(line);
            std::getline(nums, line, ',');
            element.bounds.max.x = std::stof(line) - element.bounds.min.x;
            std::getline(nums, line, ',');
            element.bounds.max.y = std::stof(line) - element.bounds.min.y;
          }
          catch (const std::exception &e) {
            fprintf(stderr, e.what());
            element.bounds = {.0f,.0f,.0f,.0f};
          }
        }
      }
      g = 0;
      if (a.child("onlclick"))g = &a;
      else if (p.child("onlclick"))g = &p;
      if (a.child("onlclick")) {
        element.attribute["onlclick"] = g->child("onlclick").attribute("type").value();
      }
      else element.attribute["onlclick"] = "Grab";

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      _interface.settocolor(element, element.color);
    }

    int Interface::LoadXML(std::string filename, Entity::Manager &emgr) {
      std::cout << "loading XML" << std::endl;
      pugi::xml_document doc;
      pugi::xml_parse_result result = doc.load_file(filename.c_str());
      Element *zptr = 0;
      for (pugi::xml_node &each : doc.child("File")) {
        if (each == doc.child("File").child("Definition"))continue;
        Entity *ent = emgr.createentity();
        Element *ptr = createelement(*ent);
        auto filedef = doc.child("File").child("Definition");
        perElement(*this, emgr, *ptr, each, filedef);
        std::cout << "Element type: " << ptr->type.c_str() << " The daddy is: " << (ptr->parent ? ptr->parent->type.c_str() : "GUI") << " Pecking Order: " << ptr->zorder << std::endl;
        TraverseElement(*this, emgr, *ptr, each, filedef, perElement);
        // (*ptr).zorder = (*zptr).zorder++;
        zptr = ptr;
      }
      return 0;
    };

    //need to split into two functions, one with additional interface parameter
    void InterfaceSubscriber::acknowledge(InterfaceEvent &msgr, std::string a) {
      if ((a == std::string("moved")) && (this->reciever->parent ? msgr.sender == this->reciever->parent : false)) {
        this->reciever->master->move(*this->reciever);
      }
    }

    void InterfaceEvent::notify(std::string a) {
      std::printf("This has happened: %d %s has %s\n", sender->owner->getid(), sender->type.c_str(), a.c_str());
      for (auto each : subscribers)((InterfaceSubscriber*)each)->acknowledge(*this, a);
    }
  }

}

