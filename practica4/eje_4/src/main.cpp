#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#define MAX_TASKS	3
#define EXERCISE	a

SemaphoreHandle_t semaphore[MAX_TASKS] = {
	xSemaphoreCreateBinary(),
	xSemaphoreCreateBinary(),
	xSemaphoreCreateBinary()
};
SemaphoreHandle_t ready = xSemaphoreCreateCounting(1, 1);

uint8_t indexArr[MAX_TASKS] = {0, 1, 2};

static void task(void *args) {
	const char *title = pcTaskGetName(NULL);
	const uint8_t index = *(static_cast<const uint8_t*>(args));

	while(1) {
		xSemaphoreTake(semaphore[index], portMAX_DELAY);
		Serial.println(title);
		xSemaphoreGive(ready);
	}
}

static void controllerTask(void *args) {
	while(1) {
		#if EXERCISE == a
			xSemaphoreGive(semaphore[0]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[2]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[1]);
			xSemaphoreTake(ready, portMAX_DELAY);
		#elif EXERCISE == b
			xSemaphoreGive(semaphore[1]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[1]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[2]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[0]);
			xSemaphoreTake(ready, portMAX_DELAY);
		#elif EXERCISE == c
			xSemaphoreGive(semaphore[2]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[2]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[2]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[0]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[1]);
			xSemaphoreTake(ready, portMAX_DELAY);
		#endif
	}
}

void setup() {
	Serial.begin(9600);

	xTaskCreate(controllerTask, "Controller task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	for (int i = 0; i < MAX_TASKS; i++) {
		xTaskCreate(task, "Task " + i, configMINIMAL_STACK_SIZE, &indexArr[i], 1, NULL);
	}

	vTaskStartScheduler();
}

void loop() {
}