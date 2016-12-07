#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(){
	int i = 0;
	for(;i<134;i++){
		fprintf(stderr, "%s\n", strerror(i));
	}
	return 1;
}
