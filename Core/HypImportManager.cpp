#include "Core.h"

dense_hash_map<HName,ImportPackage*> HypImportManager::Imports = dense_hash_map<HName,ImportPackage*>();

void HypImportManager::addImport(ImportPackage* in)
{
	guard
	for(auto it = HypImportManager::Imports.begin();it != HypImportManager::Imports.end();it++)
	{
		if(it->first == in->ID)	//Found matching entry
		{
			for(auto its = in->Files.begin();its!=in->Files.end();its++)	//Check duplicates
			{
				for(auto itz = it->second->Files.begin(); itz != it->second->Files.end();itz++)
				{
					if(*its == *itz)
						goto JL001;
				}
				//If it doesn't exist, add it
				Imports[in->ID]->Files.insert(*its);
JL001:
				continue;
			}
			//Cleanup
			delete in;
			return;
		}
	}
	Imports[in->ID] = in;
	unguard
}


//Load them and check for duplicates.
//Check import register
//This will be called on startup AND during dynamic linking which can occur at any moment in game.
void HypImportManager::loadImports()
{
	//Load the packages
	for(auto it = Imports.begin(); it != Imports.end();it++)
	{
		HypPackage* pkg = HypPackage::Open(it->second->PackageName.towchar(),FILE_READ | FILE_BIN);
		if(!pkg->file->isopen())
			pkg->Open();
		//Load package
		pkg->readHeader();
		pkg->readFileList();
		for(auto its = it->second->Files.begin(); its != it->second->Files.end();its++)
		{
			ContentEntry* file = pkg->loadSingleEntry(*its);
			//Load asset here.
			
		}
		pkg->close();	//Close the package after it's loaded
	}
}

