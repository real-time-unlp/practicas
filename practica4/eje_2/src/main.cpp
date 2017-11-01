#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>

#define MAX_TASKS	3

static UBaseType_t priority[MAX_TASKS] = {};
static uint8_t index[MAX_TASKS] = {0, 1, 2};
static SemaphoreHandle_t mutex = xSemaphoreCreateCounting(1, 1);

static void task(void *args) {
	char *title = pcTaskGetName(NULL);
	const uint8_t index = *static_cast<uint8_t*>(args);
	title[5] = index + '1';

	while(1) {
		xSemaphoreTake(mutex, portMAX_DELAY);
		Serial.println(title);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		xSemaphoreGive(mutex);
		taskYIELD();
	}
}

void setup() {
	Serial.begin(9600);
	for(int i = 0; i < MAX_TASKS; i++) {
		char prior = 0;
		Serial.print("Prioridad TASK ");
		Serial.print(i + 1);
		Serial.print(" = ");
		while(!prior) {
			while(!Serial.available());
			prior = Serial.read();
			if (prior > '0' && prior < configMAX_PRIORITIES + '0') {
				priority[i] = prior - '0';
				Serial.println(prior);
			}
			else {
				prior = 0;
			}
		}
	}

	for (int i = 0; i < MAX_TASKS; i++) {
		xTaskCreate(task, "Task  ", configMINIMAL_STACK_SIZE, &index[i], priority[i], NULL);
	}
	vTaskStartScheduler();
}

void loop() {
}