
#include "Core.h"
#include "HypOutput.h"


DLLEXPORT HypOutput::HypOutput() : dump(L"HyperEngineLog.log",FILE_WRITE), disp(_stdout) 
{
	UpdateTime();
	wsprintf(buffer,L"Log opened: %d-%d-%d %d:%d:%d",GYear,GMonth,GDay,GHours,GMinutes,GSeconds);
	dump.writeLine(buffer);
}

void HypOutput::Log(const wchar_t* form,...)
{
	wcscpy(buffer,L"Log: ");
	va_list vl;
	va_start(vl, form);
	vswprintf(buffer + 5,form, vl);
	va_end(vl);
	if(GbLogToFile)
		dump.writeLine(buffer);
	wcscat(buffer,L"\n");
	(disp)(buffer);
	
}

void HypOutput::Warn(const wchar_t* form,...)
{
	wcscpy(buffer,L"Warning: ");
	va_list vl;
	va_start(vl, form);
	vswprintf(buffer + 9,form, vl);
	va_end(vl);
	if(GbLogToFile)
		dump.writeLine(buffer);
	wcscat(buffer,L"\n");
	(disp)(buffer);
}

void HypOutput::Err(const wchar_t* form,...)
{
	wcscpy(buffer,L"Error: ");
	va_list vl;
	va_start(vl, form);
	vswprintf(buffer + 7,form, vl);
	va_end(vl);
	if(GbLogToFile)
		dump.writeLine(buffer);
	wcscat(buffer,L"\n");
	(disp)(buffer);
	//TODO: Call appExit
}

void HypOutput::_stdout(const wchar_t* buffer)
{
	std::fputws(buffer,stdout);
}