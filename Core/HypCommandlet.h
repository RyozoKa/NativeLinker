/*
	A commandlet baseclass
	Revision:
		Christopher B�k�si
*/

//Includes
#include "HypVM.h"

class HypCommandlet : public HypObject
{
	int DLLEXPORT main(int numargs, wchar_t* argv[]);
};