#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#define MAX_TASKS	3

static volatile char input = 0;

SemaphoreHandle_t semaphore[MAX_TASKS] = {
	xSemaphoreCreateBinary(),
	xSemaphoreCreateBinary(),
	xSemaphoreCreateBinary()
};
SemaphoreHandle_t ready = xSemaphoreCreateCounting(1, 0);

uint8_t indexArr[MAX_TASKS] = {0, 1, 2};

static void task(void *args) {
	char *title = pcTaskGetName(NULL);
	const uint8_t index = *(static_cast<const uint8_t*>(args));
	title[5] = index + '1';

	while(1) {
		xSemaphoreTake(semaphore[index], portMAX_DELAY);
		Serial.println(title);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		xSemaphoreGive(ready);
	}
}

static void controllerTask(void *args) {
	if (input == 'a') {
		while(1) {
			xSemaphoreGive(semaphore[0]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[2]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[1]);
			xSemaphoreTake(ready, portMAX_DELAY);
		}
	} else if(input == 'b') {
		while(1) {
			xSemaphoreGive(semaphore[1]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[1]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[2]);
			xSemaphoreTake(ready, portMAX_DELAY);
			xSemaphoreGive(semaphore[0]);
			xSemaphoreTake(ready, portMAX_DELAY);
		}
	} else if(input == 'c') {
		while(1) {
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
		}
	}
}

void setup() {
	Serial.begin(9600);
	Serial.println("Seleccione el ejercicio que quiere ejecutar. Puede ser a, b o c");

	while(!input) {
		while(!Serial.available());
		input = Serial.read();
		Serial.println();
		if (input >= 'a' && input <= 'c') {
			Serial.print("Se ha seleccionado el ejercicio ");
			Serial.println(input);
			Serial.println();
		} else if (input != 0) {
			Serial.println("Seleccione un ejercicio valido");
			input = 0;
		}
	}

	xTaskCreate(controllerTask, "Controller task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	for (int i = 0; i < MAX_TASKS; i++) {
		xTaskCreate(task, "Task  ", configMINIMAL_STACK_SIZE, &indexArr[i], 1, NULL);
	}

	vTaskStartScheduler();
}

void loop() {
}