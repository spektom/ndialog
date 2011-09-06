#include <libxml/parser.h>
#include <libxml/tree.h>
#include <fstream>
#include <string.h>

#include "NDialog.h"
#include "Dialog.h"
#include "Component.h"

void Dialog::GetXMLNodePropNum (xmlNode *node, const char *name, int *buf)
{
	char *attr = (char *) xmlGetProp (node, (const xmlChar *)name);
	if (attr) {
		*buf = atoi (attr);
	}
	else throw "No number attribute \""+ std::string(name) 
		+"\" found in XML node: "+ std::string((char *)node->name);
}

void Dialog::GetXMLNodePropStr (xmlNode *node, const char *name, std::string &buf)
{
	char *attr = (char *)xmlGetProp (node, (const xmlChar *)name);
	if (attr) {
		buf = attr;
	}
	else throw "No string attribute \""+ std::string(name) 
		+"\" found in XML node: "+ std::string((char *)node->name);
}

void Dialog::GetXMLNodeContent (xmlNode *node, std::string &buf)
{
	char *cont = (char *)node->children->content;
	if (cont) {
		buf = cont;
	}
	else throw "No contents found in XML node: " + std::string((char *)node->name);
}

void Dialog::BuildFromXML(const char *file)
{
	xmlDoc *doc;
	doc = xmlParseFile (file);
	if(!doc) throw "Cannot parse XML file: " + std::string(file);

	xmlNode *node;
	node = xmlDocGetRootElement (doc);

	while (node) 
	{
		if(node->type != XML_ELEMENT_NODE) {
			node = node->next;
			continue;
		}
		if(!strcmp ((const char*)node->name, "dialog")) {
			try {
				GetXMLNodePropNum (node, "width", &this->width);
				GetXMLNodePropNum (node, "height", &this->height);
				GetXMLNodePropStr (node, "title", this->title);
				GetXMLNodePropStr (node, "backtitle", this->backtitle);
			}
			catch (std::string e) {}

			node = node->children;
		}
		else if(!strcmp ((const char*)node->name, "button")) {
			std::string id, text;
			int left, top;
			try {
				GetXMLNodePropStr (node, "id", id);
				GetXMLNodePropNum (node, "left", &left);
				GetXMLNodePropNum (node, "top", &top);
				GetXMLNodeContent (node, text);
			}
			catch (std::string e) { throw e; }
			Component c (id, TYPE_BUTTON, newtButton(left, top, text.c_str()));
			components.push_back (c);

			node = node->next;
		}
		else if(!strcmp ((const char*)node->name, "label")) {
			std::string id, text;
			int left, top;
			try {
				GetXMLNodePropStr (node, "id", id);
				GetXMLNodePropNum (node, "left", &left);
				GetXMLNodePropNum (node, "top", &top);
				GetXMLNodeContent (node, text);
			}
			catch (std::string e) { throw e; }
			Component c (id, TYPE_LABEL, newtLabel(left, top, text.c_str()));
			components.push_back (c);

			node = node->next;
		}
		else if(!strcmp ((const char*)node->name, "editbox")) {
			std::string id, text;
			int left, top, width;
			try {
				GetXMLNodePropStr (node, "id", id);
				GetXMLNodePropNum (node, "left", &left);
				GetXMLNodePropNum (node, "top", &top);
				GetXMLNodePropNum (node, "width", &width);
			}
			catch (std::string e) { throw e; }
			GetXMLNodeContent (node, text);
			
			Component c (id, TYPE_EDITBOX, newtEntry(left, top, text.c_str(), width, NULL, 0));
			components.push_back (c);

			node = node->next;
		}
		else if(!strcmp ((const char*)node->name, "passwordbox")) {
			std::string id, text;
			int left, top, width;
			try {
				GetXMLNodePropStr (node, "id", id);
				GetXMLNodePropNum (node, "left", &left);
				GetXMLNodePropNum (node, "top", &top);
				GetXMLNodePropNum (node, "width", &width);
			}
			catch (std::string e) { throw e; }
			GetXMLNodeContent (node, text);
			
			Component c (id, TYPE_PASSWORDBOX, newtEntry(left, top, text.c_str(), width, NULL, NEWT_FLAG_PASSWORD));
			components.push_back (c);

			node = node->next;
		}
		else if(!strcmp ((const char*)node->name, "checkbox")) {
			std::string id, text;
			int left, top, value;
			try {
				GetXMLNodePropStr (node, "id", id);
				GetXMLNodePropNum (node, "left", &left);
				GetXMLNodePropNum (node, "top", &top);
				GetXMLNodePropNum (node, "value", &value);
			}
			catch (std::string e) { throw e; }
			GetXMLNodeContent (node, text);

			char def_val = (value ? '*' : ' ');
			Component c (id, TYPE_CHECKBOX, newtCheckbox(left, top, text.c_str(), def_val, NULL, NULL));
			components.push_back (c);

			node = node->next;
		}
		else if(!strcmp ((const char*)node->name, "radiobox")) {
			std::string group_id, value;
			try {
				GetXMLNodePropStr (node, "id", group_id);
			}
			catch (std::string e) { throw e; }
			GetXMLNodePropStr (node, "value", value);

			xmlNode *tmpNode = node;
			node = node->children;

			newtComponent prev_button = NULL;
			while (node) {
				if(node->type != XML_ELEMENT_NODE) {
					node = node->next;
					continue;
				}
				std::string id, text;
				int left, top;
				if(!strcmp ((const char *)node->name, "radiobutton")) {
					try {
						GetXMLNodePropStr (node, "id", id);
						GetXMLNodePropNum (node, "left", &left);
						GetXMLNodePropNum (node, "top", &top);
					}
					catch (std::string e) { throw e; }
					GetXMLNodeContent (node, text);

					int is_default = (value == id ? 1 : 0);

					Component c(id, TYPE_RADIOBUTTON, 
							newtRadiobutton(left, top, text.c_str(), is_default, prev_button));

					c.setGroupID (group_id.c_str());
					components.push_back (c);

					prev_button = c.getData();
				}
				else throw "Only tag <radiobutton> can be included in radiobox";
				node = node->next;
			}
			node = tmpNode->next;
		}
		else if(!strcmp ((const char*)node->name, "textbox")) {
			std::string id, file, text;
			int left, top, width, height;
			try {
				GetXMLNodePropStr (node, "id", id);
				GetXMLNodePropNum (node, "left", &left);
				GetXMLNodePropNum (node, "top", &top);
				GetXMLNodePropNum (node, "width", &width);
				GetXMLNodePropNum (node, "height", &height);
				GetXMLNodePropStr (node, "file", file);
			}
			catch (std::string e) { throw e; }
			
			Component c (id, TYPE_TEXTBOX,
					newtTextbox(left, top, width, height, NEWT_FLAG_WRAP|NEWT_FLAG_SCROLL));

			std::ifstream f (file.c_str());
			std::string s_tmp;
			while (!std::getline (f, s_tmp).eof()) {
				text += s_tmp + '\n';
			}
			f.close();

			newtTextboxSetText(c.getData(), text.c_str());
			components.push_back (c);

			node = node->next;
		}
		else if(!strcmp ((const char*)node->name, "textarea")) {
			std::string id, text;
			int left, top, width, height;
			try {
				GetXMLNodePropStr (node, "id", id);
				GetXMLNodePropNum (node, "left", &left);
				GetXMLNodePropNum (node, "top", &top);
				GetXMLNodePropNum (node, "width", &width);
				GetXMLNodePropNum (node, "height", &height);
				GetXMLNodeContent (node, text);
			}
			catch (std::string e) { throw e; }
			
			Component c (id, TYPE_TEXTBOX,
					newtTextbox(left, top, width, height, 0));

			newtTextboxSetText(c.getData(), text.c_str());
			components.push_back (c);

			node = node->next;
		}
		else if(!strcmp ((const char*)node->name, "listbox")) {
			std::string group_id, value;
			int left, top, width, height;
			try {
				GetXMLNodePropStr (node, "id", group_id);
				GetXMLNodePropNum (node, "left", &left);
				GetXMLNodePropNum (node, "top", &top);
				GetXMLNodePropNum (node, "width", &width);
				GetXMLNodePropNum (node, "height", &height);
			}
			catch (std::string e) { throw e; }
			GetXMLNodePropStr (node, "value", value);

			xmlNode *tmpNode = node;
			node = node->children;

			Component c(group_id, TYPE_LISTBOX, 
					newtListbox(left, top, height, NEWT_FLAG_SCROLL|NEWT_FLAG_BORDER));

			while (node) {
				if(node->type != XML_ELEMENT_NODE) {
					node = node->next;
					continue;
				}
				std::string id, text;
				if(!strcmp ((const char *)node->name, "item")) {
					try {
						GetXMLNodePropStr (node, "id", id);
					}
					catch (std::string e) { throw e; }
					GetXMLNodeContent (node, text);

					newtListboxAddEntry(c.getData(), text.c_str(), id.c_str());
				}
				else throw "Only tag <item> can be included in listbox";
				node = node->next;
			}
			newtListboxSetWidth (c.getData(), width);
			newtListboxSetCurrentByKey (c.getData(), (char *)value.c_str());
			components.push_back (c);

			node = tmpNode->next;
		}
		else if(!strcmp ((const char*)node->name, "menubox")) {
			std::string group_id, value;
			int left, top, width, height;
			try {
				GetXMLNodePropStr (node, "id", group_id);
				GetXMLNodePropNum (node, "left", &left);
				GetXMLNodePropNum (node, "top", &top);
				GetXMLNodePropNum (node, "width", &width);
				GetXMLNodePropNum (node, "height", &height);
			}
			catch (std::string e) { throw e; }
			GetXMLNodePropStr (node, "value", value);

			xmlNode *tmpNode = node;
			node = node->children;

			Component c(group_id, TYPE_LISTBOX, 
					newtListbox(left, top, height, NEWT_FLAG_SCROLL));

			while (node) {
				if(node->type != XML_ELEMENT_NODE) {
					node = node->next;
					continue;
				}
				std::string id, text;
				if(!strcmp ((const char *)node->name, "item")) {
					try {
						GetXMLNodePropStr (node, "id", id);
					}
					catch (std::string e) { throw e; }
					GetXMLNodeContent (node, text);

					newtListboxAddEntry(c.getData(), text.c_str(), id.c_str());
				}
				else throw "Only tag <item> can be included in menubox";
				node = node->next;
			}
			newtListboxSetWidth (c.getData(), width);
			newtListboxSetCurrentByKey (c.getData(), (char *)value.c_str());
			components.push_back (c);

			node = tmpNode->next;
		}
		else if(!strcmp ((const char*)node->name, "scale")) {
			std::string id;
			int left, top, width, size;
			try {
				GetXMLNodePropStr (node, "id", id);
				GetXMLNodePropNum (node, "left", &left);
				GetXMLNodePropNum (node, "top", &top);
				GetXMLNodePropNum (node, "width", &width);
				GetXMLNodePropNum (node, "size", &size);
			}
			catch (std::string e) { throw e; }

			Component c(id, TYPE_SCALE, newtScale(left, top, width, size));
			components.push_back (c);
			node = node->next;
		}
		else if(!strcmp ((const char*)node->name, "scrollbar")) {
			int left, top, height;
			try {
				GetXMLNodePropNum (node, "left", &left);
				GetXMLNodePropNum (node, "top", &top);
				GetXMLNodePropNum (node, "height", &height);
			}
			catch (std::string e) { throw e; }

			std::string id = "";
			Component c(id, TYPE_SCROLLBAR, 
				newtVerticalScrollbar(left, top, height, NEWT_COLORSET_WINDOW, NEWT_COLORSET_ACTCHECKBOX));

			components.push_back (c);
			node = node->next;
		}
		else throw "Unknown tag name: "+ std::string((const char *)node->name);
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();
}

void Dialog::Draw()
{
	newtInit();
	newtCls();

	newtCenteredWindow(this->width, this->height, this->title.c_str());
	if (this->backtitle.length() > 0) {
		newtDrawRootText(0, 0, this->backtitle.c_str());
	}

	newtComponent form = newtForm(NULL, NULL, 0);

	std::vector<Component>::iterator i;
	for (i = components.begin(); i < components.end(); ++i) {
		newtFormAddComponent(form, i->getData());
	}

	newtRunForm(form);
	newtFormDestroy(form);
	newtFinished();
}

