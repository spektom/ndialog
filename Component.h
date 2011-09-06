#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <newt.h>

enum ComponentType
{
	TYPE_NONE,
	TYPE_BUTTON,
	TYPE_LABEL,
	TYPE_EDITBOX, 
	TYPE_PASSWORDBOX, 
	TYPE_CHECKBOX,
	TYPE_RADIOBUTTON,
	TYPE_SCALE,
	TYPE_TEXTBOX, 
	TYPE_SCROLLBAR,
	TYPE_LISTBOX
};

class Component
{
private:
	std::string id;
	std::string group_id; // For radiobuttons and menus
	ComponentType type;
	newtComponent data;

public:
	Component ():id(""), group_id(""), type(TYPE_NONE), data(NULL) {}

	Component (std::string &id, ComponentType type, newtComponent data)
		:id(id), group_id(""), type(type), data(data) {}

  std::string & getID()       { return this->id;       }
  std::string & getGroupID()  { return this->group_id; }
  ComponentType getType()     { return this->type;     }
  newtComponent getData ()    { return this->data;     }

	void setID (const char *id)             { this->id = id;             }
	void setGroupID (const char *group_id)  { this->group_id = group_id; }
	void setType (ComponentType type) { this->type = type; }
	void setData (newtComponent data) { this->data = data; }

	Component & operator = (Component &c);
};

bool operator == (Component &c1, Component &c2);

#endif // COMPONENT_H
