/* OS specific includes and definitions for Linux
	Revision:
		Christopher Békési - initial build
*/

#include <dlfcn.h>
#include <sys/stat.h>
//Forward declarations
class HString;

typedef unsigned char		BYTE;
typedef unsigned int		UNIT;
typedef float				FLOAT;
typedef HString			STRING;
typedef int					INT;
typedef __int64				INT64;
typedef unsigned __int64	UNIT64;


#pragma warning(disable: 4996)
#pragma warning(disable: 4521)
#pragma warning(disable: 4244)
#pragma warning(disable: 4715)
#define _CRT_SECURE_NO_WARNINGS

#define DLLEXPORT __attribute__ ((visibility("default")))
#define DLLPROTECT   __attribute__ ((visibility("hidden")))
#define LIBEXT	L".so"
#define ldlib(filename) dlopen(filename,RTLD_LAZY)
#define ldfunc(handle,funcname) dlsym(handle,funcname)
#define DIRJOIN L'/'
#define DIRJOINSTR L"/"
#define ODIRJOIN L'\\'