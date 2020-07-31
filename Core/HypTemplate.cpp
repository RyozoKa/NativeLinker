/*

	Revision:
		Christopher Békési - initial build
*/


#include "Core.h"
//#include "HypScript.h"
//----------------------------------------
//HName

HName hashName(const char *s)
{
	guard
    union { UINT64 h; BYTE u[8]; };
    int i=0; h=strlen(s);
	char w[64];
	char* st = w;
	strcpy(w,s);
	w[h] = '\0';
	toLower(w);
	checkAlpha(w);
    while (*st) { u[i%8] += *st + i + (*st >> ((h/(i+1)) % 5)); st++; i++; }
    return h; //64-bit
	unguardf(s)
}
HName hashName(const wchar_t *s)
{
	guard
    union { UINT64 h; BYTE u[8]; };
    int i=0; h=wcslen(s);
	//Support constants
	wchar_t w[64];
	wchar_t *st = w;
	wcscpy(w,s);
	toLower(w);
	checkAlpha(w);
    while (*st) { u[i%8] += *(char*)st + i + (*(char*)st >> ((h/(i+1)) % 5)); st++; i++; }
    return h; //64-bit
	unguardf(s)
}
void toLower(wchar_t* w) //A-Z
{
	int Len = wcslen(w);
	for(int i=0;i<Len;i++)
	{
		if(w[i] >= L'A' && w[i] <= L'Z')
			w[i] = L'a' + (w[i] - L'A');
	}
}
void toLower(char* w)
{
	int Len = strlen(w);
	for(int i=0;i<Len;i++)
	{
		if(w[i] >= L'A' && w[i] <= L'Z')
			w[i] = L'a' + (w[i] - L'A');
	}
}
void checkAlpha(const wchar_t* s)
{
	const wchar_t* st=s;
	guard
	while(*s)
	{
		if((*s > L'ö' || *s < L'a') && (*s < L'0' || *s > L'9') && *s != L'_' && *s != L'@' && *s != L'?')
			throw L"Not an alpha string.";
		s++;
	}
	unguardf(st)
}
void checkAlpha(const char* s)
{
	guard
	while(*s)
	{
		if((*s > 'ö' || *s < 'a') && (*s < '0' || *s > '9') && *s != '_' && *s != '@' && *s != '?')
			throw L"Not an alpha string.";
		s++;
	}
	unguard
}
bool isAlpha(const wchar_t* s)
{
	const wchar_t* st=s;
	guard
	while(*s)
	{
		if(( *s > L'z' || *s < L'a') && ( *s > L'Z' || *s < L'A') && (*s < L'0' || *s > L'9') && *s != L'_' && *s != L'@' && *s != L'?')
			return false;
		s++;
	}
	return true;
	unguardf(st)
}
bool isAlpha(const char* s)
{
	guard
	while(*s)
	{
		if((*s > 'ö' || *s < 'a') && (*s < '0' || *s > '9') && *s != '_' && *s != '@' && *s != '?')
			return false;
		s++;
	}
	return true;
	unguard
}

Function::Function(HypBuffer& bf, HClass* c)
{
	Id = bf.getUInt64();
	META[0] = bf.getUShort();
	META[2] = bf.getUShort();					//NOTE: Too small?
	Code = (BYTE*)malloc(META[2]);
	memcpy(Code,bf.bytes + bf.pos,META[2]);
	bf.pos += META[2];
	META[1] = bf.getByte();
	name = bf.getString();
}
Function::~Function()
{
	free(Code);
	delete name;
	//TODO: Continue.
}
void Function::Serialize(HypBuffer& buf)
{
	buf.putUInt64(Id);
	buf.putUShort(META[0]);
	buf.putUShort(META[2]);
	buf.bytes = (BYTE*)realloc(buf.bytes,buf.len + META[2]);
	memcpy(buf.bytes + buf.len,Code,META[2]);
	buf.len += META[2];
	buf.putByte(META[1]);

	buf.putString(*name);
}