/*
	Hyper code compiler
		Christopher Békési - 2017-8-20

	Input the name of the project that is to be compiled.
	The project needs to have a corresponding makefile in its directory.

	Project/
			Make
			Source/
			Native/
					Source/
					Headers/
					Lib/	<-- Output lib file for static linking

	Makefile:
	Import=Core
	Import=Engine
	Build=Debug	;Release
	bSanityCheck=false
*/

DLLEXPORT bool Compile(const STRING*);