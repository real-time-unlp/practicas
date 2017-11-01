#define _WITH_GETLINE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

void die()
{
	perror(NULL);
	exit(-1);
}

void wait_ns(long ns)
{
	struct timespec sleep_params;
	sleep_params.tv_sec = ns / 1000000000L;
	sleep_params.tv_nsec = ns % 1000000000L;
	nanosleep(&sleep_params, NULL);
}

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t linecap;

	if (argc != 2)
		return -1;

	FILE * file = fopen(argv[1], "r");
	if (file == NULL)
		die();


	while (getline(&line, &linecap, file) > 0) {
		float temp;
		long ns;
		sscanf(line, "%f\t%ld", &temp, &ns);
		// fprintf(stderr, "%f\t%ld\n", temp, ns);
		wait_ns(ns);
		write(STDOUT_FILENO, &temp, sizeof temp);
	}

	fprintf(stderr, "\nSensor terminado.\n");
	free(line);
	fclose(file);
	return 0;
}
