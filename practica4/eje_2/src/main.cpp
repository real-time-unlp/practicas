#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>

#define MAX_TASKS	3

UBaseType_t priority[MAX_TASKS] = {1, 1, 1};

static void task(void *args) {
	const char *title = pcTaskGetName(NULL);

	while(1) {
		Serial.println(title);
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void setup() {
	Serial.begin(9600);

	for (int i = 0; i < MAX_TASKS; i++) {
		xTaskCreate(task, "Task " + i, configMINIMAL_STACK_SIZE, NULL, priority[i], NULL);
	}
	vTaskStartScheduler();
}

void loop() {
}