/*

Copyright (C) 2011 Thomas MAURICE

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

/**
  \file plugin.cpp
  \author Thomas Maurice
*/

#include <plugin.h>

using namespace std;

/**
  \brief Overloaded constructor which allow you to specify the
  path to the module you wish to load
  \param[in] lib the path (absolute or relative) to the module you wish to load
*/
Plugin::Plugin(std::string lib)
{
	file = lib;
	on_load = NULL;
	on_unload = NULL;
	plugininfo = NULL;
	process = NULL;
	loaded = false;
	libHandle = NULL;
}

/**
  \brief A more simple constructor, no filename is specified
*/
Plugin::Plugin()
{
	file = "";
	on_load = NULL;
	on_unload = NULL;
	plugininfo = NULL;
	process = NULL;
	loaded = false;
	libHandle = NULL;
}

/**
  \brief Changes the filename of the plugin file
  \param[in] p_file Filename (relative or absolute)
  
*/
void Plugin::set_file(std::string p_file)
{
	file = p_file;
}

int Plugin::load()
{
	#if defined (WIN32)
		/*
			Si le système d'exploitation est windows, on le charge
			à la win32 style.
		*/
		libHandle = LoadLibrary(file.c_str());
		if(libHandle == NULL)
		{
			cout << "ERROR : Unable to load plugin " << file << endl;
			return -1;
		}
		
		on_load = (int(*)())GetProcAddress(libHandle,"on_load");
		if(on_load == NULL)
		{
			cout << "ERROR : Function on_load couldn't be loaded" << endl;
		}
		on_unload = (int(*)())GetProcAddress(libHandle,"on_unload");
		if(on_unload == NULL)
		{
			cout << "ERROR : Function on_unload couldn't be loaded" << endl;
		}
		process = (int(*)(void* args))GetProcAddress(libHandle,"process");
		if(process == NULL)
		{
			cout << "ERROR : Function process couldn't be loaded" << endl;
		}
		plugininfo = (void*(*)())GetProcAddress(libHandle,"plugin_info");
		if(plugininfo == NULL)
		{
			cout << "ERROR : Function plugin_info couldn't be loaded" << endl;
		}
		
		if(on_load != NULL && on_unload != NULL && process != NULL && plugininfo != NULL)
		{
			loaded = true;
			on_load();
		}
		else
		{
			cout << "ERROR : Function " << file << " couldn't be loaded" << endl;
			return -1;
		}
		
		return 0;
		
	#elif defined (linux)
		/*
			Si le système d'exploitation est linux, on le charge
			à la Tux style.
		*/
		libHandle = dlopen(file.c_str(), RTLD_NOW);
		if(libHandle == NULL)
        {
            cout << "Erreur dlopen, lors de l'ouverture du module " << file << " : " << dlerror() << endl;
            loaded = false;
            return -1;
        }
		
		on_load = (int(*)()) dlsym(libHandle, "on_load");
		if(on_load == NULL)
		{
			cout << "dlsym error while loading the module " << file << " : " << dlerror() << endl;
			loaded = false;
		}
		on_unload = (int(*)()) dlsym(libHandle, "on_unload");
		if(on_unload == NULL)
		{
			cout << "dlsym error while loading the module " << file << " : " << dlerror() << endl;
			loaded = false;
		}
		process = (int(*)(void* args)) dlsym(libHandle, "process");
		if(process == NULL)
		{
			cout << "dlsym error while loading the module " << file << " : " << dlerror() << endl;
			loaded = false;
		}
		
		plugininfo = (void * (*)()) dlsym(libHandle, "plugin_info");
		if(plugininfo == NULL)
		{
			cout << "dlsym error while loading the module " << file << " : " << dlerror() << endl;
			loaded = false;
		}
		
		if(on_load != NULL && on_unload != NULL && process != NULL && plugininfo != NULL)
		{
			loaded = true;
			on_load();
		}
		else
		{
			cout << "ERROR : The plugin " << file << " couldn't be loaded" << endl;
			return -1;
		}
		
		return 0;
		
	#endif
	
	return 0;
}

/**
  \brief This function reloads the plugin
  
  This is the same than to perform the following :
  ``` plugin.unload();
  plugin.load();```
  
*/
int Plugin::reload()
{
	unload();
	return load();
}

/**
  Unloads the plugin by freeing the handle on the dynamic library. This will call
  the on_unload() method of the plugin.
*/
int Plugin::unload()
{
	#if defined (WIN32)
		/*
			Si le système d'exploitation est windows, on le décharge
			à la win32 style.
		*/
		if(loaded)
			on_unload();
		FreeLibrary(libHandle);
		loaded = false;
		return 0;
		
	#elif defined (linux)
		/*
			Si le système d'exploitation est linux, on le décharge
			à la Tux style.
		*/
		if(loaded)
			on_unload();
		dlclose(libHandle);
        loaded = false;
		return 0;
		
	#endif
}

/**
  \brief Processes the main function of the plugin
		  
  You can pass a struct with several fields containing all the input AND output
  information you want your plugin to process. The return code of the function is
  totally up to you, it will not make the class react in anyway. You can cast your
  pointer using the **OBJ_TO_VOID** macro from the *macro.h* file. In your plugin you
  shall use the **VOID_TO_OBJ** macro to cast it back to the type you want.
		  
  \param[in, out] args This is a pointer on void to the parameter you want to
  pass to the function. This can be the cast of the pointer on a struct, an object
  or whatever you want.
 
  \see OBJ_TO_VOID
  \see VOID_TO_OBJ
*/
int Plugin::do_process(void *args)
{
	if(loaded)
	{
		return process(args);
	}
	
	return PLUGIN_NOT_LOADED;
}

/**
  \brief Returns a pointer on custom plugin info
		  
  This function acts in a similar way than *do_process*, it returns a pointer on
  void, which is actually a memory area which you need to cast back to a type you
  can use. For exemple you can imagine that this function, in your plugin will
  return a pointer on a custom PluginInfo class which contains information such as
  the author of the plugin, its version, dependecies and so on.
  
  The cast to a pointer on an object can be done simply with the macro **VOID_TO_OBJ**.
 
  If thre plugin is not loaded properly, this will return NULL
  
  \see VOID_TO_OBJ
*/
void* Plugin::plugin_info()
{
  if(loaded)
	  return plugininfo();
	else
	  return NULL;
}

/**
  Returns true if the plugin has successfully been loaded. If one of the functions
  described at the begining of this doc is missing, for exemple, the plugin won't
  be loaded and calls to its methods will fail.
*/
bool Plugin::is_loaded()
{
	return loaded;
}

string Plugin::get_file()
{
	return file;
}
