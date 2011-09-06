#include <stdio.h>
#include "NDialog.h"
#include "Dialog.h"


int main (int argc, char **argv)
{
	if (argc != 2) {
		fprintf (stderr, "USAGE: %s <dialog.xml>\n", argv[0]);
		exit (-1);
	}

	try {
		Dialog d;
		d.BuildFromXML (argv[1]);
		d.Draw();
	}
	catch (std::string &s) {
		fprintf (stderr, "ERROR: %s\n", s.c_str());
		exit (-1);
	}

	return 0;
}
