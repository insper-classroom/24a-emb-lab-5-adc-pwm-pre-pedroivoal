#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "data.h"

#define FILTER_SIZE 5

QueueHandle_t xQueueData;

// não mexer! Alimenta a fila com os dados do sinal
void data_task(void *p) {
    vTaskDelay(pdMS_TO_TICKS(400));

    int data_len = sizeof(sine_wave_four_cycles) / sizeof(sine_wave_four_cycles[0]);
    for (int i = 0; i < data_len; i++) {
        xQueueSend(xQueueData, &sine_wave_four_cycles[i], 1000000);
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void process_task(void *p) {
    int data = 0;
    int filter[FILTER_SIZE] = {0};  // Inicializa o filtro com zeros
    int filter_pos = 0;              // Variável para armazenar a soma dos valores no filtro
    int sum;

    while (true) {
        if (xQueueReceive(xQueueData, &data, 100)) {
            
            filter[filter_pos] = data;
            filter_pos = (filter_pos+1) % FILTER_SIZE;

            sum = 0;
            for (int i = 0; i < FILTER_SIZE; i++) {
                sum += filter[i];
            }

            printf("%d\n", sum/FILTER_SIZE);
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

int main() {
    stdio_init_all();

    xQueueData = xQueueCreate(64, sizeof(int));

    xTaskCreate(data_task, "Data task ", 4096, NULL, 1, NULL);
    xTaskCreate(process_task, "Process task", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}
