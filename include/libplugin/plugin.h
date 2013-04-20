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

/*!
  \file plugin.h
  \brief Header to include to use the plugin class
  \author Thomas Maurice <tmaurice59@gmail.com>
  \version 0.1
*/

#include <iostream>
#include <string>
#include <sstream>

#include "macros.h"

#if defined (WIN32)
	#include <windows.h>
#elif defined (linux)
	#include <dlfcn.h>
#endif

/**
  \class Plugin
  \brief The main class of the library, the plugin handling one.
  
  This class is intended to provide an interface between a dynamic plugin and
  your main application. Plugins file (.dll or .so depending of your OS) are
  loaded through this class. Plugin file shall provide the following methods :
  - ```int on_load()```
  - ```int on_process(void* args)```
  - ```int on_unload()```
  - ```void* plugininfo()```
  
  If those methods are not provided, the loading of the plugin shall fail.
*/
class Plugin
{
	public:
		Plugin(std::string lib); //!< Overloaded constructor
		Plugin(); //!< The most basic constructor

		void set_file(std::string p_file); //!< Changes the filename of the plugin file
		int load(); //!< Loads the plugin
		int unload(); //!< Unloads the plugin
		int reload(); //!< Reloads the plugin
		int do_process(void* args); //!< Processes the main function of the plugin
		void* plugin_info(); //!< Returns a pointer on custom plugin info
		std::string get_file(); //!< Returns the file name of the plugin
		bool is_loaded(); //!< Returns true if the plugin is successfully loaded
		
	private:
		std::string file; //!< The filename of the plugin
		bool loaded; //!< The loading state of the plugin
		
		// Handler pour la lib
		#if defined (WIN32)
			HINSTANCE libHandle; //!< The windows lib handle
		#elif defined (linux)
			void *libHandle; //!< The linux lib handle
		#endif
		// Trois pointeurs sur les fonctions
		int (*on_load)();
		int (*process)(void* args);
		int (*on_unload)();
		void* (*plugininfo)();
};
