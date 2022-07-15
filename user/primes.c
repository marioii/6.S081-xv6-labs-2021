#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>

#define READ 0
#define WRITE 1
#define PRIME_START 2
#define PRIME_END 35

void sieve(int[2]);

int
main(int argc, char *argv[])
{
	int pid, input[2];
	if (pipe(input) < 0) {
		printf("pipe() failed\n");
		exit(1);
	}

	pid = fork();

	if (pid < 0) {
		printf("fork() failed\n");
		exit(1);
	}

	if (pid > 0) {
		// 父进程关闭管道读端
		close(input[READ]);
		int i;
		for (i = PRIME_START; i <= PRIME_END; i++)
			write(input[WRITE], &i, sizeof(int));

		// 所有数字发送完毕，发送 -1 通知子进程结束
		i = -1;
		write(input[WRITE], &i, sizeof(int));
	} else {
		close(input[WRITE]);
		sieve(input);
		exit(0);
	}

	wait(0);
	exit(0);
}

void
sieve(int left_pipe[2])
{
	int left_num = 0;
	read(left_pipe[READ], &left_num, sizeof(int));
	if (left_num == -1)
		exit(0);
	printf("prime %d\n", left_num);

	int pid, right_pipe[2];
	if (pipe(right_pipe) < 0) {
		printf("pipe() failed\n");
		exit(1);
	}

	pid = fork();
	if (pid < 0) {
		printf("fork() failed\n");
		exit(1);
	}

	if (pid > 0) {
		close(right_pipe[READ]);
		int n = 0;

		// 管道读不到数据 或者 读到 -1， 都应该结束
		while (read(left_pipe[READ], &n, sizeof(int)) && n != -1) {
			if (n % left_num != 0)
				write(right_pipe[WRITE], &n, sizeof(int));
		}

		n = -1;
		write(right_pipe[WRITE], &n, sizeof(int));
		wait(0);
		exit(0);
	} else {
		close(right_pipe[WRITE]);
		close(left_pipe[READ]);
		sieve(right_pipe);
	}
}