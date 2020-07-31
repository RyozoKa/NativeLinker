// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Core.h"
#include <iostream>
#include <stack>
#include <regex>
#include "HypCompiler.h"
void Command(const wchar_t*)
{

}

UINT a = 100;
UINT b = 200;

UINT TestF()
{
	return a + b;
}
#pragma pack(8)
class AB
{
public:
	void* ThisClass;
	//bool bT1;
	BYTE bT2;
	short T3;
	int T4;
	int T5;
	void* name;
	void* Name2;
};

int _tmain(int argc, _TCHAR* argv[])
{
	try{
		init();
		GLog = new HypOutput();
		GConsole = new HypWinConsole();
		GConsole->Initialize();
		GbLogToFile = true;
		Sleep(1000);
		GConsole->SetCallback(Command);
		
		BYTE printfoo[] =
		{
			EX_pushp,0,0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,		//Push string litteral onto the stack	@0x0000
			DBEX_callnatstatic_t,0,0,	//Call native function "Log" @0x0000
			EX_ret					//Return
		};

		BYTE callfoo[] =
		{
			EX_pushsli,0,0,
			EX_pushsli,1,0,
			EX_addss,
			DBEX_callvirt_t,0,0,
			EX_ret
		};

		HClass* C= new HClass();
		C->ID = hashName(L"Object");
		C->constants = (const STRING**)malloc(16);
		C->constants[0] = RegisterString(new STRING(L"Foo is printed"));
		C->constants[1] = RegisterString(new STRING(L"Another foo string"));
		C->Flags = C_BASE | C_NATIVE;
		C->dlib = NULL;	//No dlib for base class
		C->Name = new STRING(L"Object");

		C->Package = HypPackage::Open(L"Test.hpk",FILE_MOD);
		C->StaticGlobals = NULL;
		C->Super = NULL;
		C->vftable = (Function**)malloc(8);

			Function* F = new Function();
			F->Code = printfoo;
			F->Id = hashName(L"Foo");
			F->name = new STRING(L"Foo");
			F->META[0] = 0;
			F->META[1] = 1;
			F->META[2] = 15;
			C->vftable[0] = F;

			F = new Function();
			F->Code = callfoo;
			F->Id = hashName(L"CallFoo");
			F->name = new STRING(L"CallFoo");
			F->META[0] = 0;
			F->META[1] = 0;
			F->META[2] = 11;
			C->vftable[1] = F;
		C->META[2] = C->META[10] = 2;
		C->META[9] = 0;
		C->META[0] = 0;
		C->META[1] = 0;
		C->META[8] = 1;
		C->META[3] = 2;
		C->META[4] = 0;
		C->META[6] = 0;
		C->META[7] = 0;
		C->META[5] = 0;

		C->Serialize();
		std::cout << C->Package->flush() << '\n';
		C->Package->close();
		HypPackage* hpk = HypPackage::Open(L"Test.hpk",FILE_READ);
		std::cout << hpk->readHeader() << '\n';
		hpk->readFileList();
		hpk->readAllEntries();
		for(auto it=hpk->Files.begin();it!=hpk->Files.end();it++)
			if(it->second->ContentType & HypPackage::CLASS)
				C = HClass::loadClass(it->second,hpk);
		HObject* A;
			A = (*C->objalloc)(C);
		//ONode* o = (ONode*)A;
		std::wcout << L"NONE? " << ISNONE(A) << '\n';
		HypThread t(0);
		//Let's see what happens..
		//t.DBCallVirtualFunction(A,1);

		(*C->objdealloc)(A);
		A = NULL;
		std::wcout << ISNONE(A) << '\n';
		//t.PushParameter(new STRING(L"HELLO WORLD!"));
		//t.CallNativeStaticFunction(C,0);

		//Test path class
		STRING pr(L"TestPro");
		Compile(&pr);

		/*wchar_t* st = (wchar_t*)db_malloc(20);
		wcscpy(st,L"123456789");
		st = (wchar_t*) db_realloc(st,24);
		wcscpy(st,L"1234567890");
		std::wcout << st;
		db_free(st);*/
}
	catch(STRING* str)
	{
		std::wcout << L"History:" << str->towchar() << ENDL;
	}catch(...)
	{
		std::wcout << L"Some error occured" << ENDL;
	}
	system("pause");
	return 0;
}

// rationalcoder@gmail.com