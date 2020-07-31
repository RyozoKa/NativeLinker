

#include "Core.h"

DLLEXPORT const STRING* GSCP[1024];
UINT	Index = 0;
DLLEXPORT const STRING* RegisterString(const STRING* S)
{
	guard
		for(int i=0;i<Index;i++)
		{
			if(*GSCP[i] == *S)	
			{
				if(GSCP[i] != S)
					delete S;	//Different address, same data.
				return GSCP[i];
			}
		}
	GSCP[Index] = S;
	return GSCP[Index++];
	unguardf(((STRING*)S)->towchar())
}


void init()
{
	guard
		//Global static initialization procedures
		memset(Sizes,1,0x0200);
		Sizes[AT_INT64] = 8;
		Sizes[AT_INT]	= 4;
		Sizes[AT_SHORT] = 2;
		//Sizes[AT_BYTE]	= 1;
		//Sizes[AT_BOOL]	= 1;
		Sizes[AT_DOUBLE]= 8;
		Sizes[AT_STRING]= 8;
		Sizes[AT_CLASS]	= 8;
		Sizes[AT_OBJECT]= 8;
	unguard
}
//TODO: Need exit procedures

//Global variables
DLLEXPORT HypOutput*			GLog;
DLLEXPORT HypConsole*			GConsole;
DLLEXPORT bool					GbLogToFile;
DLLEXPORT UINT64				GMiliSeconds;
DLLEXPORT UINT64				GSeconds;
DLLEXPORT UINT64				GMinutes;
DLLEXPORT UINT64				GHours;
DLLEXPORT UINT64				GDay;
DLLEXPORT UINT64				GDayOfWeek;
DLLEXPORT UINT64				GMonth;
DLLEXPORT UINT64				GYear;

//Find heap corruption

void* db_malloc(size_t s)
{
	BYTE* ptr = (BYTE*)malloc(s + 6);
	//Create a minimal header and footer to identify heap corruption
	//Header
	USHORT* sz = (USHORT*)ptr;
	*sz = s;
	//Footer
	UINT* ptrs = (UINT*)(ptr + s + 2);
	*ptrs = 0xFDFDFDFD;

	return (void*)(ptr + 2);
}
void* db_realloc(void* tptr,size_t s)
{
	if(!tptr)
		return db_malloc(s);
	USHORT* p = (USHORT*)tptr;
	p--;
	BYTE* ptr = (BYTE*)realloc(p,s + 6);
	//Create a minimal header and footer to identify heap corruption
	//Header
	USHORT* sz = (USHORT*)ptr;
	*sz = s;
	//Footer
	UINT* ptrs = (UINT*)(ptr + s + 2);
	*ptrs = 0xFDFDFDFD;

	return (void*)(ptr + 2);
}
void db_free(void* ob)
{
	if(!ob) return;
	USHORT *sz = (USHORT*)ob;
	--sz;
	//Check if we overstepped the heap!
	BYTE *i = (BYTE*)(ob);
	i += *sz;
	assert(*(UINT*)i == 0xFDFDFDFD);
	free(sz);
}

DLLEXPORT const STRING &EMPTYSTR = *new STRING(L"");	//An empty string.