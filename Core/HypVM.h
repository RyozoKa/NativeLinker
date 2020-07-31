/*	
	Main VM execution environment.
	Revision:
		Christopher Békési - initial build
*/

#include <iostream>
//-------------------------------------------------------
//Execution thread
//L0L
#define REGISTER_OPCODE(idx,func) opcodes[idx] = &func; 
typedef void (_cdecl *func)();

class HypThread : public HypObject
{
public:
	BYTE Index;

	//String pooling
	USHORT Strptr;
	STRING Strings[4096];
	

	STRING* getTempString();
	
	DLLEXPORT HypThread(BYTE index) : Strptr(NULL)
	{
		Index = index;
	}

	//Call script associated functions
	DLLEXPORT void CallVirtualFunction(HObject*,USHORT);	//Function context object, function index
	DLLEXPORT void CallVirtualFunction_T(USHORT);	//Call from current context, skip parameter. !!!!Don't call from root!!!!
	DLLEXPORT void CallStaticFunction(HClass* c,USHORT);	
	DLLEXPORT void CallStaticFunction_T(USHORT);	
	DLLEXPORT void CallNativeFunction(HObject*,USHORT);
	DLLEXPORT void CallNativeFunction_T(USHORT);
	DLLEXPORT void CallNativeStaticFunction(HClass*,USHORT);
	DLLEXPORT void CallNativeStaticFunction_T(USHORT);

	//Debug versions
	DLLEXPORT void DBCallVirtualFunction(HObject*,USHORT);
	DLLEXPORT void DBCallVirtualFunction_T(USHORT);
	DLLEXPORT void DBCallStaticFunction(HClass* c,USHORT);	
	DLLEXPORT void DBCallStaticFunction_T(USHORT);	
	DLLEXPORT void DBCallNativeFunction(HObject*,USHORT);
	DLLEXPORT void DBCallNativeFunction_T(USHORT);
	DLLEXPORT void DBCallNativeStaticFunction(HClass*,USHORT);
	DLLEXPORT void DBCallNativeStaticFunction_T(USHORT);

	//Print stack values
	void DLLEXPORT PrintStack();
};
extern "C"
{
//opcode operands
	//@RBX <- RH operand
	//@RAX <- LH operand

	void MV2RBX();	//Move RAX to RBX

	//Register opcodes: RAX -> RXX for loading parameters
	void MVRAX2R14();
	void MVRAX2RDX();
	void MVRAX2R8();
	void MVRAX2R9();
	void MVRAX2RSI();
	void MVRAX2RDI();

	void MVRBX2R14();
	void MVRBX2RDX();
	void MVRBX2R8();
	void MVRBX2R9();
	void MVRBX2RSI();
	void MVRBX2RDI();

	//@XMM6
	void MVXMM62XMM1();
	void MVXMM62XMM2();
	void MVXMM62XMM3();
	void MVXMM62XMM4();
	void MVXMM62XMM5();
	//@XMM7
	void MVXMM72XMM1();
	void MVXMM72XMM2();
	void MVXMM72XMM3();
	void MVXMM72XMM4();
	void MVXMM72XMM5();

	//Load variable from parameters
	void MVR152RAX();
	void MVRDX2RAX();
	void MVR82RAX();
	void MVR92RAX();
	void MVRSI2RAX();
	void MVRDI2RAX();

	void MVR152RAX();
	void MVRDX2RAX();
	void MVR82RAX();
	void MVR92RAX();
	void MVRSI2RAX();
	void MVRDI2RAX();

	//@XMM6
	void MVXMM12XMM6();
	void MVXMM22XMM6();
	void MVXMM32XMM6();
	void MVXMM42XMM6();
	void MVXMM52XMM6();
	//@XMM7
	void MVXMM12XMM7();
	void MVXMM22XMM7();
	void MVXMM32XMM7();
	void MVXMM42XMM7();
	void MVXMM52XMM7();

	void RAX_DREF_B	();
	void RAX_DREF_W	();
	void RAX_DREF_DW();
	void RAX_DREF_QW();
	void RAX_DREF_D	();

	void RBX_DREF_B	();
	void RBX_DREF_W	();
	void RBX_DREF_DW();
	void RBX_DREF_QW();
	void RBX_DREF_D	();	//A dereference will only ever matter to a source @xmm7

	//Save parameters
	void S1P();		
	void S2P();		
	void S3P();		
	void S4P();		
	void S5P();		
	void S6P();

	void SD1P();
	void SD2P();
	void SD3P();
	void SD4P();
	void SD5P();
	void SD6P();



	//Restore them
	void R1P();		
	void R2P();		
	void R3P();		
	void R4P();		
	void R5P();		
	void R6P();

	void RD1P();		
	void RD2P();		
	void RD3P();		
	void RD4P();		
	void RD5P();		
	void RD6P();

	void LOADCLASS();	//Bind class

	//Externam context

	//For push operands, RAX version uses RAX for context, RBX uses RBX for context, RAX AND RBX does not overwrite.

	//Push global variable to RAX (LH operand)
	void RAX_B_PUSHG		();	//<
	void RAX_W_PUSHG		();	//<
	void RAX_DW_PUSHG		();	//<
	void RAX_QW_PUSHG		();	//<
	void XMM6_D_PUSHG		();	//<
	//To RBX					//<
	void RBX_B_PUSHG		();	//<
	void RBX_W_PUSHG		();	//<
	void RBX_DW_PUSHG		();	//<
	void RBX_QW_PUSHG		();	//<
	void XMM7_D_PUSHG		();	//<

	//Push global native to RAX
	void RAX_B_PUSHGN		();	//<
	void RAX_W_PUSHGN		();	//<
	void RAX_DW_PUSHGN		();	//<
	void RAX_QW_PUSHGN		();	//<
	void XMM6_D_PUSHGN		();	//<
	//To RBX					//<
	void RBX_B_PUSHGN		();	//<
	void RBX_W_PUSHGN		();	//<
	void RBX_DW_PUSHGN		();	//<
	void RBX_QW_PUSHGN		();	//<
	void XMM7_D_PUSHGN		();	//<

	//Push global native static to RAX
	void RAX_B_PUSHGNS		();	//<
	void RAX_W_PUSHGNS		();	//<
	void RAX_DW_PUSHGNS		();	//<
	void RAX_QW_PUSHGNS		();	//<
	void XMM6_D_PUSHGNS		();	//<
	//To RBX					//<
	void RBX_B_PUSHGNS		();	//<
	void RBX_W_PUSHGNS		();	//<
	void RBX_DW_PUSHGNS		();	//<
	void RBX_QW_PUSHGNS		();	//<
	void XMM7_D_PUSHGNS		();	//<

	//Push global static to RAX
	void RAX_B_PUSHGS		();	//<
	void RAX_W_PUSHGS		();	//<
	void RAX_DW_PUSHGS		();	//<
	void RAX_QW_PUSHGS		();	//<
	void XMM6_D_PUSHGS		();	//<
	//To RBX					//<
	void RBX_B_PUSHGS		();	//<
	void RBX_W_PUSHGS		();	//<
	void RBX_DW_PUSHGS		();	//<
	void RBX_QW_PUSHGS		();	//<
	void XMM7_D_PUSHGS		();	//<

	//This

	//Push global variable to RAX (LH operand)
	void RAX_B_PUSHG_T		();	
	void RAX_W_PUSHG_T		();	
	void RAX_DW_PUSH_T		();	
	void RAX_QW_PUSH_T		();	
	void XMM6_D_PUSH_T		();	
	//To RBX
	void RBX_B_PUSHG_T		();	
	void RBX_W_PUSHG_T		();	
	void RBX_DW_PUSHG_T		();	
	void RBX_QW_PUSHG_T		();	
	void XMM7_D_PUSHG_T		();

	//Push global native to RAX
	void RAX_B_PUSHGN_T		();
	void RAX_W_PUSHGN_T		();
	void RAX_DW_PUSHGN_T	();
	void RAX_QW_PUSHGN_T	();
	void XMM6_D_PUSHGN_T	();
	//To RBX
	void RBX_B_PUSHGN_T		();
	void RBX_W_PUSHGN_T		();
	void RBX_DW_PUSHGN_T	();
	void RBX_QW_PUSHGN_T	();
	void XMM7_D_PUSHGN_T	();

	//Push global native static to RAX
	void RAX_B_PUSHGNS_T	();
	void RAX_W_PUSHGNS_T	();
	void RAX_DW_PUSHGNS_T	();
	void RAX_QW_PUSHGNS_T	();
	void XMM6_D_PUSHGNS_T	();
	//To RBX
	void RBX_B_PUSHGNS_T	();
	void RBX_W_PUSHGNS_T	();
	void RBX_DW_PUSHGNS_T	();
	void RBX_QW_PUSHGNS_T	();
	void XMM7_D_PUSHGNS_T	();

	//Push global static to RAX
	void RAX_B_PUSHGS_T		();
	void RAX_W_PUSHGS_T		();
	void RAX_DW_PUSHGS_T	();
	void RAX_QW_PUSHGS_T	();
	void XMM6_D_PUSHGS_T	();
	//To RBX
	void RBX_B_PUSHGS_T		();
	void RBX_W_PUSHGS_T		();
	void RBX_DW_PUSHGS_T	();
	void RBX_QW_PUSHGS_T	();
	void XMM7_D_PUSHGS_T	();

	void RAX_B_PUSHL();		//< Offset is inverse, meaning offset' = Size - offset
	void RAX_W_PUSHL();		
	void RAX_DW_PUSHL();		
	void RAX_QW_PUSHL();		
	void XMM6_D_PUSHL();	

	void RBX_B_PUSHL();	
	void RBX_W_PUSHL();	
	void RBX_DW_PUSHL();
	void RBX_QW_PUSHL();
	void XMM7_D_PUSHL();
	
	void RBX_PUSHLI();
	void XMM7_PUSHLI();

	void RAX_PUSHSLI();		// 2b offset
	void RBX_PUSHSLI();

	void RAX_PUSHLP();		// Pointer to local var
	void RBX_PUSHLP();

	void RAX_PUSHGP();		//Global pointer 
	void RBX_PUSHGP();

	void RAX_PUSHGNP();		//Global native pointer
	void RBX_PUSHGNP();	

	void RAX_PUSHGSP();		//Global static pointer
	void RBX_PUSHGSP();

	void RAX_PUSHGNSP();	//Global native static pointer
	void RBX_PUSHGNSP();

	void RAX_PUSHGP_T();	//From this context
	void RBX_PUSHGP_T();	

	void RAX_PUSHGNP_T();
	void RBX_PUSHGNP_T();

	void RAX_PUSHGSP_T();
	void RBX_PUSHGSP_T();

	void RAX_PUSHGNSP_T();
	void RBX_PUSHGNSP_T();	

	void DUPSTR			();
	void CLEARSTR		();

	void CALLNAT();			//<
	void CALLNAT_T();		//<
	void CALLVIRT();		//<
	void CALLVIRT_T();		//<
	void CALLSTATIC();		//< Virtual static*
	void CALLSTATIC_T();	//<
	void CALLSTATNAT();		//<
	void CALLSTATNAT_T();	//<

	void ASSI();	//< Int64
	void ASSIB();	//< Bytes
	void ASSIW();	//< Shorts
	void ASSIDW();	//< Int32
	void ASSID();	//< Doubles
	void ASSIS();	//< Strings, 
	void ASSISI();	//< String = int
	void ASSISD();	//<
	void ASSISO();	//<
	void ASSIO();	//<

	
	void ITOD();	//<
	void DTOI();	//<
	void ITOS();	//<
	void UITOS();	//<
	void DTOS();	//<

	void LSTRNULL	();		//Construct local string

	void ASSIZ	();
	void ASSIBZ	();
	void ASSIWZ ();
	void ASSIDWZ();
	void ASSIDZ ();

	void ADD	();
	void ADDD	();
	void ADDID	();	//D + I (double)
	void ADDDI	();
	void ADDDS	();
	void ADDIS	();
	void ADDS	();
	void ADDSS	();

	void SUB	();
	void SUBD	();
	void SUBID	();
	void SUBDI	();
	void SUBS	();

	void DIV	();
	void DIVD	();
	void DIVID	();
	void DIVDI	();

	void MULTI	();
	void MULTID	();
	void MULTIID();
	void MULTIDI();
	void MULTIUI();
	void MULTIIU();
	void MULTIU	();

	void REMI	();
	void REMU	();
	void REMUI	();
	void REMIU	();
	void REMD	();

	void INCI	();
	void INCD	();

	void DECI	();
	void DECD	();

}