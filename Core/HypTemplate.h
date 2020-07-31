/*
	Various templates and structures.
	Revision:
		Christoher Békési - initial build
*/
#pragma once
#include "HypWin32.h"
#include <iostream>


//Forward declarations
class HObject;
class HClass;
//---------------------------------------------------
//Hashed identifiers.
typedef UINT64 HName;

//Object node
typedef BYTE ONode;

//Hashing names
DLLEXPORT HName hashName(const char *s);
DLLEXPORT HName hashName(const wchar_t *s);

//Uppercase to lowercase
DLLEXPORT void	toLower(wchar_t*);
DLLEXPORT void	toLower(char*);

//Check and throw GPF if string is not alpha
DLLEXPORT void	checkAlpha(const wchar_t*);
DLLEXPORT void	checkAlpha(const char*);

DLLEXPORT bool	isAlpha(const wchar_t*);
DLLEXPORT bool	isAlpha(const char*);

//--------------------------------------------------
//A script function

struct Function{
		HName			Id;								//HName based identification					0x00
		BYTE*			Code;							//A bytearray of instructions					0x08
		STRING*			name;							//Textual name of the function (For debugging)	0x10
		USHORT META[4];									//0=NumLocals, 2=CodeSize						0x18 - 0x1A

		void Serialize(HypBuffer&);

		//Function loader
		Function(HypBuffer&,class HClass*);
		~Function();

		//Temporary
		Function(void) {}

		
};