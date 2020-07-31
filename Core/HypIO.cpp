
#include "Core.h"

//HypPath
HypPath HypPath::getParent()
{
	guard
		return HypPath(path.substr(0,path.findexact(L"/\\")));
	unguard
}
HypPath HypPath::getFirst()
{
	guard
		if(path.findexact(L"/\\") != -1)
			return HypPath(path.substr(path.findexact(L"/\\")));
		return *this;
	unguard
}
STRING	HypPath::getDrive()	//Linux?
{
	guard
		return path.substr(0,path.findexact(L"/\\"));
	unguard
}
bool	HypPath::isValid()
{
	guard
		struct stat info;
		if( stat( path.tochar(), &info ) != 0 )
			return false;	//Invalid
		else if( info.st_mode & S_IFDIR ) 
			return true;
		return false;	//Not a dir
	unguard
}
bool	HypPath::exists()
{
	return isValid();
}
bool	HypPath::hasSubFolder(STRING s)
{
	STRING ST;
	if(s[0] != DIRJOIN)
	{
		ST = path;
		ST += DIRJOINSTR;
		ST += s;
	}
	else ST = path + s;
	struct stat info;
	if( stat(ST.tochar(), &info ) != 0 )
		return false;	//Invalid
	else if( info.st_mode & S_IFDIR ) 
			return true;
	return false;
}
void HypPath::fixSeparator(STRING& S)
{
	guard
		for(int i=0;i<S.length();i++)
			if(S[i] == ODIRJOIN)
				S[i] = DIRJOIN;
	unguard
}
HypPath::HypPath(HString& p)
{
guard
	//See what we have..
	//Relative or absolute?
	if(p[0] == DIRJOIN || p[0] == L'.')
	{
		path = Root.path;//Relative
		path += p;
	}
	else path = p;
	if(path[path.length()-1] == DIRJOIN)
		path[path.length()-1] = L'\0';
	//Filename?
	else if(path.findlast(L".") > path.findlast(L"/\\"))
		path = path.substr(0,path.findlast(L"/\\"));
	
unguard	
}
HypPath::HypPath(const wchar_t* p)
{
	guard
	if(!Root)
		getRootDirectory();
	//See what we have..
	if(!p || !*p)
		path = Root.path;
	//Relative or absolute?
	else if(p[0] == DIRJOIN || p[0] == L'.' || p[0] == ODIRJOIN)
	{
		path = Root.path;//Relative
		if(wcslen(p) > 1)
			path += (wchar_t*)p;
	}
	else path = p;
	if(path[path.length()-1] == DIRJOIN)
	{
		path[path.length()-1] = L'\0';
		path.nlength--;
	}
	//Filename?
	else if(path.findlast(L".") > path.findlast(L"/\\"))
		path = path.substr(0,path.findlast(L"/\\"));
	if(p[0] == ODIRJOIN)
		fixSeparator(path);
unguard	
}
std::vector<HString> HypPath::getFiles(HString f)
{
guard
	if(!path.isEmpty())
	{
		std::vector<HString> files;
		WDIR *dir;
		wdirent *ent;
		if ((dir = wopendir (path.towchar())) != NULL) 
		{
			while ((ent = wreaddir (dir)) != NULL)
			{
				HString str(ent->d_name);
				if(str == L"." || str == L"..") continue;//Hidden files
				if(f.isEmpty())
					files.push_back(str);//No filter
				else if(str.substr(str.length() - f.length(),f.length()) == f)//We have a filter
					files.push_back(str);
			}
		}
		wclosedir(dir);
		return files;
	}
unguard
}
HypPath HypPath::Root;
HypPath& HypPath::getRootDirectory()
{
	guard
		wchar_t* d = STRING::getWchr();
#ifdef WIN32
		GetModuleFileNameW( NULL, d, MAX_PATH );
#else	//LINUX.. TODO:TEST!
		
		wcs_readlink( L"/proc/self/exe", d, PATH_MAX );
#endif
		if(!Root)
		{
			Root = STRING(d);
			Root.path = Root.path.substr(0,Root.path.findlast(L"/\\"));
		}
		return Root;
	unguard
}
void HypPath::GoToAbsolute(const wchar_t* p)
{
	guard
		path = p;
	unguard
}
void HypPath::GoToRelative(const wchar_t* p)
{
	guard
		if(p[0] == DIRJOIN)
			path += (wchar_t*)p;
		else if(p[0] == ODIRJOIN)
		{
			path += (wchar_t*)p;
			fixSeparator(path);
		}
		else
		{
			path += DIRJOINSTR;
			path += (wchar_t*)p;
		}
	unguard
}
void HypPath::create()
{
	system(STRING(L"mkdir -p ").append(path).tochar());	//Crossplatform eyy
}

dense_hash_map<HName,HypPackage*> HypPackage::Packages = dense_hash_map<HName,HypPackage*>();

HypFile::HypFile(const wchar_t* filename,BYTE flags) : pos(NULL)
{
guard
	read	=		!!(flags & FILE_READ	);
	write	=		!!(flags & FILE_WRITE	);
	append	=		!!(flags & FILE_APP		);
	binary	=		!!(flags & FILE_BIN		);
	eol = false;
	for(int i=wcslen(filename) - 1;i>=0;i--)
	{
		if(filename[i] == L'/' || filename[i] == L'\\')
		{
			this->filename.append(&filename[i+1]);
			if(i > 0)
				filepath.append(filename,i+1);
			break;
		}
		else if(i == 0)
			this->filename.append(filename);
	}
	if(write)
		create();
	stream.open((filepath + this->filename).towchar(), flags);
	stream.imbue(std::locale::classic());
unguardf(filename)
}
HypFile::HypFile(HString* fname,BYTE flags) : pos(NULL)
{
guard
	read	=		!!(flags & FILE_READ	);
	write	=		!!(flags & FILE_WRITE	);
	append	=		!!(flags & FILE_APP		);
	binary	=		!!(flags & FILE_BIN		);
	eol = false;
	split(fname);
	if(write)
		create();
	stream.open((filepath + filename).towchar(), flags);
	stream.imbue(std::locale::classic());
unguardf(*fname)
}
bool HypFile::Open(const wchar_t* filename,BYTE flags)
{
	guard
	read	=		!!(flags & FILE_READ	);
	write	=		!!(flags & FILE_WRITE	);
	append	=		!!(flags & FILE_APP		);
	binary	=		!!(flags & FILE_BIN		);
	eol = false;
	if(stream.is_open())
		stream.close();
	stream.clear();
	this->filename.clear();
	this->filepath.clear();
	for(int i=wcslen(filename) - 1;i>=0;i--)
	{
		if(filename[i] == L'/' || filename[i] == L'\\')
		{
			this->filename.append(&filename[i+1]);
			if(i > 0)
				filepath.append(filename,i+1);
			break;
		}
		else if(i == 0)
			this->filename.append(filename);
	}
	if(write)
		create();
	stream.open((filepath + this->filename).towchar(), flags);
	stream.imbue(std::locale::classic());
	return isopen();
	unguardf(filename)
}
bool HypFile::setrwable(bool binary)
{
	stream.open((filepath + filename).towchar(),FILE_WRITE | binary? FILE_READ | FILE_BIN: FILE_READ);
	return stream.is_open();
}
bool HypFile::exists()
{
guard
	  struct stat buffer;   
	  return (stat ((filepath+filename).tochar(), &buffer) == 0);
unguard
}
void HypFile::create()
{
guard
	if(exists()) return;
	stream.open((filepath + filename).towchar(),std::ios_base::out);
	stream.close();
unguard
}
void HypFile::split(HString* file)
{
guard
	if(file->find(L"/\\") == -1)
	{
		filename = *file;
		filepath = L"";
	}
	else
	{
		filename = file->substr(file->findlast(L"/\\") + 1);
		filepath = file->substr(0,file->findlast(L"/\\") + 1);
	}
unguard
}
bool HypFile::writeable()
{
guard
	if(!exists())
		return false;
	if(!stream)
		return false;
	if(!stream.is_open())
		return false;
	return true;
unguard
}
void HypFile::writeByte(BYTE in)
{
guard
	stream.write((char*)&in,1);//Memory alignment of 2 bytes
unguard
}
void HypFile::writeInt(int in)
{
guard
	stream.write((char*)&in,sizeof(int));
unguard
}
void HypFile::writeUInt(UINT in)
{
guard
	stream.write((char*)&in,sizeof(in));
unguard
}
void HypFile::writeUInt64(UINT64 in)
{
guard
	stream.write((char*)&in,sizeof(in));
unguard
}
void HypFile::writeInt64(INT64 in)
{
guard
	stream.write((char*)&in,sizeof(in));
unguard
}
void HypFile::writeLong(long in)
{
guard
	stream.write((char*)&in,sizeof(in));
unguard
}
void HypFile::writeFloat(float in)
{
guard
	stream.write((char*)&in,sizeof(in));
unguard
}
void HypFile::writeShort(SHORT in)
{
guard
	stream.write((char*)&in,sizeof(in));
unguard
}
void HypFile::writeUShort(USHORT in)
{
guard
	stream.write((char*)&in,2);
unguard
}
void HypFile::writeHName(HName& in)
{
guard
	writeUInt64(in);
unguard
}
void HypFile::writeDouble(double in)	//Fix this
{
guard
	union {
		double float_variable;
		char temp_array[sizeof(in)];
	} u;
	u.float_variable = in;
	stream.write(u.temp_array,sizeof(in)/2);
unguard
}
void HypFile::writeString(HString& str)
{
guard
	//BYTE* bytes = str.tobytes();
	short len = str.length();
	writeUShort(len);
	const wchar_t* strm = str.towchar();
	stream.write((char*)strm,len * sizeof(wchar_t));
unguard
}
void HypFile::writeLine(HString *line)
{
//	assert(binary);
guard
	const wchar_t* ptr = line->towchar();
	char buffer[128];
	INT len = line->length();
	for(int i=0;i<len;i++)
		buffer[i] = (ptr[i] & 0xFF);
	buffer[len] = '\0';
	stream << buffer << '\n';
	stream.flush();
unguard
}
void HypFile::writeLine(const wchar_t *line)
{
guard
	char tbuffer[128];
	INT len = wcslen(line);
	for(int i=0;i<len;i++)
		tbuffer[i] = (line[i] & 0xFF);
	tbuffer[len] = '\0';
	stream << tbuffer << '\n';
	stream.flush();
unguard
}
HString HypFile::readLine()
{
guard
	HString str;
	char sbuf[128];
	eol = !stream.getline(sbuf,127,L'\n');
	str.append(sbuf);
	return str;
unguard
}
wchar_t* HypFile::readRawLine()
{
guard
	//Locale specific
	char tbuf[128];
	stream.getline(tbuf,127,L'\n');
	INT in = strlen(tbuf);
	for(int i=0;i<in;i++)
		buf[i] = (tbuf[i] & 0xFF);
	buf[in] = L'\0';
	return buf;
unguard
}
BYTE HypFile::readByte()
{
guard
	BYTE byte;
	stream.read((char*)&byte,1);
	return byte;
unguard
}
int HypFile::readInt()
{
guard
	int in;
	stream.read((char*)&in,sizeof(INT));
	return in;
unguard
}
UINT HypFile::readUInt()
{
guard
	UINT in;
	stream.read((char*)&in,sizeof(UINT));
	return in;
unguard
}
INT64 HypFile::readInt64()
{
guard
	INT64 in;
	stream.read((char*)&in,sizeof(INT64));
	return in;
unguard
}
UINT64 HypFile::readUInt64()
{
guard
	UINT64 in;
	stream.read((char*)&in,sizeof(UINT64));
	return in;
unguard
}
SHORT HypFile::readShort()
{
guard
	SHORT in;
	stream.read((char*)&in,sizeof(SHORT));
	return in;
unguard
}
USHORT HypFile::readUShort()
{
guard
	USHORT in;
	stream.read((char*)&in,sizeof(USHORT));
	return in;
unguard
}
float HypFile::readFloat()
{
guard
	union {
    float f;
    char bytes[sizeof(float)];
 } u;
 stream.read(u.bytes,sizeof(float));
 return u.f;
unguard
}
double HypFile::readDouble()
{
guard
	union {
		double f;
		char bytes[sizeof(double)];
	} u;
	stream.read(u.bytes,sizeof(double));
	return u.f;
 unguard
}
long HypFile::readLong()
{
guard
	union{
		long un;
		char bytes[sizeof(long)];
	}lo;
	stream.read(lo.bytes,sizeof(long));
	return lo.un;
unguard
}
HString* HypFile::readString()
{
guard 
	return new HString(this);
unguard
}




//===========================================================================================================================
//HypPackage

//Static open package
HypPackage* HypPackage::Open(const wchar_t* filen, BYTE flags)
{
	guard
	//Split the name and extension
	STRING S = filen;
	int start = S.findlast(DIRJOINSTR) + 1;
	if(start == -1)
		start = 0;
	int end = S.findlast(L".");
	if(end == -1)
		end = 0;
	else
		end -= start;
	HName ID = hashName(S.substr(start,end).towchar());
	auto it = HypPackage::Packages.find(ID);
	if(HypPackage::Packages.empty() || it == HypPackage::Packages.end() )
	{
		HypPackage* P = new HypPackage(filen,flags);
		P->ID = ID;
		HypPackage::Packages[ID] = P;
		return P;
	}
	if(!it->second->file->isopen())
		it->second->file->setrwable(true);
	return it->second;
	unguardf(filen)
}
inline HypPackage* HypPackage::Find(HName n)
{
	auto it = Packages.find(n);
	return it != Packages.end()?it->second:NULL;
}
void HypPackage::Open()
{
	if(file->isopen()) return;
	file->setrwable(true);
}
void HypPackage::generateGUID()
{
guard
	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
	HString str;
	str = (((now->tm_year + 1900) + now->tm_mon + 1 ) + now->tm_hour + now->tm_min + now->tm_sec) + sizeof(this);
	//TODO: Finish GUID 
unguard
}
bool HypPackage::readHeader()
{
	guard
		if(Flags & L_HEAD || !file->isopen()) 
			return false;
	Flags |= L_HEAD;
	//Check if this is a valid package
	UINT PID = file->readUInt();
	if(!(PID & HYPPACKAGE))
		return false;	//This is not a HypPackage.
	ID		= file->readUInt64();
	GUID[0] = file->readUInt64();
	GUID[1] = file->readUInt64();
	NumObjects = file->readUShort();
	return true;
	unguardf(file->filename.towchar())
}
bool HypPackage::readFileList()
{
	guard
	if(Flags & L_LIST || !(Flags & L_HEAD) || !file->isopen()) return false;
	Flags |= L_LIST;
	for(int i=0;i<NumObjects;i++)
	{
		ContentEntry& File = *new ContentEntry;
		File.ID = file->readUInt64();
		File.FileName.deserialize(file);
		File.NumImports = file->readUShort();
		File.Flags = file->readByte();
		File.ContentType = file->readUShort();
		File.ImportTable = new ImportPackage[File.NumImports];
		for(int j=0;j<File.NumImports;j++)
		{
			ImportPackage& Im = File.ImportTable[j];
			Im.ID = file->readUInt64();
			Im.PackageName.deserialize(file);
			USHORT NumFiles = file->readUShort();
			for(int k=0;k<NumFiles;k++)
				Im.Files.insert(file->readUInt64());
		}
		File.Size = file->readUInt();
		File.Position = file->getPos();	//Next bytes will be the file data itself. Caching for fast search.
		File.Data = NULL;
		//Skip the filedata here
		file->setPos(file->getPos() + File.Size);
		Files[File.ID] = &File;
	}

	USHORT size = file->readUShort();
	if(STP.size() == 0)
	for(int i=0;i<size;i++)
		STP.push_back(file->readString());
	else return false;
	size = file->readUShort();
	for(int i=0;i<size;i++)
	{
		_HClass* CS = new _HClass;
		CS->ID = file->readUInt64();
		CS->Super = file->readUInt64();
		CS->NumVars = file->readUShort();
		CS->VFuncs = file->readUShort();
		CS->Flags = file->readByte();
		for(int i=0;i<CS->NumVars;i++)
		{
			_HVar* V = new _HVar;
			V->ID = file->readUInt64();
			V->Type.TypeID = file->readUInt64();
			V->Offset = file->readUShort();
			V->Type.Type = file->readUInt();
			CS->Vars.push_back(V);
		}
		for(int i=0;i<CS->VFuncs;i++)
		{
			_HFunc* F = new _HFunc;
			F->ID = file->readUInt64();
			F->Type.TypeID = file->readUInt64();
			F->Type.Type = file->readUInt();
			F->Index = file->readUShort();
			BYTE NumPar = file->readByte();
			for(int j=0;j<NumPar;j++)
			{
				_HVar* V = new _HVar;
				V->ID = file->readUInt64();
				V->Type.TypeID = file->readUInt64();
				V->Offset = file->readUShort();
				V->Type.Type = file->readUInt();
				F->Parameters.push_back(V);
			}
			CS->Funcs.push_back(F);
		}
	}
	return true;
	unguardf(file->filename.towchar())
}
bool HypPackage::readAllEntries(bool bToLoaders)
{
	guard
	//Register this package in the list of loaded packages
	if(Flags & L_ALLDATA || !(Flags & L_LIST) || !file->isopen()) return false;
	Flags |= L_ALLDATA;
	for(auto it=Files.begin();it!=Files.end();it++)
	{
		file->setPos(it->second->Position);
		it->second->Data = new BYTE[it->second->Size];
		file->stream.read((char*)it->second->Data,it->second->Size);
		for(int j=0;j<it->second->NumImports;j++)
				HypImportManager::addImport(it->second->ImportTable + j);	//Move the Import Table to the manager, cleanup is hereby handled by the manager.
		it->second->Flags |= L_LOADED;
		if(bToLoaders)
		{
			//TODO: Implement loader redirect
		}
	}
	unguardf(file->filename.towchar())
}
inline ContentEntry* HypPackage::loadSingleEntry(HName N)
{
	guard
	if(!(Flags & L_LIST) || !file->isopen()) return false;
	assert(!Files.empty());	//Make sure file list is loaded.
	for(auto it = Files.begin(); it != Files.end();it++)
	{
		if(it->first== N)
		{
			if(it->second->Flags & L_LOADED && it->second->Data)
				return it->second;	
			//If file hasn't been loaded already, do so.
			it->second->Flags |= L_LOADED;
			file->setPos(it->second->Position);
			file->stream.read((char*)it->second->Data,it->second->Size);
			for(int j=0;j<it->second->NumImports;j++)
				HypImportManager::addImport(it->second->ImportTable + j);
		}
	}
	return NULL;
	unguardf(file->filename.towchar())
}
ContentEntry* HypPackage::getEntry(HName ID)
{
	auto it = Files.find(ID);
	return it==Files.end()?NULL:it->second;
}
bool HypPackage::flush()
{
	guard
	if(!file->isopen())
		return false;
	file->setPos(0);
	//Write header
	file->writeUInt(HYPPACKAGE);
	file->writeUInt64(ID);
	file->writeUInt64(GUID[0]);
	file->writeUInt64(GUID[1]);
	file->writeUShort(NumObjects);
	//Write files
	for(auto it=Files.begin();it!=Files.end();it++)
	{
		guard
		if(!it->second->Data)
			throw L"Error: NULL data for asset";	//Should never happen
		file->writeUInt64(it->second->ID);
		file->writeString(it->second->FileName);	//<<
		file->writeUShort(it->second->NumImports);
		file->writeByte(it->second->Flags);
		file->writeUShort(it->second->ContentType);
		for(USHORT j=0;j<it->second->NumImports;j++)
		{
			file->writeUInt64(it->second->ImportTable[j].ID);
			file->writeString(it->second->ImportTable[j].PackageName);
			file->writeUShort(it->second->ImportTable[j].Files.size());
			for(auto its = it->second->ImportTable[j].Files.begin();its != it->second->ImportTable[j].Files.end();its++)
				file->writeUInt64(*its);
		}
		file->writeUInt(it->second->Size);
		file->stream.write((char*)it->second->Data,it->second->Size);
		//Done?
		unguardf(it->second->FileName.towchar())
	}
	file->writeUShort(STP.size());
	for(auto it=STP.begin();it != STP.end();it++)
		file->writeString(**it);
	file->writeUShort(CST.size());
	for(auto it = CST.begin(); it != CST.end(); it++)
	{
		_HClass* CS = *it;
		file->writeUInt64(CS->ID);
		file->writeUInt64(CS->Super);
		file->writeUShort(CS->NumVars);
		file->writeUShort(CS->VFuncs);
		file->writeByte(CS->Flags);
		for(auto ita = CS->Vars.begin(); ita != CS->Vars.end(); ita++)
		{
			_HVar* V = *ita;
			file->writeUInt64(V->ID);
			file->writeUInt64(V->Type.TypeID);
			file->writeUShort(V->Offset);
			file->writeUInt(V->Type.Type); 
		}
		for(auto ita = CS->Funcs.begin(); ita != CS->Funcs.end(); ita++)
		{
			_HFunc* F = *ita;
			file->writeUInt64(F->ID);
			file->writeUInt64(F->Type.TypeID);
			file->writeUInt(F->Type.Type);
			file->writeUShort(F->Index);
			file->writeByte(F->Parameters.size());
			for(auto itb = F->Parameters.begin(); itb != F->Parameters.end(); itb++)
			{
				_HVar* V = *itb;
				file->writeUInt64(V->ID);
				file->writeUInt64(V->Type.TypeID);
				file->writeUShort(V->Offset);
				file->writeUInt(V->Type.Type); 
			}
		}

	}
	file->close();
	return true;
	unguardf(file->filename.towchar())
}
inline ContentEntry* HypPackage::NewContent(HName ID)
{
	auto it = Files.find(ID);
	if(it == Files.end())
	{
		Files[ID] = new ContentEntry;
		Files[ID]->Data = NULL;
		Files[ID]->ID = ID;
		NumObjects++;
		return Files[ID];
	}
	return it->second;
}
USHORT HypPackage::addConstant(STRING* S)
{
	guard
	//Check if string already exists
	for(int i=0;i<STP.size();i++)
		if(*STP[i] == *S)
			return i;
	//Else push the string
	STP.push_back(S);
	return STP.size() - 1;
	unguardf(S->towchar())
}

//Buffer
void HypBuffer::putByte(BYTE tbyte)
{
guard
	bytes = (BYTE*)realloc(bytes,len + 1);
	len += 1;
	bytes[len-1] = tbyte;
unguard
}
BYTE HypBuffer::getByte()
{
guard
	if(len < pos + 1) return 0;
	return bytes[pos++];
unguard
}
void HypBuffer::putInt(int num)
{
guard
	bytes = (BYTE*)realloc(bytes,len + 4);
	len += 4;
	*(int*)(bytes + len - 4) = num;
unguard
}
int HypBuffer::getInt()
{
guard
	if(len < pos + 4) return 0;
	pos += 4;
	return *(int*)&bytes[pos - 4];
unguard
}
UINT HypBuffer::getUInt()
{
	
	if(len < pos + 4) return 0;
	pos += 4;
	return *(UINT*)&bytes[pos - 4];
}
void HypBuffer::putUInt(UINT fl)
{
guard
	bytes = (BYTE*)realloc(bytes,len + 4);
	len += 4;
	*(UINT*)(bytes + len - 4) = fl;
unguard
}
void HypBuffer::putFloat(float fl)
{
guard
	bytes = (BYTE*)realloc(bytes,len + 4);
	len += 4;
	*(float*)(bytes + len - 4) = fl;
unguard
}
float HypBuffer::getFloat()
{
guard
	if(len < pos + 4) return 0;
	pos += 4;
	return *(float*)&bytes[pos - 4];
unguard
}
void HypBuffer::putDouble(double db)
{
guard
	bytes = (BYTE*)realloc(bytes,len + 8);
	len += 8;
	*(double*)(bytes + len - 8) = db;
	unguard
}
double HypBuffer::getDouble()
{
guard
	if(len < pos + 8) return 0;
	pos += 8;
	return *(double*)(&bytes[pos - 8]);
unguard
}
void HypBuffer::putString(HString& str)
{
guard
	if(len == 0)
		bytes = (BYTE*)malloc((str.length() * 2) + 2);
	else
		bytes = (BYTE*)realloc(bytes,len + (str.length() * 2) + 2);
	*(USHORT*)(bytes + len) = (USHORT)str.length();
	memcpy(bytes + len + 2,str.nstring,str.length() * 2);
	len += (str.length() * 2) + 2;
unguard
}
HString* HypBuffer::getString()
{
guard
	//No safety check.
	HString* hs = new HString(bytes + pos);
	pos += (hs->length()*2) + 2;
	return hs;
unguard
}
void HypBuffer::putInt64(INT64 l)
{
guard
	bytes = (BYTE*)realloc(bytes,len + 8);
	len += 8;
	*(INT64*)(bytes + len - 8) = l;
unguard
}
void HypBuffer::putUInt64(UINT64 l)
{
guard
	bytes = (BYTE*)realloc(bytes,len + 8);
	len += 8;
	*(UINT64*)(bytes + len - 8) = l;
unguard
}
void HypBuffer::putShort(SHORT l)
{
guard
	bytes = (BYTE*)realloc(bytes,len + 2);
	len += 2;
	*(SHORT*)(bytes + len - 2) = l;
unguard
}
SHORT HypBuffer::getShort()
{
guard
	if(len < pos + 2) return 0;
	pos += 2;
	return *(SHORT*)(&bytes[pos - 2]);
unguard
}
void HypBuffer::putUShort(USHORT l)
{
guard
	bytes = (BYTE*)realloc(bytes,len + 2);
	len += 2;
	*(USHORT*)(bytes + len - 2) = l;
unguard
}
USHORT HypBuffer::getUShort()
{
guard
	if(len < pos + 2) return 0;
	pos += 2;
	return *(USHORT*)(&bytes[pos - 2]);
unguard
}
INT64 HypBuffer::getInt64()
{
guard
	if(len < pos + 8) return 0;
	pos += 8;
	return *(INT64*)(&bytes[pos - 8]);
unguard
}
UINT64 HypBuffer::getUInt64()
{
guard
	if(len < pos + 8) return 0;
	pos += 8;
	return *(UINT64*)(&bytes[pos - 8]);
unguard
}
bool HypBuffer::eof()
{
guard
	return pos==len;
unguard
}
