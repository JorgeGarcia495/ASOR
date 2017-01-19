#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {

	if(argc != 2) {
		printf("Usage: %s <dirName>\n", argv[0]);
		return -1;
	}

	DIR *mydir = opendir(argv[1]);

	if(mydir == NULL) {
		printf("Can't open directory: %s\n", strerror(errno));
		return -1;
	}

	struct dirent *current;
	struct stat filedata;
	unsigned long int totalsize;
	size_t nameLength = strlen(argv[1]);

	char *linkname;

	do{
		current = readdir(mydir);

		if(current != NULL) {
			char *absolutePath = malloc(sizeof (char)*(nameLength + strlen(current->d_name) + 3));

			strcpy(absolutePath, argv[1]);
			strcat(absolutePath, "/");
			strcat(absolutePath, current->d_name);

			if(stat(absolutePath, &filedata) == -1) {
				printf("Error analyzing file: %s\n", strerror(errno));
				free(absolutePath);
				return -1;
			}

			switch(filedata.st_mode & S_IFMT) {

				case S_IFLNK:
					/* Is it a symlink? */

					linkname = malloc(filedata.st_size + 1);
					printf("Leido: %s\nSupuesto: %s\n", readlink(absolutePath, linkname, filedata.st_size +1), filedata.st_size+1);
					printf("%s -> %s\n", current->d_name, linkname);
					free(linkname);

					break;

				case S_IFREG:
					/* Is it a regular file? */

					printf("%s", current->d_name);
					totalsize += ((filedata.st_blksize/8)*filedata.st_blocks);
					if(S_IXUSR & filedata.st_mode)
						printf("*");
					printf("\n");


					break;

				case S_IFDIR:
					/* Is it a directory? */
		
					if(S_ISDIR(filedata.st_mode))
						printf("%s/\n", current->d_name);

					break;
			}

			free(absolutePath);

		}

	}while(current!=NULL);

	printf("Size of all files: %ikB\n", (int) (totalsize/1024));
	closedir(mydir);
	return 0;
}
