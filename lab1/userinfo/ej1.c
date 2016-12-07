#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>

int main(){

	uid_t myuid = getuid();
	gid_t mygid = getgid();
	uid_t myeuid = geteuid();
	gid_t myegid = getegid();
	
	struct passwd *mypass;

	mypass = getpwuid(myuid);

	printf("My uid is %d\n", myuid);
	printf("My gid is %d\n", mygid);
	printf("My euid is %d\n", myeuid);
	printf("My egid is %d\n", myegid);


	printf("Login name: %s\n", mypass->pw_name);
	printf("Home directory: %s\n", mypass->pw_dir);
	printf("Real user name: %s\n", mypass->pw_gecos);

	printf("Password: %s\n", mypass->pw_passwd);


	return 1;
}

