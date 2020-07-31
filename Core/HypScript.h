/*
	The very first object in the script hierarchy
	Revision:
		Christopher Békési - initial build
*/
#pragma once

#pragma warning(disable : 4005)


//Class implementation, outside of class
#define IMPLEMENT_CLASS(ThisClass,ScriptClass)	\
	extern "C"	{	\
	HObject* ScriptClass##C(HClass* c)	{ return new(c) ThisClass(); }\
	void ScriptClass##D(HObject* o)	{ delete o; }}\
	HClass* ThisClass::Static##ScriptClass = NULL;



//Script/Native function glue
#define SCRIPT_CLASS(ScriptClass, ID) \
	static HClass* Static##ScriptClass;\
	static HClass* ScriptClass##Class() {if(!Static##ScriptClass) Static##ScriptClass = HClass::findClass(ID); return Static##ScriptClass;}

//Register functions
#define REGISTER_FUNCTIONS(ScriptClass) extern "C" {	void AAREG##ScriptClass(HObject::func* nvftable){ 

//#define REGISTER_STATIC_FUNCTION(NativeClass,Function,Index) c->SNatives[Index] = &NativeClass::Function;

//Map function to class
#define REGISTER_FUNCTION(NativeClass, tfunc, index, Return, ...)\
	nvftable[index] = (HObject::func)static_cast<Return (NativeClass::*) (__VA_ARGS__)>(&NativeClass::tfunc);

#define END_REGISTER_FUNCTIONS } }

extern "C" { extern void __fastcall RegisterFunction(HObject::func*, void (*)(...) ); }

//In-class macros

//Declare native function
#define IMPLEMENT_FUNCTION(fname)			void fname(void);
#define IMPLEMENT_STATIC_FUNCTION(fname)	static void fname(class HClass*);

//Within the class itself
#define DECLARE_CLASS(TClass,SuperClass) typedef TClass ThisClass;typedef SuperClass Super;

#define DECLARE_BASE_CLASS(TClass) typedef TClass ThisClass;

//Must be implemented once for a library
#define IMPLEMENT_PACKAGE BOOL APIENTRY Dllmain(	HMODULE hModule,\
													DWORD  ul_reason_for_call,\
													LPVOID lpReserved\
												)\
{	return TRUE;	}

//Internal sanity check
#define NONE 0x1
#define ISNONE(obj)  (!obj ||  (*(reinterpret_cast<ONode*>(obj)-1) & 0x1))
#define ISVALID(obj) (obj && !(*(reinterpret_cast<ONode*>(obj)-1) & 0x1))
//|1b header| HObject
// ^ & 0x1 
// 0x1 header == NONE, 0x0 == VALID

//------------------------------------------------------
//A script class representative
#pragma pack(8)

class HClass : public HypObject
{
public:
	HName				ID;				//Hashed identifier															0x00
	USHORT				Flags;			//Class specifiers															0x08
	STRING*				Name;			//Class name in text														0x10
	class HypPackage*	Package;		//Package that this class came from											0x18
	void*				dlib;			//Native library handle														0x20
	HClass*				Super;			//Parent class																0x28

	//Function table
	Function**			vftable;		//																			0x30
	//Function**			svftable;		//Static functions table.

	//Constant string table
	const STRING**		constants;		//A table holding index relative pointers from the global pool table.		0x38

	/*
		Native interface functions, always used to create an object, therefore must be assigned.
		Must also point to the last native derivative class.
	*/
	typedef HObject*	(__cdecl *CREATE)(HClass*);
	typedef void		(__cdecl *FREE)(HObject*);
	CREATE	objalloc;																								//0x40
	FREE	objdealloc;																								//0x48

	//Native functions
	
	HObject::func*					nvftable;																//0x50

	//Static native functions
	//typedef void (__fastcall *stfunc)(class HypThread*);
	//USHORT					SNativeIndex;
	//stfunc*					SNatives;

	//-----------------
	//Static variables
	//-----------------
	BYTE* StaticGlobals;	//Script statics																		0x58
	BYTE* StaticNGlobals;	//Native statics mapped																	0x60	

	//Serialize interface, does not handle dependencies.
DLLEXPORT	bool Serialize();

	//Serializer meta-data TODO: Implement or remove serializer.
	USHORT	META[12];	// 0=Variables, 1=NVftable, 2=Vftable, 3=Constants, 4=Statics, 5=none, 6=none, 7=none 8=none 9=__repn 10=__appn 0x68
	//vftable meta-data
	USHORT*		__repT;

	//Class register
	static dense_hash_map<HName,HClass*> Classes;
DLLEXPORT	static HClass* loadClass(struct ContentEntry*,class HypPackage*);
DLLEXPORT	static HClass* findClass(HName,HName = 0);

	//Destructor
	~HClass();

//private:
DLLEXPORT	HClass() {}

};

//------------------------------------------------------
//A script object representative, an instance of a ScriptClass
//Alignment must be 8 bytes for every script member.

//Must be defined for each class
#define THISCLASS HObject

//For manual evaluation
#define OBJFUNCS 1

//
class HObject : public HypObject
{
public:
	UINT					Index;																														//	0x00
	BYTE*					Globals;						//A primitive array to the global variables in this object for script variables.				0x08
	void**					nvftable;						//Native functions																				0x10
	Function**				vftable;						//Script function vftable																		0x18
	SCRIPT_CLASS(Object,50806739422707081)	//TODO:Add ID here
	//Script visible variables starts here.
	//Anything declared hereafter, including subclasses must also be declared in the corresponding script class.
	//For native only classes and objects, declare the variable as a UINT64, and later you can change it in the native subclass,
	//but it must be a pointer.
	HClass*					Class;					//A pointer to the corresponding class				--exposed to the script: 0x0000000000000000			0x20
	STRING*					Name;					//Name of this object								--exposed to the script: 0x0000000000000008			0x28
	bool					bDelete;				//Garbage flag										--exposed to the script: 0x000000000000000F

	//Object allocators
DLLEXPORT	void* operator new(size_t,HClass*);
DLLEXPORT	void operator delete(void*);

	//Static register
	static HObject* AllObjects[10240];			//All valid objects
	static USHORT ROP;							//Object pointer
	static dense_hash_set<ONode*>	Bin;		//Deleted object nodes

	typedef HObject ThisClass;

	typedef void(HObject::*func)(...);

	//Static Native functions
	static void Log(HClass*);
	HObject() {}
	~HObject() {}
};

#define THISCLASS HObjectT

class HObjectT : public HObject
{
public:
	DECLARE_CLASS(HObjectT,HObject)
	void Log2(class HypThread*);
	SCRIPT_CLASS(ObjectT,0)
};

//----------------------------------------------------------------
//HypScript bytecode

/*
	For the sake of simplifying null pointing
	Every object variable will be a (**) pointer-to pointer, and that
	pointer node will be stored in the object stack.
*/

enum{
	EX_MV2RBX,

	EX_MVRAX2R14,
	EX_MVRAX2RDX,
	EX_MVRAX2R8	,
	EX_MVRAX2R9	,
	EX_MVRAX2RSI,
	EX_MVRAX2RDI,

	EX_MVRBX2R14,
	EX_MVRBX2RDX,
	EX_MVRBX2R8	,
	EX_MVRBX2R9	,
	EX_MVRBX2RSI,
	EX_MVRBX2RDI,

	EX_MVXMM02XMM1,
	EX_MVXMM02XMM2,
	EX_MVXMM02XMM3,
	EX_MVXMM02XMM4,
	EX_MVXMM02XMM5,

	EX_MVXMM72XMM1,
	EX_MVXMM72XMM2,
	EX_MVXMM72XMM3,
	EX_MVXMM72XMM4,
	EX_MVXMM72XMM5,

	RAX_DREF_B	,
	RAX_DREF_W	,
	RAX_DREF_DW	,
	RAX_DREF_QW	,
	RAX_DREF_D	,
	
	RBX_DREF_B	,
	RBX_DREF_W	,
	RBX_DREF_DW	,
	RBX_DREF_QW	,
	RBX_DREF_D	,

	EX_S1P,		
	EX_S2P,		
	EX_S3P,		
	EX_S4P,		
	EX_S5P,		
	EX_S6P,

	EX_R1P,		
	EX_R2P,		
	EX_R3P,		
	EX_R4P,		
	EX_R5P,		
	EX_R6P,

	EX_SD1P,
	EX_SD2P,
	EX_SD3P,
	EX_SD4P,
	EX_SD5P,
	EX_SD6P,
	
	EX_RD1P,
	EX_RD2P,
	EX_RD3P,
	EX_RD4P,
	EX_RD5P,
	EX_RD6P,

	//Loading variables
	EX_RAX_B_PUSHG		,
	EX_RAX_W_PUSHG		,
	EX_RAX_DW_PUSHG		,	
	EX_RAX_QW_PUSHG		,	
	EX_XMM6_D_PUSHG		,	
			
	EX_RBX_B_PUSHG		,
	EX_RBX_W_PUSHG		,
	EX_RBX_DW_PUSHG		,	
	EX_RBX_QW_PUSHG		,	
	EX_XMM7_D_PUSHG		,	
		
	EX_RAX_B_PUSHGN		,	
	EX_RAX_W_PUSHGN		,	
	EX_RAX_DW_PUSHGN	,	
	EX_RAX_QW_PUSHGN	,	
	EX_XMM6_D_PUSHGN	,	

	EX_RBX_B_PUSHGN		,	
	EX_RBX_W_PUSHGN		,	
	EX_RBX_DW_PUSHGN	,	
	EX_RBX_QW_PUSHGN	,	
	EX_XMM7_D_PUSHGN	,	
			
	EX_RAX_B_PUSHGNS	,	
	EX_RAX_W_PUSHGNS	,	
	EX_RAX_DW_PUSHGNS	,	
	EX_RAX_QW_PUSHGNS	,	
	EX_XMM6_D_PUSHGNS	,	
			
	EX_RBX_B_PUSHGNS	,	
	EX_RBX_W_PUSHGNS	,	
	EX_RBX_DW_PUSHGNS	,	
	EX_RBX_QW_PUSHGNS	,	
	EX_XMM7_D_PUSHGNS	,	
		
	EX_RAX_B_PUSHGS		,	
	EX_RAX_W_PUSHGS		,	
	EX_RAX_DW_PUSHGS	,	
	EX_RAX_QW_PUSHGS	,	
	EX_XMM6_D_PUSHGS	,	

	EX_RBX_B_PUSHGS		,	
	EX_RBX_W_PUSHGS		,	
	EX_RBX_DW_PUSHGS	,	
	EX_RBX_QW_PUSHGS	,	
	EX_XMM7_D_PUSHGS	,	
		
	EX_RAX_B_PUSHG_T	,	
	EX_RAX_W_PUSHG_T	,	
	EX_RAX_DW_PUSH_T	,	
	EX_RAX_QW_PUSH_T	,	
	EX_XMM6_D_PUSH_T	,	
				
	EX_RBX_B_PUSHG_T	,	
	EX_RBX_W_PUSHG_T	,	
	EX_RBX_DW_PUSHG_T	,	
	EX_RBX_QW_PUSHG_T	,	
	EX_XMM7_D_PUSHG_T	,	
			
	EX_RAX_B_PUSHGN_T	,	
	EX_RAX_W_PUSHGN_T	,	
	EX_RAX_DW_PUSHGN_T	,
	EX_RAX_QW_PUSHGN_T	,
	EX_XMM6_D_PUSHGN_T	,
		
	EX_RBX_B_PUSHGN_T	,	
	EX_RBX_W_PUSHGN_T	,	
	EX_RBX_DW_PUSHGN_T	,
	EX_RBX_QW_PUSHGN_T	,
	EX_XMM7_D_PUSHGN_T	,
			
	EX_RAX_B_PUSHGNS_T	,
	EX_RAX_W_PUSHGNS_T	,
	EX_RAX_DW_PUSHGNS_T	,	
	EX_RAX_QW_PUSHGNS_T	,	
	EX_XMM6_D_PUSHGNS_T	,	
		
	EX_RBX_B_PUSHGNS_T	,
	EX_RBX_W_PUSHGNS_T	,
	EX_RBX_DW_PUSHGNS_T	,	
	EX_RBX_QW_PUSHGNS_T	,	
	EX_XMM7_D_PUSHGNS_T	,	
		
	EX_RAX_B_PUSHGS_T	,	
	EX_RAX_W_PUSHGS_T	,	
	EX_RAX_DW_PUSHGS_T	,
	EX_RAX_QW_PUSHGS_T	,
	EX_XMM6_D_PUSHGS_T	,
		
	EX_RBX_B_PUSHGS_T	,	
	EX_RBX_W_PUSHGS_T	,	
	EX_RBX_DW_PUSHGS_T	,
	EX_RBX_QW_PUSHGS_T	,
	EX_XMM7_D_PUSHGS_T	,


	EX_rax_b_pushl,			//Push local variable onto the stack			(USHORT index, UINT64 mask)
	EX_rax_w_pushl,
	EX_rax_dw_pushl,
	EX_rax_qw_pushl,
	EX_xmm6_d_pushl,

	EX_rbx_b_pushl,	
	EX_rbx_w_pushl,
	EX_rbx_dw_pushl,
	EX_rbx_qw_pushl,
	EX_xmm7_d_pushl,

	EX_rbx_pushli,
	EX_xmm7_pushli,

	EX_rax_pushsli,
	EX_rbx_pushsli,			//Push a string litteral onto the stack

	EX_rax_pushlp,			//Push a pointer of local variable
	EX_rbx_pushlp,

	EX_rax_pushgp,			//Global pointer
	EX_rbx_pushgp,

	EX_rax_pushgnp,			//Native global pointer
	EX_rbx_pushgnp,

	EX_rax_pushgsp,			//Global static pointer
	EX_rbx_pushgsp,

	EX_rax_pushgnsp,		//Global native static pointer
	EX_rbx_pushgnsp,

	EX_rax_pushgscp,		//Global static pointer class context
	EX_rbx_pushgscp,

	EX_rax_pushgp_t,		//Global pointer [this]
	EX_rbx_pushgp_t,

	EX_rax_pushgnp_t,		//Global native pointer [this]
	EX_rbx_pushgnp_t,

	EX_rax_pushgsp_t,		//Global static pointer [this]
	EX_rbx_pushgsp_t,

	EX_rax_pushgnsp_t,		//Global native static pointer [this]
	EX_rbx_pushgnsp_t,

	//Invoke functions, Push parameters left to right, and finally this pointer if needed
	EX_callnat,			//Call a native function
	EX_callnat_t,		//Call a native function [this]
	EX_callvirt,		//Call a virtual function
	EX_callvirt_t,		//Call a virtual function [this]
	EX_callstatic,		//Call a static function from constant context
	EX_callstatic_t,	//Call a static function from current context
	EX_callnatstatic,	//Call a static native function
	EX_callnatstatic_t,

	/*
		Every type will need its own version of an operator handling all possible operands
		in order to skip an extra instruction of type conversion
	*/
	//Assign	(pointer-to variable)
	EX_assiqw,				//Assign qword ( push left hand first )
	EX_assib,				//Assign byte
	EX_assiw,				//Assign word
	EX_assidw,				//Assign dword
	EX_assid,				//Assign double

	EX_assis,				//Assign strings
	EX_assisi,				//Assign integer/short/byte to string
	EX_assisd,				//Assign double to string
	EX_assiso,				//Assign object name to string
	EX_assio,				//Assign object to object	(pointer-to pointer)


	//dynamic type-conversion
	EX_itod,				//Convert int64/int/short to double	(Warning to unsigned overflow!!)
	EX_dtoi,				//Convert double to int64/int/short/byte	(Warning to unsigned overflow!!)

	//String type-conversion (Should only be used if necessary, such as passing parameters or returning?)
	EX_itos,				//Signed integer to string
	EX_uitos,				//unsigned to string
	EX_dtos,				//double to string

	//String specific opcodes
	EX_lstrnull,			//Initialize local string to zero

	EX_dupstr,
	EX_clearstr,

	//Assign zero
	EX_assiz,
	EX_assibz,
	EX_assiwz,
	EX_assidwz,
	EX_assiqwz,
	EX_assidz,

	//Arithmetic operators
	//For the sake of efficiency with intrinsic operators we create one opcode for every possible combination.
	//This will require a lot more opcodes but it will avoid the extra jumps to other opcodes allowing heavy iterative math functions to
	//Run even twice as fast.
	EX_add,					//Addition integers
	EX_addd,				//Addition doubles
	EX_addid,				//Addition integer to double
	EX_adddi,				//Addition double to integer
	EX_addds,				//Addition double to string
	EX_addis,				//Addition int to string
	EX_adds,				//Add strings
	EX_addss,				//Add strings with space

	EX_sub,					//Subtract integers
	EX_subd,				//Subtract doubles
	EX_subid,				//Subtract integer from double
	EX_subdi,				//Subtract double from integer
	EX_subs,				//Subtract strings

	EX_div,
	EX_divd,
	EX_divid,				//Integer to double
	EX_divdi,				//Double to integer

	EX_multi,
	EX_multid,
	EX_multiid,
	EX_multidi,
	EX_multiui,
	EX_multiiu,
	EX_multiu,

	EX_remi,
	EX_remu,
	EX_remui,
	EX_remiu,
	EX_remd,				//Left operand first.

	EX_inci,				//Pointer to variable
	EX_incd,
	EX_deci,				//
	EX_decd,





	EX_ret			= 0x10000
};

//Datasizes
#define	T_BYTE  0xff
#define	T_WORD  0xffff
#define	T_DWORD 0xffffffff
#define	T_QWORD 0xffffffffffffffff

//Function flags
enum {
	F_STATIC	= 0x00000400,
	F_VIRTUAL	= 0x00020000,
	F_NATIVE	= 0x00040000,
	F_FINAL		= 0x00080000,
	F_OPERATOR	= 0x00100000
};

//Class types
enum {
	C_NATIVE	= 0x01,		//Is native
	C_ABSTRACT	= 0x02,		//Is abstract
	C_STATIC	= 0x04,		//Is static class
	C_BASE		= 0x08,		//Is base class
	C_NOZERO	= 0x10,		//Don't zero initialize variables.
	C_NATIVELOA = 0x20,		//Has natives linked
	//(For compiler only)
	C_PUBLIC	= 0x100,		//Class is publicly available
	C_PRIVATE	= 0x200,		//Class is private
	C_PROTECTED = 0x400,
	C_CALC		= 0x800	,		//Calculated offsets
	C_FCALC		= 0x1000		//Calculated function tables

};

//Type modifiers
enum {
	AT_UNSIGNED  = 0x00000001,
	AT_INT64	 = 0x00000002,
	AT_INT		 = 0x00000004,
	AT_SHORT	 = 0x00000008,
	AT_BYTE		 = 0x00000010,
	AT_BOOL		 = 0x00000020,
	AT_DOUBLE	 = 0x00000040,
	AT_STRING	 = 0x00000080,
	AT_CLASS	 = 0x00000100,
	AT_OBJECT	 = 0x00000200,
	AT_STATIC	 = 0x00000400,
	AT_PUBLIC	 = 0x00000800,
	AT_PRIVATE	 = 0x00001000,
	AT_PROTECTED = 0x00002000,
	AT_OUT		 = 0x00004000,
	AT_CONST	 = 0x00008000,
	AT_VOID		 = 0x00010000
};

extern BYTE Sizes[0x0200];


//------------------------------------------------------------------
//Macros


#define P_GET_STRING(name)	STRING*				name; name = (STRING*)			Stack.Parameters++;
#define P_GET_INT(name)		INT					name; name = (INT)				*Stack.Parameters; Stack.Parameters++;
#define P_GET_UINT(name)	UINT				name; name = (UINT)				*Stack.Parameters; Stack.Parameters++;
#define P_GET_BYTE(name)	BYTE				name; name = (BYTE)				*Stack.Parameters; Stack.Parameters++;
#define P_GET_SHORT(name)	SHORT				name; name = (SHORT)			*Stack.Parameters; Stack.Parameters++;
#define P_GET_USHORT(name)	USHORT				name; name = (USHORT)			*Stack.Parameters; Stack.Parameters++;
#define P_GET_INT64(name)	INT64				name; name = (INT64)			*Stack.Paramaters; Stack.Parameters++;
#define P_GET_UINT64(name)	UINT64				name; name = (UINT64)			*Stack.Parameters; Stack.Paramaters++;
#define P_GET_OBJECT(name)	HObject*			name; name = (HObject*)			Stack.Paramaters++;

//Guards
#if 1
#define guard try{
#define unguard }catch(STRING* str){str->append(L" <- ");str->append(__FUNCTIONW__);throw str;}catch(const wchar_t* str){STRING* FUNC = new(HString::StringClass()) STRING(str);FUNC->append(L" "__FUNCTIONW__);throw FUNC;}catch(...){STRING* FUNC = new(HString::StringClass()) STRING(__FUNCTIONW__);throw FUNC;}
#define unguardf(wchar) }catch(STRING* str){str->append(L" <- ");str->append(__FUNCTIONW__).append(L"(").append(wchar).append(L")");throw str;}catch(const wchar_t* str){STRING* FUNC = new(HString::StringClass()) STRING(str);FUNC->append(L"\n"__FUNCTIONW__);FUNC->append(L"(");FUNC->append(wchar);FUNC->append(L")");throw FUNC;}catch(...){STRING* FUNC = new(HString::StringClass()) STRING(__FUNCTIONW__);FUNC->append(L"(").append(wchar).append(L")");throw FUNC;}
#define debugf(str,...)
#else
#define guard
#define unguard
#define unguardf(wchar)
#define debugf(str,...) GLog->Log(str,__VA_ARGS__)
#endif
//#define REGISTER_MEMBER(mname,type)	const_cast<type>(mname) = (type)Globals[GlobalIndex++].data;

/*
Example of a native class

HypTestClass.h
This file has been automatically generated by

IMPLEMENT_CLASS(TestClass,HObject)

class DLLEXPORT TestClass : public HObject
{
LOADCLASS(HypTestClass,HObject)
int index;
class TestActor* TActor;
IMPLEMENT_FUNCTION(TestClass);

REGISTER_NATIVE
REGISTER_FUNCTION(TestClass);
REGISTER_NATIVE_END
}

*/