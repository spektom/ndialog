#include "Component.h"

Component & Component::operator = (Component &c)
{
	this->type = c.type;
	this->id = c.id;
	this->group_id = c.group_id;
	this->data = c.data;

	return *this;
}

bool operator == (Component &c1, Component &c2)
{
	return (c1.getType() == c2.getType()
			&& c1.getID() == c2.getID()
			&& c1.getGroupID() == c2.getGroupID());
}
