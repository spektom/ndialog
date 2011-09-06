#ifndef DIALOG_H
#define DIALOG_H

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <vector>
#include <string>
#include "NDialog.h"
#include "Component.h"

class Dialog
{
private:
	  std::vector<Component> components;

		int width;
		int height;
		int timeout;

		std::string backtitle;
		std::string title;

private:
		void GetXMLNodePropNum (xmlNode *node, const char *name, int *buf);
		void GetXMLNodePropStr (xmlNode *node, const char *name, std::string &buf);
		void GetXMLNodeContent (xmlNode *node, std::string &buf);

public:
		Dialog():width(80), height(25), backtitle(""), title("") {}
		~Dialog() {
		}


		void BuildFromXML (const char *file);
		void Draw ();
};

#endif // DIALOG_H
