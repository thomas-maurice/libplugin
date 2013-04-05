#include <iostream>
#include "../libplugin.h"

using namespace std;

struct foo
{
	int i;
	string str;
};

/*
	These four functions are compulsory for the lib, a module won't charge if
	they are not provided. They shall be in an extern "c" context so that libdl
	will see them.
*/
extern "C"
{
	int DllExport on_load(); // Wat do on loading ?
	int DllExport on_unload(); // Wat do on unloading
	int DllExport process(void *args); // The main function
	void* DllExport plugin_info(); // If you want to pass some info about
	// the plugin in a user-defined structure, use a void pointer.
}

int DllExport on_load()
{
	cout << "Loading..." << endl;
	return 0;
}

int DllExport on_unload()
{
	cout << "Unloading..." << endl;
	return 0;
}

int DllExport process(void *args)
{
	//We cast it from void to *foo
	//foo *s = (foo*)args;
	VOID_TO_OBJ(args, foo, s);
	// Let's print it
	cout << s->i << " | " << s->str << endl;
	// Let's change it
	s->i = 0;
	return 0;
}

void* DllExport plugin_info() {
	// No info provided
	return NULL;
}
