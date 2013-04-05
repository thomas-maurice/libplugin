#include <iostream>
#include <cstdlib>
#include <libplugin.h>

/*
	Exemple program for the lib. Here we create a simple structure with an int and
	a string. We'll pass it to the module which will modify it.
*/
using namespace std;

struct foo
{
	int i;
	string str;
};

int main()
{
	// We load the module
	#if defined (linux)
		Plugin plugin("./mod.so");
	#elif defined (WIN32)
		Plugin plugin("./mod.dll");
	#endif
	// Loading...
	plugin.load();
	// Explicit enough
  if(!plugin.is_loaded()) {
    cout << "Error: Unable to load module `mod`" << endl;
    return EXIT_FAILURE;
  }
  // We create a new pointer on the struct
	foo *s = new foo;
	s->i = -1; // Configure it
	s->str = "foooobar";
	// Cast it into a void pointer named "args"
	OBJ_TO_VOID(s, arg);
	// Passit to the module
	plugin.do_process(arg);
	// OMG s->i is changed !
	cout << s->i << endl;
	// Unload the plugin
	plugin.unload();
	// We destroy the pointer
	delete s;
	return 0;
}
