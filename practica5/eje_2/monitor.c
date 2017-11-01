#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define LOW 1
#define HIGH -1

void die()
{
	perror(NULL);
	exit(-1);
}

#define WRITE 1
#define READ 0
int pipes[2];

void *func_a(void *arg)
{
	float incoming;
	while (read(STDIN_FILENO, &incoming, sizeof(float)) > 0) {
		write(pipes[WRITE], &incoming, sizeof(float));
		if (incoming > 90.0)
			fprintf(stderr, "Mayor a 90C: %f\n", incoming);
	}
	fprintf(stderr, "Thread A: EOF\n");
	close(pipes[WRITE]);
	return NULL;
}

void *func_b(void *arg)
{
	float values[3];
	float last;
	while (read(pipes[READ], &last, sizeof(float)) > 0) {
		values[2] = values[1];
		values[1] = values[0];
		values[0] = last;
	}

	float avg = (values[0] + values [1] + values[2]) / 3.0f;
	fprintf(stderr, "Thread B: El promedio de los últimos tres valores es %f\n", avg);
	close(pipes[READ]);
	return NULL;
}

int main()
{
	pthread_t thread_a, thread_b;
	struct sched_param param_a, param_b;

	if (pipe(pipes))
		die();

	if (pthread_create(&thread_a, NULL, func_a, NULL))
		die();
	if (pthread_create(&thread_b, NULL, func_b, NULL))
		die();

	param_a.sched_priority = HIGH;
	pthread_setschedparam(thread_a, SCHED_RR, &param_a);
	param_a.sched_priority = LOW;
	pthread_setschedparam(thread_a, SCHED_RR, &param_b);

	if (pthread_join(thread_a, NULL))
		die();
	if (pthread_join(thread_b, NULL))
		die();

	return 0;
}
