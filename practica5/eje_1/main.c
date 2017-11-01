#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define N 2
#define FREQ 100
#define PERIOD (1000000000L / FREQ)
#define ITERATIONS 1000

long long avg[N];
timer_t timer;

void die()
{
	perror(NULL);
	abort();
}

void *start_routine(void *arg)
{
	long long average = 0;
	struct timespec sleep_time, prev, current;
	sleep_time.tv_sec = 0;
	sleep_time.tv_nsec = PERIOD;
	clock_gettime(CLOCK_MONOTONIC, &prev);

	for (int i = 0; i < ITERATIONS; i++) {
		if (nanosleep(&sleep_time, NULL))
			die();

		clock_gettime(CLOCK_MONOTONIC, &current);
		//printf("Current: %ld\nPrevious: %ld\n\nDifference: %ld\n\n", current.tv_nsec, prev.tv_nsec, current.tv_nsec - prev.tv_nsec);
		average += ((current.tv_nsec - prev.tv_nsec + (current.tv_sec - prev.tv_sec) * 1000000000L) - PERIOD);
		memcpy(&prev, &current, sizeof(struct timespec));
	}

	*((long long*)arg) = average / ITERATIONS;
	return NULL;
}

int main()
{
	pthread_t threads[N];

	for (int i = 0; i < N; i++)
		if (pthread_create(threads + i, NULL, start_routine, avg + i))
			die();
	

	for (int i = 0; i < N; i++)
		if (pthread_join(threads[i], NULL))
			die();

	for (int i = 0; i < N; i++)
		printf("El promedio del thread %i es %lld nanosegundos.\n", i, avg[i]);

	return 0;
}
