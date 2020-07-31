/*	Linux specific implementation of platform functions


*/

#ifdef _LINUX_

#include "HypLinux.h"
#include <ctime>
#include <time.h>
#include <sys/time.h>
#include "Core.h"

void UpdateTime()
{
time_t t = time(0);   // get time now
struct tm * now = localtime( & t );
struct timeval start;
gettimeofday(&start, NULL);
GMiliSeconds	= start.tv_usec/1000;
GSeconds		= now->tm_sec;
GMinutes		= now->tm_min;
GHours			= now->tm_hour;
GDay			= now->tm_mday;
GDayOfWeek		= now->tm_wday;
GMonth			= now->tm_mon;
GYear			= now->tm_year;
}

#endif