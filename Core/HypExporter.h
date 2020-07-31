/*	Hyper Script compiler
	Revision:
		Christopher Békési - initial build
*/
#pragma once

#include "Core.h"
#include <array>
//Export containers for metadata and typechecking during compilation


//Info about a single line
struct LineInfo
{
	std::vector<STRING*> Tokens;
	USHORT Line;
	wchar_t* filename;
};

//Preprocessor definition
struct Definition
{
	STRING* ReplaceWith;
	STRING* Token;
	bool bFunction;
};

struct PFDefinition
{
	STRING* ReplaceWith;
	STRING* Token;
	INT		NumParams;
	wchar_t* Params[16];
};

enum ScopeLevel
{
	L_CLASS,
	L_FUNCTION,
	L_PARAMETERS,
	L_IFELSE,
	L_SWITCH,
	L_FOR,
	L_WHILE,
	L_DO,
	L_STRUCT,
	L_GLOBAL
};

class HypClassExporter : public HypObject
{
public:
	DLLEXPORT HypClassExporter(STRING outer) : NumWarnings(NULL), LineNum(NULL), TokenNum(NULL) { _outer = outer;}
	DLLEXPORT ~HypClassExporter() {}
	//Compile a script file
	DLLEXPORT void MakeFile(const wchar_t* filename);
	//Parse the preprocessor definitions
	DLLEXPORT void PreprocessFile(const wchar_t* filename);
	//Parse declarations
	DLLEXPORT void ParseFile(const wchar_t* filename);
	//Compile the collected data
	DLLEXPORT void Compile();

	//Error handling
	void Error(const wchar_t* str,INT Line,const wchar_t* token=NULL);



	//Import list of all dependent classes
	std::vector<HClassContainer*>	ImportList;
	//Compiled classes
	std::vector<HClassContainer*>	Classes;
	//All the lines in this class
	std::vector<LineInfo*> Lines;
	//Preprocessor definitions
	std::vector<Definition> Defs;
	std::vector<PFDefinition> PFDefs;

	INT NumWarnings;

private:
		//Current token data
	LineInfo	Line;
	USHORT		LineNum;
	wchar_t*	Token;
	USHORT		TokenNum;
	bool		end;
	STRING		_outer;


	wchar_t* NEXT_TOKEN(USHORT move);
	STRING* CHECK_DEF(STRING*);
	bool	IS_IDENTIFIER(wchar_t* str); 
	bool	HAS_TOKEN(std::vector<STRING*> &tokens,const wchar_t* in);
	bool	SKIP(STRING*);
	bool	CMP(wchar_t*,wchar_t*);
	bool	CMP(STRING&,wchar_t*);
	bool	HAS_FUNCTION(HFunctionContainer& func, HClassContainer &_class);
	bool	HAS_CLASS(HClassContainer& _class,std::vector<HClassContainer*> & _classes);
	bool	HAS_VARIABLE(HVariableContainer& func, HClassContainer &_class);
	bool	IS_TYPE(wchar_t* str);
};