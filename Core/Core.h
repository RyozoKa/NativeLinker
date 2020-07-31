/*	This file is ment to keep everything necessary for exporting the dll further

*/
#pragma once

//Console management
#define HIDE_CONSOLE if(GConsole) GConsole->Hide();
#define SHOW_CONSOLE if(GConsole) GConsole->Show();

//Function timing
#define START_CLOCK clock_t time = clock();
#define STOP_CLOCK time = clock() - time; GLog->Log (L"It took me %d clicks (%f seconds).\n",time,((float)time)/CLOCKS_PER_SEC);

//Forward declarations
class HypObject;
		class HObject;
			class HString;
		class HClass;
		class HypBuffer;
		class HypFileManager;
			class HypFile;
			class HypPath;
			class HypBuffer;
			class HypBuffer;
			class HypPackage;
		class HypOutput;
		class HypConsole;
			class HypWinConsole;
		class HypImportManager;

//Includes
#include <emmintrin.h>
#include "densehash\dense_hash_map.h"
#include "densehash\dense_hash_set.h"
using namespace google;
#include <math.h>
#include "HypMath.h"
#include "HypObject.h"
#include "HypTemplate.h"
#include "HypScript.h"
#include "HypString.h"
#include "HypVM.h"
#include "HypImportManager.h"
#include "HypIO.h"
#include "HypOutput.h"
#include "HypWindowsConsole.h"

//Global string pool table
DLLEXPORT extern const STRING* GSCP[1024];
DLLEXPORT extern const STRING* RegisterString(const STRING*);

//Global variables
DLLEXPORT extern HypOutput*				GLog;
DLLEXPORT extern HypConsole*			GConsole;
DLLEXPORT extern bool					GbLogToFile;

//Global time
DLLEXPORT extern UINT64					GMiliSeconds;
DLLEXPORT extern UINT64					GSeconds;
DLLEXPORT extern UINT64					GMinutes;
DLLEXPORT extern UINT64					GHours;
DLLEXPORT extern UINT64					GDay;
DLLEXPORT extern UINT64					GDayOfWeek;
DLLEXPORT extern UINT64					GMonth;
DLLEXPORT extern UINT64					GYear;

DLLEXPORT extern const STRING &EMPTYSTR;


DLLEXPORT void* db_malloc(size_t);
DLLEXPORT void* db_realloc(void*,size_t);
DLLEXPORT void  db_free(void*);


DLLEXPORT void init();

/*
	Still TODO:
	Correct const arguments
	Go through and finalize optimizations

*/