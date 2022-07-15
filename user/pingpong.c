#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1
#define BUFSIZE 4

const char *PING = "ping";
const char *PONG = "pong";

int main(int argc, char **argv) {
	int pid, p1[2], p2[2];
	char buf[BUFSIZE];

	if (pipe(p1) < 0 || pipe(p2) < 0) {
		printf("pipe() failed\n");
		exit(1);
	}
	
	pid = fork();
	if (fork < 0) {
		printf("fork() failed\n");
		exit(1);
	}

	if(pid > 0) { // parent process
		write(p1[WRITE], PING, strlen(PING));
		wait(0);
		read(p2[READ], buf, BUFSIZE);
		printf("%d: received %s\n", getpid(), buf);
	} else { // child process
		read(p1[READ], buf, BUFSIZE);
		printf("%d: received %s\n", getpid(), buf);
		write(p2[WRITE], PONG, strlen(PONG));
	}
	exit(0);
}