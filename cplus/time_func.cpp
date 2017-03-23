#include <time.h>
#include <sys/time.h> // gettimeofday()
#include <iostream>
#include <unistd.h>

void about_local_time()
{
	std::cout << "about_local_time()" << std::endl;

	time_t now = time(NULL);
	std::cout << "now = " << now << std::endl;

	// struct tm :
	// int tm_sec;     /* seconds (0 - 60) */
	// int tm_min;     /* minutes (0 - 59) */
	// int tm_hour;    /* hours (0 - 23) */
	// int tm_mday;    /* day of month (1 - 31) */
	// int tm_mon;     /* month of year (0 - 11) */
	// int tm_year;    /* year - 1900 */
	// int tm_wday;    /* day of week (Sunday = 0) */
	// int tm_yday;    /* day of year (0 - 365) */
	// int tm_isdst;   /* is summer time in effect? */
	// char *tm_zone;  /* abbreviation of timezone name */
	// long tm_gmtoff; /* offset from UTC in seconds */
	// 

	struct tm detail;
	localtime_r(&now, &detail); // get tm by time_t

	std::cout << "tm_sec = " << detail.tm_sec << std::endl;
	std::cout << "tm_min = " << detail.tm_min << std::endl;
	std::cout << "tm_hour = " << detail.tm_hour << std::endl;
	std::cout << "tm_wday = " << detail.tm_wday << std::endl;

	// zero hour, min, sec
	detail.tm_hour = 0;
	detail.tm_min = 0;
	detail.tm_sec = 0;

	time_t day_start_time = mktime(&detail); // get time_t by tm
	std::cout << "day_start_time = " << day_start_time << std::endl;
	
}

void about_time_val()
{
	std::cout << std::endl << "about_time_val()" << std::endl;
	/*
	struct timeval
	{
		time_t tv_sec; // second since Jan. 1, 1970
		susecond_t tv_usec; // microseconds
	}
	*/

	struct timeval tv;    
	gettimeofday(&tv,NULL); // get timeval from 1970

	long msTime1 = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	long usTime1 = tv.tv_sec * 1000000 + tv.tv_usec;
	std::cout << "1 =" <<  msTime1 << "ms" << std::endl;
	std::cout << "1 =" <<  usTime1 << "us" << std::endl;

	sleep(1);
	gettimeofday(&tv,NULL);    
	long msTime2 = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	long usTime2 = tv.tv_sec * 1000000 + tv.tv_usec;
	std::cout << "2 =" <<  msTime2 << "ms" << std::endl;
	std::cout << "2 =" <<  usTime2 << "us" << std::endl;


	std::cout << "total =" <<  msTime2 - msTime1 << "ms" << std::endl;
	std::cout << "total =" <<  usTime2 - usTime1 << "us" << std::endl;

	std::cout << "tv.tv_sec=" << tv.tv_sec << " tv.tv_usec=" << tv.tv_usec << std::endl;
}

int main(int argc, char **argv)
{
	
	about_local_time();
	about_time_val();

	return 0;
}
