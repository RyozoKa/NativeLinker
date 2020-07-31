/*
	String class
	16bit characters
	Revision:
		Christopher Békési - Initial build 2015-06-21
*/

#if WIN32 /*MSVC*/
#include "HypWin32.h"
#pragma once
#endif

#include <vector>

#define THISCLASS HString


class DLLEXPORT HString : public HObject
{
public:

	 DECLARE_CLASS(HString,HObject);
	 SCRIPT_CLASS(String,59806657181221513);

	 HString(void);
	 HString(const wchar_t *str,int len = 0);
	 HString(const char *str,int len = 0);
	 HString(const wchar_t str);
	 HString(HString& in);
	 HString(const HString& in);
	 HString(BYTE* bytes);//for a bytearray
	 HString(class HypFile*);
	 ~HString(void);
	 void deserialize(BYTE*);
	 void deserialize(class HypFile*);
	 void StrCpy(HString& dest, HString& from);
	 std::vector<HString*> split(wchar_t* str);
	 std::vector<STRING> splitCode();
	 bool ReplaceAt(const wchar_t* str,INT start,INT stop);
	 bool ReplaceWith(const wchar_t* str,const wchar_t* with);
	 INT Count(const wchar_t* str);
	 int length() const;
	 const wchar_t& at(int pos) const;
	 bool isEmpty();
	 const wchar_t* towchar() const;
	 char* tochar();
	 HString tolower();
	 void dolower();
	 BYTE* tobytes();
	 HString* clear();
	 HString toupper();
	 void doupper();
	 void swap(HString& from);
	 HString substr(int start, int end=0);
	 void detach(int start, int end);
	 int find(wchar_t*,int index=0);//Returns -1 if nothing found
	 int findlast(wchar_t*, int index=0);
	 int findexact(wchar_t*,int index=0);
	 bool beginsWith(const wchar_t*) const;
	 bool endsWith(const wchar_t*) const;
	 wchar_t& operator [] (int pos);
	 friend std::wostream& operator << (std::wostream & OS,HString *S);
	 friend std::wostream& operator << (std::wostream & OS,HString S);
	 friend std::wistream& operator >> (std::wistream & IS,HString *S);
	 HString& append(const wchar_t* in, int len = 0);
	 HString& append(const HString& in);
	 HString& appendStart(const wchar_t*);
	 HString& operator = (const HString& S);
	 HString& operator = (const wchar_t* S);
	 HString& operator = (INT S);
	 HString& operator = (UINT S);
	 HString& operator = (INT64 S);
	 HString& operator = (UINT64 S);
	 HString& operator = (SHORT S);
	 HString& operator = (USHORT S);
	 HString& operator = (DOUBLE S);
	 HString& operator = (FLOAT S);
	 HString& operator = (BYTE S);
	DLLEXPORT friend  bool operator ==(const HString&, const HString&);
	DLLEXPORT friend  bool operator |=(const HString&, const HString&);
	DLLEXPORT friend  bool operator |=(const HString&, const wchar_t*);
	DLLEXPORT friend  bool operator ==(const HString&, const wchar_t*);
	DLLEXPORT friend  bool operator !=(const HString&, const HString&);
	DLLEXPORT friend  bool operator !=(const HString&, const wchar_t*);
	DLLEXPORT friend HString operator + (HString&,HString &);
	DLLEXPORT friend HString operator + (HString&,wchar_t*);
	private:
	wchar_t* nstring;
	USHORT nlength;
	friend class HypBuffer;
	friend class HypPath;

	//static string allocation
	static char ch[128][128];
	static USHORT idx;
	static wchar_t wch[128][128];
	static USHORT widx;
public:
	static char* getChr();
	static wchar_t* getWchr();
};
 DLLEXPORT HString* operator+= (HString& S0,HString& S1);
 DLLEXPORT HString* operator+= (HString& S0,wchar_t* S1);
 DLLEXPORT HString* operator+= (HString& S0,UINT64 S1);
 DLLEXPORT HString* operator+= (HString& S0,INT64 S1);
 DLLEXPORT HString* operator+= (HString& S0,float S1);
 DLLEXPORT HString* operator+= (HString& S0,double S1);
 DLLEXPORT HString* operator-= (HString& S0,const wchar_t* s1);

 // Optimized std functions

 bool appwcscmp(const wchar_t*,const wchar_t*);
 bool appwcscmp_ci(const wchar_t*,const wchar_t*);

/*
THE END
*/