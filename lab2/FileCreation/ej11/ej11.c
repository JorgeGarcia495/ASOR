#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <inttypes.h>
#include <time.h>

int main(int argc, char* argv[]) {
	struct stat buf;
	const char hardl[5] = ".hard";
	const char syml[4] = ".sym";
	char hardFile[15];
	char softFile[15];

	strcpy(hardFile,argv[1]);
	strcpy(softFile,argv[1]);
	strcat(hardFile,hardl);
	strcat(softFile,syml);
	
	if(stat(argv[1],&buf) == -1) {
		perror("Stat Error");
		return -1;
	}

	switch(buf.st_mode & S_IFMT) {
		case S_IFREG:
			if(link(argv[1],hardFile) != 0) {
				printf("%s\n",hardFile);
				perror("Error in hard linking");
				return -1;
			}
			if(symlink(argv[1],softFile) != 0) {
				perror("Error in soft linking");
				return -1;
			}
			printf("Correctly linked.\n");
			break;
		default:
			printf("That doesn't look like a regular file. Is it? %s\n", argv[1]);
			return -1;
	}	
		return 0;
}
