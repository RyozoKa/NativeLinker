/*
*	Package dependency manager, responsible for loading dependencies
*	And building dependency tree.
*	2017-07-13
*	Revision:
*		Christopher Békési
*/

//1 Package list will be loaded
//2 dependencies will be added during package load, and checked against duplicates.
//	In case a duplicate is found, its object tables will be merged
//3 First step of loading imports is to discard all the packages that has already been loaded.

//Import structure
struct ImportPackage
{
	HName  ID;
	STRING PackageName;
	dense_hash_set<HName> Files;
};

class HypImportManager
{
public:
	static void addImport(ImportPackage*);		//-- Add a new import to the table.
	static void loadImports();					//-- Recursively import dependencies.
	static void cleanupImports();				//-- Cleanup imports?

private:
	//Import tree
	static dense_hash_map<HName,ImportPackage*> Imports;
};