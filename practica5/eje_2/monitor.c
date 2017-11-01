#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void die()
{
	perror(NULL);
	exit(-1);
}

void *func_a(void *arg)
{
	return NULL;
}

void *func_b(void *arg)
{
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

	return 0;
}
