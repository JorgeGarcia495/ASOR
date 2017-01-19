#include <unistd.h>
#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){
	
	if(argc!=2){
		printf("Usage: %s <pid>\n", argv[0]);
		return -1;
	}

	int pid = atoi(argv[1]);

	int sch_num = sched_getscheduler(pid);

	if(sch_num == -1) {
		printf("Unable to get scheduler: %s\n", strerror(errno));
		return -1;
	}

	char *str_sch = malloc(sizeof(char)*14);

	switch(sch_num) {
		case SCHED_OTHER:
			str_sch = strcpy(str_sch, "SCHED_OTHER");
			break;
		case SCHED_FIFO:
			str_sch = strcpy(str_sch, "SCHED_FIFO");
			break;
		case SCHED_RR:
			str_sch = strcpy(str_sch, "SCHED_RR");
			break;
		default:
			str_sch = strcpy(str_sch, "UNIDENTIFIED");
			break;
	}


	struct sched_param prior;
	
	if(sched_getparam(pid, &prior) == -1) {
		printf("Unable to get priority: %s\n", strerror(errno));
		free(str_sch);
		return -1;
	}

	printf("pid %i has sched %s, priority %i (min %i max %i)\n", pid, str_sch, prior.sched_priority, sched_get_priority_min(sch_num), sched_get_priority_max(sch_num));

	free(str_sch);
	return 0;
}

