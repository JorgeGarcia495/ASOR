#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>


int main() {
	mode_t mymask = umask(~((mode_t) S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP));
	printf("Umask ha valido %o\n", mymask);
	int fp = open("testfile.txt", O_CREAT, 0777);
	close(fp);
	umask(mymask);
	return 0;
}

