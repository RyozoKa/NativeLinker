#include "Core.h"
#include "HypCompiler.h"

//Illegal identifiers
const wchar_t *Illegals[] = {
	//Primitives
	/*L"string",
	L"class",
	L"struct",
	L"unsigned",
	L"short",
	L"int",
	L"int64",
	L"byte",
	L"double",
	L"bool",*/
	//Modifiers
	L"native",
	L"abstract",
	L"static",
	L"_pointer_",
	L"abstract",
	//Misc
	L"this",
	L"for",
	L"foreach",
	L"do",
	L"while",
	L"if",
	L"else",
	L"return",
	L"break",
	L"continue",
	L"new",
	L"extends",
	L"(",
	L")",
	L"[",
	L"]",
	L"+" ,
	L"-" ,
	L"*" ,
	L"/" ,
	L"^^",
	L"%" ,
	L"+=",
	L"-=",
	L"*=",
	L"/=",
	L"^^=",
	L"^=" ,
	L"|=" ,
	L"&=" ,
	L"&&" ,
	L"||" ,
	L"==" ,
	L"!=" ,
	L"&"  ,
	L"|"  ,
	L"!"  ,
	L"^"  ,
	L"<<" ,
	L">>" ,
	L">>>",
	L"{",
	L"}"
};

//Project-wide compiler options
bool bDebug = false;
bool bAutoSanity = false;
//Register
dense_hash_map<HName,HypPackage*> Imports;	//Import packages from makefile, Symbol table prepared and loaded.

//Preprocessor definition structure
struct _PDef
{
	std::vector<STRING> Parameters;	//In raw text only for preprocessing
	STRING Identifier;
	std::vector<STRING> Tokens;
};

enum
{
	EXP_LSE		= 0x0001,	//Left single expression
	EXP_RSE		= 0x0002,	//Right single expression
	EXP_UNARY	= 0x0004,
	EXP_FUNC	= 0x0008,
	EXP_IF		= 0x0010,
	EXP_IFELSE	= 0x0020,
	EXP_ELSE	= 0x0040,
	EXP_IFCLUSTER=0x0080,
	EXP_WHILE	= 0x0100,
	EXP_FOR		= 0x0200,
	EXP_DOWHILE = 0x0400,
	EXP_DOUNTIL = 0x0800
};

//Expression unit
struct _HExprUnit
{
	//Type of expression
	USHORT ExprT;
	//Line info for compiler alerts.
	UINT Line;
	// operand can be a single token of expression, or another expression branch
	void* LUnit;	//LH Operand, single token or another branch
	STRING Op;		//Operator sign
	void* RUnit;	//RH Operand

	__forceinline _HExprUnit() : LUnit(NULL), RUnit(NULL), ExprT(NULL), Line(NULL) {}

	__forceinline ~_HExprUnit()
	{
		if(ExprT & EXP_LSE)
			delete ((STRING*)LUnit);
		else
			delete ((_HExprUnit*)LUnit);
			
		if(ExprT & EXP_RSE)
			delete ((STRING*)RUnit);
		else
			delete ((_HExprUnit*)RUnit);
			
	}

};
struct _HFuncExpr : public _HExprUnit
{
	__forceinline _HFuncExpr() { ::_HExprUnit(); ExprT |= EXP_FUNC; }
	std::vector<_HExprUnit*>	Parameters;
};
struct _HIFExpr : public _HExprUnit
{
	__forceinline _HIFExpr() { ::_HExprUnit(); }
	std::vector<_HExprUnit*> Expressions;
};
struct _HWhileExpr	: public _HExprUnit
{
	__forceinline _HWhileExpr() { ::_HExprUnit(); }
	std::vector<_HExprUnit*> Expressions;
};
struct _HForExpr : public _HExprUnit
{
	__forceinline _HForExpr()  { ::_HExprUnit();	}
	std::vector<_HExprUnit*> Expressions;
};

//Error handling
UINT Errors=0, Warnings=0;
void Error(const wchar_t*,...);
void Expect(const STRING& Token, const wchar_t* ToBe);	//Expect a token
void ExpectIdentifier(const STRING&);
void ExpectArgument(const STRING&);
//Warnings
void Warn(const wchar_t*,...);
//Verbose
void Verb(const wchar_t*,...);

//Current data
UINT Line=0;
HypFile CFile;
HypPath P(DIRJOINSTR);


//Token functions
const STRING& CurrentToken();
const STRING& NextToken();
const STRING& NextTokenLazy();
const STRING& NextTokenLazyIdentifier();
const STRING& NextTokenIdentifier();	//Use this to parse the source code, in order to support Preprocessor macros
const STRING& PrevToken(INT i=0);
UINT TIndex=0,LEIndex=0;
std::vector<STRING> Tokens;

_HCompUnit* CU = NULL;
ContentEntry* CE = NULL;
//Project-wide definitions
dense_hash_map<HName,_PDef*> PreDefs;

//Compilation units
dense_hash_set<_HCompUnit*> Units;

//Compiled classes
std::vector<HClass*> ClassList;

_PDef* ParsePreDef();
bool ParseBool();

//Apply preprocessor definition to a token array.
void ApplyPreDef(_PDef*);

bool IsLegalIdentifier(const STRING &);
bool IsOperator(const STRING &);
bool IsUnaryOperator(const STRING &);
bool IsRightUnaryOperator(const STRING &);
bool IsBoolOperator(const STRING &);
bool IsValueOperator(const STRING &);
bool IsCmpOperator(const STRING &);
bool IsTransparent();
bool IsFunctionCall();
bool IsLitteral(const STRING&);

//For debugging
void PrintExpressions(_HExprUnit*);

//Compiler parsers
_HClass* ParseCDecl();
bool ParseVariable(std::vector<_HVar*> &);	//Special treatment
_HVar* ParseParameter();
_HType ParseType();
_HFunc* ParseFunction();
_HExprUnit* ParseExpression(_HExprUnit* E = NULL, bool bUnary = false, bool bLSE = false, bool bFirst = false);
_HIFExpr* ParseIf(_HFunc*);
_HWhileExpr* ParseWhile(_HFunc*);
_HForExpr*	ParseFor(_HFunc*);
_HWhileExpr* ParseDo(_HFunc*);
//Get pre-processor definition if found, otherwise NULL
_PDef* GetPreDef(const STRING&);

//Compiler
_HClass* GetClass(HName);
void PreCalcOffsets(_HClass*);
void PreCalcVftable(_HClass*);
_HFunc* GetSuperFunc(_HClass*, _HFunc*);	//If it exists

void CompileFunction(_HFunc*,HypBuffer&);

//Compiler steps
void ParseFile(const STRING&);

bool Compile(const STRING* Project)
{
	guard
	//Locate makefile and load imports
	P.GoToRelative(Project->towchar());
	if(!P.exists())
		throw L"Project directory was not found!";

	//Load imports and read settings
	STRING str = P.path;
	str += (wchar_t*)DIRJOINSTR;
	str +=  L"Make";
	CFile.Open(str.towchar(),FILE_READ);
	if(!CFile.exists())
		throw L"Make file was not found!";
	STRING S = CFile.readLine();
	if(CFile.Eol())
		Error(L"Makefile is empty");
	Line = 1;
	while(S.isEmpty())
	{
		//Skip empty lines
		S = CFile.readLine();
		++Line;
		if(CFile.Eol())
			Error(L"Unexpected end of file.");
	}
	//Tokens.insert(Tokens.end(),TO.begin(),TO.end());
	Tokens = S.splitCode();
	while(!CFile.Eol())
	{
		guard
		
		if(CurrentToken() |= L"import")
		{
			guard
			Expect(NextToken(),L"=");

			HypPackage *pkg = HypPackage::Open(NextToken().towchar(),FILE_READ);
			if(!pkg->file->exists())
				Error(L"Can't find asset: %s",pkg->file->filename.towchar());
			pkg->readHeader();
			pkg->readFileList();
			Imports[pkg->ID] = pkg;	//TODO:Test this.
			unguardf(L"Import loader")
		}
		else if(CurrentToken() |= L"bAutoSanity")
			bAutoSanity = ParseBool();
		else if(CurrentToken() |= L"#define")
		{
			_PDef* P = ParsePreDef();
			PreDefs[hashName(P->Identifier.towchar())] = P;
		}
		else if(CurrentToken() |= L"Build")
		{
			Expect(NextToken(),L"=");
			bDebug = (NextToken() |= L"debug");
		}
		NextTokenLazy();
		unguardf(L"Makefile parser")
	}

	//Maybe this shouldn't be kept for release mode.
	GLog->Log(L"**********************************");
	GLog->Log(L"**	Makefile summary	**");
	GLog->Log(L"**********************************");
	GLog->Log(L"** Debug: %s", bDebug?L"True":L"False");
	GLog->Log(L"** Auto-Sanity: %s", bAutoSanity?L"True":L"False");
	GLog->Log(L"**********************************");

	//Now we must parse the source code files
	HypPath SCP(P);
	HypPath DSP;
	HypPackage* Pk = HypPackage::Open((DSP.path + DIRJOINSTR + *(STRING*)Project + L".hpk").towchar(),FILE_WRITE );
	
	SCP.GoToRelative(L"/Src");
	std::vector<STRING> Files = SCP.getFiles(L"hsc");
	//Can be done async
	for(int i=0;i<Files.size();++i)
	{
		GLog->Log(L"Parsing: %s",(SCP.path + DIRJOINSTR + Files[i]).towchar());
		ParseFile(SCP.path + DIRJOINSTR + Files[i]);
	}
	GLog->Log(L"Binding classes");


	//Must be synced
	//======================================================================================
	GLog->Log(L"Preparing...");
	for(auto it = Units.begin();it != Units.end();++it)
	{
		CU = *it;
		for(int i=0;i<CU->Classes.size();++i)
		{
			_HClass* C = CU->Classes[i];
			//Bind classes
			if(C->Super)
			{
				C->SuperClass = GetClass(C->Super);
				if(!C->SuperClass)
					Error(L"Superclass for class'%s' was not found",C->Name.towchar());
			}
		}
	}
	//Second iteration involves summarizing, variables, virtual functions, static virtual functions,
	//native functions, static native functions
	for(auto it = Units.begin();it != Units.end();++it)
	{
		CU = *it;
		for(int i=0;i<CU->Classes.size();++i)
		{
			PreCalcOffsets(CU->Classes[i]);	//Also checks global variables for Class presence and binds them to their class(es).
			PreCalcVftable(CU->Classes[i]);
		}
	}
	//=======================================================================================

	for(auto it = Units.begin();it != Units.end();++it)
	{
		CU = *it;
		for(int i=0;i<CU->Classes.size();++i)
		{
			_HClass* C = CU->Classes[i];
			GLog->Log(L"Compiling: %s",C->Name.towchar());
			//Create the class binary
			CE = Pk->NewContent(CU->Classes[i]->ID);
			CE->ContentType = HypPackage::CLASS;
			//Create the buffer and serialize header
			HypBuffer BF;
			BF.putInt64(C->ID);
			BF.putUShort(C->Flags);
			BF.putUShort(C->GOffset);
			BF.putUShort(C->GSOffset);
			BF.putUShort(C->NumConst);	//Deal with them later
			BF.putUShort(C->NatFuncs);
			if( !(C->Flags & C_BASE))
			{
				if(C->SuperClass->PKG)
					BF.putUInt64(C->SuperClass->PKG->ID);
				else
					BF.putUInt64(Pk->ID);
				BF.putUInt64(C->SuperClass->ID);

				BF.putUShort(C->VFuncs);
				//Get the appN from VFuncs - SuperClass::VFuncs
				BF.putUShort(C->VFuncs - C->SuperClass->VFuncs);
				for(int j=0;j<C->Funcs.size();++j)
				{
					if(!C->Funcs[j]->SuperFunc)	//If this is not a replacement function
					{
						CompileFunction(C->Funcs[j],BF);
					}
				}
				BF.putUShort(C->repT.size());
				for(int j=0;j<C->repT.size();++j)
				{
					BF.putUShort(C->repT[j]->Index);
					CompileFunction(C->repT[j],BF);
				}
			}
			else
			{
				BF.putUShort(C->VFuncs);
				for(int j=0;j<C->Funcs.size();++j)
				{
					CompileFunction(C->Funcs[j],BF);
				}
			}
			for(int j=0;j<C->NumConst;++j)
				BF.putUShort(Pk->addConstant((STRING*)C->Constants[i]));

			CE->Data = BF.bytes;
			CE->Size = BF.len;
			CE->FileName = C->Name;
			CE->ID = C->ID;
		}
	}

	GLog->Log(L"=============== %i error(s), %i warning(s) ===============",Errors,Warnings);
	return true;
	unguard
}

_HClass* GetClass(HName N)
{
	guard

		//1st check current compiling units, then check imports.
		for(auto it = Units.begin(); it != Units.end(); ++it)
		{
			_HCompUnit* CCU = *it;
			for(int i=0;i<CCU->Classes.size();++i)
			{
				if(CCU->Classes[i]->ID == N)
					return CCU->Classes[i];
			}
		}
		for(auto it = Imports.begin();it != Imports.end(); ++it)
		{
			HypPackage* P = (*it).second;
			for(auto it2 = P->CST.begin();it2 != P->CST.end();++it2)
			{
				if((*it2)->ID == N)	//Found it
				{
					assert(CE != NULL);
					//Check if the import is already present
					for(int i=0;i < CE->NumImports; ++i)
					{
						ImportPackage* T = CE->ImportTable;
						if(T->ID == P->ID)	//Found the package
						{
							auto it3 = T->Files.begin();
							for(;it3 != T->Files.end(); ++it3)//Check for the file
							{
								if(*it3 == N)
								{
									//Map the package to the meta class here
									(*it2)->PKG = P;
									//Already exists
									return *it2;
								}
							}
							//Add it
							(*it2)->PKG = P;
							T->Files.insert(N);
							return *it2;
						}
					}
					//Create new package, and add it
					(*it2)->PKG = P;
					++CE->NumImports;
					CE->ImportTable = (ImportPackage*) realloc(CE->ImportTable,sizeof(ImportPackage) * CE->NumImports);
					CE->ImportTable[CE->NumImports - 1].Files.insert(N);
					CE->ImportTable[CE->NumImports - 1].ID = P->ID;
					return *it2;
				}
			}
		}
		return NULL;
	unguard
}
void PreCalcOffsets(_HClass* C)
{
	guard
	assert(C);
	int residue = 0;
	int GOffset = 0, GSOffset = 0, GNOffset = 0, GNSOffset = 0;
	int NumFuncs = 0, NumSFuncs = 0, NumNatFuncs = 0, NumNatSFuncs = 0;
	if(C->SuperClass && !(C->SuperClass->Flags & C_CALC)) //C_CALC = already summarized
	{
		PreCalcOffsets(C->SuperClass);
		GOffset			= C->SuperClass->GOffset;
		GSOffset		= C->SuperClass->GSOffset;
		GNOffset		= C->SuperClass->GNOffset;
		GNSOffset		= C->SuperClass->GNSOffset;
	}
	if(C->Flags & C_NATIVE)
	{
		for(auto it = C->Vars.begin(); it != C->Vars.end(); ++it)
		{
			Line = (*it)->Line;
			//Check if the type exists
			if((*it)->Type.Type & (AT_OBJECT | AT_CLASS) )
			{
				(*it)->ClassBase = GetClass((*it)->Type.TypeID);
				if(!(*it)->ClassBase)
					Error(L"Unknown type for variable '%s'",(*it)->Name.towchar());
			}
			if((*it)->Type.Type & AT_STATIC)
			{
				residue = GNSOffset % Sizes[(*it)->Type.Type];
				if(residue)
					GNSOffset += (Sizes[(*it)->Type.Type] - residue);						//Align offset
				(*it)->Offset = GNSOffset;													//Assign offset
				GNSOffset += Sizes[(*it)->Type.Type];	
			}
			else
			{
				residue = GNOffset % Sizes[(*it)->Type.Type];
				if(residue)
					GNOffset += (Sizes[(*it)->Type.Type] - residue);						//Align offset
				(*it)->Offset = GNOffset;													//Assign offset
				GNOffset += Sizes[(*it)->Type.Type];										//Advance offset by datasize
			}
			
		}
	}
	else
	{
		for(auto it = C->Vars.begin(); it != C->Vars.end(); ++it)
		{
			Line = (*it)->Line;
			if((*it)->Type.Type & (AT_OBJECT | AT_CLASS) )
			{
				(*it)->ClassBase = GetClass((*it)->Type.TypeID);
				if(!(*it)->ClassBase)
					Error(L"Unknown type for variable '%s'",(*it)->Name.towchar());
			}

			if((*it)->Type.Type & AT_STATIC )
			{
				residue = GSOffset % Sizes[(*it)->Type.Type];
				if(residue)
					GSOffset += (Sizes[(*it)->Type.Type] - residue);						
				(*it)->Offset = GSOffset;													
				GSOffset += Sizes[(*it)->Type.Type];										
			}
			else
			{
				residue = GOffset % Sizes[(*it)->Type.Type];
				if(residue)
					GOffset += (Sizes[(*it)->Type.Type] - residue);							
				(*it)->Offset = GOffset;													
				GOffset += Sizes[(*it)->Type.Type];										
			} 
		}
	}
	C->GOffset  =	GOffset ;
	residue = GOffset % 8;	//Align to 8 bytes
	if(residue)
		C->GOffset += (8 - residue);

	C->GSOffset =	GSOffset;
	residue = GSOffset % 8;
	if(residue)
		C->GSOffset += (8 - residue);

	C->GNOffset =	GNOffset;
	residue = GNOffset % 8;
	if(residue)
		C->GNOffset += (8 - residue);

	C->GNSOffset =	GNSOffset;
	residue = GNSOffset % 8;
	if(residue)
		C->GNSOffset += (8 - residue);
	unguard
}
void PreCalcVftable(_HClass* C)
{
	guard
	int NIndex = 0;
	int Index  = 0;
	if(C->Flags & C_FCALC)
		return;	//Already done.
	if(C->SuperClass && C->repT.empty())
	{
		PreCalcVftable(C->SuperClass);
		NIndex = C->SuperClass->NatFuncs;
		Index  = C->SuperClass->VFuncs;
		//This will be redundant but faster.
		for(int i=0;i<C->Funcs.size();++i)
		{
			_HFunc* F = GetSuperFunc(C->SuperClass,C->Funcs[i]);
			if(F)	//This function will be replaced
			{
				C->Funcs[i]->Index = F->Index;
				C->Funcs[i]->SuperFunc = F;
			}
				
			//Calculate indices
			if( ! (C->Funcs[i]->Type.Type & F_NATIVE))
			{
				if(F)
					C->repT.push_back(C->Funcs[i]);
				else
					C->Funcs[i]->Index = Index++;
			}
			else
				C->Funcs[i]->Index = NIndex++;
			//GLog->Log(L"Function: %s with index: %i in childclass: %s ReplacedWith=%s",C->Funcs[i]->Name.towchar(),C->Funcs[i]->Index,C->Name.towchar(),F?F->Name.towchar():L"");
		}
	}
	else
	{
		for(int i=0;i<C->Funcs.size();++i)
		{
			if(C->Funcs[i]->Type.Type & F_NATIVE)
				C->Funcs[i]->Index = NIndex++;
			else
				C->Funcs[i]->Index = Index++;

			//GLog->Log(L"Function: %s with index: %i in baseclass: %s Native: %i",C->Funcs[i]->Name.towchar(),C->Funcs[i]->Index,C->Name.towchar(), C->Funcs[i]->Type.Type & F_NATIVE);
		}
	}
	C->NatFuncs = NIndex;
	C->VFuncs	= Index;
	C->Flags |= C_FCALC;
	unguard
}
_HFunc* GetSuperFunc(_HClass* C, _HFunc* F)
{
	guard
		for(int i=0;i<C->Funcs.size();++i)
		{
			_HFunc* CF = C->Funcs[i];
			if(CF->Name |= F->Name)
			{
				Line = F->Line;
				if(CF->Type.Type != F->Type.Type)
					Error(L"Function '%s' overrides function from class '%s' but returntype or modifier(s) mismatch",F->Name.towchar(),C->Name.towchar());
				//Check parameters
				if(CF->Parameters.size() != F->Parameters.size())
					Error(L"Function '%s' overrides function from class '%s' but parameters mismatch",F->Name.towchar(),C->Name.towchar());
				for(int j=0;j<CF->Parameters.size();++j)
					if(CF->Parameters[j]->Type.Type != F->Parameters[j]->Type.Type)
						Error(L"Function '%s' overrides function from class '%s' but parameter '%s' mismatch",F->Name.towchar(),C->Name.towchar(),F->Parameters[j]->Name.towchar());
				return CF;
			}
		}
	if(C->SuperClass)
		return GetSuperFunc(C->SuperClass,F);

	return NULL;
	unguard
}

void CompileFunction(_HFunc* F, HypBuffer& B)
{
	guard

	unguard
}

//Error handling
void Error(const wchar_t* err,...)
{
	wchar_t errb[128];
	va_list vl;
	va_start(vl, err);
	vswprintf(errb,err, vl);
	va_end(vl);
	Errors++;
	GLog->Err(L"%s at line:%u in file:%s \n ",errb,Line,CFile.filename.towchar());
	GLog->Log(L"=============== %i error(s), %i warning(s) ===============",Errors,Warnings);
	throw L"Failed compilation";
}
void Expect(const STRING& Token, const wchar_t* ToBe)
{
	if(appwcscmp_ci(Token.towchar(),ToBe))
		return;
	Error(L"Expected token '%s', got '%s'",ToBe,Token.towchar());
}
void ExpectIdentifier(const STRING& Token)
{
	if(IsLegalIdentifier(Token))
		return;
	Error(L"'%s' is not a valid identifier",Token.towchar());
}
void Warn(const wchar_t* err,...)
{
	wchar_t errb[128];
	va_list vl;
	va_start(vl, err);
	vswprintf(errb,err, vl);
	va_end(vl);
	Warnings++;
	GLog->Warn(L"%s at line:%i in file:%s \n ",errb,Line,CFile.filename.towchar());
}
void Verb(const wchar_t* err,...)
{
	wchar_t errb[128];
	va_list vl;
	va_start(vl, err);
	vswprintf(errb,err, vl);
	va_end(vl);
	GLog->Log(L"%s at line:%i in file:%s \n ",errb,Line,CFile.filename.towchar());
}

//For makefile
bool ParseBool()
{
	if((NextToken() == L"=") && (NextToken() |= L"True"))
		return true;
	return false;
}
//For makefile and source code
_PDef* ParsePreDef()
{
	guard
	//Is it redefined?
	_PDef* Def;

	auto itdef = PreDefs.find(hashName(NextToken().towchar())); //ID 1
	if( itdef != PreDefs.end())
	{
		Def = itdef->second;
		Def->Parameters.clear();
		Def->Tokens.clear();
		Warn(L"%s already defined, this will overwrite that definition",CurrentToken().towchar());
	}
	else
		Def = new _PDef;
	Def->Identifier = CurrentToken();
	int i=2;
	//Do we have parameters?
	if(NextToken() == L"(")
	{
		//Parse parameters, get the first parameter manually
		ExpectIdentifier(NextToken());
		Def->Parameters.push_back(CurrentToken());
		for(NextToken();CurrentToken() == L",";NextToken())
		{
			ExpectIdentifier(NextToken());
			Def->Parameters.push_back(CurrentToken());
		}
		Expect(CurrentToken(),L")");
		NextToken();
	}
	//Parse the rest of the tokens
	for(;TIndex<Tokens.size();++TIndex)
	{
		if(CurrentToken() == L"\\" && TIndex + 1 == Tokens.size())	//New line
			NextToken();
		Def->Tokens.push_back(CurrentToken());
	}
	--TIndex;
	return Def;
	unguardf(CurrentToken().towchar());
}

void ApplyPreDef( _PDef* def)
{
	guard
		//This assumes that the incoming arguments match.
		//CurrentToken() == def->Identifier
		//Create a copy of the definition tokens.
		std::vector<STRING> _PTokens = def->Tokens;
		int i = TIndex;
		
		if(def->Parameters.size() > 0 && NextToken() == L"(")//Do we have parameters?
		{
			int l = -1;//Parameters
			for(;CurrentToken() == L"," || CurrentToken() == L"(";NextToken())//Caller parameters to be inlined
			{
				++l;	//For the defined parameter
				NextToken();
				for(int k=0;k<_PTokens.size();++k)	//Search through the definition text for the tokens to be replaced.
				{
					if(_PTokens[k] |= def->Parameters[l])	//If we found a token to replace
					{
						//Check for operators. We may very well encounter stacked operators such as #A##example,
						//This means that we first have to deal with the ## operator and then the # operator. If one wishes to concat a stringitized token to another one can do so without
						//Having to use the concat operator, simply #A "Example" creating two strings next to eachother which will automatically be added together if next to eachother, and only if they are litterals.
						//Replace the token first
						_PTokens[k] = CurrentToken();
						if(k < (_PTokens.size()-1) && _PTokens[k+1] == L"##")
						{
							_PTokens.erase(_PTokens.begin() + k + 1);	//Remove the operator token
							_PTokens[k+1].appendStart(_PTokens[k].towchar());		//Concat
							_PTokens.erase(_PTokens.begin() + k);		//Remove the left hand operand
							++k;
						}
						if(k > 0 && _PTokens[k-1] == L"#")
						{
							_PTokens[k].append(L"\"");
							_PTokens[k].appendStart(L"\"");
							_PTokens.erase(_PTokens.begin() + k - 1);	//Remove the operator token
							--k;
						}
					}
				}
			}
			Expect(CurrentToken(),L")");
		}
		//Garbage should now be between i - j, and tokens should be filled in at i.
		Tokens.erase(Tokens.begin() + i, Tokens.begin() + TIndex + 1);	//Remove garbage
		Tokens.insert(Tokens.begin() + i,_PTokens.begin(),_PTokens.end());
		TIndex = i;
	unguard
}

//Parse a source file
void ParseFile(const STRING& FileName)
{
	guard
		CFile.Open(FileName.towchar(),FILE_READ);
		CU = new _HCompUnit;
		CU->Name = CFile.filename;
		GLog->Log(L"Parsing %s",CU->Name.towchar());
		Tokens.clear();
		TIndex = 0;
		Line = 0;
		std::vector<STRING> V;
		while(V.empty() && !CFile.Eol())
		{
			V = CFile.readLine().splitCode();
			++Line;
		}
		if(CFile.Eol())
			Error(L"Unexpected end of file");
		Tokens.insert(Tokens.end(),V.begin(),V.end());
		_PDef * P;
		if(isAlpha(Tokens[TIndex].towchar()) && (P = GetPreDef(Tokens[TIndex])) != NULL)
			ApplyPreDef(P);
		while(!CFile.Eol())
		{
			guard
			if(CurrentToken() |= L"#define")
			{
				_PDef* P = ParsePreDef();
				CU->PreDefs[hashName(P->Identifier.towchar())] = P;
			}
			else if(CurrentToken() |= L"#exec")
				Error(L"Exec not yet implemented");
			else
			{
				//We should have a class..
				CU->Classes.push_back(ParseCDecl());
			}
			unguardf(CurrentToken().towchar())
			NextTokenLazyIdentifier();
		}
		Units.insert(CU);
	unguard
}

_HClass* ParseCDecl()
{
	//Must support multi-line
	guard
		UINT Flags = 0;
		//Save token stack
		UINT CToken = TIndex;
		//Check access, don't assume it will be there.
		if(CurrentToken() |= L"public")
		{
			Flags |= C_PUBLIC;
			NextTokenIdentifier();
		}
		else if(CurrentToken() |= L"private")
		{
			Flags |= C_PRIVATE;
			NextTokenIdentifier();
		}
		else if(CurrentToken() |= L"protected")
		{
			Flags |= C_PROTECTED;
			NextTokenIdentifier();
		}
		else
		{
			Verb(L"No access level specified, public assumed.");
			Flags |= C_PUBLIC;
		}

		//Next parse class options
		while(!CFile.Eol())
		{
			if(CurrentToken() |= L"native")
			{
				if(Flags & C_NATIVE)	//!LOL
					Error(L"Native modifier duplicate");
				Flags |= C_NATIVE;
				NextTokenIdentifier();
			}
			else if(CurrentToken() |= L"static")
			{
				if(Flags & C_STATIC)	//!LOL
					Error(L"Static modifier duplicate");
				Flags |= C_STATIC;
				NextTokenIdentifier();
			}
			else if(CurrentToken() |= L"abstract")
			{
				if(Flags & C_ABSTRACT)	//!LOL
					Error(L"Abstract modifier duplicate");
				Flags |= C_ABSTRACT;
				NextTokenIdentifier();
			}
			else break;
		}
		if(!appwcscmp_ci(CurrentToken().towchar(),L"class"))
			Error(L"Unexpected token '%s'",CurrentToken());
		_HClass * C = new _HClass;
		C->Flags = Flags;
		C->Name = NextToken();
		if(!IsLegalIdentifier(C->Name))
			Error(L"%s is not a legal identifier",C->Name.towchar());
		C->ID = hashName(C->Name.towchar());

		if(!appwcscmp_ci(NextToken().towchar(),L"extends"))
		{
			if(!appwcscmp_ci(PrevToken(1).towchar(),L"Object"))
				Error(L"Class %s must subclass Object or other derivative",C->Name.towchar());
			else
			{
				C->Flags |= C_BASE;
				C->Super		= NULL;
				C->SuperClass	= NULL;
				if(!(C->Flags & C_NATIVE) || !(C->Flags & C_ABSTRACT))
					Error(L"Base class Object must be declared as native and abstract.");
			}
		}
		else	//Extends...
		{
			NextTokenIdentifier();
			//NextToken();
			if(!IsLegalIdentifier(CurrentToken().towchar()))
				Error(L"Expected identifier before token '%s'",CurrentToken().towchar());
			C->Super = hashName(CurrentToken().towchar());
			C->SuperName = CurrentToken();
			NextTokenIdentifier();
		}
		Expect(CurrentToken(),L"{");	//Opening bracket
		//To be continued..
		NextTokenIdentifier();
		_HFunc* F;
		C->NumVars = 0;
		while(!CFile.Eol() && CurrentToken() != L"}")
		{
			if(ParseVariable(C->Vars))
				++(C->NumVars);
			else if((F = ParseFunction()) != NULL)
			{
				C->Funcs.push_back(F);
			}
			else
				Error(L"Unexpected token: %s",CurrentToken().towchar());
			NextTokenLazyIdentifier();
		}

		Expect(CurrentToken(),L"}");
		Expect(NextToken(),L";");
		return C;
	unguardf(CurrentToken().towchar())
}
_HVar *ParseParameter()
{
	guard
	UINT i = TIndex;	//safe copy.
	_HType T;
	UINT B=0;
	//Look for modifiers
	while(!CFile.Eof())
	{
		if(CurrentToken() |= L"_pointer_")
			B |= AT_OUT;
		else if(CurrentToken() |= L"const")
			B |= AT_CONST;
		else break;
			//..
		NextTokenIdentifier();
	}
	//Get type info
	T = ParseType();
	T.Type |= B;
	if(T.Type == 0)//Definitly not a variable
	{
		LEIndex = TIndex;
		TIndex = i;
		return NULL;
	}
	NextTokenIdentifier();
	_HVar* V = new _HVar;
	V->ID = NULL;
	//Name is optional for declarations
	if(CurrentToken() != L"," && CurrentToken() != L")" )
	{
		if(IsLegalIdentifier(CurrentToken()))
		{
			
			V->Name = CurrentToken();
			V->ID = hashName(V->Name.towchar());
		}
		else
		{
			delete V;
			Error(L"Expected a ')' or ',', got '%s'",CurrentToken().towchar());
		}
	}
	else --TIndex;
	V->Type = T;
	return V;
	unguardf(CurrentToken().towchar())
}
bool ParseVariable(std::vector<_HVar*> &Vars)
{
	guard
	UINT i = TIndex;	//safe copy.
	_HType T;
	UINT B=0;
	//Look for modifiers
	while(!CFile.Eof())
	{
		if(CurrentToken() |= L"static")
			B |= AT_STATIC;
		else if(CurrentToken() |= L"native"){}
		else break;
			//..
		NextTokenIdentifier();
	}
	//Get type info
	T = ParseType();
	T.Type |= B;
	if(T.Type == 0)//Definitly not a variable
	{
		LEIndex = TIndex;
		TIndex = i;
		return false;
	}
	NextTokenIdentifier();
	if( !IsLegalIdentifier(CurrentToken()) || (NextToken() != L"," && CurrentToken() != L";" && CurrentToken() != L"=") )
	{
		LEIndex = TIndex;
		TIndex = i;
		return false;
	}
	--TIndex;
	_HVar* V = new _HVar;
	V->Line = Line;
	V->Type = T;
	V->Name = CurrentToken();
	V->ID = hashName(V->Name.towchar());
	Vars.push_back(V);
	if(NextTokenIdentifier() == L"=")
	{
		if(IsLegalIdentifier(NextTokenIdentifier()) || IsLitteral(CurrentToken()))
			V->InitValue = CurrentToken();
		else Error(L"Token %s is not a literal value.",CurrentToken().towchar());
		NextTokenIdentifier();
	}
	if(CurrentToken() == L",")
	{
		while(CurrentToken() == L",")
		{
			ExpectIdentifier(NextTokenIdentifier());
			V = new _HVar;
			V->ID = hashName(CurrentToken().towchar());
			V->Name = CurrentToken();
			Vars.push_back(V);
			if(NextTokenIdentifier() == L"=")
			{
				if(IsLegalIdentifier(NextTokenIdentifier()) || IsLitteral(CurrentToken()))
					V->InitValue = CurrentToken();
				else Error(L"Token %s is not a literal value.",CurrentToken().towchar());
				NextTokenIdentifier();
			}
		}
	}
	Expect(CurrentToken(),L";");
	
	return true;
	unguardf(CurrentToken().towchar())
}
_HFunc* ParseFunction()
{
	guard
		UINT i = TIndex;	//safe copy.
	_HType T;
	UINT B=0;
	//Look for modifiers
	while(!CFile.Eof())
	{
		if(CurrentToken() |= L"static")
			B |= F_STATIC;
		else if(CurrentToken() |= L"native")
			B |= F_NATIVE;
		else if(CurrentToken() |= L"final")
			B |= F_FINAL;
		else if(CurrentToken() |= L"operator")
			B |= F_OPERATOR;
		else break;
			//..
		NextTokenIdentifier();
	}
	//Get type info
	T = ParseType();
	T.Type |= B;
	if(T.Type == 0)
	{
		LEIndex = TIndex;
		TIndex = i;
		return NULL;
	}
	NextTokenIdentifier();
	if(
		(!IsLegalIdentifier(CurrentToken()) && !IsOperator(CurrentToken())) 
		|| NextToken() != L"(")
	{
		LEIndex = TIndex;
		TIndex = i;
		return NULL;
	}
	_HFunc* F = new _HFunc;
	F->Name = PrevToken(1);
	F->Type = T;
	F->Line = Line;
	if(F->Type.Type & F_OPERATOR && !IsOperator(F->Name))
		Error(L"Function declared as operator must have operator identifier, not '%s'",F->Name.towchar());
	bool bDecOnly = false;
	//Parse parameters
	if(NextTokenIdentifier() != L")")
	{
		_HVar* P;
		--TIndex;
		while(CurrentToken() == L"(" || CurrentToken() == L",")
		{
			NextTokenIdentifier();
			if((P = ParseParameter()) != NULL)
			{
				F->Parameters.push_back(P);
				if(P->ID == NULL)
					bDecOnly = true;
			}
			else
				break;
			NextTokenIdentifier();
		}
	}//Expect to have ')' here
	Expect(CurrentToken(),L")");
	NextTokenIdentifier();
	if(bDecOnly)
		Expect(CurrentToken(),L";");
	if(CurrentToken() == L";")
		return F;

	//Now get the function body
	Expect(CurrentToken(),L"{");
	NextTokenIdentifier();
	//Scope index
	UINT Scope = 0;
	F->NumLocals = 0;
	_HExprUnit* U;
	while( CurrentToken() != L"}" || Scope != 0 )
	{
		if(CurrentToken() == L"{")
			++Scope;
		else if(CurrentToken() == L"}")
			--Scope;
		else if(ParseVariable(F->Locals))
			++(F->NumLocals);
		else if((U = ParseIf(F)) != NULL)
		{
			F->Expressions.push_back(U);
			PrintExpressions(U);
			std::wcout << ENDL;
			--TIndex;
		}
		else if((U = ParseWhile(F)) != NULL)
		{
			F->Expressions.push_back(U);
			PrintExpressions(U);
			std::wcout << ENDL;
			--TIndex;
		}
		else if((U = ParseDo(F)) != NULL)
		{
			F->Expressions.push_back(U);
			PrintExpressions(U);
			std::wcout << ENDL;
			--TIndex;
		}
		else if((U = ParseFor(F)) != NULL)
		{
			F->Expressions.push_back(U);
			PrintExpressions(U);
			std::wcout << ENDL;
			--TIndex;
		}
		else if((U = ParseExpression(NULL,false,false,true)) != NULL)
		{
			F->Expressions.push_back(U);
			PrintExpressions(U);
			Expect(CurrentToken(),L";");
			std::wcout << ENDL;
		}
		else
			Error(L"Unexpected token %s",CurrentToken().towchar());
		//F->Tokens.push_back(CurrentToken());
		NextTokenIdentifier();
	}
	Expect(CurrentToken(),L"}");
	return F;
	unguard
}
_HExprUnit* ParseExpression(_HExprUnit* E, bool bUnary, bool bLSE, bool bFirst)
{
	guard

	UINT in = TIndex;
	_HExprUnit* U = NULL;
	USHORT EXPR = 0;
	void* L=0, *R=0;

	if(E != NULL)
	{
			L = E;
			EXPR |= bLSE;
			goto Next;
	}
	//See if we have a LH operand
	//First check for a normal expression
	guard
	if(IsLegalIdentifier(CurrentToken()) || IsLitteral(CurrentToken())) //( A & B )
	{
		//Start by parsing the LH operand
		L = new STRING(CurrentToken());
		EXPR |= EXP_LSE;
		NextTokenIdentifier();
	}
	else if(CurrentToken() == L"(" )	// (--A) && B, (A--) && B, (A[i]) && B, (A()) && B
	{
		if(!IsTransparent())	// (--A) is safe to parse here
		{
			NextTokenIdentifier();
			L = ParseExpression();
			Expect(CurrentToken(),L")");
			NextTokenIdentifier();
			if(CurrentToken() == L";")
				return (_HExprUnit*)L;
			if(!L)
				Error(L"Illegal expression before token.");
		}
		else if(IsLegalIdentifier(CurrentToken()) || IsLitteral(CurrentToken()))
		{
			NextTokenIdentifier();
			L = new STRING(CurrentToken());
			EXPR |= EXP_LSE;
			Expect(NextTokenIdentifier(),L")");
			NextTokenIdentifier();
			if(CurrentToken() == L";")
				Error(L"Unexpected end of expression");
		}
		else Error(L"Illegal token %s",CurrentToken().towchar());
	}
	//implicit expressions without parenthesis!
	// --A && B, A-- && B, A[i] && B, A() && B... A && B && C && D || E == (A && B) && (C && D) || E))))
	else if(IsOperator(CurrentToken()))
	{
		//Some left unary
		if(IsUnaryOperator(CurrentToken()))
		{
			_HExprUnit* J = new _HExprUnit();
			J->Line = Line;
			J->Op = CurrentToken();
			//J->ExprT = EXP_RSE;
			J->ExprT |= EXP_UNARY;
			/*if(NextTokenIdentifier() != L"(")
			{
				ExpectIdentifier(CurrentToken());
				J->RUnit = new STRING(CurrentToken());
				J->ExprT |= EXP_RSE;
				NextTokenIdentifier();
			}
			else*/
			NextTokenIdentifier();
			J->RUnit = ParseExpression(NULL,true);

			//(--A);? Every closing parenthesis will cause a return of the current expression.
			if(CurrentToken() == L")" || CurrentToken() == L";" || CurrentToken() == L"]")
				return J;	//Single expression
			L = J;
		}
		else
			Error(L"Unexpected operator");
	}
	unguardf(L"Prologe")
	Next:
	
	guard
	//Function call
	if(CurrentToken() == L"(")
	{
		U = new _HFuncExpr();
		U->Line = Line;
		U->ExprT = EXPR;
		U->ExprT |= EXP_FUNC;
		U->LUnit = L;	//Function name
		if(!L)
			Error(L"Expected a left operand before operator");
		while(CurrentToken() != L")")
		{
			NextTokenIdentifier();
			if(CurrentToken() == L")")
				break;
			//We want an identifier, expression, or a litteral as an argument
			_HExprUnit* Ar= ParseExpression();
			if(!Ar)
				Error(L"Unrecognized expression in function argument");
			((_HFuncExpr*)U)->Parameters.push_back(Ar);
			if(CurrentToken() != L",")
				Expect(CurrentToken(),L")");
		}
		NextTokenIdentifier();
		if(IsOperator(CurrentToken()))
		{
			_HExprUnit* Un = new _HExprUnit();
			Un->Line = Line;
			Un->LUnit = U;
			U = Un;
		}
	//	if(NextTokenIdentifier() == L";")
	//		return U;
	}
	else if(IsRightUnaryOperator(CurrentToken()))
	{
		U = new _HExprUnit();
		U->Line = Line;
		U->ExprT = EXPR;
		U->LUnit = L;	//Expect this not to be null
		if(!L)
			Error(L"Expected a left operand before operator");
		if(CurrentToken() == L"[")
		{
			if(NextTokenIdentifier() == L"]")
				Error(L"Expected operand before token.");
			if(CurrentToken() == L"(" && IsTransparent())
				Error(L"Parenthesis with no more arguments is illegal here.");
			if(NextTokenIdentifier() != L"]")
			{
				--TIndex;
				U->RUnit = ParseExpression();//TODO:MUST BE FIXED!
			}
			else
			{
				--TIndex;
				U->RUnit = new STRING(CurrentToken());
				U->ExprT |= EXP_RSE;
				NextTokenIdentifier();
			}
			if(!U->RUnit)
				Error(L"Illegal expression before token.");
			Expect(CurrentToken(),L"]");
			U->Op = L"[]";
		}
		else
		{
			U->Op = CurrentToken();
			U->ExprT |= EXP_LSE;
		}
		NextTokenIdentifier();
		if(IsOperator(CurrentToken()))
		{
			_HExprUnit* Un = new _HExprUnit();
			Un->Line = Line;
			Un->LUnit = U;
			U = Un;
	
		}
	}
	unguardf(L"Post operator prologe")
	guard
	if(CurrentToken() == L"." && bFirst)
	{
		if(!U)
			U = new _HExprUnit();
		U->Line = Line;
		NextTokenIdentifier();
		TIndex = in;
		U->LUnit = ParseExpression();
		//((_HExprUnit*)U->RUnit)->ExprT |= ((U->ExprT & EXP_RSE) >> 1);
		U->ExprT &= ~EXP_LSE;
	}
	if((bUnary && CurrentToken() != L"." ))
	{
		if(!U)
		{
			U = new _HExprUnit();
			U->Line = Line;
		}
		if(!U->LUnit)
		{
			U->LUnit = L;	//Expect this not to be null
			U->ExprT = EXPR;
		}
		return U;
	}
	if(IsOperator(CurrentToken()))
	{
		if(!U)
		{
			U = new _HExprUnit();
			U->Line = Line;
		}
		if(!U->LUnit)
		{
			U->LUnit = L;	//Expect this not to be null
			U->ExprT = EXPR;
		}
		if(!L)
			Error(L"Expected a left operand before operator");
		U->Op = CurrentToken();
	}
	else 
	{
		if(!U)
		{
			U = new _HExprUnit();
			U->ExprT = EXPR;
			U->LUnit = L;
		}
		return U;

	}
	NextTokenIdentifier();
	unguardf(L"Operator")
	guard
	//R-Side, at this point we only check for errors since we know it's at least part of an expression.
	if(IsLegalIdentifier(CurrentToken()) || IsLitteral(CurrentToken())) //( A & B )
	{
		//Start by parsing the LH operand
		U->RUnit = new STRING(CurrentToken());
		U->ExprT |= EXP_RSE;
		NextTokenIdentifier();
	}
	else if(CurrentToken() == L"(" )	// (--A) && B, (A--) && B, (A[i]) && B, (A()) && B
	{
		if(!IsTransparent())	// (--A) is safe to parse here
		{
			NextTokenIdentifier();
			U->RUnit = ParseExpression(); 
			Expect(CurrentToken(),L")");
			NextTokenIdentifier();
			if(!U->RUnit)
				Error(L"Illegal expression before token.");
		}
		else if(IsLegalIdentifier(CurrentToken()) || IsLitteral(CurrentToken()))
		{
			NextTokenIdentifier();
			U->RUnit = new STRING(CurrentToken());
			U->ExprT |= EXP_RSE;
			Expect(NextTokenIdentifier(),L")");
			NextTokenIdentifier();
		}
		else Error(L"Illegal token %s",CurrentToken().towchar());
	}
	else if(IsOperator(CurrentToken()))
	{
		//Some left unary
		if(IsUnaryOperator(CurrentToken()))
		{
			_HExprUnit* J = new _HExprUnit();
			J->Line = Line;
			J->Op = CurrentToken();
			//J->ExprT = EXP_RSE;
			J->ExprT |= EXP_UNARY;
			/*if(NextTokenIdentifier() != L"(")
			{
				ExpectIdentifier(CurrentToken());
				J->RUnit = new STRING(CurrentToken());
				J->ExprT |= EXP_RSE;
				NextTokenIdentifier();
			}
			else*/
			NextTokenIdentifier();
			J->RUnit = ParseExpression(NULL,true);

			//(--A);? Every closing parenthesis will cause a return of the current expression.
			//if(CurrentToken() == L")" || CurrentToken() == L";" || CurrentToken() == L"]")
			//	return J;	//Single expression
			U->RUnit = J;
		}
		else
			Error(L"Unexpected operator");
	}
	else
		Error(L"Expected identifier or literal, got: %s",CurrentToken().towchar());
	unguardf(L"Epiloge")
	guard
	//Epiloge post-op
	if(CurrentToken() == L"(")
	{
		R = U->RUnit;
		U->RUnit = new _HFuncExpr();
		((_HExprUnit*)U->RUnit)->Line = Line;
		U->ExprT &= ~EXP_RSE;
		//Implemet function calls from array variables here..
		((_HExprUnit*)U->RUnit)->ExprT = (EXP_LSE | EXP_FUNC);

		((_HExprUnit*)U->RUnit)->LUnit = R;	//Function name
		if(!R)
			Error(L"Expected a left operand before operator");
		while(CurrentToken() != L")")
		{
			NextTokenIdentifier();
			if(CurrentToken() == L")")
				break;
			//We want an identifier, or a litteral as an argument
			_HExprUnit *Ar= ParseExpression();
			if(!Ar)
				Error(L"Unrecognized expression in function argument");
			((_HFuncExpr*)U->RUnit)->Parameters.push_back(Ar);
			if(CurrentToken() != L",")
				Expect(CurrentToken(),L")");
		}
		NextTokenIdentifier();
	}
	else if(IsRightUnaryOperator(CurrentToken()))
	{
		R = U->RUnit;
		U->RUnit = new _HExprUnit();
		((_HExprUnit*)U->RUnit)->Line = Line;
		((_HExprUnit*)U->RUnit)->ExprT = (EXP_UNARY | ((U->ExprT & EXP_RSE) >> 1) );
		U->ExprT &= ~EXP_RSE;
		((_HExprUnit*)U->RUnit)->LUnit = R;	//Expect this not to be null
		if(!R)
			Error(L"Expected a left operand before operator");
		if(CurrentToken() == L"[")
		{
			if(NextTokenIdentifier() == L"]")
				Error(L"Expected operand before token.");
			//else if(CurrentToken() == L"(")
			//	Error(L"Parenthesis is illegal here.");
			if(CurrentToken() == L"(" && IsTransparent())
				Error(L"Parenthesis with no more arguments is illegal here.");
			if(NextTokenIdentifier() != L"]")
			{
				--TIndex;
				((_HExprUnit*)U->RUnit)->RUnit = ParseExpression();//TODO:MUST BE FIXED!
			}
			else
			{
				--TIndex;
				((_HExprUnit*)U->RUnit)->RUnit = new STRING(CurrentToken());
				((_HExprUnit*)U->RUnit)->ExprT |= EXP_RSE;
				NextTokenIdentifier();
			}
			if(!((_HExprUnit*)U->RUnit)->RUnit)
				Error(L"Illegal expression before token.");
			Expect(CurrentToken(),L"]");
			((_HExprUnit*)U->RUnit)->Op = L"[]";
		}
		else
		{
			((_HExprUnit*)U->RUnit)->Op = CurrentToken();
			//((_HExprUnit*)U->RUnit)->ExprT |= EXP_RSE;
		}
		NextTokenIdentifier();
	}
	unguardf(L"Epilog post operation");
	//Check for continuation.. (A && B && C)
	//if(IsOperator(CurrentToken()))//TODO: distinguish boolean types with integer ones
	//	return ParseExpression(U);
	//else if(CurrentToken() == L")" || CurrentToken() == L"]" || CurrentToken() == L";" || CurrentToken() == L",")
	//	return U;
	
	
	if( ((IsValueOperator(CurrentToken()) || IsCmpOperator(CurrentToken())  ) && !U->Op.isEmpty() && IsBoolOperator(U->Op)) || U->Op == L"=" || (CurrentToken() == L"." && U->Op != L"."  /*&& U->Op != L"&&"*/) )
	{
		//if(U->RUnit)
	//	{
	//		if(U->ExprT & EXP_RSE)
	//			delete (STRING*)U->RUnit;
			//else
			//	delete (_HExprUnit*)U->RUnit;
			//U->ExprT *
	//	}
	//	--TIndex;
		U->RUnit = ParseExpression(U->RUnit?(_HExprUnit*)U->RUnit:E,bUnary,((U->ExprT & EXP_RSE) >> 1));
		//((_HExprUnit*)U->RUnit)->ExprT |= ((U->ExprT & EXP_RSE) >> 1);
		U->ExprT &= ~EXP_RSE;
		if(/*IsBoolOperator(U->Op) && !*/IsCmpOperator(U->Op))
			return U;
	}
	if(CurrentToken() == L")" || CurrentToken() == L"]" || CurrentToken() == L";" || CurrentToken() == L"," || ( IsBoolOperator(CurrentToken()) && !IsCmpOperator(CurrentToken()) && ((IsValueOperator(U->Op) || IsCmpOperator(U->Op)) /*&& U->Op != L"."*/  ) ) || ( (bUnary || U->Op == L".") && CurrentToken() != L"." ))
		return U;
	else
		return ParseExpression(U,bUnary);
	unguard
}
_HIFExpr* ParseIf(_HFunc* F)
{
	guard
		_HIFExpr* IF = 0;
		UINT in = TIndex;
		if(CurrentToken() |= L"if")
		{
			IF = new _HIFExpr();
			IF->ExprT |= EXP_IFCLUSTER;
			_HIFExpr* CIF = new _HIFExpr();
			CIF->ExprT |= EXP_IF;
			IF->Expressions.push_back(CIF);
			Expect(NextTokenIdentifier(), L"(");
			NextTokenIdentifier();
			if(!IsTransparent())
				CIF->LUnit = ParseExpression();
			else if(IsLegalIdentifier(CurrentToken()))
			{
				CIF->LUnit = new STRING(CurrentToken());
				CIF->ExprT = EXP_LSE;
				NextTokenIdentifier();
			}
			Expect(CurrentToken(),L")");
			_HExprUnit* U = NULL;
			if(NextTokenIdentifier() != L"{")
			{
				if((U = ParseIf(F)) != NULL)
				{
					CIF->Expressions.push_back(U);
				}
				else if((U = ParseExpression(NULL,false,false,true)) != NULL)
				{
					CIF->Expressions.push_back(U);
					Expect(CurrentToken(),L";");	//TODO: Fix cases for (A && B); expressions.
					NextTokenIdentifier();
				}
				else
					Error(L"Unexpected token %s",CurrentToken().towchar());
			}
			else
			{
				NextTokenIdentifier();

				UINT Scope = 0;
				_HExprUnit* U;
				while( CurrentToken() != L"}" || Scope != 0 )
				{
					if(CurrentToken() == L"{")
						++Scope;
					else if(CurrentToken() == L"}")
						--Scope;
					//else if(ParseVariable(F->Locals))
					//	++(F->NumLocals);
					else if((U = ParseIf(F)) != NULL)
					{
						CIF->Expressions.push_back(U);
						continue;
					}
					else if((U = ParseExpression(NULL,false,false,true)) != NULL)
					{
						CIF->Expressions.push_back(U);
						Expect(CurrentToken(),L";");	//TODO: Fix cases for (A && B); expressions.
						//NextTokenIdentifier();
					}
					else
						Error(L"Unexpected token %s",CurrentToken().towchar());
					//F->Tokens.push_back(CurrentToken());
					NextTokenIdentifier();
				}
				Expect(CurrentToken(),L"}");
				NextTokenIdentifier();
			}
		}
		JL001:
		if(CurrentToken() |= L"else" )
		{
			if(!IF)
				Error(L"'else' was not paired with initial 'if'");
			if(NextTokenIdentifier() |= L"if")
			{
				_HIFExpr* CIF = new _HIFExpr();
				CIF->ExprT |= EXP_IFELSE;
				IF->Expressions.push_back(CIF);
				Expect(NextTokenIdentifier(), L"(");
				NextTokenIdentifier();
				if(!IsTransparent())
					CIF->LUnit = ParseExpression();
				else if(IsLegalIdentifier(CurrentToken()))
				{
					CIF->LUnit = new STRING(CurrentToken());
					NextTokenIdentifier();
				}
				Expect(CurrentToken(),L")");
				_HExprUnit* U = NULL;
				if(NextTokenIdentifier() != L"{")
				{
					if((U = ParseIf(F)) != NULL)
					{
						CIF->Expressions.push_back(U);
					}
					else if((U = ParseExpression(NULL,false,false,true)) != NULL)
					{
						CIF->Expressions.push_back(U);
						Expect(CurrentToken(),L";");	//TODO: Fix cases for (A && B); expressions.
						NextTokenIdentifier();
					}
					else
						Error(L"Unexpected token %s",CurrentToken().towchar());
					
				}
				else
				{
					NextTokenIdentifier();

					UINT Scope = 0;
					_HExprUnit* U;
					while( CurrentToken() != L"}" || Scope != 0 )
					{
						if(CurrentToken() == L"{")
							++Scope;
						else if(CurrentToken() == L"}")
							--Scope;
						//else if(ParseVariable(F->Locals))
						//	++(F->NumLocals);
						else if((U = ParseIf(F)) != NULL)
						{
							CIF->Expressions.push_back(U);
							continue;
						}
						else if((U = ParseExpression(NULL,false,false,true)) != NULL)
						{
							CIF->Expressions.push_back(U);
							Expect(CurrentToken(),L";");
							
						}
						else
							Error(L"Unexpected token %s",CurrentToken().towchar());
						//F->Tokens.push_back(CurrentToken());
						NextTokenIdentifier();
					}
					Expect(CurrentToken(),L"}");
					NextTokenIdentifier();
				}
				goto JL001;
			}
			else
			{
				_HIFExpr* CIF = new _HIFExpr();
				CIF->ExprT |= EXP_ELSE;
				IF->Expressions.push_back(CIF);
				_HExprUnit* U = NULL;
				if(CurrentToken() != L"{")
				{
					if((U = ParseIf(F)) != NULL)
					{
						CIF->Expressions.push_back(U);
					}
					else if((U = ParseExpression(NULL,false,false,true)) != NULL)
					{
						CIF->Expressions.push_back(U);
						Expect(CurrentToken(),L";");	//TODO: Fix cases for (A && B); expressions.
						NextTokenIdentifier();
					}
					else
						Error(L"Unexpected token %s",CurrentToken().towchar());
				}
				else
				{
					NextTokenIdentifier();

					UINT Scope = 0;
					_HExprUnit* U;
					while( CurrentToken() != L"}" || Scope != 0 )
					{
						if(CurrentToken() == L"{")
							++Scope;
						else if(CurrentToken() == L"}")
							--Scope;
						//else if(ParseVariable(F->Locals))
						//	++(F->NumLocals);
						else if((U = ParseIf(F)) != NULL)
						{
							CIF->Expressions.push_back(U);
							continue;
						}
						else if((U = ParseExpression(NULL,false,false,true)) != NULL)
						{
							CIF->Expressions.push_back(U);
							Expect(CurrentToken(),L";");	//TODO: Fix cases for (A && B); expressions.
							//NextTokenIdentifier();
						}
						else
							Error(L"Unexpected token %s",CurrentToken().towchar());
						//F->Tokens.push_back(CurrentToken());
						NextTokenIdentifier();
					}
					Expect(CurrentToken(),L"}");
					NextTokenIdentifier();
				}
			}
		}
		if(!IF)
			TIndex = in;
		return IF;
	unguard
}
_HWhileExpr* ParseWhile(_HFunc* F)
{
	guard
		_HWhileExpr* U = NULL;
		if(CurrentToken() |= "while")
		{
			guard
			Expect(NextTokenIdentifier(),L"(");
			NextTokenIdentifier();
			U = new _HWhileExpr();
			U->LUnit = ParseExpression(NULL,false,false,true);
			U->ExprT |= EXP_WHILE;
			if(!U->LUnit)
				Error(L"Expression expected in while loop");
			Expect(CurrentToken(),L")");
			Expect(NextTokenIdentifier(),L"{");
			NextTokenIdentifier(); 
			UINT Scope = 0;
			_HExprUnit* UC;
			while( CurrentToken() != L"}" || Scope != 0 )
			{
				if(CurrentToken() == L"{")
					++Scope;
				else if(CurrentToken() == L"}")
					--Scope;
				//else if(ParseVariable(F->Locals))
				//	++(F->NumLocals);
				else if((UC = ParseIf(F)) != NULL)
				{
					U->Expressions.push_back(U);
					continue;
				}
				else if((UC = ParseExpression(NULL,false,false,true)) != NULL)
				{
					U->Expressions.push_back(UC);
					Expect(CurrentToken(),L";");	//TODO: Fix cases for (A && B); expressions.
					//NextTokenIdentifier();
				}
				else
					Error(L"Unexpected token %s",CurrentToken().towchar());
				//TODO: add rest of the parsers
				NextTokenIdentifier();
			}
			Expect(CurrentToken(),L"}");
			NextTokenIdentifier();
			unguardf(L"parse while")
		}
		return U;
	unguard
}
_HForExpr* ParseFor(_HFunc* F)
{
	guard
		_HForExpr* U = NULL;
		if(CurrentToken() |= L"for")
		{
			guard
			Expect(NextTokenIdentifier(),L"(");
			U = new _HForExpr();
			U->ExprT |= EXP_FOR;
			if(NextTokenIdentifier() != L";")
			{
				if(!ParseVariable(F->Locals))
					Error(L"Unexpected token '%s'",CurrentToken().towchar());
				Expect(CurrentToken(),L";");
			}
			if(NextTokenIdentifier() != L";")
			{
				if((U->LUnit = ParseExpression(NULL,false,false,true)) == NULL)
					Error(L"Expected conditional expression");
				Expect(CurrentToken(),L";");
			}
			if(NextTokenIdentifier() != L";")
			{
				if((U->RUnit = ParseExpression(NULL,false,false,true)) == NULL)
					Error(L"Expected third expression");
			}
			Expect(CurrentToken(),L")");
			Expect(NextTokenIdentifier(),L"{");
			NextTokenIdentifier(); 
			UINT Scope = 0;
			_HExprUnit* UC;
			while( CurrentToken() != L"}" || Scope != 0 )
			{
				if(CurrentToken() == L"{")
					++Scope;
				else if(CurrentToken() == L"}")
					--Scope;
				//else if(ParseVariable(F->Locals))
				//	++(F->NumLocals);
				else if((UC = ParseIf(F)) != NULL)
				{
					U->Expressions.push_back(U);
					continue;
				}
				else if((UC = ParseExpression(NULL,false,false,true)) != NULL)
				{
					U->Expressions.push_back(UC);
					Expect(CurrentToken(),L";");	//TODO: Fix cases for (A && B); expressions.
					//NextTokenIdentifier();
				}
				else
					Error(L"Unexpected token %s",CurrentToken().towchar());
				//TODO: add rest of the parsers
				NextTokenIdentifier();
			}
			Expect(CurrentToken(),L"}");
			NextTokenIdentifier();
			unguardf(L"Parse for loop")
		}
		return U;
	unguard
}
_HWhileExpr* ParseDo(_HFunc* F)
{
	guard
		_HWhileExpr* U = NULL;
		if(CurrentToken() |= L"do")
		{
			Expect(NextTokenIdentifier(),L"{");
			NextTokenIdentifier();
			U = new _HWhileExpr();
			UINT Scope = 0;
			_HExprUnit* UC;
			while( CurrentToken() != L"}" || Scope != 0 )
			{
				if(CurrentToken() == L"{")
					++Scope;
				else if(CurrentToken() == L"}")
					--Scope;
				//else if(ParseVariable(F->Locals))
				//	++(F->NumLocals);
				else if((UC = ParseIf(F)) != NULL)
				{
					U->Expressions.push_back(U);
					continue;
				}
				else if((UC = ParseExpression(NULL,false,false,true)) != NULL)
				{
					U->Expressions.push_back(UC);
					Expect(CurrentToken(),L";");	//TODO: Fix cases for (A && B); expressions.
					//NextTokenIdentifier();
				}
				else
					Error(L"Unexpected token %s",CurrentToken().towchar());
				//TODO: add rest of the parsers
				NextTokenIdentifier();
			}
			Expect(CurrentToken(),L"}");
			NextTokenIdentifier();
			if(CurrentToken() |= L"while")
			{
				Expect(NextTokenIdentifier(),L"(");
				NextTokenIdentifier();
				U->LUnit = ParseExpression(NULL,false,false,true);
				U->ExprT |= EXP_DOWHILE;
				if(!U->LUnit)
				Error(L"Expression expected in while loop");
				Expect(CurrentToken(),L")");
			}
			else if(CurrentToken() |= L"until")
			{
				Expect(NextTokenIdentifier(),L"(");
				NextTokenIdentifier();
				U->LUnit = ParseExpression(NULL,false,false,true);
				U->ExprT |= EXP_DOUNTIL;
				if(!U->LUnit)
				Error(L"Expression expected in while loop");
				Expect(CurrentToken(),L")");
			}
			else
				Error(L"Expected while/until after 'do'");
			NextTokenIdentifier();
		}
		return U;
	unguard
}

//Handle illegal combinations
_HType ParseType()
{
	guard
		_HType N;
		N.Type = 0;
		N.TypeID = 0;
		if(CurrentToken() |= L"unsigned")
		{
			NextTokenIdentifier();
			if((CurrentToken() |= L"double") 
				|| (CurrentToken() |= L"byte") 
				|| (CurrentToken() |= L"string")
				|| (CurrentToken() |= L"class")
				|| (CurrentToken() |= L"bool"))
				Error(L"unsigned can't be combined with double, byte or any object type");
			N.Type |= AT_UNSIGNED;
		}
		if(CurrentToken() |= L"int")
			N.Type |= AT_INT;
		else if(CurrentToken() |= L"int64")
			N.Type |= AT_INT64;
		else if(CurrentToken() |= L"short")
			N.Type |= AT_SHORT;
		else if(CurrentToken() |= L"byte" )
			N.Type |= AT_BYTE;
		else if(CurrentToken() |= L"bool" )
			N.Type |= AT_BOOL;
		else if(CurrentToken() |= L"double")
			N.Type |= AT_DOUBLE;
		else if(CurrentToken() |= L"void")
			N.Type |= AT_VOID;
		//Object types
		else if(CurrentToken() |= L"string")
			N.Type |= AT_STRING;
		else if(CurrentToken() |= L"class")
		{
			if(NextToken() == L"<")
			{
				ExpectIdentifier(NextToken());	//Needs a class
				N.TypeID = hashName(CurrentToken().towchar());
				Expect(NextToken(),L">");
			}
			N.Type |= AT_CLASS;
		}
		else if(IsLegalIdentifier(CurrentToken()))
		{
			N.Type |= AT_OBJECT;
			N.TypeID = hashName(CurrentToken().towchar());
			if(N.Type & AT_UNSIGNED)
				Error(L"Illegal combination of unsigned and class instance");
		}
		return N;
	unguard
}
_PDef* GetPreDef(const STRING& S)
{
	guard
		HName N = hashName(S.towchar());
		auto it = PreDefs.find(N);
		if(it == PreDefs.end() && CU != NULL)
		{
			it = CU->PreDefs.find(N);
			if(it != CU->PreDefs.end())
				return it->second;
			return NULL;
		}
		return it->second;
	unguard
}

const STRING& CurrentToken()
{
	assert(TIndex >= 0 && TIndex < Tokens.size());
	return Tokens[TIndex];
}
const STRING& NextToken()
{
	guard
	++TIndex;
	while(TIndex >= Tokens.size())	//Pump more tokens
	{
		STRING S = CFile.readLine();
		if(CFile.Eol())
			Error(L"Unexpected end of file.");
		++Line;
		while(S.isEmpty())
		{
			//Skip empty lines
			S = CFile.readLine();
			++Line;
			if(CFile.Eol())
				Error(L"Unexpected end of file.");
		}
		std::vector<STRING> ST = S.splitCode();
		Tokens.insert(Tokens.end(),ST.begin(),ST.end());
	}
	return Tokens[TIndex];
	unguard
}
const STRING& NextTokenIdentifier()
{
	//Almost the same but we check every identifier token to the Definition database.
	guard
	++TIndex;
	while(TIndex >= Tokens.size())	//Pump more tokens
	{
		STRING S = CFile.readLine();
		if(CFile.Eol())
			return EMPTYSTR;
		++Line;
		while(S.isEmpty())
		{
			//Skip empty lines
			S = CFile.readLine();
			++Line;
			if(CFile.Eol())
				return EMPTYSTR;
		}
		std::vector<STRING> ST = S.splitCode();
		Tokens.insert(Tokens.end(),ST.begin(),ST.end());
	}
	_PDef * P;
	if(isAlpha(Tokens[TIndex].towchar()) && (P = GetPreDef(Tokens[TIndex])) != NULL)
		ApplyPreDef(P);

	return Tokens[TIndex];
	unguard
}
const STRING& NextTokenLazy()
{
	guard
	++TIndex;
	while(TIndex >= Tokens.size())	//Pump more tokens
	{
		STRING S = CFile.readLine();
		if(CFile.Eol())
			return EMPTYSTR;
		++Line;
		while(S.isEmpty())
		{
			//Skip empty lines
			S = CFile.readLine();
			++Line;
			if(CFile.Eol())
				return EMPTYSTR;
		}
		std::vector<STRING> ST = S.splitCode();
		Tokens.insert(Tokens.end(),ST.begin(),ST.end());
	}
	return Tokens[TIndex];
	unguard
}
const STRING& NextTokenLazyIdentifier()
{
	guard
	++TIndex;
	while(TIndex >= Tokens.size())	//Pump more tokens
	{
		STRING S = CFile.readLine();
		if(CFile.Eol())
			return EMPTYSTR;
		++Line;
		while(S.isEmpty())
		{
			//Skip empty lines
			S = CFile.readLine();
			++Line;
			if(CFile.Eol())
				return EMPTYSTR;
		}
		std::vector<STRING> ST = S.splitCode();
		Tokens.insert(Tokens.end(),ST.begin(),ST.end());
	}
	_PDef * P;
	if(isAlpha(Tokens[TIndex].towchar()) && (P = GetPreDef(Tokens[TIndex])) != NULL)
		ApplyPreDef(P);
	return Tokens[TIndex];
	unguard
}
const STRING& PrevToken(INT i)
{
	assert(TIndex - i >= 0);
	return Tokens[TIndex - i];
}

bool IsLegalIdentifier(const STRING &Token)
{
	if(Token.beginsWith(L"0x") || Token.beginsWith(L"0b"))
		return false;
	if(Token.at(0) > L'0' && Token.at(0) < L'9')
		return false;
	if(Token.length() > 32)
		return false;
	if(!isAlpha(Token.towchar()))
		return false;
	for(int i=0;i<(sizeof(Illegals)/sizeof(Illegals[0]));++i)
		if((Token |= Illegals[i]))
			return false;
	return true;
}
bool IsOperator(const STRING &Token)
{
	guard
		return 
		Token == L"+" ||
		Token == L"-" ||
		Token == L"*" ||
		Token == L"/" ||
		Token == L"^^" ||
		Token == L"%" ||
		Token == L"+=" ||
		Token == L"-=" ||
		Token == L"*=" ||
		Token == L"/=" ||
		Token == L"^^=" ||	//Power exponent
		Token == L"^=" ||	//XOR
		Token == L"|=" ||
		Token == L"&=" ||
		Token == L"&&" ||
		Token == L"||" ||
		Token == L"==" ||
		Token == L"!=" ||
		Token == L"&" ||
		Token == L"|" ||
		Token == L"!" ||
		Token == L"^" ||
		Token == L"<<" ||
		Token == L">>" ||
		Token == L"=" ||
		Token == L"--" ||
		Token == L"++" ||
		Token == L"[" ||
		Token == L"." ||
		Token == L">" ||
		Token == L"<" ||
		Token == L">=" ||
		Token == L"<=" ||
		Token == L"." ||
		Token == L"new" ||
		Token == L">>>";

	unguard
}
bool IsValueOperator(const STRING &Token)
{
	guard
		return 
		Token == L"+" ||
		Token == L"-" ||
		Token == L"*" ||
		Token == L"/" ||
		Token == L"^^" ||
		Token == L"%" ||
		Token == L"+=" ||
		Token == L"-=" ||
		Token == L"*=" ||
		Token == L"/=" ||
		Token == L"^^=" ||	//Power exponent
		Token == L"^=" ||	//XOR
		Token == L"|=" ||
		Token == L"&=" ||
		Token == L"&" ||
		Token == L"|" ||
		Token == L"!" ||
		Token == L"^" ||
		Token == L"<<" ||
		Token == L">>" ||
		Token == L"=" ||
		Token == L"--" ||
		Token == L"++" ||
		Token == L"[" ||
		Token == L"." ||
		Token == L">>>";

	unguard
}
bool IsCmpOperator(const STRING &Token)
{
	guard
		return 
		Token == L">" ||
		Token == L"<" ||
		Token == L"==" ||
		Token == L">=" ||
		Token == L"<=" ||
		Token == L"!=";
	unguard
}
bool IsRightUnaryOperator(const STRING &Token)
{
	guard
		return
		Token == L"++" ||
		Token == L"--" ||
		Token == L"[";
		
	unguard
}
bool IsUnaryOperator(const STRING &Token)
{
	guard
		return
		Token == L"-" ||
		Token == L"+" ||
		Token == L"++" ||
		Token == L"--" ||
		Token == L"[" ||
		Token == L"new" ||
		Token == L"!" ;
		
	unguard
}
bool IsBoolOperator(const STRING &Token)
{
	guard
		return
		Token == L"&&" ||
		Token == L"||" ||
		Token == L">" ||
		Token == L"<" ||
		Token == L"==" ||
		Token == L">=" ||
		Token == L"<=" ||
		Token == L"!=" ||
		Token == L"^^";
	unguard
}
bool IsTransparent()
{
	guard
		//Convenient check for weither a pair of parenthesis should be treated as transparent or not
		UINT i = TIndex;
		NextTokenIdentifier();	//Take first parenthesis into account.
		if( IsLegalIdentifier(CurrentToken()) && (NextTokenIdentifier() == L")" || CurrentToken() == L",") )
		{
			TIndex = i;
			return true;
		}
		TIndex = i;
		return false;
	unguard
}
bool IsFunctionCall()
{
	guard
		UINT i = TIndex;
		if(!IsLegalIdentifier(CurrentToken()))
		{
			TIndex = i;
			return false;
		}
		NextTokenIdentifier();
		if(NextTokenIdentifier() != L"(")
		{
			TIndex = i;
			return false;
		}
		NextTokenIdentifier();
		while(CurrentToken() != L")")
		{
			NextTokenIdentifier();
			//We want an identifier, expression, or a litteral as an argument
			if(!ParseExpression())
				Error(L"Unrecognized function argument");
			if(NextTokenIdentifier() != L",")
				Expect(CurrentToken(),L")");
		}
		TIndex = i;
		return true;
	unguard
}
bool IsLitteral(const STRING& S)
{
	guard
		if(S.beginsWith(L"\"") && S.endsWith(L"\""))
			return true;
		
		if(S.beginsWith(L"0x"))
		{
			for(int i=2;i<S.length();++i)
			{
				if( (S.at(i) > L'9' || S.at(i) < L'0' ) && ( S.at(i) < L'a' || S.at(i) > L'f') && ( S.at(i) < L'A' || S.at(i) > L'F'))
					return false;
			}
		}
		else if(S.beginsWith(L"0b"))
		{
			for(int i=2;i<S.length();++i)
			{
				if(S.at(i) != L'1' && S.at(i) != L'0')
					return false;
			}
		}
		else if(S.beginsWith(L"0"))
		{
			for(int i=2;i<S.length();++i)
			{
				if(S.at(i) > L'8' || S.at(i) < L'0' )
					return false;
			}
		}
		else
		{
			for(int i=0;i<S.length();++i)
				if( (S.at(i) > L'9' || S.at(i) < L'0') && S.at(i) != L'.' )
					return false;
		}
		return true;
	unguard
}
void ExpectArgument(const STRING& Token)//Obsolete
{
	guard
		if(!IsLitteral(Token) )
			ExpectIdentifier(Token);
	unguard
}
void PrintExpressions(_HExprUnit* A)
{
	guard
		//Recursive
		assert(A);


	if(A->ExprT & EXP_LSE)
		std::wcout << L" " << ((STRING*)A->LUnit)->towchar() << L" ";
	else if(A->LUnit)//if(!(A->ExprT & EXP_UNARY))
	{
		std::wcout << L"(";//GLog->Log(L"(");
		PrintExpressions((_HExprUnit*)A->LUnit);
		std::wcout << L")";
	}

	if(!A->Op.isEmpty())
		std::wcout << A->Op.towchar();

	if(A->ExprT & EXP_RSE) 
		std::wcout << L" " << ((STRING*)A->RUnit)->towchar() << L" ";
	else if(A->RUnit)
	{
		std::wcout << (L"(");
		PrintExpressions((_HExprUnit*)A->RUnit);
		std::wcout << (L")");
	}

	if(A->ExprT & EXP_FUNC)
	{
		std::wcout << L"(";
		for(int i=0;i<((_HFuncExpr*)A)->Parameters.size();++i)
		{
			PrintExpressions((_HExprUnit*)((_HFuncExpr*)A)->Parameters[i]);
			if(((_HFuncExpr*)A)->Parameters.size()-1 > i)
				std::wcout << L",";
		}
		std::wcout << L")";
	}
	else if(A->ExprT & EXP_IFCLUSTER)
	{
		for(int i=0;i<((_HIFExpr*)A)->Expressions.size();++i)
			PrintExpressions(((_HIFExpr*)A)->Expressions[i]);
	}
	else if(A->ExprT & (EXP_IF | EXP_IFELSE | EXP_ELSE))
	{
		if(A->ExprT & EXP_IF)
			std::wcout << L"if";
		else if(A->ExprT & EXP_IFELSE)
			std::wcout << L"else if";
		else if(A->ExprT & EXP_ELSE)
			std::wcout << L"else";
		std::wcout << L"\n{\n";
		for(int i=0;i<((_HIFExpr*)A)->Expressions.size();++i)
		{
			std::wcout << L"\t";
			PrintExpressions(((_HIFExpr*)A)->Expressions[i]);
			std::wcout << L"\n";
		}
		std::wcout << L"}\n";
	}
	else if(A->ExprT & EXP_WHILE)
	{
		std::wcout << L"while(";
		PrintExpressions((_HExprUnit*)A->LUnit);
		std::wcout << L")\n{\n";
		for(int i=0;i<((_HWhileExpr*)A)->Expressions.size();++i)
		{
			std::wcout << L"\t";
			PrintExpressions(((_HWhileExpr*)A)->Expressions[i]);
			std::wcout << L"\n";
		}
		std::wcout << L"}";
	}
	else if(A->ExprT & EXP_FOR)
	{
		std::wcout << L"for( ;";
		PrintExpressions((_HExprUnit*)A->LUnit);
		std::wcout << L";";
		PrintExpressions((_HExprUnit*)A->RUnit);
		std::wcout << L")\n{\n";
		for(int i=0;i<((_HForExpr*)A)->Expressions.size();++i)
		{
			std::wcout << L"\t";
			PrintExpressions(((_HForExpr*)A)->Expressions[i]);
			std::wcout << L"\n";
		}
		std::wcout << L"}";
	}
	else if(A->ExprT & (EXP_DOWHILE | EXP_DOUNTIL))
	{
		std::wcout << L"do" << ENDL;
		std::wcout << L"{" << ENDL;
		for(int i=0;i<((_HWhileExpr*)A)->Expressions.size();++i)
		{
			std::wcout << L"\t";
			PrintExpressions(((_HWhileExpr*)A)->Expressions[i]);
			std::wcout << L"\n";
		}
		std::wcout << L"}" << ENDL;
		if(A->ExprT & EXP_DOWHILE)
			std::wcout << L"while(";
		else
			std::wcout << L"until(";
		PrintExpressions((_HExprUnit*)A->LUnit);
		std::wcout << L")" << ENDL;
	}

	unguard
}