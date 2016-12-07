#include <sys/utsname.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

//
//struct utsname {
//	char sysname[];    /* Operating system name (e.g., "Linux") */
//	char nodename[];   /* Name within "some implementation-defined network" */
//	char release[];    /* Operating system release (e.g., "2.6.28") */
//	char version[];    /* Operating system version */
//	char machine[];    /* Hardware identifier */
//#ifdef _GNU_SOURCE
//	char domainname[]; /* NIS or YP domain name */
//#endif
//}
//


int main(){

	struct utsname buf;
	if(uname(&buf) == -1) {
		perror("Uname error.");
		return -1;
	}

	printf("Operating system name: %s\n", buf.sysname);
	printf("Name within some implementation-defined network: %s\n", buf.nodename);
	printf("Operating system release: %s\n", buf.release);
	printf("Operating system version: %s\n", buf.version);
	printf("Hardware identifier: %s\n", buf.machine);
//	printf("NIS or YP domain name: %s\n", buf.domainname);

	return 1;
}
