#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[]){

	long links_max;
	long pathsize_max;
	long filenamesize_max;

	char* path = argv[0];

	links_max = pathconf(path, _PC_LINK_MAX);
	pathsize_max = pathconf(path, _PC_PATH_MAX);
	filenamesize_max = pathconf(path, _PC_NAME_MAX);

	if(links_max == -1)
		perror("Can't find maximum number of links");
	else
		printf("Maximum number of links: %ld\n", links_max);

	if(pathsize_max == -1)
		perror("Can't find maximum size of path");
	else
		printf("Maximum size of path: %ld\n", pathsize_max);

	if(filenamesize_max == -1)
		perror("Can't find maximum size of a filename");
	else
		printf("Maximum size of a filename: %ld\n", filenamesize_max);

	return 1;
}

