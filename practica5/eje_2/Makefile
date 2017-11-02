all: sensor monitor

sensor: sensor.c
	cc -std=c99 -D_POSIX_C_SOURCE=200809 -Wall sensor.c -g -o sensor
monitor: monitor.c
	cc -std=c99 -D_POSIX_C_SOURCE=200809 -Wall -lrt -pthread monitor.c -g -o monitor

clean:
	rm -rf *.o sensor monitor