#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>


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

	int old_stdout = dup2(fd,1);

	if(old_stdout==-1){
		perror("Error duplicating file.");
		return -1;
	}

	printf("Redirecting...\n");
	puts("redirect puts");

	dup2(old_stdout,fd);

	return 0;
}
