#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#define STD_OUTPUT  1
#define ERR_OUTPUT  2

int main(int argc, char *argv[]){

	if(argc != 2){
		printf("Usage: ./ej1 <filename>\n");
		return -1;
	}

	int fd = open(argv[1], O_CREAT | O_RDWR, 0777);

	if(fd==-1){
		perror("Error opening file.");
		return -1;
	}

	int old_stdout = dup2(fd,STD_OUTPUT);

	if(old_stdout==-1){
		perror("Error duplicating file to std_out.");
		return -1;
	}
	int old_stdout2 = dup2(fd,ERR_OUTPUT);

	if(old_stdout2==-1){
		perror("Error duplicating file to std_err.");
		return -1;
	}

	printf("Redirecting...\n");
	puts("redirect puts");
	perror("Redirecting error");

	dup2(old_stdout,STD_OUTPUT);

	dup2(old_stdout2,ERR_OUTPUT);

	return 0;
}
