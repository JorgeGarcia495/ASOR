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
	char ftype[20];
	
	if(stat(argv[1],&buf) == -1) {
		perror("Stat Error");
		return -1;
	}

	switch(buf.st_mode & S_IFMT) {
		case S_IFSOCK:
			strcpy(ftype, "Socket");
			break;
		case S_IFLNK:
			strcpy(ftype, "Symbolic link");
			break;
		case S_IFREG:
			strcpy(ftype, "Regular file");
			break;
		case S_IFBLK:
			strcpy(ftype, "Block device");
			break;
		case S_IFDIR:
			strcpy(ftype, "Directory");
			break;
		case S_IFCHR:
			strcpy(ftype, "Character device");
			break;
		case S_IFIFO:
			strcpy(ftype, "FIFO");
			break;
		default:
			printf("Are you sure?\n");
			return -1;
	}
	printf("Major: %d\n", major(buf.st_dev));
	printf("Minor: %d\n", minor(buf.st_dev));
	printf("Inode number: %ju\n", (uintmax_t) buf.st_ino);
	printf("File type: %s\n", ftype);
	printf("Last time the file was accessed: %lld.%.9ld\n", (long long)buf.st_atim.tv_sec, buf.st_atim.tv_nsec);
	return 0;
}
