/*	Abstract console class interface.
	Windows and Linux support
	Revision:
		Christopher Békési - Initial build
*/

#pragma once

//Includes
#include "HString.h"


class HypConsole : public HypObject
{
public:
	bool bIsVisible;

	//Commandline callback feed
	typedef void (*callback)(const char*);
	static callback SendCommand;

	//Main Console interfaces
	DLLEXPORT virtual void Initialize() {}
	DLLEXPORT virtual void Hide() {}
	DLLEXPORT virtual void Show() {}
	DLLEXPORT bool inline Visible()		{ return bIsVisible; }
	DLLEXPORT void inline SetCallback(callback func)	{ SendCommand = func; } 
};