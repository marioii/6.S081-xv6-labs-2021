#include <kernel/types.h>
#include <user/user.h>

#define STDERR 2

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(STDERR, "Usage: sleep some seconds\n");
		exit(1);
	}

	// argv[1] * 10 转换成秒
	sleep(atoi(argv[1]) * 10);
	exit(0);
}