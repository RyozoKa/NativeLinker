/*	Native input/output controller interface

*/

#pragma once

#include "HypConsole.h"
#include "HypIO.h"

class HypOutput : public HypObject
{
public:
	HypFile dump;
	typedef void (*dispatch)(const wchar_t*);
	dispatch disp;
	DLLEXPORT HypOutput();

	DLLEXPORT void Log(const wchar_t*,...);
	DLLEXPORT void Warn(const wchar_t*,...);
	DLLEXPORT void Err(const wchar_t*,...);
	DLLEXPORT void inline SetStrmPtr(dispatch ds) {disp = ds;}
private:
	static void _stdout(const wchar_t*);
	wchar_t buffer[128];
};