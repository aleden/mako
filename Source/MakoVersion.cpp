#include "MakoVERSION.h"
#include "MakoException.h"
#include <cstring>
#include <ctime>
#include <cstdlib>

MAKO_BEGIN_NAMESPACE

UInt32 GetMonthNumFromName(const char* month)
{
	if (strcmp(month, "Jan") == 0)
		return 0;
	else if (strcmp(month, "Feb") == 0)
		return 1;
	else if (strcmp(month, "Mar") == 0)
		return 2;
	else if (strcmp(month, "Apr") == 0)
		return 3;
	else if (strcmp(month, "May") == 0)
		return 4;
	else if (strcmp(month, "Jun") == 0)
		return 5;
	else if (strcmp(month, "Jul") == 0)
		return 6;
	else if (strcmp(month, "Aug") == 0)
		return 7;
	else if (strcmp(month, "Sep") == 0)
		return 8;
	else if (strcmp(month, "Oct") == 0)
		return 9;
	else if (strcmp(month, "Nov") == 0)
		return 10;
	else if (strcmp(month, "Dec") == 0)
		return 11;

	throw Exception(Text("GetMonthNumFromName() failed."));
}

// \param[out] time The time to fill out from the information
// \param[in] month The name of the month as a 3 letter long c-string
// \param[in] day The number of the day of the month as a c-string
// \param[in] year The number of the year as a c-string
// \param[in] hour (Optional) The hours passed in the day as a c-string
// \param[in] minute (Optional) The minutes passed in the hour as a c-string
// \param[in] second (Optional) The seconds passed in the minute as a c-string
void GetSecondsSince(time_t& time,
					 const char* month,
					 const char* day,
					 const char* year,
					 const char* hour = nullptr,
					 const char* minute = nullptr,
					 const char* second = nullptr)
{
	UInt32 monthNum = GetMonthNumFromName(month);
	UInt32 dayNum   = static_cast<UInt32>(atoi(day));
	UInt32 yearNum  = static_cast<UInt32>(atoi(year));
	UInt32 hourNum  = hour   != nullptr ? static_cast<UInt32>(atoi(hour))   : 0;
	UInt32 minNum   = minute != nullptr ? static_cast<UInt32>(atoi(minute)) : 0;
	UInt32 secNum   = second != nullptr ? static_cast<UInt32>(atoi(second)) : 0;

	// tm_sec    seconds after the minute    0-61*
	// tm_min    minutes after the hour      0-59
	// tm_hour   hours since midnight        0-23
	// tm_mday   day of the month            1-31
	// tm_mon    months since January        0-11
	// tm_year   years since 1900	
	// tm_wday   days since Sunday           0-6
	// tm_yday   days since January 1        0-365
	// tm_isdst	 Daylight Saving Time flag	 > 0 or == 0 *
	// 
	// *tm_sec is generally 0-59. Extra range to accommodate for 
	// leap seconds in certain systems.
	//
	// The Daylight Saving Time flag (tm_isdst) is greater than 
	// zero if Daylight Saving Time is in effect, zero if Daylight 
	// Saving Time is not in effect, and less than zero if the 
	// information is not available.
	
	tm timeInfoStarted;
	memset(&timeInfoStarted, 0, sizeof(tm));
	timeInfoStarted.tm_sec   = secNum;
	timeInfoStarted.tm_min   = minNum;
	timeInfoStarted.tm_hour  = hourNum;
	timeInfoStarted.tm_mday  = dayNum;
	timeInfoStarted.tm_mon   = monthNum;
	timeInfoStarted.tm_year  = yearNum - 1900;
	
	// The original values of the members tm_wday and tm_yday of timeptr 
	// are ignored.
	timeInfoStarted.tm_wday  = -1;
	timeInfoStarted.tm_yday  = -1;
	
	timeInfoStarted.tm_isdst = -1;

	time = mktime(&timeInfoStarted);
}

Float32 GetMakoVersion()
{
#if 0
	// The mako game engine was started on Dec 18, 2008
	//static char monthStarted [] = "Dec";
	//static char dayStarted   [] = "18";
	//static char yearStarted  [] = "2008";


	// __TIME__ = "22:41:03"    (hh:mm:ss)
	// __DATE__ = "Jun 15 2009" (Mmm dd yyyy)
	
	static char monthCompiled [4] = { __DATE__[0], __DATE__[1], __DATE__[2], '\0' };
	static char dayCompiled   [3] = { __DATE__[4], __DATE__[5], '\0' };
	static char yearCompiled  [5] = { __DATE__[7], __DATE__[8], __DATE__[9], __DATE__[10], '\0' };
	static char hourCompiled  [3] = { __TIME__[0], __TIME__[1], '\0' };
	static char minCompiled   [3] = { __TIME__[3], __TIME__[4], '\0' };
	static char secCompiled   [3] = { __TIME__[6], __TIME__[7], '\0' };


	// Does not need to be computed again.
	time_t secondsSinceStarted = 1229587200;
	//GetSecondsSince(secondsSinceStarted, monthStarted, dayStarted, yearStarted);

	time_t secondsSinceCompiled;
	GetSecondsSince(secondsSinceCompiled, monthCompiled, dayCompiled, yearCompiled,
		hourCompiled, minCompiled, secCompiled);


	return static_cast<Float32>(secondsSinceCompiled - secondsSinceStarted)/21000000.f;
#else
	return 1.f;
#endif
}

MAKO_END_NAMESPACE                                                                                                                                                                                                                                                                                                                                                                                       