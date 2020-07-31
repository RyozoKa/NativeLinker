/*
	Language specific objects
	Revision:
		Christopher Békési - Initial build
*/

//Includes
#include "Core.h"

BYTE Sizes[0x0200];

IMPLEMENT_CLASS(HObject,Object)

//Register static native functions TODO: Replace with macros
REGISTER_FUNCTIONS(Object)
		//nvftable[0] = (HObject::func)static_cast<long long>(&HObject::Log);
		RegisterFunction(nvftable, static_cast<void (*) (class HClass*)>( &HObject::Log) );
END_REGISTER_FUNCTIONS


dense_hash_map<HName,HClass*> HClass::Classes = dense_hash_map<HName,HClass*>();
HObject* HObject::AllObjects[10240];
USHORT	 HObject::ROP =0;
dense_hash_set<ONode*> HObject::Bin = dense_hash_set<ONode*>();

//Object constructor/destructor
//NOTE: Use allocated block from package instead of creating a new one and copying values.

HClass* HClass::loadClass(ContentEntry* data,HypPackage* pkg)
{
guard
	//Load class header data
	HClass* Class			= new HClass();
	HypBuffer buf(data->Data,data->Size,true);
	Class->ID				= buf.getUInt64();
	Class->Flags			= buf.getUShort();
	Class->Name				= &data->FileName;
	Class->Package			= pkg;
	Class->META[0]			= buf.getUShort();
	Class->META[4]			= buf.getUShort();
	Class->META[3]			= buf.getUShort();
	Class->constants		= (const STRING**)malloc(Class->META[3] * 8);
	Class->StaticGlobals	= (BYTE*)malloc(Class->META[4]);
	//Native table
	Class->META[1]			= buf.getUShort();
	Class->nvftable			= (void**)malloc(Class->META[1] * 8);

	if( !(Class->Flags & C_BASE) )
	{
		//If we can't find the superclass, then we have to load it.
		//<Package>.<Class>
		HName Package	 = buf.getUInt64();
		HName SuperClass = buf.getUInt64();
		HClass* nodeowner = HClass::findClass(SuperClass);
		if(!nodeowner)
		{
			guard
				HypPackage* nsrc = HypPackage::Find(Package);
				if(!nsrc) 
					throw (new STRING(L"Internal error: Dependency was not loaded for "))->append(Class->Name->towchar());
				
				ContentEntry* data = nsrc->loadSingleEntry(SuperClass);
				
				if(data->Flags & HypPackage::L_LOADED)
					throw L"Internal error: Class data loaded but class wasn't found.";
				
				nodeowner = HClass::loadClass(data,nsrc);	//Load superclass here
			unguardf(L"Recursive load class")
		}
		Class->Super = nodeowner;
		
		//Copy superclass' native table
		memcpy(Class->nvftable,nodeowner->nvftable,nodeowner->META[1] * 8);
		//Build vftable
		Class->META[2] = buf.getUShort();
		//Copy vftable from parent
		Class->vftable = (Function**)malloc(Class->META[2] * 8);
		memcpy(Class->vftable,nodeowner->vftable,Class->Super->META[2] * 8);
		//Then add functions here..
		Class->META[10] = buf.getUShort();
		for(int i=0;i<Class->META[10];i++)
			*(Class->vftable + Class->Super->META[2] + i) = new Function(buf,Class);
		Class->META[9] = buf.getUShort();
		Class->__repT = (USHORT*)malloc(Class->META[9] * 2);
		for(int i=0;i<Class->META[9];i++)
		{
			Class->__repT[i] = buf.getUShort();
			*(Class->vftable + Class->__repT[i]) = new Function(buf,Class);
		}

		//Native class
		if(Class->Flags & C_NATIVE)
		{
			guard
			STRING _Create, _Free;
			_Create.append(Class->Name->towchar());
			_Create.append(L"C");
			_Free.append(Class->Name->towchar());
			_Free.append(L"D");
			if(!pkg->dlib)
				if(!(pkg->dlib = (void*)ldlib(Class->Name->towchar()))) throw L"Internal error: Failed to bind to native library.";
			if(!(Class->objalloc	= (CREATE)ldfunc(pkg->dlib,_Create.tochar()))) throw L"Internal error: native object constructor missing.";
			if(!(Class->objdealloc	=	(FREE)ldfunc(pkg->dlib,_Create.tochar()))) throw L"Internal error: native object destructor missing.";
			//Register native functions

			void (*reg)(HClass*);
			STRING S = L"AAREG";
			S.append(*Class->Name);
			reg = (void (*)(HClass*))ldfunc(pkg->dlib,S.tochar());
			reg(Class);	//Call class registration.
			unguardf(Class->Name->towchar())
		}
		else
		{
			for(HClass* C = Class->Super; C != NULL; C = C->Super)
			{
				if(C->Flags & C_NATIVE)
				{
					Class->objalloc = C->objalloc;
					Class->objdealloc = C->objdealloc;
				}
			}
		}
	}
	else
	{
		//Set constant null meta-data
		Class->META[9]  = 0;
		Class->META[6] = 0;
		//Build vftable
		Class->META[2] = buf.getUShort();
		Class->vftable = (Function**)malloc(Class->META[2] * 8);
		//Then add functions here..
		Class->META[10] = Class->META[2];	//vftable empty.
		for(int i=0;i<Class->META[10];i++)
			*(Class->vftable + i) = new Function(buf,Class);

		//Baseclass will always be native
		Class->objalloc = &ObjectC;
		Class->objdealloc = &ObjectD;
		AAREGObject(Class);
	}
	//Deserialize constant table
	for(int i=0;i<Class->META[3];i++)
		Class->constants[i] = RegisterString(pkg->STP[buf.getUShort()]);
	//Add class to register
	Classes[Class->ID] = Class;
	return Class;
unguard
}

__forceinline HClass* HClass::findClass(HName n, HName Package)
{
	if(Package)	//Check if package != NULL only once
	{
		for(auto it = Classes.begin(); it != Classes.end();++it)
			if(it->first == n && it->second->Package->ID == Package)
				return it->second;
	}
	else
	{
		for(auto it = Classes.begin(); it != Classes.end();++it)
			if(it->first == n)
				return it->second;
	}
	return NULL;
}

//TODO: Finish serializer, or remove it.
bool HClass::Serialize()
{
	if(!Package)
		return false;
	ContentEntry* File = Package->NewContent(ID);
	//Cleanup data if any
	if(File->Data)
	{
		free(File->Data);
		File->Data = NULL;
		File->Flags &= ~HypPackage::L_LOADED;
	}
	HypBuffer buf;
	buf.putUInt64(ID);
	buf.putUShort(Flags);
	buf.putUShort(META[0]);
	buf.putUShort(META[4]);
	buf.putUShort(META[3]);
	buf.putUShort(META[1]);
	
	if( !(Flags & C_BASE) )
	{
		buf.putUInt64(Super->Package->ID);
		buf.putUInt64(Super->ID);

		//vftable
		buf.putUShort(META[2]);
		buf.putUShort(META[10]);
		for(USHORT i=0;i<META[10];i++)
			vftable[Super->META[2] + i]->Serialize(buf);
		buf.putUShort(META[9]);
		for(USHORT i=0;i<META[9];i++)
		{
			buf.putUShort(__repT[i]);
			vftable[__repT[i]]->Serialize(buf);
		}
	}
	else
	{
		buf.putUShort(META[2]);
		for(USHORT i=0;i<META[10];i++)
			vftable[i]->Serialize(buf);
	}
	//Serialize constants.
	for(int i=0;i<META[3];i++)
		buf.putUShort(Package->addConstant((STRING*)constants[i]));

	File->Data = buf.bytes;
	File->Size = buf.len;
	File->FileName = *Name;
	File->ContentType = HypPackage::CLASS;
	File->ID = ID;
	//Import dependency is not handled here.
}

HClass::~HClass()
{
	delete Name;
	//Clean up functions.
	if(!(Flags & C_BASE))
	{
		//vftable
		for(int i=Super->META[2];i<META[2];i++)
			delete vftable[i];
		for(int i=0;i<META[9];i++)
			delete vftable[__repT[i]];
		free(__repT);
		free(vftable);
	}
	else
	{
		for(int i=0;i<META[2];i++)
			delete vftable[i];
		free(vftable);
	}
	free(StaticGlobals);
}

void* HObject::operator new(size_t s,HClass* c)
{
	static UINT index = 0;	//Should probably not be here.
	BYTE* o = (BYTE*)malloc(s + 1);
	*o = NULL;
	++o;
	((HObject*)o)->Class			= c;
	((HObject*)o)->nvftable			= c->nvftable;
	((HObject*)o)->Index			= (UINT64)index;
	((HObject*)o)->Globals			= (BYTE*)malloc(c->META[0]);
	((HObject*)o)->vftable			= c->vftable;
	STRING * str = new STRING(*c->Name);
	str->append(index);
	((HObject*)o)->Name = str;
	index++;
	AllObjects[ROP++] = (HObject*)o;
	return o;
}

void HObject::operator delete(void* v)
{
	delete ((HObject*)v)->Name;
	free(((HObject*)v)->Globals);
	AllObjects[((HObject*)v)->Index] = NULL;
	ONode* b = (ONode*)v;
	b--;
	*b |= 0x1;
	realloc(b,1);
	Bin.insert(b);
}


//Object native functions -----------------------------------------------------------
//Static native function Log(String);
void HObject::Log(HypThread* Thread)
{
	STRING* S = NULL;
	GLog->Log(L"%s",S->towchar());
}