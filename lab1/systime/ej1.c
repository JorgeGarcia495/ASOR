#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <locale.h>

int main(){

	time_t rawtime;
	long test_time;
	int i = 0;

	struct tm * timeinfo;
	struct timeval mytime;
	setlocale(LC_TIME, "es_ES.UTF-8");
	setlocale(LC_ALL, "");
	time(&rawtime);
	printf("Time: %s", ctime(&rawtime)); 

	gettimeofday(&mytime,NULL);

	test_time = mytime.tv_usec;

	while(i<10000){
		i++;
	}
	gettimeofday(&mytime,NULL);
	printf("Difference time: %ld\n", mytime.tv_usec-test_time);

	printf("Locale is: %s\n", setlocale(LC_TIME, ""));
	timeinfo = localtime(&rawtime);
	printf("We are in %d\n", 1900+timeinfo->tm_year);

	return 1;
}

