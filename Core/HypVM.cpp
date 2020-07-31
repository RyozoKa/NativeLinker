/*
	Main file for all intrinsic functions happening in the VM.
	Revision:
		Christopher Békési - Initial build
*/

#include "Core.h"

extern "C" { void* opcodes[257]; }
void initVM()
{


	REGISTER_OPCODE(EX_pushg,PUSHG)
	REGISTER_OPCODE(EX_MV2RBX	,MV2RBX	)
	REGISTER_OPCODE(EX_MV2R14	,MV2R14	)
	REGISTER_OPCODE(EX_MV2RDX	,MV2RDX	)
	REGISTER_OPCODE(EX_MV2R8	,MV2R8	)
	REGISTER_OPCODE(EX_MV2R9	,MV2R9	)
	REGISTER_OPCODE(EX_MV2RSI	,MV2RSI	)
	REGISTER_OPCODE(EX_MV2RDI	,MV2RDI	)
	REGISTER_OPCODE(EX_MV2XMM1	,MV2XMM1)
	REGISTER_OPCODE(EX_MV2XMM2	,MV2XMM2)
	REGISTER_OPCODE(EX_MV2XMM3	,MV2XMM3)
	REGISTER_OPCODE(EX_MV2XMM4	,MV2XMM4)
	REGISTER_OPCODE(EX_MV2XMM5	,MV2XMM5)
	REGISTER_OPCODE(EX_S1P		,S1P	)
	REGISTER_OPCODE(EX_S2P		,S2P	)
	REGISTER_OPCODE(EX_S3P		,S3P	)
	REGISTER_OPCODE(EX_S4P		,S4P	)
	REGISTER_OPCODE(EX_S5P		,S5P	)
	REGISTER_OPCODE(EX_S6P		,S6P	)
	REGISTER_OPCODE(EX_R1P		,R1P	)
	REGISTER_OPCODE(EX_R2P		,R2P	)
	REGISTER_OPCODE(EX_R3P		,R3P	)
	REGISTER_OPCODE(EX_R4P		,R4P	)
	REGISTER_OPCODE(EX_R5P		,R5P	)
	REGISTER_OPCODE(EX_R6P		,R6P	)
	REGISTER_OPCODE(EX_SD1P		,SD1P	)
	REGISTER_OPCODE(EX_SD2P		,SD2P	)
	REGISTER_OPCODE(EX_SD3P		,SD3P	)
	REGISTER_OPCODE(EX_SD4P		,SD4P	)
	REGISTER_OPCODE(EX_SD5P		,SD5P	)
	REGISTER_OPCODE(EX_SD6P		,SD6P	)
	REGISTER_OPCODE(EX_RD1P		,RD1P	)
	REGISTER_OPCODE(EX_RD2P		,RD2P	)
	REGISTER_OPCODE(EX_RD3P		,RD3P	)
	REGISTER_OPCODE(EX_RD4P		,RD4P	)
	REGISTER_OPCODE(EX_RD5P		,RD5P	)
	REGISTER_OPCODE(EX_RD6P		,RD6P	)
	REGISTER_OPCODE(EX_pushgn,PUSHGN)
	REGISTER_OPCODE(EX_pushgs,PUSHGS)
	/*REGISTER_OPCODE(EX_pushgsc,PUSHGSC)
	REGISTER_OPCODE(EX_pushg_t,PUSHG_T)
	REGISTER_OPCODE(EX_pushgn_t,PUSHGN_T)
	REGISTER_OPCODE(EX_pushgs_t,PUSHGS_T)
	REGISTER_OPCODE(EX_pushl,PUSHL)
	REGISTER_OPCODE(EX_pushp,PUSHP)
	REGISTER_OPCODE(EX_pushli,PUSHLI)
	REGISTER_OPCODE(EX_pushsli,PUSHSLI)
	REGISTER_OPCODE(EX_pushlp,PUSHLP)
	REGISTER_OPCODE(EX_pushgp,PUSHGP)
	REGISTER_OPCODE(EX_pushgnp,PUSHGNP)
	REGISTER_OPCODE(EX_pushgsp,PUSHGSP)
	REGISTER_OPCODE(EX_pushgscp,PUSHGSCP)
	REGISTER_OPCODE(EX_pushgp_t,PUSHGP_T)
	REGISTER_OPCODE(EX_pushgnp_t,PUSHGNP_T)
	REGISTER_OPCODE(EX_pushgsp_t,PUSHGSP_T)
	REGISTER_OPCODE(EX_callnat,CALLNAT)
	REGISTER_OPCODE(EX_callnat_t,CALLNAT_T)
	REGISTER_OPCODE(EX_callvirt,CALLVIRT)
	REGISTER_OPCODE(EX_callvirt_t,CALLVIRT_T)
	REGISTER_OPCODE(EX_callstatic,CALLSTATIC)
	REGISTER_OPCODE(EX_callstatic_c,CALLSTATIC_C)
	REGISTER_OPCODE(EX_callstatic_t,CALLSTATIC_T)
	REGISTER_OPCODE(EX_callnatstatic,CALLSTATNAT)
	REGISTER_OPCODE(EX_callnatstatic_c,CALLSTATNAT_C)
	REGISTER_OPCODE(EX_callnatstatic_t,CALLSTATNAT_T)
	REGISTER_OPCODE(DBEX_callnat,DBCALLNAT)
	REGISTER_OPCODE(DBEX_callnat_t,DBCALLNAT_T)
	REGISTER_OPCODE(DBEX_callvirt,DBCALLVIRT)
	REGISTER_OPCODE(DBEX_callvirt_t,DBCALLVIRT_T)
	REGISTER_OPCODE(DBEX_callstatic,DBCALLSTATIC)
	REGISTER_OPCODE(DBEX_callstatic_c,DBCALLSTATIC_C)
	REGISTER_OPCODE(DBEX_callstatic_t,DBCALLSTATIC_T)
	REGISTER_OPCODE(DBEX_callnatstatic,DBCALLSTATNAT)
	REGISTER_OPCODE(DBEX_callnatstatic_c,DBCALLSTATNAT_C)
	REGISTER_OPCODE(DBEX_callnatstatic_t,DBCALLSTATNAT_T)
	REGISTER_OPCODE(EX_assi,ASSI)
	REGISTER_OPCODE(EX_assib,ASSIB)
	REGISTER_OPCODE(EX_assiw,ASSIW)
	REGISTER_OPCODE(EX_assidw,ASSIDW)
	REGISTER_OPCODE(EX_assis,ASSIS)
	REGISTER_OPCODE(EX_assisi,ASSISI)
	REGISTER_OPCODE(EX_assisd,ASSISD)
	REGISTER_OPCODE(EX_assiso,ASSISO)
	REGISTER_OPCODE(EX_assio,ASSIO)
	REGISTER_OPCODE(EX_assid,ASSID)
	REGISTER_OPCODE(EX_itod,ITOD)
	REGISTER_OPCODE(EX_dtoi,DTOI)
	REGISTER_OPCODE(EX_itos,ITOS)
	REGISTER_OPCODE(EX_uitos,UITOS)
	REGISTER_OPCODE(EX_dtos,DTOS)
	REGISTER_OPCODE(EX_dref,DREF)
	REGISTER_OPCODE(EX_linstrnull,LINSTRNULL)
	REGISTER_OPCODE(EX_linstr,LINSTR)
	REGISTER_OPCODE(EX_lstrnull,LSTRNULL)
	REGISTER_OPCODE(EX_lintnull,LINTNULL)
	REGISTER_OPCODE(EX_gstrnull,GSTRNULL)
	REGISTER_OPCODE(EX_gintnull,GINTNULL)
	REGISTER_OPCODE(EX_gsstrnull,GSSTRNULL)
	REGISTER_OPCODE(EX_gsintnull,GSINTNULL)
	REGISTER_OPCODE(EX_add,ADD)
	REGISTER_OPCODE(EX_addd,ADDD)
	REGISTER_OPCODE(EX_addid,ADDID)
	REGISTER_OPCODE(EX_adddi,ADDDI)
	REGISTER_OPCODE(EX_addds,ADDDS)
	REGISTER_OPCODE(EX_addis,ADDIS)
	REGISTER_OPCODE(EX_adds,ADDS)
	REGISTER_OPCODE(EX_addss,ADDSS)
	REGISTER_OPCODE(EX_sub,SUB)
	REGISTER_OPCODE(EX_subd,SUBD)
	REGISTER_OPCODE(EX_subid,SUBID)
	REGISTER_OPCODE(EX_subdi,SUBDI)
	REGISTER_OPCODE(EX_subs,SUBS)
	REGISTER_OPCODE(EX_div,DIV)
	REGISTER_OPCODE(EX_divd,DIVD)
	REGISTER_OPCODE(EX_divid,DIVID)
	REGISTER_OPCODE(EX_divdi,DIVDI)
	REGISTER_OPCODE(EX_multi,MULTI)
	REGISTER_OPCODE(EX_multid,MULTID)
	REGISTER_OPCODE(EX_multiid,MULTIID)
	REGISTER_OPCODE(EX_multidi,MULTIDI)
	REGISTER_OPCODE(EX_multiui,MULTIUI)
	REGISTER_OPCODE(EX_multiiu,MULTIIU)
	REGISTER_OPCODE(EX_multiu,MULTIU)
	REGISTER_OPCODE(EX_remi,REMI)
	REGISTER_OPCODE(EX_remu,REMU)
	REGISTER_OPCODE(EX_remui,REMUI)
	REGISTER_OPCODE(EX_remiu,REMIU)
	REGISTER_OPCODE(EX_remd,REMD)
	REGISTER_OPCODE(EX_inci,INCI)
	REGISTER_OPCODE(EX_incd,INCD)
	REGISTER_OPCODE(EX_deci,DECI)
	REGISTER_OPCODE(EX_decd,DECD)*/


}

STRING* HypThread::getTempString()
{
	
		Strptr &= 0x7F;
		return &Strings[Strptr++];
	
}


typedef void (*a)(class HypThread*);
typedef void (HypObject::*b)(class HypThread*);

//Function calls
/*void HypThread::CallVirtualFunction(HObject* o, USHORT i)
{
		Function* F = o->vftable[i];
		//Set up parameters
		//UINT64* Parameters = this->Parameters;
		//this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		//for(int i=0;i<F->META[1];i++)
		//	this->Parameters[i] = pop();
		//Registers keep the parameters now
		//Save current state
		//push((UINT64)node);
		//push((UINT64)Local);
		//push((UINT64)Code);
		//push((UINT64)_this);

		//Setup environment
		//node = F;
		//_this = o;
		//Code = node->Code;	//Get code position
		//StaticGlobals = node->nodeowner->StaticGlobals;
		//Globals			= _this->Globals;
		//Local = (BYTE*)_alloca(node->META[0]);
		//constants = node->nodeowner->constants;

		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		//reset state
		_this			= (HObject*)pop();
		Code			= (BYTE*)pop();
		Local			= (BYTE*)pop();
		node			= (Function*)pop();
		Globals			= _this->Globals;
this->Parameters		= Parameters;//(UINT64*)pop();
		//constants		= node->nodeowner->constants;
}
void HypThread::CallVirtualFunction_T(USHORT i)
{
		Function* F = _this->vftable[i];
		//Set up parameters
		UINT64* Parameters = this->Parameters;
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		bool _bRunning = bRunning;	//Save previous state
		if(bRunning)	//Save state
		{
			//Save current state
			push((UINT64)node);
			push((UINT64)Local);
			push((UINT64)Code);
		}
		else bRunning = true;
		//Setup environment
		node = F;
		Code = node->Code;	//Get code position
		Local = (BYTE*)_alloca(node->META[0]);
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		if(_bRunning)
		{
			//reset state
			Code			= (BYTE*)pop();
			Local			= (BYTE*)pop();
			node			= (Function*)pop();
	this->Parameters		= Parameters;//(UINT64*)pop();
		}
		else bRunning = false;
}
void HypThread::CallStaticFunction(HClass* c,USHORT i)
{
		Function* F = c->vftable[i];
		//Set up parameters
		UINT64* Parameters = this->Parameters;
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		bool _bRunning = bRunning;	//Save previous state
		if(bRunning)	//Save state
		{
		//Save current state
			push((UINT64)node);
			push((UINT64)Local);
			push((UINT64)Code);
		}
		else bRunning = true;
		//Setup environment
		node = F;
		Code = node->Code;	//Get code position
		StaticGlobals = node->nodeowner->StaticGlobals;
		Local = (BYTE*)_alloca(node->META[0]);
		constants = node->nodeowner->constants;
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		if(_bRunning)
		{
		//reset state
			Code			= (BYTE*)pop();
			Local			= (BYTE*)pop();
			node			= (Function*)pop();
			StaticGlobals	= node->nodeowner->StaticGlobals;
	this->Parameters		= Parameters;
			constants		= node->nodeowner->constants;
		}
		else bRunning = false;
}
void HypThread::CallStaticFunction_T(USHORT i)
{
		Function* F = node->nodeowner->vftable[i];
		//Set up parameters
		UINT64* Parameters = this->Parameters;
		this->Parameters = (UINT64*)_alloca(node->META[1] * 8);
		for(int i=0;i<node->META[1];i++)
			this->Parameters[i] = pop();

		bool _bRunning = bRunning;	//Save previous state
		if(bRunning)	//Save state
		{
			//Save current state
			push((UINT64)node);
			push((UINT64)Local);
			push((UINT64)Code);
		}
		else bRunning = true;
		//Setup environment
		node = F;
		Code = node->Code;	//Get code position
		Local = (BYTE*)_alloca(node->META[0]);
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		if(_bRunning)
		{
			//reset state
			Code			= (BYTE*)pop();
			Local			= (BYTE*)pop();
			node			= (Function*)pop();
	this->Parameters		= Parameters;
		}
		else bRunning = false;
}
void HypThread::CallNativeFunction(HObject* o,USHORT i)
{
		//Skip environment initialization, should work fine as long as bRunning is set to false while only
			//Native code is executed, or not running at all. Native functions are free to call script functions
			//from the native function, and then bRunning will be set to true and environment will be set up.
		b B = *(b*)&o->nvftable[i];
		(o->*B)(this);
}
void HypThread::CallNativeFunction_T(USHORT i)
{
		b B = *(b*)&_this->nvftable[i];
		(_this->*B )(this);
}

void HypThread::CallNativeStaticFunction(HClass* c,USHORT i)
{
		a A = *(a*)&c->nvftable[i];
		A(this);
}
void HypThread::CallNativeStaticFunction_T(USHORT i)
{
		a A = *(a*)&node->nodeowner->nvftable[i];
		A(this);
}

//Debug versions
void HypThread::DBCallVirtualFunction(HObject* o, USHORT i)
{
	guard

		if(ISNONE(o))
			GLog->Warn(L"Attempted to call function offset (%d) on null/invalid pointer from function (%s) on thread: %d",i,node?node->name->towchar():L"Root",Index);
		Function* F = o->vftable[i];
		//Set up parameters
		UINT64* Parameters = this->Parameters;
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();

		bool _bRunning = bRunning;	//Save previous state
		if(bRunning)	//Save state
		{
			//Save current state
			push((UINT64)node);
			push((UINT64)Local);
			push((UINT64)Code);
			push((UINT64)_this);
		}
		else bRunning = true;
		//Setup environment
		node			= F;
		_this			= o;
		Code			= node->Code;	//Get code position
		StaticGlobals	= node->nodeowner->StaticGlobals;
		Globals			= _this->Globals;
		Local			= (BYTE*)_alloca(node->META[0]);
		constants		= node->nodeowner->constants;
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		if(_bRunning)
		{
			//reset state
			_this			= (HObject*)pop();
			Code			= (BYTE*)pop();
			Local			= (BYTE*)pop();
			node			= (Function*)pop();
			StaticGlobals	= node->nodeowner->StaticGlobals;
			Globals			= _this->Globals;
	this->Parameters		= Parameters;//(UINT64*)pop();
			constants		= node->nodeowner->constants;
		}
		else {bRunning = false; node = NULL;}
	unguard
}
void HypThread::DBCallVirtualFunction_T(USHORT i)
{
	guard
		if(ISNONE(_this))
			GLog->Warn(L"Attempted to call function offset (%d) on null/invalid pointer from function (%s) on thread: %d",i,node?node->name->towchar():L"Root",Index);
		Function* F = _this->vftable[i];
		UINT64* Parameters = this->Parameters;
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();

		bool _bRunning = bRunning;	//Save previous state
		if(bRunning)	//Save state
		{
			//Save current state
			push((UINT64)node);
			push((UINT64)Local);
			push((UINT64)Code);
		}
		else bRunning = true;
		//Setup environment
		node = F;
		Code = node->Code;	//Get code position
		Local = (BYTE*)_alloca(node->META[0]);
		
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		if(_bRunning)
		{
			//reset state
			Code			= (BYTE*)pop();
			Local			= (BYTE*)pop();
			node			= (Function*)pop();
	this->Parameters		= Parameters;//(UINT64*)pop();
		}
		else {bRunning = false; node = NULL;}
	unguard
}
void HypThread::DBCallStaticFunction(HClass* c,USHORT i)
{
	guard
		if(!c)
			GLog->Warn(L"Attempted to call static function offset (%d) on null/invalid class pointer from function (%s) on thread: %d",i,node?node->name->towchar():L"Root",Index);
		Function *F = c->vftable[i];
		//Set up parameters
		UINT64* Parameters = this->Parameters;
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		bool _bRunning = bRunning;	//Save previous state
		if(bRunning)	//Save state
		{
		//Save current state
			push((UINT64)node);
			push((UINT64)Local);
			push((UINT64)Code);
		}
		else bRunning = true;
		//Setup environment
		node = F;
		Code = node->Code;	//Get code position
		StaticGlobals = node->nodeowner->StaticGlobals;
		Local = (BYTE*)_alloca(node->META[0]);
		constants = node->nodeowner->constants;
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		if(_bRunning)
		{
		//reset state
			Code			= (BYTE*)pop();
			Local			= (BYTE*)pop();
			node			= (Function*)pop();
			StaticGlobals	= node->nodeowner->StaticGlobals;
	this->Parameters		= Parameters;
			constants		= node->nodeowner->constants;
		}
		else {bRunning = false; node = NULL;}
	unguard
}
void HypThread::DBCallStaticFunction_T(USHORT i)
{
	guard
		if(node) 
			GLog->Warn(L"Attempted to call static function offset (%d) on null/invalid class pointer from function (%s) on thread: %d",i,node?node->name->towchar():L"Root",Index);

		Function* F = node->nodeowner->vftable[i];
		//Set up parameters
		UINT64* Parameters = this->Parameters;
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();

		bool _bRunning = bRunning;	//Save previous state
		if(bRunning)	//Save state
		{
			//Save current state
			push((UINT64)node);
			push((UINT64)Local);
			push((UINT64)Code);
		}
		else bRunning = true;
		//Setup environment
		node = F;
		Code = node->Code;	//Get code position
		Local = (BYTE*)_alloca(node->META[0]);
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		if(_bRunning)
		{
			//reset state
			Code			= (BYTE*)pop();
			Local			= (BYTE*)pop();
			node			= (Function*)pop();
	this->Parameters		= Parameters;
		}
		else {bRunning = false; node = NULL;}
	unguard
}
void HypThread::DBCallNativeFunction(HObject* o,USHORT i)
{
	guard	//Skip environment initialization, should work fine as long as bRunning is set to false while only
			//Native code is executed, or not running at all. Native functions are free to call script functions
			//from the native function, and then bRunning will be set to true and environment will be set up.
		b B = *(b*)&o->nvftable[i];
		(o->*B)(this);
	unguard
}
void HypThread::DBCallNativeFunction_T(USHORT i)
{
	guard	
		b B = *(b*)&_this->nvftable[i];
		(_this->*B )(this);
	unguard
}
void HypThread::DBCallNativeStaticFunction(HClass* c,USHORT i)
{
	guard
		a A = *(a*)&c->nvftable[i];
		A(this);
	unguard
}
void HypThread::DBCallNativeStaticFunction_T(USHORT i)
{
	guard
		a A = *(a*)&node->nodeowner->nvftable[i];
		A(this);
	unguard
}

void DLLEXPORT HypThread::PrintStack()
{
		//Print the stack
		GLog->Log(L"=========== Stack dump ============");
		for(UINT i=0;i<Stackptr;i++)
			GLog->Log(L"0x%16X",Stack[i]);
}

//Opcodes
void PUSHG()
{
		push( *((*(HObject**)pop())->Globals + *(USHORT*)Code) & *(UINT64*)(Code+2) );
		node->Code += 10;
}
void PUSHGN()
{
	
		push( *( ((UINT64*)&(*(HObject**)pop())->Class) + *(USHORT*)Code ) & *(UINT64*)(Code+2)  );
		node->Code += 10;
	
}
void PUSHGS()
{
	
		push( *((*(HObject**)pop())->Class->StaticGlobals + *(USHORT*)Code) & *(UINT64*)(Code+2) );
		Code += 10;
	
}
void PUSHGSC()
{
	
		push( *((*(HClass**)pop())->StaticGlobals + *(USHORT*)Code) & *(UINT64*)(Code+2) );
		Code += 10;
	
}
void PUSHG_T()
{
	
		push( *(Globals + *(USHORT*)Code) & *(UINT64*)(Code+2) );
		Code += 10;
	
}
void PUSHGN_T()
{
	
		push( *( ((UINT64*)&_this->Class) + *(USHORT*)Code ) & *(UINT64*)(Code+2) );
		Code += 10;
	
}
void PUSHGS_T()
{
	
		push( *(StaticGlobals + *(USHORT*)Code) & *(UINT64*)(Code+2) );
		Code += 10;
	
}
void PUSHL()
{
	
		push( *(Local + *(USHORT*)Code) & *(UINT64*)(Code+2) );
		Code += 10;
	
}
void PUSHP()
{
	
		push( *(Parameters + *(USHORT*)Code) & *(UINT64*)(Code+2));
		Code += 10;
	
}
void PUSHLI()
{
	
		push(*(UINT64*)Code);
		Code += 8;
	
}
void PUSHSLI()
{
	
		push( (UINT64)*(constants + *(USHORT*)Code) );
		Code += 2;
	
}
void PUSHLP()
{
	
		push((UINT64)(Local + *(UINT*)Code));
		Code += 4;
	
}
void PUSHGP()
{
	
		push( (UINT64) ((*(HObject**)pop())->Globals + *(USHORT*)Code) );
		Code += 4;
	
}
void PUSHGNP()
{
	
		push( (UINT64) ( ( (UINT64*)&(*(HObject**)pop())->Class ) + *(USHORT*)Code) );
		Code += 4;
	
}
void PUSHGSP()
{
	
		push( (UINT64) ((*(HObject**)pop())->Class->StaticGlobals + *(USHORT*)Code));
		Code += 4;
	
}
void PUSHGSCP()
{
	
		push( (UINT64) (HClass::findClass(*(UINT64*)Code)->StaticGlobals + *(USHORT*)(Code+8)) );
		Code += 12;
	
}
void PUSHGP_T()
{
	
		push( (UINT64)(Globals + *(USHORT*)Code) );
		Code += 4;
	
}
void PUSHGNP_T()
{
	
		push( (UINT64)( ( (UINT64*)&_this->Class ) + *(USHORT*)Code) );
		Code += 4;
	
}
void PUSHGSP_T()
{
	
		push( (UINT64)(StaticGlobals + *(USHORT*)Code) );
		Code += 4;
	
}
//Invoke opcodes
//Push parameters first, then if needed, context too. Left to right
void CALLNAT()
{
	//
	//All we do is call the native function
	HObject* t = ((HObject*)pop());
	b B = *(b*)&t->nvftable[*(USHORT*)(Code)];
	Code += 2;
	(t->*B)(this);
	//
}
void CALLNAT_T()
{
	//
		b B = *(b*)&_this->nvftable[*(USHORT*)(Code)];
		Code += 2;
		(_this->*B )(this);
	//
}
void CALLVIRT()
{
		//Find the function
		HObject* tptr=(HObject*)pop();
		Function *F = *(tptr->vftable + *(USHORT*)Code);
		Code += 2;
		//Get our parameters
		UINT64* Parameters = this->Parameters;		//Safe copy
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		//Save current state
		push((UINT64)node);
		push((UINT64)Local);
		push((UINT64)Code);
		push((UINT64)_this);
		//Setup environment
		_this = tptr;
		Code = F->Code;	//Get code position
		node = F;
		Globals = _this->Globals;
		Local = (BYTE*)_alloca(F->META[0]);
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		//reset state
		_this			= (HObject*)pop();
		Code			= (BYTE*)pop();
		Local			= (BYTE*)pop();
		node			= (Function*)pop();
		Globals			= _this->Globals;
this->Parameters		= Parameters;//(UINT64*)pop();
}
void CALLVIRT_T()
{
	//
		Function *F = *(_this->vftable + *(USHORT*)Code);
		Code += 2;

		//Get our parameters
		UINT64* Parameters = this->Parameters;		//Safe copy
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();

		//Save current state
		push((UINT64)node);
		//push((UINT64)Parameters);	Redundant
		push((UINT64)Local);
		push((UINT64)Code);
		Code = F->Code;	//Get code position

		//Setup environment
		node = F;
		Local = (BYTE*)_alloca(F->META[0]);

		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}


		//reset state
		Code			= (BYTE*)pop();
		Local			= (BYTE*)pop();
		node			= (Function*)pop();
this->Parameters		= Parameters;//(UINT64*)pop();
	//
}
void CALLSTATIC()
{
		Function* F = *(HClass::findClass(*(UINT64*)Code)->vftable + *(USHORT*)(Code+8));
		Code += 10;
		//Get our parameters
		UINT64* Parameters = this->Parameters;		//Safe copy
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		//Save current state
		push((UINT64)node);
		push((UINT64)Local);
		push((UINT64)Code);
		Code = F->Code;	//Get code position
		//Setup environment
		node = F;
		Local = (BYTE*)_alloca(F->META[0]);
		//Natives = (*_this)->Natives; Implement static natives
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		//reset state
		Code			= (BYTE*)pop();
		Local			= (BYTE*)pop();
		node			= (Function*)pop();
this->Parameters		= Parameters;//(UINT64*)pop();
}
void CALLSTATIC_C()
{
		Function* F = *(((HClass*)pop())->vftable + *(USHORT*)Code);
		Code += 2;
		//Get our parameters
		UINT64* Parameters = this->Parameters;		//Safe copy
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		//Save current state
		push((UINT64)node);
		push((UINT64)Local);
		push((UINT64)Code);
		Code = F->Code;	//Get code position
		//Setup environment
		node = F;
		Local = (BYTE*)_alloca(F->META[0]);
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		//reset state
		Code			= (BYTE*)pop();
		Local			= (BYTE*)pop();
		node			= (Function*)pop();
this->Parameters		= Parameters;//(UINT64*)pop();
}
void CALLSTATIC_T()
{
		Function* F = *(node->nodeowner->vftable + *(USHORT*)Code);
		Code += 2;
		//Get our parameters
		UINT64* Parameters = this->Parameters;		//Safe copy
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		//Save current state
		push((UINT64)node);
		push((UINT64)Local);
		push((UINT64)Code);
		Code = F->Code;	//Get code position
		//Setup environment
		node = F;
		Local = (BYTE*)_alloca(F->META[0]);
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		//reset state
		Code			= (BYTE*)pop();
		Local			= (BYTE*)pop();
		node			= (Function*)pop();
this->Parameters		= Parameters;//(UINT64*)pop();
}
void CALLSTATNAT()
{
	//All we do is call the native function
	
	a A = *(a*)&HClass::findClass(*(UINT64*)(Code))->nvftable[*(USHORT*)(Code)];
	Code += 10;
	A(this);
}
void CALLSTATNAT_C()
{
	a A = *(a*)&((HClass*)pop())->nvftable[*(USHORT*)(Code)];
	Code += 2;
	A(this);
}
void CALLSTATNAT_T()
{	
	a A = *(a*)&node->nodeowner->nvftable[*(USHORT*)(Code)];
	Code += 2;
	A(this);
}
//Invoke opcodes for debug mode.. Not done yet

void DBCALLNAT()
{
	guard
	HObject* t = ((HObject*)pop());
	b B = *(b*)&t->nvftable[*(USHORT*)(Code)];
	Code += 2;
	(t->*B)(this);
	unguard
}
void DBCALLNAT_T()
{
	guard
		b B = *(b*)&_this->nvftable[*(USHORT*)(Code)];
		Code += 2;
		(_this->*B )(this);
	unguard
}
void DBCALLVIRT()
{
	guard
		//Find the function
		HObject* tptr=(HObject*)pop();
		Function *F = *(tptr->vftable + *(USHORT*)Code);
		Code += 2;
		//Get our parameters
		UINT64* Parameters = this->Parameters;		//Safe copy
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		//Save current state
		push((UINT64)node);
		push((UINT64)Local);
		push((UINT64)Code);
		push((UINT64)_this);
		//Setup environment
		_this = tptr;
		Code = F->Code;	//Get code position
		node = F;
		Globals = _this->Globals;
		Local = (BYTE*)_alloca(F->META[0]);
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		//reset state
		_this			= (HObject*)pop();
		Code			= (BYTE*)pop();
		Local			= (BYTE*)pop();
		node			= (Function*)pop();
		Globals = _this->Globals;
this->Parameters		= Parameters;//(UINT64*)pop();
unguardf(node->name->towchar())
}
void DBCALLVIRT_T()
{
	guard
		Function *F = *(_this->vftable + *(USHORT*)Code);
		Code += 2;
		//Get our parameters
		UINT64* Parameters = this->Parameters;		//Safe copy
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		//Save current state
		push((UINT64)node);
		push((UINT64)Local);
		push((UINT64)Code);
		//Setup environment
		node = F;
		Local = (BYTE*)_alloca(F->META[0]);
		Code = F->Code;
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		//reset state
		Code			= (BYTE*)pop();
		Local			= (BYTE*)pop();
		node			= (Function*)pop();
this->Parameters		= Parameters;
	unguard
}
void DBCALLSTATIC()
{
	guard
		Function* F = *(HClass::findClass(*(UINT64*)Code)->vftable + *(USHORT*)(Code+8));
		Code += 10;
		//Get our parameters
		UINT64* Parameters = this->Parameters;		//Safe copy
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		//Save current state
		push((UINT64)node);
		push((UINT64)Local);
		push((UINT64)Code);
		Code = F->Code;	//Get code position
		//Setup environment
		node = F;
		Local = (BYTE*)_alloca(F->META[0]);
		//Natives = (*_this)->Natives; Implement static natives

		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}

		//reset state
		Code			= (BYTE*)pop();
		Local			= (BYTE*)pop();
		node			= (Function*)pop();
this->Parameters		= Parameters;//(UINT64*)pop();
	unguardf(node->name->towchar())
}
void DBCALLSTATIC_C()
{
	guard
		Function* F = *(((HClass*)pop())->vftable + *(USHORT*)Code);
		Code += 2;
		//Get our parameters
		UINT64* Parameters = this->Parameters;		//Safe copy
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		//Save current state
		push((UINT64)node);
		push((UINT64)Local);
		push((UINT64)Code);
		Code = F->Code;	//Get code position
		//Setup environment
		node = F;
		Local = (BYTE*)_alloca(F->META[0]);
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		//reset state
		Code			= (BYTE*)pop();
		Local			= (BYTE*)pop();
		node			= (Function*)pop();
this->Parameters		= Parameters;//(UINT64*)pop();
	unguardf(node->name->towchar())

}
void DBCALLSTATIC_T()
{
	guard
		Function* F = *(node->nodeowner->vftable + *(USHORT*)Code);
		Code += 2;
		//Get our parameters
		UINT64* Parameters = this->Parameters;		//Safe copy
		this->Parameters = (UINT64*)_alloca(F->META[1] * 8);
		for(int i=0;i<F->META[1];i++)
			this->Parameters[i] = pop();
		//Save current state
		push((UINT64)node);
		push((UINT64)Local);
		push((UINT64)Code);
		Code = F->Code;	//Get code position
		//Setup environment
		node = F;
		Local = (BYTE*)_alloca(F->META[0]);
		//Execute opcodes
		while(*Code & 0xFF)
		{
			Code++;	//Increment pointer first, then call opcode with 
			(this->*opcodes[*(Code-1)])();	//Annoying when the increment is done after the function jump..
		}
		//reset state
		Code			= (BYTE*)pop();
		Local			= (BYTE*)pop();
		node			= (Function*)pop();
this->Parameters		= Parameters;//(UINT64*)pop();
	unguardf(node->name->towchar())

}
void DBCALLSTATNAT()
{
	guard
	a A = *(a*)&HClass::findClass(*(UINT64*)(Code))->nvftable[*(USHORT*)(Code)];
	Code += 10;
	A(this);
	unguard
}
void DBCALLSTATNAT_C()
{
	guard
	a A = *(a*)&((HClass*)pop())->nvftable[*(USHORT*)(Code)];
	Code += 2;
	A(this);
	unguard
}
void DBCALLSTATNAT_T()
{
	guard
	a A = *(a*)&node->nodeowner->nvftable[*(USHORT*)(Code)];
	Code += 2;
	A(this);
	unguard
}

	//Operators
void ASSI()
{
	
		*(UINT64*)pop() = pop();
	
}
void ASSIB()
{
	
		*(BYTE*)pop() = pop();
	
}
void ASSIW()
{
	
		*(USHORT*)pop() = pop();
	
}
void ASSIDW()
{
	
		*(UINT*)pop() = pop();
	
}
void ASSIS()
{
	
		**((STRING**)pop()) = *((STRING*)pop());
	
}
void ASSISI()
{
	
		**((STRING**)pop()) = (UINT64)pop();
	
}
void ASSISD()
{
	
		**((STRING**)pop()) = (double)pop();
	
}
void ASSISO()
{
	
		**((STRING**)pop()) = *(*(HObject**)pop())->Name;
	
}
void ASSIO()
{
	
		*((HObject***)pop()) = (HObject**)pop();
	
}
void ASSID()
{
	
		push( (UINT64&) *(double*)pop() = (double&)Stack[--Stackptr] );
	
}
	//Dynamic conversions
void ITOD()
{
	
	double d= (double)(INT64&)Stack[Stackptr-1];
	Stack[Stackptr-1] = (UINT64&)d;
	
}
void DTOI()
{
	
	INT64 i = (INT64)(double&)Stack[Stackptr-1];
	Stack[Stackptr-1] = (UINT64&)i;
	
}
void ITOS()
{
	
		push((UINT64)&(*getTempString() = (INT64&)Stack[--Stackptr]));
	
}
void UITOS()
{
	
		push((UINT64)&(*getTempString() = Stack[--Stackptr]));
	
}
void DTOS()
{
	
		push((UINT64)&(*getTempString() = (double&)Stack[--Stackptr]));
	
}
void DREF()
{
	
		push(*(UINT64*)pop());
	
}
void LINSTRNULL()
{
	
		*(Local + *(UINT*)Code) = (UINT64)(getTempString()->clear());
		Code += 4;
	
}
void LINSTR()
{
	
		*(Local + *(UINT*)Code) = (UINT64)&(*getTempString() = *(STRING*)pop());
		Code += 4;
	
}
void LSTRNULL()
{
	
		((STRING*)*(Local + *(UINT*)Code))->clear();
		Code += 4;
	
}
void LINTNULL()
{
	
		*(Local + *(UINT*)Code) = NULL;
		Code += 4;
	
}
void GSTRNULL()
{
	
		((STRING*)*(Globals + *(UINT*)Code))->clear();
		Code += 4;
	
}
void GINTNULL()
{
	
		*(Globals + *(UINT*)Code) = NULL;
		Code += 4;
	
}
void GSSTRNULL()
{
	
		((STRING*)*(Globals + *(UINT*)Code))->clear();
		Code += 4;
	
}
void GSINTNULL()
{
	
		*(Globals + *(UINT*)Code) = NULL;
		Code += 4;
	
}
void ADD()
{
	
		push( pop() + pop() );
	
}
void ADDD()
{
	
		push( (double&)Stack[--Stackptr] + (double&)Stack[--Stackptr] );
	
}
void ADDID()
{
	
		double d = (double&)Stack[--Stackptr] + (double)Stack[--Stackptr];
		push((UINT64&)d);
	
}
void ADDDI()
{
	
		double d = (double&)Stack[--Stackptr];
		push( Stack[--Stackptr] + (UINT64)d );
	
}
void ADDDS()
{
	//NOTE: TEST
		push((UINT64) ((*getTempString() = *(STRING*)pop()) += (double&)Stack[--Stackptr]));
	
}
void ADDIS()
{
	//NOTE: TEST
		push((UINT64) ((*getTempString() = *(STRING*)pop()) += Stack[--Stackptr]));
	
}
void ADDS()
{
	
		push((UINT64) ((*getTempString() = *(STRING*)pop()) += *(STRING*)pop()));
	
}
void ADDSS()
{
	
		push((UINT64) (*((*getTempString() = *(STRING*)pop()) += L" ") += *(STRING*)pop()));
	
}
// SUB
void SUB()
{
	
		push( pop() - pop() );
	
}
void SUBD()
{
	
		push( (double&)Stack[--Stackptr] - (double&)Stack[--Stackptr] );
	
}
void SUBID()
{
	
		double d = (double&)Stack[--Stackptr] - (double)Stack[--Stackptr];
		push((UINT64&)d);
	
}
void SUBDI()
{
	
		double d = (double&)Stack[--Stackptr];
		push( Stack[--Stackptr] - (UINT64)d );
	
}
void SUBS()
{
	
		push((UINT64) ((*getTempString() = *(STRING*)pop()) -= ((STRING*)pop())->towchar() ));
	
}
void DIV()
{
	
		push( pop() / pop() );
	
}
void DIVD()
{
	
		push( (double&)Stack[--Stackptr] / (double&)Stack[--Stackptr] );
	
}
void DIVID()//NOTE: Heh...
{
	
		Stack[--Stackptr] = (UINT64&)(double&)Stack[--Stackptr] / (double)pop();
	
}
void DIVDI()
{
	
		Stack[Stackptr++] =  pop()  / (UINT64)(double&)Stack[--Stackptr];
	
}
void MULTI()
{
	
		push( (INT64&)Stack[--Stackptr] * (INT64&)Stack[--Stackptr] );
	
}
void MULTID()
{
	
		push((UINT64&) (double&)Stack[--Stackptr] * (double&)Stack[--Stackptr] );
	
}
void MULTIID()
{
	
		push( (UINT64&) (double&)Stack[--Stackptr] * (INT64&)Stack[--Stackptr] );
	
}
void MULTIDI()
{
	
		(INT64&)Stack[Stackptr++] = ((INT64&)Stack[--Stackptr] * (INT64)(double&)Stack[--Stackptr]);
	
}
void MULTIUI()
{
	
		(INT64&)Stack[Stackptr++] = ((INT64&)Stack[--Stackptr] * (INT64)Stack[--Stackptr]);
	
}
void MULTIIU()
{
	
		Stack[Stackptr++] = (Stack[--Stackptr] * (UINT64)(INT64&)Stack[--Stackptr]);
	
}
void MULTIU()
{
	
		push(pop() * pop());
	
}
void REMI()
{
	
		(INT64&)Stack[Stackptr++] = (INT64&)Stack[--Stackptr] % (INT64&)Stack[--Stackptr];
	
}
void REMU()
{
	
		push(pop() % pop());
	
}
void REMUI()
{
	
		(INT64&)Stack[Stackptr++] = (INT64&)Stack[--Stackptr] % (INT64)Stack[--Stackptr];
	
}
void REMIU()
{
	
			push( pop() % (UINT64)(INT64&)Stack[--Stackptr]);
	
}
void REMD()//TODO: Replace fmod with reminder.
{
	
		(double&)Stack[Stackptr++] = fmod((double&)Stack[--Stackptr], (double&)Stack[--Stackptr]);
	
}
void INCI()
{
	
	(*(UINT64*)Stack[Stackptr])++;
	
}
void INCD()
{

	(*(double*)Stack[Stackptr])++;

}
void DECI()
{
	
	(*(UINT64*)Stack[Stackptr])++;
	
}
void DECD()
{
	(*(double*)Stack[Stackptr])++;
}
*/