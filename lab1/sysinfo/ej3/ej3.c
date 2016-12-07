#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>


int main(){
	long child_max;
	long length_max;
	long files_max;

	child_max = sysconf(_SC_CHILD_MAX);
	length_max = sysconf(_SC_ARG_MAX);
	files_max = sysconf(_SC_OPEN_MAX);

	if(child_max == -1){
		perror("Can't find maximum number of fle processes");
	} else {
		printf("Maximum number of child processes: %ld\n", child_max);
	}
	if(length_max == -1){
		perror("Can't find maximum length of arguments");
	} else {
		printf("Maximum length of arguments: %ld\n", length_max);
	}
	if(files_max == -1){
		perror("Can't find maximum number of files");
	} else {
		printf("Maximum number of files: %ld\n", files_max);
	}

	return 1;
}
