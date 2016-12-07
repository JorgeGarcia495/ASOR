#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

extern int errno;

int main(){

	int errnum;
	int newuid = 0;

	uid_t myuid = getuid();
	gid_t mygid = getgid();
	uid_t myeuid = geteuid();
	gid_t myegid = getegid();

	printf("My uid is %d\n", myuid);
	printf("My gid is %d\n", mygid);
	printf("My euid is %d\n", myeuid);
	printf("My egid is %d\n", myegid);

	/* Check for error occurrence and notify it with the appropiate call */	

	if(setuid(0)==-1){
		perror("Error:");
		printf("Value of errno: %d\n", errno);
		printf("El string es: %s\n", strerror(errno));
	}

		

		/*
	if(mygid != 0){
		errnum = EPERM;
		fprintf(stderr, "Value of errno: %d\n", errnum);
		fprintf(stderr, "Not in root group: %s\n", strerror(errnum));
		/*
		perror("Error printed by perror");
		
		return 1;	
	}

	if(newuid > 1000 || newuid < 0){
		errnum = EINVAL;
		fprintf(stderr, "Value of errno: %d\n", errnum);
		fprintf(stderr, "Uid not valid: %s\n", strerror(errnum));
		/*
		perror("Error printed by perror");
	
		return 1;
	}
	setuid(newuid);
	*/
	return 1;
}

