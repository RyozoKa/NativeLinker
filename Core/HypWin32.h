/* OS specific includes and definitions
	Revision:
		Christopher Békési - initial build
*/

//Forward declarations
#pragma once
class HString;

typedef unsigned char		BYTE;
typedef unsigned int		UNIT;
typedef float				FLOAT;
typedef HString			STRING;
typedef int					INT;
typedef __int64				INT64;
typedef unsigned __int64	UNIT64;


#include <Windows.h>
#include <ctime>

#pragma warning(disable: 4996)
#pragma warning(disable: 4521)
#pragma warning(disable: 4244)
#pragma warning(disable: 4715)
#pragma warning(disable: 4267)
#pragma warning(disable: 4995)
#define _CRT_SECURE_NO_WARNINGS

//Library loading

#define ldlib(filename) LoadLibrary(filename)
#define ldfunc(handle,funcname) GetProcAddress((HMODULE)handle,funcname)
#define LIBEXT	L".dll"
#define DLLPROTECT //Nothing on windows
#define DIRJOIN L'\\'
#define DIRJOINSTR L"\\"
#define ODIRJOIN L'/'
//OS specific GUI management
void ErrorExit(LPTSTR lpszFunction);

//OS specific time management
void UpdateTime();