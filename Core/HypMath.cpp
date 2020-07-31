#include "Core.h"

__forceinline UINT64 appAbs(INT64 v) 
{
  return v > 0? v : ~v + 1;
}

__forceinline void numDigits(UINT64 number, UINT64 *OutCnt, UINT64 *OutFactor, UINT64 *OutAlloc)
{
	if(number < 10)
	{
		*OutCnt		= 1;
		*OutFactor	= 1;
		*OutAlloc	= 4;
	}
	else if(number < 100)
	{
		*OutCnt		= 2;
		*OutFactor	= 10;
		*OutAlloc	= 6;
	}
	else if(number < 1000)
	{
		*OutCnt		= 3;
		*OutFactor	= 100;
		*OutAlloc	= 8;
	}
	else if(number < 10000)
	{
		*OutCnt		= 4;
		*OutFactor	= 1000;
		*OutAlloc	= 10;
	}
	else if(number < 100000)
	{
		*OutCnt		= 5;
		*OutFactor	= 10000;
		*OutAlloc	= 12;
	}
	else if(number < 1000000)
	{
		*OutCnt = 6;
		*OutFactor = 100000;
		*OutAlloc= 14;
	}
	else if(number < 10000000)
	{
		*OutCnt = 7;
		*OutFactor = 1000000;
		*OutAlloc= 16;
	}
	else if(number < 100000000) 
	{
		*OutCnt = 8;
		*OutFactor = 10000000;
		*OutAlloc= 18;
	}
	else if(number < 1000000000) 
	{
		*OutCnt = 9;
		*OutFactor = 100000000;
		*OutAlloc= 20;
	}
	else if(number < 10000000000) 
	{
		*OutCnt = 10;
		*OutFactor = 1000000000;
		*OutAlloc= 22;
	}
	else if(number < 100000000000) 
	{
		*OutCnt = 11;
		*OutFactor = 10000000000;
		*OutAlloc= 24;
	}
	else if(number < 1000000000000) 
	{
		*OutCnt = 12;
		*OutFactor = 100000000000;
		*OutAlloc= 26;
	}
	else if(number < 10000000000000) 
	{
		*OutCnt = 13;
		*OutFactor = 1000000000000;
		*OutAlloc= 28;
	}
	else if(number < 100000000000000) 
	{
		*OutCnt = 14;
		*OutFactor = 10000000000000;
		*OutAlloc= 30;
	}
	else if(number < 1000000000000000) 
	{
		*OutCnt = 15;
		*OutFactor = 100000000000000;
		*OutAlloc= 32;
	}
	else if(number < 10000000000000000) 
	{
		*OutCnt = 16;
		*OutFactor = 1000000000000000;
		*OutAlloc= 34;
	}
	else if(number < 100000000000000000) 
	{
		*OutCnt = 17;
		*OutFactor = 10000000000000000;
		*OutAlloc= 36;
	}
	else if(number < 1000000000000000000) 
	{
		*OutCnt = 18;
		*OutFactor = 100000000000000000;
		*OutAlloc= 38;
	}
	else
	{
		*OutCnt = 19;
		*OutFactor = 1000000000000000000;
		*OutAlloc= 40;
	}
}