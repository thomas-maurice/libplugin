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

#include <plugin.h>

using namespace std;

Plugin::Plugin(string lib)
{
	file = lib;
	on_load = NULL;
	on_unload = NULL;
	plugininfo = NULL;
	process = NULL;
	loaded = false;
	libHandle = NULL;
}

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

int Plugin::reload()
{
	unload();
	return load();
}

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

int Plugin::do_process(void *args)
{
	if(loaded)
	{
		return process(args);
	}
	
	return PLUGIN_NOT_LOADED;
}

void* Plugin::plugin_info()
{
	return plugininfo();
}

bool Plugin::is_loaded()
{
	return loaded;
}

string Plugin::get_file()
{
	return file;
}
