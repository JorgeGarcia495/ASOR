#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[]){

	struct addrinfo thints;

	memset(thints, 0, sizeof(thints));
	thints.ai_flags=0;
	thints.ai_family=AF_UNSPEC;
	thints.ai_socktype=SOCK_STREAM;
	thints.ai_protocol=IPPROTO_TCP;

	struct addrinfo *res = NULL;

	if(getaddrinfo("www.example.com","http", &thints, &res) != 0) {
		perror("fallo1");
		return -1;
	}


