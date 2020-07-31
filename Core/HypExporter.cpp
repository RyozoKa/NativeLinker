/*	Class exporter functions
	
*/

#include "HypExporter.h"
#include <regex>

wchar_t* HypClassExporter::NEXT_TOKEN(USHORT move)
{
	guard
		if(Lines.at(LineNum)->Tokens.size() == 0)
			LineNum++;
		else if(Lines.size() == LineNum +1 && Lines[LineNum]->Tokens.size() == TokenNum)
		{
			LineNum++;
			Token = L"";
			return Token;
		}
	if(!move)
	{
		if(Lines.at(LineNum)->Tokens.size() == TokenNum)				//At last token of this line
			return (Lines.at(LineNum+1)->Tokens.at(0)->towchar());
		else
			return (Lines.at(LineNum)->Tokens.at(TokenNum+1)->towchar());
	}
	else
	{
		if(Lines.at(LineNum)->Tokens.size() == TokenNum)
		{
			TokenNum = 0;
			Token = Lines.at(++LineNum)->Tokens.at(TokenNum++)->towchar();
			return Token;
		}
		else
		{
			Token = Lines.at(LineNum)->Tokens.at(TokenNum++)->towchar();
			return Token;
		}
	}
	unguard
}

bool HypClassExporter::IS_IDENTIFIER(wchar_t* str)
{
guard
	std::wregex id(L"[_a-zA-Z0-9][?@a-zA-Z0-9]+");
	return std::regex_match(str,id);
unguard
}

STRING* HypClassExporter::CHECK_DEF(STRING* token)
{
	return token;
}

//	Handle error messages and exit compiler
//
void HypClassExporter::Error(const wchar_t* str,INT Line,const wchar_t* token)
{
guard
	//Handle an error message here
	if(token == NULL)
		GLog->Err(L"Error: %s Line: %d \n Compilation failed: 1 error %d warnings\n",str,Line,NumWarnings);
	else
		GLog->Err(L"Error: %s Token: %s Line: %d \n Compilation failed: 1 error, %d warnings\n",str,token,Line,NumWarnings);
	throw L"";
unguard
}

void HypClassExporter::PreprocessFile(const wchar_t* filename)
{
guard
	GLog->Log(L"Preprocessing %s",filename);
	//split up the the file into tokens
	HypFile file(filename,FILE_READ);
	if(!file.isopen()) throw filename;
	INT CurrLine=0;
	do
	{
		CurrLine++;
		STRING str = file.readLine();
		//Parse preprocessor definitions
		if(str[0] == L'#')
		{
			if(str.findexact(L"define") == 1)
			{
				std::vector<STRING*> tokens = str.splitCode(L" ");
				wchar_t endl = L'\\';
				STRING tstr;
				INT index = 0;
				if(str.Count(L":") == 0) //If we don't have a separator
					Error(L"missing ':' separator in preprocessor definition",CurrLine);
				tstr = str.substr(str.find(L":") + 1);
				while(tstr[tstr.findlast(L"\\",tstr.length() - 4)] == endl)				//Append a new line
				{
					if(tstr[tstr.findlast(L"\\",tstr.length() - 4)] == endl)
						tstr = tstr.substr(0,tstr.findlast(L"\\"));
					STRING fstr = file.readLine();
					CurrLine++;
					tstr.append(STRING(L" ").append(fstr));
				}
				if(tokens[2]->towchar()[0] == L':')					//Pure replacement 
				{
					Definition d;
					d.Token = tokens[1];
					//Get the entire line
					d.ReplaceWith = new STRING();
					d.ReplaceWith->append(tstr);
					Defs.push_back(d);
				}
				else if(tokens[2]->towchar()[0] == L'(' )	//!!We have a function -- or do we? #define Test (DWORD) 
				{
					PFDefinition d;
					d.NumParams = 0;
					d.Token = tokens[1];
					//Get the parameters
					for(int i=3;i<tokens.size();i++)
					{
						if(IS_IDENTIFIER(tokens[i]->towchar()))
							d.Params[d.NumParams++] = tokens[i]->towchar();
						else if(tokens[i]->towchar()[0] == L')')
							break;
						else if(tokens[i]->towchar()[0] == L',')
							continue;
						else
							Error(L"illegal parameter identifier in preprocessor definition",CurrLine,tokens[i]->towchar());
					}
					d.ReplaceWith = new STRING();
					d.ReplaceWith->append(tstr);
					PFDefs.push_back(d);
				}
				else
					Error(L"unexpected token in preprocessor definition",CurrLine,tokens[2]->towchar());
			}
		}
	} while(!file.Eol());

unguard
}

//Check if a token resides in the vector
//
//
bool HypClassExporter::HAS_TOKEN(std::vector<STRING*> &tokens, const wchar_t* in)
{
	for(int i=0;i<tokens.size();i++)
		if(wcscmp(tokens[i]->towchar(),in) == 0)
			return true;
	return false;
}

//	Skip a line in the parser stage of the compiler
//	Ensured: str!=NULL
//
bool bSkipNext = false;	//For multiline preprocessing
bool HypClassExporter::SKIP(STRING* str)
{
	if(str->findexact(L"#define") != -1)	return true;
	else if(str->Count(L"\\") > 0)	{ bSkipNext = true; return true; }
	else if(bSkipNext)				{ bSkipNext = false; return true; }
	for(int i=0;i<str->length();i++)
	{
		if(str->at(i) > L' ')
			return false;
	}
	return true;
}

//	Check if two tokens are case insensetively equal
//
//
bool	HypClassExporter::CMP(wchar_t* in1,wchar_t* in2)
{
	if(wcslen(in1) == 0)
		return false;
	return (STRING(in1).toupper() == STRING(in2).toupper());
}

bool	HypClassExporter::CMP(STRING& in1,wchar_t* in2)
{
	return (in1.toupper() == STRING(in2).toupper());
}

bool HypClassExporter::HAS_FUNCTION(HFunctionContainer& func,HClassContainer &_class)
{
	if(_class.NumFunctions < 2)
		return false;
	for(int i=0;i<_class.NumFunctions-1;i++)
	{
		if(*_class.Functions[i].Name == *func.Name && _class.Functions[i].NumParameters == func.NumParameters)
		{
			for(int j=0;j<_class.Functions[i].NumParameters;j++)
				if(*_class.Functions[i].Parameters[j].TypeName != *func.Parameters[j].TypeName)
					return false;
		}
		else return false;
	}
	return true;
}

bool HypClassExporter::HAS_CLASS(HClassContainer& _class,std::vector<HClassContainer*> & _classes)
{
	if(_classes.size() < 1)
		return false;
	for(int i=0;i<_classes.size();i++)
	{
		if(*_class.Name == *_classes.at(i)->Name)			// if class is already declared
			return true;
		else return false;
	}
	return true;
}

bool HypClassExporter::HAS_VARIABLE(HVariableContainer& var,HClassContainer &_class)
{
	if(_class.NumVariables < 2)
		return false;
	for(int i=0;i<_class.NumVariables-1;i++)
	{
		if(*_class.Variables[i].Name == *var.Name)
			return true;
		else return false;
	}
	return true;
}

bool	HypClassExporter::IS_TYPE(wchar_t* type)
{
	return		(CMP(Token,L"byte")		|| 
				CMP(Token,L"short")		|| 
				CMP(Token,L"ushort")	|| 
				CMP(Token,L"int")		|| 
				CMP(Token,L"uint")		|| 
				CMP(Token,L"int64")		|| 
				CMP(Token,L"uint64")	||
				CMP(Token,L"float")		||
				CMP(Token,L"double")	||
				CMP(Token,L"string")	||
				IS_IDENTIFIER(Token)	);
}

void HypClassExporter::ParseFile(const wchar_t* filename)
{
	guard

	//split up the the file into tokens
	GLog->Log(L"Parsing %s",filename);
	HypFile file(filename,FILE_READ);
	if(!file.isopen()) throw filename;
	UINT CurrLine=0;
	do
	{
		CurrLine++;
		STRING str = file.readLine();

		if((str.length() == 0 || SKIP(&str)) && !file.Eof()) continue;
		else if((str.length() == 0 || SKIP(&str)) && file.Eof()) break;
		
		if(str.findexact(L"#ifdef") != -1)
		{
			for(int i=0;i<Defs.size();i++)
			{
				if(str.findexact(Defs[i].Token->towchar()) != -1)//True
				{
					std::vector<STRING*> Tokens = str.splitCode(L" ");
					std::vector<STRING*> Reps	= Defs[i].ReplaceWith->splitCode(L" ");
					if(Tokens.size() > 2)	//Check for its value //True/False
					{

						if(wcscmp(Tokens[2]->towchar(),Reps[0]->towchar()) == 0)
						{	//True
							CurrLine++;
							str = file.readLine();
							goto JXT001;
						}
						else
						{
							while(!file.Eol() && str.findexact(L"#endif") == -1)
							{
								str = file.readLine();
								CurrLine++;
							}
							if(file.Eol() && str.findexact(L"#endif") == -1)
								Error(L"unexpect end of file, missing #endif argument",CurrLine);
							str = file.readLine();
							CurrLine++;
						}
					}
					else
					{	//True
						CurrLine++;
						str = file.readLine();
						goto JXT001;
					}
				}
				else	//False
				{
					while(!file.Eol() && str.findexact(L"#endif") == -1)
					{
						str = file.readLine();
						CurrLine++;
					}
					if(file.Eol() && str.findexact(L"#endif") == -1)
						Error(L"unexpect end of file, missing #endif argument",CurrLine);
					str = file.readLine();
					CurrLine++;

				}
			}
		}
JXT001:
		//Checks@rep
		if((str.length() == 0 || str.findexact(L"#endif") != -1 || str.findexact(L"#ifdef") != -1 /*???*/) && !file.Eol())
			continue;
		else if(file.Eol() && str.length() == 0)
			break;
		std::vector<STRING*> Tokens = str.splitCode(L" ");

		for(int i=0;i<Tokens.size();i++)
		{
			//Replace preprocessor function definitions 
			for(int j=0;j<PFDefs.size();j++)
			{
				if(Tokens[i]->towchar()[0] == L'(' && (wcscmp(Tokens[i - 1]->towchar(),PFDefs[j].Token->towchar()) == 0) )
				{	//So we have a function
					//If we have multiple lines till end of parameters
					if(str.Count(L")") == 0)
						while(!HAS_TOKEN(Tokens,L")"))
						{
							if(file.Eol())
								Error(L"unexpected end of file",CurrLine);
							CurrLine++;
							STRING tstr = file.readLine();
							str.append(L" ");
							str.append(tstr);
							std::vector<STRING*> newtokens = tstr.splitCode(L" ");
							Tokens.insert(Tokens.end(),newtokens.begin(),newtokens.end());
						}
					//!!Safety checks
					INT pos = str.find(L"(");
					if(str.substr(pos,str.find(L")") + 1 - pos).Count(L",") != (PFDefs[j].NumParams-1))
						Error(L"the number of arguments does not match the parameter list",CurrLine);
					std::vector<STRING*> parames;
					INT end=0;
					for(int k=i+1;k<Tokens.size();k++)
					{
						if(Tokens[k]->towchar()[0] == L')')
						{
							end = k;
							break;
						}
						else if(Tokens[k]->towchar()[0] == L',')
							continue;
						else
							parames.push_back(new STRING(*Tokens[k]));
					}
					INT beg = i-1;
					for(int k=beg;k<end;k++)
						delete Tokens[k];
					Tokens.erase(Tokens.begin() + beg, Tokens.begin() + end + 1);
					std::vector<STRING*> part = PFDefs[j].ReplaceWith->splitCode(L" ");
					Tokens.insert(Tokens.begin() + beg, part.begin(), part.end());
				}
			}
			for(int j=0;j<Defs.size();j++)
			{
				if(wcscmp(Tokens[i]->towchar(),Defs[j].Token->towchar()) == 0)
				{
					delete Tokens[i];
					Tokens.erase(Tokens.begin() + i);
					if(Defs[j].ReplaceWith != NULL)
					{
						std::vector<STRING*> newtokens = Defs[j].ReplaceWith->splitCode(L" ");
						Tokens.insert(Tokens.begin() + i,newtokens.begin(),newtokens.end());
					}
				}
			}

		}
		
				LineInfo *line = new LineInfo();
		for(int i=0;i<Tokens.size();i++)
			line->Tokens.push_back(Tokens[i]);
		line->Line = CurrLine;
		line->filename = const_cast<wchar_t*>(filename);
		Lines.push_back(line);
	} while(!file.Eol());


	unguard
}

//Compile the collected data
//Compiled bytecode will create a HClass in each container
//
void HypClassExporter::Compile()
{
guard
	
/*
	for(int i=0;i<Defs.size();i++)
	{
		delete Defs[i].ReplaceWith;
		delete Defs[i].Token;
	}
	for(int i=0;i<PFDefs.size();i++)
	{
		delete PFDefs[i].Token;
		delete PFDefs[i].ReplaceWith;
		for(int j=0;j<PFDefs[i].NumParams;j++)
			delete[] PFDefs[i].Params[j];
	}
	Defs.clear();
	PFDefs.clear();
	TokenNum = 0;
	Token = Lines[0]->Tokens[0]->towchar();
	LineNum = 0;
	end = false;
	HClassContainer* _class = NULL;
	std::stack<ScopeLevel> Scope;
	BYTE Permission = V_PUBLIC;
	NEXT_TOKEN(1);

	do{
		if(CMP(Token,L"class"))
		{
			Permission = V_PUBLIC;
			_class = new HClassContainer();	//@eax!4
			Scope.push(L_CLASS);
			if(!IS_IDENTIFIER(NEXT_TOKEN(1)))
				Error(L"illegal identifier",Lines[LineNum]->Line,Token);
//			_class->Name = AppHashName(STRING(Token).tolower().towchar()); TODO: Implement this!
				debugf(L"Class: %s",Token);
			if(HAS_CLASS(*_class,Classes))
				Error(L"class already declared in this package",Lines[LineNum]->Line,Token);
			//_class->Class = new HClass();
			//_class->Class->Package = _outer;
			_class->Class->Flags =0;
			NEXT_TOKEN(1);
			if(CMP(Token,L"extends"))
			{
				NEXT_TOKEN(1);
					debugf(L"Parent: %s",Token);
				if(!IS_IDENTIFIER(Token))
					Error(L"illegal identifier",Lines[LineNum]->Line,Token);
//				_class->ParentID = AppHashName(STRING(Token).tolower().towchar()); TODO: Implement this!
				NEXT_TOKEN(1);
			}
			while (CMP(Token,L"native") || CMP(Token,L"abstract") || CMP(Token,L"static"))
			{
				if(CMP(Token,L"native"))
					_class->Class->Flags |= C_NATIVE;
				else if(CMP(Token,L"abstract"))
					_class->Class->Flags |= C_ABSTRACT;
				else if(CMP(Token,L"static"))
					_class->Class->Flags |= C_STATIC;
				else if(CMP(Token,L";"))
					break;
				else
					Error(L"missing ';' before token",Lines[LineNum]->Line,Token);
				NEXT_TOKEN(1);
			}
#ifdef _DEBUG
			if(_class->Class->Flags & C_NATIVE)
				debugf(L"Flag: Native");
			if(_class->Class->Flags & C_ABSTRACT)
				debugf(L"Flag: Abstract");
			if(_class->Class->Flags & C_STATIC)
				debugf(L"Flag: Static");
#endif
			if(wcscmp(Token,L"{"))
				Error(L"missing '{' before token",Lines[LineNum]->Line,Token);
			NEXT_TOKEN(1);
		}
		else if(CMP(Token,L"var"))//Variable declaration
		{
			if(!_class)
				Error(L"variable declaration must be within a class",Lines[LineNum]->Line,Token);
			HVariableContainer &var = _class->Variables[_class->NumVariables++];
			var.Flags =0;
			NEXT_TOKEN(1);
			while(CMP(Token,L"const") || CMP(Token,L"static"))
			{
				if(CMP(Token,L"const"))
					var.Flags |= V_CONST;
				else if(CMP(Token,L"static"))
					var.Flags |= V_STATIC;
				debugf(L"Var mod: %s",Token);
				NEXT_TOKEN(1);
			}
			var.Flags |= Permission;
			debugf(L"Access: %s",var.Flags & V_PUBLIC?L"public":var.Flags & V_PRIVATE?L"private":var.Flags & V_PROTECTED?L"protected":L"error");
			if(IS_TYPE(Token))
				1;
//				var.TypeName = AppHashName(STRING(Token).tolower().towchar()); TODO: Implement this!
			else
				Error(L"expected a type specifier",Lines[LineNum]->Line,Token);
			NEXT_TOKEN(1);
			if(!IS_IDENTIFIER(Token))
				Error(L"illegal identifier for variable declaration",LineNum+1,Token);
			//var.Name = AppHashName(STRING(Token).tolower().towchar()); TODO: Implement this!
			if(HAS_VARIABLE(var,*_class))
				Error(L"variable has already been declared in this scope",Lines[LineNum]->Line,Token);
			debugf(L"Var name: %s",Token);
			NEXT_TOKEN(1);
			if(!CMP(Token,L";"))
				Error(L"missing ';' before token",Lines[LineNum]->Line,Token);
			NEXT_TOKEN(1);
		}
		else if(CMP(Token,L"public"))
		{
			if(Scope.top() != L_CLASS)
				Error(L"'public' modifier is only allowed in a class scope",Lines[LineNum]->Line);
			NEXT_TOKEN(1);
			if(!CMP(Token,L":"))
				Error(L"missing ':' before token",Lines[LineNum]->Line,Token);
			NEXT_TOKEN(1);
			Permission = V_PUBLIC;
		}
		else if(CMP(Token,L"private"))
		{
			
			if(Scope.top() != L_CLASS)
				Error(L"'private' modifier is only allowed in a class scope",Lines[LineNum]->Line);
			NEXT_TOKEN(1);
			if(!CMP(Token,L":"))
				Error(L"missing ':' before token",Lines[LineNum]->Line,Token);
			NEXT_TOKEN(1);
			Permission = V_PRIVATE;
		}
		else if(CMP(Token,L"protected"))
		{
			
			if(Scope.top() != L_CLASS)
				Error(L"'protected' modifier is only allowed in a class scope",Lines[LineNum]->Line);
			NEXT_TOKEN(1);
			if(!CMP(Token,L":"))
				Error(L"missing ':' before token",Lines[LineNum]->Line,Token);
			NEXT_TOKEN(1);
			Permission = V_PROTECTED;
		}
		else if(CMP(Token,L"function") || CMP(Token,L"final") || CMP(Token,L"native") || CMP(Token,L"static") )
		{
			if(Scope.top() != L_CLASS)
				Error(L"function is only allowed in a class scope",Lines[LineNum]->Line);
			HFunctionContainer &func = _class->Functions[_class->NumFunctions++];
			func.Flags =0;
			while(!CMP(Token,L"function"))
			{
				if(CMP(Token,L"native"))
					func.Flags |= F_NATIVE;
				else if(CMP(Token,L"final"))
					func.Flags |= F_FINAL;
				else if(CMP(Token,L"static"))
					func.Flags |= F_STATIC;
				else
					Error(L"unknown token after function modifier",Lines[LineNum]->Line,Token);
				NEXT_TOKEN(1);
			}
			debugf(L"Function token: %s",Token);
			NEXT_TOKEN(1);
			if(	!IS_TYPE(Token)	)
				Error(L"illegal or missing return type specifier",Lines[LineNum]->Line,Token);
			//func.ReturnType = AppHashName(STRING(Token).tolower().towchar()); TODO: Implement this!
			NEXT_TOKEN(1);
			if(!IS_IDENTIFIER(Token))
				Error(L"missing identifier before",Lines[LineNum]->Line,Token);
//			func.Name = AppHashName(STRING(Token).tolower().towchar()); TODO: Implement this!
			NEXT_TOKEN(1);
			if(!CMP(Token,L"("))
				Error(L"missing '(' before token",Lines[LineNum]->Line,Token);
			NEXT_TOKEN(1);
			Scope.push(L_PARAMETERS);
			while(!CMP(Token,L")"))
			{
				if(CMP(Token,L","))
				{
					NEXT_TOKEN(1);
					continue;
				}
				HVariableContainer &var = func.Parameters[func.NumParameters++];
				if(IS_TYPE(Token))
				{
//					var.TypeName = AppHashName(STRING(Token).tolower().towchar()); TODO: Implement this!
					NEXT_TOKEN(1);
					if(CMP(Token,L",") && !IS_IDENTIFIER(Token))
						Error(L"missing identifier before",Lines[LineNum]->Line,Token);
//					var.Name = AppHashName(STRING(Token).tolower().towchar()); TODO: Implement this!
				}
				else Error(L"expected a type specifier",Lines[LineNum]->Line,Token);
				NEXT_TOKEN(1);
			}
			if(!CMP(Token,L")"))
				Error(L"missing ')' before",Lines[LineNum]->Line,Token);
			Scope.pop();
			if(HAS_FUNCTION(func,*_class))
				Error(L"function already declared",Lines[LineNum]->Line,Token);
			NEXT_TOKEN(1);
			if(!CMP(Token,L"{") && !CMP(Token,L";"))
				Error(L"missing '{' or';' before",Lines[LineNum]->Line,Token);
			if(CMP(Token,L";"))
			{
				NEXT_TOKEN(1);
				continue;
			}
			NEXT_TOKEN(1);
			while(!HAS_TOKEN(Lines[LineNum]->Tokens,L"}"))
			{
				func.Lines.push_back(Lines[LineNum++]);
			}
			func.Lines.push_back(Lines[LineNum]);
			TokenNum =Lines[LineNum]->Tokens.size();
			NEXT_TOKEN(1);
			Scope.pop();
			//NEXT_TOKEN(1);
		}
		else if(CMP(Token,L"}") && Scope.top() > 0)
		{
			NEXT_TOKEN(1);
			if(CMP(Token,L"}") || !_class)
				Error(L"unexpected '}'",Lines[LineNum]->Line);
			debugf(L"Scope: $i",Scope.top());
			Scope.pop();
			Classes.push_back(_class);
			_class = NULL;
		}
		else
			Error(L"unexpected token",Lines[LineNum]->Line,Token);
	}while(!(Lines.size() == (LineNum) && Lines[min(LineNum,Lines.size()-1)]->Tokens.size() == TokenNum));

	if(Classes.size() < 1) return;

	for(int i=0;i<Classes.size();i++)
	{
		for(int j=0;j<Classes[i]->NumFunctions;j++)
		{
			for(int k=0;k<Classes[i]->Functions[j].Lines.size();k++)
			{
				for(int l=0;l<Classes[i]->Functions[j].Lines[k]->Tokens.size();l++)
					GLog->Log(L"Token: %s",Classes[i]->Functions[j].Lines[k]->Tokens[l]->towchar());
			}
			GLog->Log(L"_________________________________________________");
		}
	}*/

unguard
}