/*
	General file input/output help library
	Revision:
		Christopher Békési - initial build 2015-06-22
*/
#pragma once
#include <fstream>
#include <time.h>
#include <stdio.h>
#include  <io.h>
#include  <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <assert.h>
#include "Windows\dirent.h"

#pragma warning(disable:4251)

//File ops
enum{
FILE_READ = 0x01,
FILE_WRITE= 0x02,
FILE_BIN  = 0x20,
FILE_APP  = 0x08,
FILE_MOD  = FILE_READ | FILE_WRITE
};

//Forward declarations


//Constants
#define MAX_IMPORT 128
#define OBJECT_POS 33

//================================================================================================================================
//Basic file management

class DLLEXPORT HypFileManager : public HypObject
{
public:

	static bool renamefile(wchar_t* from, wchar_t* to) { return rename(HString(from).tochar(),HString(to).tochar())==0?true:false; }
	static bool removefile(wchar_t* file) { return remove(HString(file).tochar())==0?true:false; }
};

//==================================================================================================================================
//Relative paths must be relative to the root folder.
//Anything else will be considered invalid.
//Any other paths must be absolute and contain a path from the drive.
//Since linux doesn't have driver letters, I am forced to set up some basic rules to distinguish relative vs absolute paths
//So any path that starts with a slash is relative. Any absolute path may not begin with a slash!
//Calls to GoToRelative or hasSubFolder must not but may contain separators.
//Call to GoToAbsolute may however contain separators
//Test linux input..

class DLLEXPORT HypPath : public HypFileManager
{
public:
 HypPath(HString&);
 HypPath(const wchar_t*);
 HypPath(){ if(!Root) getRootDirectory(); else path = Root.path; }
 HypPath(HypPath& p)
 {
	 path = p.path;
 }
 HypPath				getParent();
 HypPath				getFirst();
 STRING				getDrive();
 bool					isValid();
 bool					exists();
 bool					hasSubFolder(STRING);
 void					GoToAbsolute(const wchar_t*);	//User takes full responsibility of input
 void					GoToRelative(const wchar_t*);
 static void			fixSeparator(STRING&);
 void					create();
 std::vector<HString> getFiles(HString filter=L"");
 operator bool()	{	return !path.isEmpty(); }

 static	HypPath&		getRootDirectory();

	HString path;
	static HypPath Root;
};

//=================================================================================================================================================
//A single file representation

class HypFile : public HypFileManager
{
public:
//DEFAULT_NAME
DLLEXPORT	HypFile() {}	//Placeholder
DLLEXPORT	HypFile(const wchar_t* filename,BYTE flags=0x02);
DLLEXPORT	HypFile(HString* filename,BYTE flags=0x02); //Binary is only used if first bool is set to true;
DLLEXPORT	~HypFile() { stream.close(); }
DLLEXPORT	HString	getLastError() { return lasterror; }
DLLEXPORT	bool		Open(const wchar_t* filename, BYTE flags=0x02);
DLLEXPORT	bool		setrwable(bool binary); /*Returns true if sucessfully opened file for writing and reading*/
DLLEXPORT	bool		isopen() { return stream.is_open(); }
DLLEXPORT	bool		exists();
DLLEXPORT	void		create();
DLLEXPORT	void		close() { stream.close(); }
DLLEXPORT	long		getsize() { struct stat stat_buf;long rc=stat((filepath + filename).tochar(),&stat_buf);return rc == 0 ? stat_buf.st_size : -1; }
DLLEXPORT	void		writeByte(BYTE byte);//Only single byte
DLLEXPORT	void		writeInt(INT wr);
DLLEXPORT	void		writeUInt(UINT wr);
DLLEXPORT	void		writeShort(SHORT wr);
DLLEXPORT	void		writeUShort(USHORT wr);
DLLEXPORT	void		writeInt64(INT64 wr);
DLLEXPORT	void		writeUInt64(UINT64 wr);
DLLEXPORT	void		writeDouble(DOUBLE write);
DLLEXPORT	void		writeLong(long wr);
DLLEXPORT	void		writeFloat(FLOAT wr);
DLLEXPORT	void		writeHName(HName& wr);
DLLEXPORT	void		writeString(HString& wr);
DLLEXPORT	void		writeLine(HString *str);//Only for nonbinary
DLLEXPORT	void		writeLine(const wchar_t *str);
DLLEXPORT	HString	readLine();//Only for nonbinary
DLLEXPORT	wchar_t*	readRawLine();
DLLEXPORT	BYTE		readByte();
DLLEXPORT	UINT		readUInt();
DLLEXPORT	INT			readInt();
DLLEXPORT	UINT64		readUInt64();
DLLEXPORT	INT64		readInt64();
DLLEXPORT	DOUBLE		readDouble();
DLLEXPORT	long		readLong();
DLLEXPORT	FLOAT		readFloat();
DLLEXPORT	SHORT		readShort();
DLLEXPORT	USHORT		readUShort();
DLLEXPORT	HString*	readString();
DLLEXPORT	bool		writeable();
DLLEXPORT	bool		readable();
DLLEXPORT	void		setPos(long tpos) {pos = tpos; stream.seekg(pos);}
DLLEXPORT	long		getPos() {return stream.tellg();}
DLLEXPORT	bool		Eof() {return stream.eof();}
DLLEXPORT	bool		Eol() { return eol; }
DLLEXPORT	int		Bad()	{return stream.badbit; }
DLLEXPORT	HString	getFilename() { return filename;}
DLLEXPORT	HString	getFilepath() { return filepath; }
public:	//TODO: Set to private
	HString filename;
	HString filepath;
	std::fstream stream;
	HString lasterror;
	long pos;
	bool eol;
	void split(HString* filename);
	bool read;
	bool write;
	bool append;
	bool binary;
	wchar_t buf[128];

	friend class HypPackage;
};

//==================================================================================================================================================
//A Package representation, which can basically contain any type of data that is to be presented in the game.
/*
*	Data model
*	Package ID(4b)| GUID(16b) | NumObjects(2b) | Content Table |
*	
*	Content Table
*	ID(8b) | FileName(STRING) | NumImport(2b) | Flags(1b) | Object Type(2b) | Import Table | Size(4b) | Position(8b) | Data
*	
*	Import Table
*	ID(8b) | NumFiles(2b) | ID Table(NumFiles*2b)
*
*	Class symbol table: (For static linking)
*	Class index: 2b USHORT
*			  Super class ID 8b
*			  NumVars 2b USHORT
*			  NumFunctions 2b USHORT
*			Variables:Type hash | ID | USHORT offset | Static? 1b BYTE flag 0x1
*			Functions:Return type has | ID | USHORT index | Static? 1b BYTE flag 0x1
*					NumParameters 1b BYTE
*				Parameters: Type hash | ID
*
*
*
*/

//Meta info holders.
struct _HType
{
	HName TypeID;
	UINT	Type;
};
struct _HVar
{
	HName ID;
	_HType Type;
	USHORT Offset;
	//Compiler data
	UINT Line;
	STRING Name;
	STRING InitValue;	//Not yet implemented.
	struct _HClass* ClassBase;	//For class types.. Or objects
};
struct _HFunc
{
	HName ID;
	_HType Type;
	USHORT Index;
	UINT Line;
	std::vector<_HVar*> Parameters;
	//Compiler data
	STRING Name;
	std::vector<_HVar*> Locals;
	BYTE NumLocals;
	std::vector<struct _HExprUnit*> Expressions;
	_HFunc* SuperFunc;
};
struct _HClass
{
	HName ID;
	STRING Name;
	STRING SuperName;
	HName Super;
	USHORT NatFuncs;
	USHORT VFuncs;
	USHORT NumVars;
	USHORT GOffset;	//Global variables offset
	USHORT GSOffset; //Static globals offset
	USHORT GNOffset; //Native global offset
	USHORT GNSOffset;
	USHORT NumConst; //Number of constant strings in this pool
	USHORT Flags;		//C_Native, C_Abstract, C_Base, C_Public, C_Private, C_Protected
	HypPackage* PKG;	//Package this class belongs to
	std::vector<_HVar*> Vars;
	std::vector<_HFunc*> Funcs;
	std::vector<_HFunc*> repT;	//vftable virtual function override
	std::vector<const STRING*> Constants;
	dense_hash_map<HName,struct _PDef*> PreDefs;
	_HClass* SuperClass;
	_HClass() : ID(NULL), Super(NULL), NumVars(NULL), Flags(NULL), SuperClass(NULL), NumConst(NULL), PKG(NULL) {}
};
struct _HCompUnit
{
	STRING Name;
	
	//Compiler data
	std::vector<_HClass*> Classes;
	dense_hash_map<HName,struct _PDef*> PreDefs;
};
struct ContentEntry
	{
		HName ID;
		STRING FileName;
		USHORT NumImports;
		BYTE Flags;
		USHORT ContentType;
		ImportPackage* ImportTable;
		UINT Size;
		UINT64 Position;
		BYTE* Data;
		ContentEntry() : NumImports(NULL), Flags(NULL) {}
	};
class DLLEXPORT HypPackage : public HypFileManager
{
public:

	enum{
		HYPPACKAGE	= 0xABCDEF17,
		MUSIC		= 0x1,
		TEXTURE		= 0x2,
		MAP			= 0x4,
		CLASS		= 0x8,
		SOUND		= 0x10,
		FILE		= 0x20,	//Only allowed in general purpose.
	};

	enum{
		L_HEAD		= 0x01,
		L_LIST		= 0x02,
		L_LOADED	= 0x04,
		L_ALLDATA	= 0x08
	};
 void generateGUID();
 bool readHeader();									//-- Must always be called before reading any other data. Returns true if valid HypPackage.
 bool readFileList();								//-- Create a list of fileheaders. Stored in Files;
 inline ContentEntry*	loadSingleEntry(HName ID);	//-- Read a single file into the table, useful for dynamic load or imports
 bool readAllEntries(bool bToLoaders=false);		//-- Read all files in this package.
 bool flush();										//-- Save package
 void close() { file->close(); }
 __forceinline ContentEntry*	getEntry(HName ID);

 USHORT					addConstant(STRING*);
		void			Open();
 inline ContentEntry*	NewContent(HName ID);
 static HypPackage*		Open(const wchar_t*,BYTE);
 static HypPackage*		Find(HName);					//-- Find a package that has already been loaded
 static void			UnloadPackages();


//private:
	HypFile *file;

	//Header data
	UINT64 GUID[2];
	USHORT NumObjects;
	HName ID;

	//File data
	dense_hash_map<HName,ContentEntry*> Files;

	//String table. This must be a linear ordered list and can therefore not be a set of hash bins
	std::vector<STRING*>	STP;

	dense_hash_set<_HClass*> CST;	//Class symbol table (for static linking)

	//Internal checks
	BYTE Flags;

	//Native package if any.
	void* dlib;

	//Constructor(s)/Destructor(s)
	HypPackage(const wchar_t* filename, BYTE flags) : NumObjects(NULL), Flags(NULL), dlib(NULL)
	{
	guard
		file = new HypFile(filename,flags | FILE_BIN);
		if(flags & FILE_READ && !file->exists())
			throw new STRING(STRING(L"Attempted to read non-existing file: ").append(filename));
	unguard
	}
	~HypPackage()
	{
		file->close();
		delete file;
		//Clean up files and headers
		for(auto it=Files.begin();it!=Files.end();it++)
		{
			if(it->second->Data)
				delete it->second->Data;
			delete it->second;
			//Import table should be taken care of by the import manager.
			
		}
		for(auto it = CST.begin(); it != CST.end();it++)
		{
			for(auto ita = (*it)->Funcs.begin(); ita != (*it)->Funcs.end();ita++)
			{
				for(auto itb = (*ita)->Parameters.begin(); itb != (*ita)->Parameters.end();itb++)
					delete *itb;
				delete *ita;
			}
			delete *it;
		}
	}

	static dense_hash_map<HName,HypPackage*> Packages;

	friend HypImportManager;
	friend HClass;
};

class DLLEXPORT HypBuffer : public HypObject
{
public:
	BYTE *bytes;
	UINT len;
	UINT pos;
	bool bReadOnly;
	HypBuffer(BYTE* d,UINT Size,bool rdOnly=0) : len(Size), pos(NULL),bytes(d), bReadOnly(rdOnly) {}
	HypBuffer(bool rdOnly=0) : len(NULL), pos(NULL), bReadOnly(rdOnly),bytes(NULL) {}
	HypBuffer(UINT size, bool rdOnly=0) : len(size), pos(NULL), bReadOnly(rdOnly) {bytes = (BYTE*)malloc(size);}
	/*Buffer should create new space for the data, and buffer should be allocated on the heap and not on the stack*/
	/*Not foolproof. */
	void clear() { free(bytes); }
	UINT	getSize() {return len;}
 	void	putByte(BYTE byte);
	void	putInt(INT num);
	void	putUInt(UINT num);
	void	putShort(SHORT num);
	void	putUShort(USHORT num);
	void	putFloat(float fl);
	void	putDouble(double db);
	void	putString(HString& str);
	void	putInt64(INT64 longs);
	void	putUInt64(UINT64 longs);
 	BYTE	getByte();
	INT		getInt();
	UINT	getUInt();
	SHORT	getShort();
	USHORT	getUShort();
	FLOAT	getFloat();
	double	getDouble();
	HString* getString();
	INT64	getInt64();
	UINT64	getUInt64();
	bool	eof();
	void	flush() { free(bytes); pos=0;len=0; }


};


/*
THE END
*/