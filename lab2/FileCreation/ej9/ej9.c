#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	struct stat buf;
	if(stat(argv[1],&buf) == -1) {
		perror("Stat Error");
		return -1;
	}

	printf("ID of device containing the file: %s\n", (string) buf.st_dev);
	printf("Inode number: %s\n", (string) buf.st_ino);
