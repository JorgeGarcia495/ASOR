#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int *fp = open("testfile.txt", O_CREAT, 0666);
	close(fp);
	return 0;
}

