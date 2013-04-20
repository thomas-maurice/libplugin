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

#ifndef MACROS_HEADER
#define MACROS_HEADER

	#if defined (linux)
		#define DllExport //!< DllExport macro you should use to define your plugin functions.
	#elif defined (WIN32)
		#define DllExport __declspec(dllexport) //!< DllExport macro you should use to define your plugin functions.
	#endif
	
	/**
	  This macro is used to cast a pointer onto an object 'obj' to a pointer to
	  void 'out' (of course you can change the names)
	*/
	#define OBJ_TO_VOID(obj, out) void *out = (void*)obj;
	
	/**
	  This macro is used to cast a pointer onto void 'vd' to a pointer onto an object
	  'out' with the type 'type'. So if I call ```VOID_TO_OBJ(arg, char, str)``` then
	  it will be the same as ```char* str = (char*) arg``` and it will cast arg to a string.
	*/
	#define VOID_TO_OBJ(vd, type, out) type *out = (type*) vd;
	
	/** A simple define value */
	#define PLUGIN_NOT_LOADED -1
#endif
