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

#include <iostream>
#include <string>
#include <sstream>

#include "macros.h"

#if defined (WIN32)
	#include <windows.h>
#elif defined (linux)
	#include <dlfcn.h>
#endif

class Plugin
{
	public:
		Plugin(std::string lib);
		Plugin();
		void set_file(std::string p_file);
		int load();
		int unload();
		int reload();
		int do_process(void* args);
		void* plugin_info();
		std::string get_file();
		bool is_loaded();
		
	private:
		std::string file;
		bool loaded;
		
		// Handler pour la lib
		#if defined (WIN32)
			HINSTANCE libHandle;
		#elif defined (linux)
			void *libHandle;
		#endif
		// Trois pointeurs sur les fonctions
		int (*on_load)();
		int (*process)(void* args);
		int (*on_unload)();
		void* (*plugininfo)();
};
