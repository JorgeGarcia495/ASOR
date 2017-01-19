#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
//#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	struct flock myflock;
	int fd = open(argv[1],O_WRONLY);
	//int fd = open(argv[1],O_CREAT | O_RDWR);
	int ret = 0;

	if(fd==-1){
		perror("Couldn't open file.");
		return -1;
	}

	myflock.l_type   = F_WRLCK;   /* F_RDLCK, F_WRLCK, F_UNLCK    */
	myflock.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
	myflock.l_start  = 0;        /* Offset from l_whence         */
	myflock.l_len    = 0;        /* length, 0 = to EOF           */
	myflock.l_pid    = getpid(); /* our PID                      */    


	/* Try to get the lock */

	ret = fcntl(fd,F_GETLK,&myflock);

	if(ret==-1){
		perror("Error getting lock.");
		close(fd);
		return -1;
	}

	if(myflock.l_type==F_WRLCK) {
		printf("The process %ld has already a write lock.\n", myflock.l_pid);
		printf("Process locked\n");
		close(fd);
		return -1;
	}

	if(myflock.l_type==F_RDLCK) {
		printf("The process %ld has already a read lock.\n", myflock.l_pid);
		printf("Process locked\n");
		close(fd);
		return -1;
	}

	/* Set our lock type */

	myflock.l_type   = F_WRLCK;   /* F_RDLCK, F_WRLCK, F_UNLCK    */

	if(fcntl(fd, F_SETLK, &myflock) == -1) {
		if(errno == EACCES || errno == EAGAIN) {
			perror("Already locked by another process.\n");
			close(fd);
			return -1;
		} else {
			perror("Unexpected error setting lock\n");
			close(fd);
			return -1;
		}
	} else {
		/* Lock granted */

		/* Perform I/O operations */
		time_t mytime;
		mytime = time(NULL);
		struct tm * mylocaltime;
		mylocaltime = localtime(&mytime);

		const char * buf[100], * format = "Tiempo %T\n";

		int length = strftime(buf, 100, format, mylocaltime);

		if(length == 0){
			puts("Coludn format time");
			exit(EXIT_FAILURE);
		}

		write(fd, buf, length);

		/* Sleep for 30 seconds */
		
		sleep(30);

		/* Unlock the locked bytes */

		myflock.l_type = F_UNLCK;
		myflock.l_whence = SEEK_SET;
		myflock.l_start = 0;
		myflock.l_len = 0;

		if(fcntl(fd, F_SETLK, &myflock) == -1){
			perror("Error removing lock");
			close(fd);
			return -1;
		}
	}
	close(fd);
	return 0;
}
