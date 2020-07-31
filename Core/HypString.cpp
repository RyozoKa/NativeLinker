
#include "Core.h"
#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <wctype.h>

//Engine glue logic

IMPLEMENT_CLASS(HString,String);
REGISTER_FUNCTIONS(String)
REGISTER_FUNCTION(HString,operator=,OBJFUNCS,HString&,INT);
END_REGISTER_FUNCTIONS



char HString::ch[128][128];
wchar_t HString::wch[128][128];
USHORT HString::idx = 0;
USHORT HString::widx = 0;

char* HString::getChr()
{
	if(idx == 128)
		idx = 0;
	return ch[idx++];
}
wchar_t* HString::getWchr()
{
	if(widx == 128)
		widx = 0;
	return wch[widx++];
}
HString::HString(void) : nstring(NULL)
{
	guard
	nlength = 1;
	nstring = NULL;
	unguard
}
int HString::findlast(wchar_t* in, int index)
{
	guard
	int k = -1;
	for(int i=index;i<length();i++)
	{
		for(int j=0;j<wcslen(in);j++)
		{
			if(nstring[i] == in[j])
				k = i;
		}
	}
	return k;
	unguardf(nstring)
}
HString::HString(BYTE* bytes) : nstring(NULL)
{
	guard
	nlength = ((bytes[1] << 8) | bytes[0]) + 1;
	nstring = (wchar_t*) db_malloc(nlength);
	wchar_t* str = (wchar_t*)(bytes + 2);
	for(INT short_index = 0; short_index < nlength - 1;short_index++)
	{//Breaking here often means that you forgot to input a wide C string instead of a multibyte char. This is to deserialize a byte array.
		nstring[short_index] = *str++;
	}
	nstring[nlength-1]=L'\0';
	unguard
}
HString::HString(HypFile* f) : nstring(NULL)
{
	guard
		nlength = f->readUShort() + 1;
		nstring = (wchar_t*) db_malloc(nlength);
		for(INT short_index = 0; short_index < nlength - 1;short_index++)
		{
			nstring[short_index] = (wchar_t)f->readUShort();
		}
	nstring[nlength-1]=L'\0';
	unguard
}
void HString::deserialize(BYTE* bytes)
{
	guard
	nlength = ((bytes[1] << 8) | bytes[0]) + 1;
	//db_free(nstring);	//In case it's not empty.
	nstring = (wchar_t*) db_malloc(nlength);
	wchar_t* str = (wchar_t*)(bytes + 2);
	for(INT short_index = 0; short_index < nlength - 1;short_index++)
	{//Breaking here (Unhandled exception) often means that you forgot to input a wide C string instead of a multibyte char. This is to deserialize a byte array.
		nstring[short_index] = *str++;
	}
	nstring[nlength-1]=L'\0';
	unguardf(nstring)
}
void HString::deserialize(HypFile* f)
{
	guard
		nlength = f->readUShort() + 1;
		nstring = (wchar_t*) db_malloc(nlength);
		for(INT short_index = 0; short_index < nlength - 1;short_index++)
		{//Breaking here often means that you forgot to input a wide C string instead of a multibyte char. This is to deserialize a byte array.
			nstring[short_index] = (wchar_t)f->readUShort();
		}
	nstring[nlength-1]=L'\0';
	unguard
}
HString::HString(const wchar_t* in, int len) : nstring(NULL)
{
	guard
	nlength = len > 0 ? len +1 : wcslen(in)+1;
	nstring = (wchar_t*) db_malloc(nlength * sizeof(wchar_t));
	//wcscpy_s(nstring,nlength-1,in);
		for(int i=0;i<nlength;i++)
			nstring[i] = in[i];
	nstring[nlength-1]=L'\0';
	unguardf(nstring)
}
HString::HString(const char* in, int len) : nstring(NULL)
{
	guard
	nlength = len > 0 ? len +1 : strlen(in)+1;
	nstring = (wchar_t*) db_malloc(nlength * sizeof(wchar_t));
	for(int i=0;i<nlength-1;i++)
		nstring[i] = in[i];
	nstring[nlength-1]=L'\0';
	unguardf(nstring)
}
HString::HString(const wchar_t in) : nstring(NULL)
{
	guard
	nlength = wcslen(&in)+1;
	nstring = (wchar_t*) db_malloc(nlength * sizeof(wchar_t));
	wcscpy(nstring,&in);
	nstring[nlength-1]=L'\0';
	unguardf(nstring)
}
HString::HString(HString& in) : nstring(NULL)
{
	guard
	nlength = in.nlength;
	nstring = (wchar_t*) db_malloc(nlength * sizeof(wchar_t));
	StrCpy(*this,const_cast<HString&>(in));
	unguardf(nstring)
}
HString::HString(const HString& in) : nstring(NULL)
{
	guard
	nlength = in.nlength;
	nstring = (wchar_t*) db_malloc(nlength * sizeof(wchar_t));
	StrCpy(*this,const_cast<HString&>(in));
	unguardf(nstring)
}
HString::~HString()
{
	db_free(nstring);
}
void HString::StrCpy(HString& dest,HString& from)
{
	guard
		if(from.nlength < 2) return;
	for(int i=0;i<from.nlength;i++)
		dest.nstring[i]=from.nstring[i];
	unguardf(nstring)
}
std::vector<HString*> HString::split(wchar_t *str)
{
	guard
	std::vector<HString*> words;
	HString *word = new HString();
	bool bCommentBlock=false,bCommentLine=false;
	wchar_t last;
	if(isEmpty()) return words;
	for(int i=0;i<=length();i++)
	{
		/*if((at(i) == L'/' && at(i+1) == L'*'))
			bCommentBlock = true;
		else if(at(i) == L'/' && at(i+1) == L'/')
			bCommentLine = true;
		*///else if(bCommentBlock && (at(i) == L'*' && at(i+1) == L'/'))
		//{
		//	bCommentBlock = false;
		//	i++;
		//	continue;
		//}
		//else if(bCommentLine && at(i) == L'\n')
		//{
		//	bCommentLine = false;
		//	continue;
		//}
		//if(bCommentBlock || bCommentLine) continue;

		/*if(at(i) == ';' || at(i) == L'(' || at(i) == L'[' || at(i) == L']' || at(i) == L')' || at(i) == L'{' || at(i) == L'}' || at(i) == L',' || at(i) == L'.') //Special characters
		{
			if(!word->isEmpty())
			{
				words.push_back((word));
				word = new HString();
			}
			words.push_back(new HString(L";"));
			continue;
		}*/
		if(!word->isEmpty() && (at(i) == str[0] || (at(i) == L'\n' && last != L'\n')))
		{
			words.push_back((word));
			word = new HString();
		}
		else if(at(i) != L'\n')
			word->append(at(i));
		last = at(i);
	}
	if(!word->isEmpty())
		words.push_back(word);//Last word might be missing
	return words;
	unguardf(nstring)
}
std::vector<HString> HString::splitCode()
{
	guard
	std::vector<HString> words;
	HString word;
	bool bCommentBlock=false,bCommentLine=false;
	wchar_t last;
	if(isEmpty()) return words;
	for(int i=0;i<=length();++i)
	{
		if((at(i) == L'/' && at(i+1) == L'*'))
			bCommentBlock = true;
		else if(at(i) == L'/' && at(i+1) == L'/')
			bCommentLine = true;
		else if(bCommentBlock && (at(i) == L'*' && at(i+1) == L'/'))
		{
			bCommentBlock = false;
			i++;
			continue;
		}
		else if(bCommentLine && at(i) == L'\n')
		{
			bCommentLine = false;
			continue;
		}
		if(bCommentBlock || bCommentLine) continue;
		if( (i + 3 < nlength) &&
			((at(i) == L'>' && at(i + 1) == L'>' && at(i + 2) == L'>' && at(i + 3) == L'=')
			))
		{
			if(!word.isEmpty())
			{
				words.push_back((word));
				word.clear();
			}
			words.push_back(HString( nstring + i, 4) );
			++i;
			continue;
		}
		
		if( (i + 2 < nlength) &&
			((at(i) == L'>' && at(i + 1) == L'>' && at(i + 2) == L'>')
			|| (at(i) == L'>' && at(i + 1) == L'>' && at(i + 2) == L'=')
			|| (at(i) == L'<' && at(i + 1) == L'<' && at(i + 2) == L'=')
			))
		{
			if(!word.isEmpty())
			{
				words.push_back((word));
				word.clear();
			}
			words.push_back(HString( nstring + i, 3) );
			++i;
			continue;
		}
		if( (i + 1 < nlength) && 
		   ((at(i) == L'#' && at(i + 1) == L'#')
		||	(at(i) == L'+' && at(i + 1) == L'+')
		||	(at(i) == L'-' && at(i + 1) == L'-')
		||	(at(i) == L'|' && at(i + 1) == L'|')
		||	(at(i) == L'&' && at(i + 1) == L'&')
		||	(at(i) == L'+' && at(i + 1) == L'=')
		||	(at(i) == L'-' && at(i + 1) == L'=')
		||	(at(i) == L'*' && at(i + 1) == L'=')
		||	(at(i) == L'/' && at(i + 1) == L'=')
		||	(at(i) == L'&' && at(i + 1) == L'=')
		||	(at(i) == L'|' && at(i + 1) == L'=')
		||	(at(i) == L'=' && at(i + 1) == L'=')
		||	(at(i) == L'~' && at(i + 1) == L'=')
		||	(at(i) == L'<' && at(i + 1) == L'<')
		||	(at(i) == L'>' && at(i + 1) == L'>')
		))
		{
			if(!word.isEmpty())
			{
				words.push_back((word));
				word.clear();
			}
			words.push_back(HString( nstring + i, 2) );
			++i;
			continue;
		}

		
		if(
				at(i) == L':' 
			||	at(i) == L';' 
			||	at(i) == L'(' 
			||	at(i) == L'[' 
			||	at(i) == L']' 
			||	at(i) == L')' 
			||	at(i) == L'{' 
			||	at(i) == L'}' 
			||	at(i) == L',' 
			||	at(i) == L'>'
			||	at(i) == L'<'
			||	( at(i) == L'.' && ( at(i-1) > L'9' || at(i-1) < L'0') )
			||	at(i) == L'+'
			||	at(i) == L'-'
			||	at(i) == L'*'
			||	at(i) == L'/'
			||	at(i) == L'^'
			||	at(i) == L'%'
			||	at(i) == L'!'
			||	( at(i) == L'#' && substr(i,7) != L"#define") 
			||	at(i) == L'=' ) //Special characters
		{
			if(!word.isEmpty())
			{
				words.push_back((word));
				word.clear();
			}
			words.push_back(HString(at(i)));
			continue;
		}
		if(!word.isEmpty() && (at(i) == L' ' || (at(i) == L'\n' && last != L'\n')))
		{
			words.push_back((word));
			word.clear();
		}
		else if(at(i) != L'\n' && at(i) > L' ')
			word.append(at(i));
		last = at(i);
	}
	if(!word.isEmpty())
		words.push_back(word);//Last word might be missing
	return words;
	unguardf(nstring)
}
bool HString::ReplaceAt(const wchar_t* str,INT start,INT stop)
{ //THIS IS MY STRING start = 5 stop = 7
	guard
	INT len = wcslen(str);
	if(start > nlength || stop > nlength || stop < start)
		throw L"Error: Start or Stop position illegel";
	INT diff = len - (stop - start);
	if(diff > 0)
	{
		nstring = (wchar_t*)db_realloc(nstring,(nlength + diff) * sizeof(wchar_t));	//Do we need to resize?
		nlength += diff;
		for(int i=nlength-1;i>= stop;i--)
			nstring[i] = nstring[i - diff];
	}
	if(diff >= 0)
	{
		for(int i=0;i<len;i++)
			nstring[i + start] = str[i];
	}
	if(diff < 0)
	{
		//Deal with a smaller string here, let's just skip the db_reallocation
		for(int i=stop;i<nlength;i++)
		{
			nstring[i + diff] = nstring[i];
		}
		for(int i=0;i<len;i++)
		{
			nstring[i + start] = str[i];
		}
	}
	return true;
	unguardf(nstring)
}
bool HString::ReplaceWith(const wchar_t* str,const wchar_t* with)
{
	guard
	INT index = this->findexact(const_cast<wchar_t*>(str));
	if(index != -1)
		return ReplaceAt(with,index,wcslen(str) + index);
	return false;
	unguardf(nstring)
}
int HString::length() const
{
	return nlength-1;
}
const wchar_t& HString::at(int pos) const
{
	return nstring[pos];
}
bool HString::isEmpty()
{
	return nlength < 2;
}
void HString::swap(HString& swap)
{
	guard
		wchar_t* buf = (wchar_t*)alloca(nlength*2);
		wcscpy(buf,nstring);
		wcscpy(nstring,swap.nstring);
		wcscpy(swap.nstring,buf);
		short i = nlength;
		nlength = swap.nlength;
		swap.nlength = i;
	unguardf(nstring)
}
HString HString::substr(int start,int num)
{
	guard
		if(start > length())
		throw L"Start index can't be greater than the length of the string.";
	if(start + num > length() || num == 0)
				num = length() - start;
	if(num < 0)
		throw L"Number of characters can't be less than 0.";
	return HString(nstring + start,num);
	unguardf(nstring)
}
int HString::find(wchar_t* in,int index)
{
	guard
	for(int i=index;i<length();i++)
		for(int j=0;j<wcslen(in);j++)
			if(nstring[i] == in[j])
				return i;
	return -1;
	unguardf(nstring)
}
int HString::findexact(wchar_t* in,int index)
{
	guard
	int len = wcslen(in);
	int len2 = length();
	for(int i=index;i<=length() - len;i++)//4-2
	{
		if(appwcscmp(substr(i,len).towchar(),in))
			return i;
	}
	return -1;
	unguardf(nstring)
}
bool HString::beginsWith(const wchar_t* in) const 
{
	guard
		size_t len = wcslen(in);
		if(len > nlength) return false;
		for(int i=0;i<len; i++)
			if(in[i] != nstring[i]) return false;
		return true;
	unguard
}
bool HString::endsWith(const wchar_t* in) const
{
	guard
		size_t len = wcslen(in);
		size_t factor = (nlength - len) -1;
		if(len > nlength) return false;
		for(int i=0;i<len; i++)
			if(in[i] != nstring[factor + i]) return false;
		return true;
	unguard
}
HString* HString::clear()
{
	guard
	if(nstring)
	nstring[0] = L'\0';//(wchar_t*)db_realloc(nstring,1);
	nlength = 1;
	unguard
}
void HString::detach(int start, int num)
{
	guard
	if(start == 0)
	{
			HString tmp(*this);
			*this = tmp.substr(num,length() - num);//Done? 4
	}
	else if(start + num == length())//at end
	{
		HString tmp(*this);
		*this = tmp.substr(0,start);
	}
	else
	{
		HString tmp(*this);
		HString A(tmp.substr(0,start));
		HString B(tmp.substr(num+1,length() - (start + num)));
		*this = (A+B);
	}
	unguardf(nstring)
}
INT HString::Count(const wchar_t* str)
{
	guard
		INT Count=0;
		for(int i=0;i<nlength-1;i++)
		{
			if(str[0] == nstring[i])
				Count++;
		}
		return Count;
	unguardf(nstring)
}
bool operator == (const HString& f, const HString& s)
{
	guard
		if(f.nlength == s.nlength)
		return appwcscmp(f.nstring,s.nstring);
	else return false;
	unguardf(f.nstring)
}
bool operator |= (const HString& g, const HString& d)
{
	guard
		wchar_t* f = g.nstring,*s = d.nstring;	//Optimize
	if(g.nlength == d.nlength)	//NOTE: May not support locales properly
	{
		if(!appwcscmp_ci(f,s))
			return false;
		return true;
	}
	else return false;
	unguardf(g.nstring)
}
bool operator |= (const HString& g, const wchar_t* d)
{
	guard
		const wchar_t* f = g.nstring,*s = d;
	size_t len = wcslen(d);
	if(g.nlength == len+1)	//NOTE: May not support locales properly
	{
		if(!appwcscmp_ci(f,s))
			return false;
		return true;
	}
	else return false;
	unguardf(g.nstring)
}
bool operator == (const HString& g, const wchar_t* s)
{
	if(g.nlength-1 == wcslen(s))
		return appwcscmp(g.nstring,(wchar_t*)s);
	else return false;
}
bool operator != (const HString& f, const HString& s)
{
	guard
	if(f.nlength == s.nlength)
		return !appwcscmp(f.nstring,s.nstring);
	else return true;
	unguardf(f.nstring)
}
bool operator != (const HString& g, const wchar_t* s)
{
	if(g.nlength-1 == wcslen(s))
		return !appwcscmp(g.nstring,(wchar_t*)s);
	else return true;
}
wchar_t& HString::operator [] (int pos)
{
	return nstring[pos];
}
std::wostream& operator <<(std::wostream & OS, HString *S)
{
	guard
	OS << S->nstring;
	return OS;
	unguardf(S->nstring)
}
std::wostream& operator <<(std::wostream & OS, HString S)
{
	guard
	OS << S.nstring;
	return OS;
	unguardf(S.nstring)
}
std::wistream& operator >>(std::wistream & IS,HString *S)
{
	guard
	wchar_t input[100];
	IS.getline(input,100);
	S->append(input);
/*	int i = 0;
	while(input[i] != '\0' && i<100)
		i++;
	S.nlength = i+1;
	delete[]S.nstring;
	S.nstring = new wchar_t[(i+1)];
	for(int j = 0; j < (i) ; j++)
		S.nstring[j] = input[j];
	S.nstring[i] = '\0';
	delete[]input;*/
	return IS;
	unguardf(S->towchar())
}
HString& HString::operator =(const HString &S)
{
	guard
	nlength = S.nlength;
	nstring = (wchar_t*)db_realloc(nstring,nlength * 2);
	wcscpy(nstring,S.nstring);
	return *this;
	unguardf(nstring)
}
HString& HString::operator = (const wchar_t* S)
{
	guard
	nlength = wcslen(S)+1;
	nstring = (wchar_t*)db_realloc(nstring,nlength * 2);
	wcscpy(nstring,S);
	//nstring[nlength-1] = L'\0';
	return *this;
	unguardf(nstring)
}
HString& HString::operator = (INT S)
{
	guard
	UINT64 Factor, Alloc, Count;
	bool bNeg = (S < 0);
	S = appAbs(S);
	numDigits(S,&Count,&Factor,&Alloc);
	nstring = (wchar_t*) db_realloc(nstring,Alloc + (nlength * 2) + ( bNeg ? 2 : 0 ) );

	//Attempt to keep everything in registers, reuse this variable   
	Alloc = nlength > 0? nlength  - 1 : 0;

	if(bNeg)
		*( nstring + Alloc ) = L'-';
	Alloc += 2;

	for(;;)
	{
		*( nstring + Alloc ) = L'0' + ( S % Factor);
		Factor /= 10;
		Alloc += 2;
		if(Factor < 10) 
			break;
	}
	nlength += Count;
	return *this;
	unguardf(nstring)
}
HString& HString::operator = (UINT S)
{
	guard
	UINT64 Factor, Alloc, Count;
	numDigits(S,&Count,&Factor,&Alloc);
	nstring = (wchar_t*) db_realloc(nstring,Alloc + (nlength * 2) );

	//Attempt to keep everything in registers, reuse this variable   
	Alloc = nlength > 0? nlength  - 1 : 0;

	Alloc += 2;

	for(;;)
	{
		*( nstring + Alloc ) = L'0' + ( S % Factor);
		Factor /= 10;
		Alloc += 2;
		if(Factor < 10) 
			break;
	}
	nlength += Count;
	return *this;
	unguardf(nstring)
}
HString& HString::operator = (INT64 S)
{
	guard
	UINT64 Factor, Alloc, Count;
	bool bNeg = (S < 0);
	S = appAbs(S);
	numDigits(S,&Count,&Factor,&Alloc);
	nstring = (wchar_t*) db_realloc(nstring,Alloc + (nlength * 2) + ( bNeg ? 2 : 0 ) );

	//Attempt to keep everything in registers, reuse this variable   
	Alloc = nlength > 0? nlength  - 1 : 0;

	if(bNeg)
		*( nstring + Alloc ) = L'-';
	Alloc += 2;

	for(;;)
	{
		*( nstring + Alloc ) = L'0' + ( S % Factor);
		Factor /= 10;
		Alloc += 2;
		if(Factor < 10) 
			break;
	}
	nlength += Count;
	return *this;
	unguardf(nstring)
}
HString& HString::operator = (UINT64 S)
{
	guard
	UINT64 Factor, Alloc, Count;
	numDigits(S,&Count,&Factor,&Alloc);
	nstring = (wchar_t*) db_realloc(nstring,Alloc + (nlength * 2) );

	//Attempt to keep everything in registers, reuse this variable   
	Alloc = nlength > 0? nlength  - 1 : 0;

	Alloc += 2;

	for(;;)
	{
		*( nstring + Alloc ) = L'0' + ( S % Factor);
		Factor /= 10;
		Alloc += 2;
		if(Factor < 10) 
			break;
	}
	nlength += Count;
	return *this;
	unguardf(nstring)
}
HString& HString::operator = (SHORT S)
{
	guard
	UINT64 Factor, Alloc, Count;
	bool bNeg = (S < 0);
	S = appAbs(S);
	numDigits(S,&Count,&Factor,&Alloc);
	nstring = (wchar_t*) db_realloc(nstring,Alloc + (nlength * 2) + ( bNeg ? 2 : 0 ) );

	//Attempt to keep everything in registers, reuse this variable   
	Alloc = nlength > 0? nlength  - 1 : 0;

	if(bNeg)
		*( nstring + Alloc ) = L'-';
	Alloc += 2;

	for(;;)
	{
		*( nstring + Alloc ) = L'0' + ( S % Factor);
		Factor /= 10;
		Alloc += 2;
		if(Factor < 10) 
			break;
	}
	nlength += Count;
	return *this;
	unguardf(nstring)
}
HString& HString::operator = (USHORT S)
{
	guard
	UINT64 Factor, Alloc, Count;
	numDigits(S,&Count,&Factor,&Alloc);
	nstring = (wchar_t*) db_realloc(nstring,Alloc + (nlength * 2) );

	//Attempt to keep everything in registers, reuse this variable   
	Alloc = nlength > 0? nlength  - 1 : 0;

	Alloc += 2;

	for(;;)
	{
		*( nstring + Alloc ) = L'0' + ( S % Factor);
		Factor /= 10;
		Alloc += 2;
		if(Factor < 10) 
			break;
	}
	nlength += Count;
	return *this;
	unguardf(nstring)
}
HString& HString::operator = (DOUBLE S)
{
	guard
	UINT64 Factor, Alloc, Count, Num;
	DOUBLE T;
	bool bNeg = (S < 0);
	S = fabs(S);
	Num =	(UINT64)S;
	S   -=	(DOUBLE)Num;
	numDigits(Num,&Count,&Factor,&Alloc);
	nstring = (wchar_t*) db_realloc(nstring,Alloc + (nlength * 2) + ( bNeg ? 2 : 0 ) + 30 );
	nlength += Count;

	//Attempt to keep everything in registers, reuse this variable   
	Alloc = nlength > 0? nlength  - 1 : 0;

	if(bNeg)
		*( nstring + Alloc ) = L'-';
	Alloc += 2;

	for(;;)
	{
		*( nstring + Alloc ) = L'0' + ( Num % Factor);
		Factor /= 10;
		Alloc += 2;
		if(Factor < 10) 
			break;
	}
	for(Count = 0;;++Count)
	{
		S *= 10;
		Factor = (UINT64) S;		//Reuse this variable, truncate the fraction
		T = (DOUBLE) Factor;		//Convert to an integer using cvtsd2si SSE2
		S -= T;
		*( nstring + Alloc ) = L'0' + ( S * 10);
		if(Count == 14)
			break;	//We're done.
	}
	return *this;
	unguardf(nstring)
}
HString& HString::operator = (FLOAT S)
{
	guard
	UINT64 Factor, Alloc, Count, Num;
	DOUBLE T;
	bool bNeg = (S < 0);
	S = fabs(S);
	Num =	(UINT64)S;
	S   -=	(DOUBLE)Num;
	numDigits(Num,&Count,&Factor,&Alloc);
	nstring = (wchar_t*) db_realloc(nstring,Alloc + (nlength * 2) + ( bNeg ? 2 : 0 ) + 30 );
	nlength += Count;

	//Attempt to keep everything in registers, reuse this variable   
	Alloc = nlength > 0? nlength  - 1 : 0;

	if(bNeg)
		*( nstring + Alloc ) = L'-';
	Alloc += 2;

	for(;;)
	{
		*( nstring + Alloc ) = L'0' + ( Num % Factor);
		Factor /= 10;
		Alloc += 2;
		if(Factor < 10) 
			break;
	}
	for(Count = 0;;++Count)
	{
		S *= 10;
		Factor = (UINT64) S;		//Reuse this variable, truncate the fraction
		T = (DOUBLE) Factor;		//Convert to an integer using cvtsd2si SSE2
		S -= T;
		*( nstring + Alloc ) = L'0' + ( S * 10);
		if(Count == 6)
			break;	//We're done.
	}
	return *this;
	unguardf(nstring)
}
HString& HString::operator = (BYTE S)
{
	guard
	UINT64 Factor, Alloc, Count;
	bool bNeg = (S < 0);
	S = appAbs(S);
	numDigits(S,&Count,&Factor,&Alloc);
	nstring = (wchar_t*) db_realloc(nstring,Alloc + (nlength * 2) + ( bNeg ? 2 : 0 ) );

	//Attempt to keep everything in registers, reuse this variable   
	Alloc = nlength > 0? nlength  - 1 : 0;

	if(bNeg)
		*( nstring + Alloc ) = L'-';
	Alloc += 2;

	for(;;)
	{
		*( nstring + Alloc ) = L'0' + ( S % Factor);
		Factor /= 10;
		Alloc += 2;
		if(Factor < 10) 
			break;
	}
	nlength += Count;
	return *this;
	unguardf(nstring)
}
HString& HString::append(const wchar_t* in, int slen)
{
	guard	
	size_t len = slen>0?slen:wcslen(in);
	nstring = (wchar_t*)db_realloc(nstring,(nlength + len) * 2);
	for(int i=0;i<len;i++) 
		*(nstring + nlength - 1 + i) = *(in + i);
	nlength += len;
	nstring[nlength-1] = 0;
	return *this;
	unguard
}
HString& HString::append(const HString& in)
{
	return append(in.nstring);
}
HString& HString::appendStart(const wchar_t* in)
{
	guard
		size_t len = wcslen(in);
		nlength += len;
		nstring = (wchar_t*)db_realloc(nstring,nlength * 2);
		for(int i=nlength-1; i>=len;--i)
			nstring[i] = nstring[i - len];
		for(int i=0;i<len;++i)
			nstring[i] = in[i];
		nstring[nlength-1] = L'\0';
		return *this;
	unguard
}
HString operator + (HString& S1, HString & S2)//Concat two strings
{
	//Need to create a variable on the stack
	HString str(S1);
	return (str.append(S2));
}
HString operator + (HString& S1, wchar_t* S2)//Concat two strings
{
	//Need to create a variable on the stack
	HString str(S1);
	return (str.append(S2));
}
__forceinline const wchar_t* HString::towchar() const
{
	return nstring;
}
char* HString::tochar()
{
	char* buffer = getChr();
	size_t len = wcstombs ( buffer, nstring, 128 );
	buffer[len] = '\0';
	return buffer;
}
//TODO: Optimize
HString HString::tolower()
{
	wchar_t c;
	wchar_t* lower = (wchar_t*)alloca(nlength * 2);
	for(int i=0;i<nlength;i++)
	{
		c=nstring[i];
		lower[i]=towlower(c);
	}
	lower[nlength-1]='\0';
    HString str(lower);
	return str;
}
HString HString::toupper()
{
	wchar_t c;
	wchar_t* upper = (wchar_t*)alloca(nlength * 2);
	for(int i=0;i<nlength;i++)
	{
		c=nstring[i];
		upper[i]=towupper(c);
	}
	upper[nlength-1]='\0';
    HString str(upper);
	return str;
}
void HString::dolower()
{
	guard
	for(int i=0;i<nlength-1;i++)
		nstring[i]=towlower(nstring[i]);
	unguard
}
void HString::doupper()//TODO: Optimize away the duplicated array
{
	guard
	for(int i=0;i<nlength-1;i++)
		nstring[i]=towupper(nstring[i]);
	unguard
}
BYTE* HString::tobytes()
{
  guard
	if(nlength<2) return NULL;
	//We need an array that can fit ( this limits to 2 bytes for each character )
  BYTE* bytes = (BYTE*)db_malloc((nlength * 2)+2);
  bytes[0] = (BYTE) (nlength & 0x00FF);//First two bytes contains the length of the array
  bytes[1] = (BYTE) ((nlength & 0xFF00) >> 8);
  int byte_index;
  int iterations = nlength;
  byte_index = 2;
  for(INT short_index = 0; short_index < iterations; short_index++)
  {
    bytes[byte_index++] = (BYTE) (nstring[short_index] & 0x00FF); 
    bytes[byte_index++] = (BYTE) ((nstring[short_index] & 0xFF00) >> 8);
  }

  return bytes;
  unguardf(nstring)
}
HString* operator += (HString& S0, HString& S1)
{
	return &S0.append(S1);
}
HString* operator += (HString& S0,wchar_t* S1)
{
	return &S0.append(S1);
}
//Optimize!
HString* operator +=(HString& S0,UINT64 s1)
{
	wchar_t chr[20];
	swprintf(chr, L"%u", s1);
	return &S0.append(chr);
}		 
HString* operator +=(HString& S0,INT64 s1)
{
	wchar_t chr[19];
	swprintf(chr, L"%i", s1);
	return &S0.append(chr);
}	 
HString* operator +=(HString& S0,float s1)
{
	wchar_t chr[326];
	swprintf(chr, L"%f", s1);
	return &S0.append(chr);
}		 
HString* operator +=(HString& S0,double s1)
{
	wchar_t chr[326];
	swprintf(chr, L"%.13f", s1);
	return &S0.append(chr);
}
HString* operator-= (HString& S0,const wchar_t* s1)
{
	int pos = S0.findexact((wchar_t*)s1);
	if(pos != -1)
		S0.detach(pos,wcslen(s1));
	return &S0;
}
bool appwcscmp_ci(const wchar_t* a,const wchar_t* b)
{
	guard
		size_t as = wcslen(a),bs = wcslen(b);
	if(as == bs)
	{
		for(int i=0;i<as;++i)
		{
			if(a[i] != b[i] && ( ( (a[i] < L'A' || a[i] > L'z')  || (b[i] < L'A' || b[i] > L'z') ) || abs(a[i] - b[i]) != 0x20) )
				return false;
		}
		return true;
	}
	else return false;
	unguardf(a)
}
bool appwcscmp(const wchar_t* a,const wchar_t* b)
{
	guard
		size_t as = wcslen(a),bs = wcslen(b);
	if(as == bs)
	{
		for(int i=0;i<as;i++)
			if(a[i] != b[i])
				return false;
		return true;
	}
	else return false;
	unguardf(a)
}

/*
THE END
*/
