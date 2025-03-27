#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

const uint32_t LED_PIN = 12;//pino do led
#define BUTTON_PIN 5//pino do botao
volatile int botao_est = 0;// variavel do estado do botao


// Fubção para ler o estado do bootao
void verifica_botao(void *pvParameters) {
for(;;){
        if (!gpio_get(BUTTON_PIN)) {
            botao_est = 1; //botao pressionado
            printf("Botao foi pressionado\n"); 
        }else {
            botao_est = 0; //botao no pressionado
            printf("Botao nao foi pressionado\n"); 
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // debounce 
    }
}

// Tarefa para controlar o LED com base nas notificações
void Led(void *pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Esperar pela notificação
        printf("LED LIGADO\n");
        gpio_put(LED_PIN, 1); // Ligar o LED
        
        // Mantém o LED aceso enquanto o botão estiver pressionado
        while (botao_est == 1) {
            vTaskDelay(pdMS_TO_TICKS(10)); // Checa rapidamente
        }
        
        printf("LED DESLIGADO\n");
        gpio_put(LED_PIN, 0); // Desligar o LED
    }
}

// Tarefa para processar o estado do botão e notificar a tarefa do LED
void Process(void *pvParameters) {
    TaskHandle_t *ledTaskHandle = (TaskHandle_t)pvParameters;
    for (;;) {
        if (botao_est == 1) {
            printf("Botao pressionado!!! notificando tarefa do LED\n"); // Imprimir evento de notificação
            xTaskNotifyGive(ledTaskHandle); // Notificar a tarefa do LED para ligar o LED
        }    vTaskDelay(pdMS_TO_TICKS(100)); // Verificar a cada 100ms
    }
}
int main() {
    TaskHandle_t ledTaskHandle = NULL;

    // inicializa entrade e saida
    stdio_init_all();
    gpio_init(LED_PIN);//inicializa o pino do led
    gpio_set_dir(LED_PIN, GPIO_OUT);//coloca como saida

    //inicializa o pino do botao
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    // tarefas
    xTaskCreate(Led, "Tarefa do LED", 128, NULL, 3, &ledTaskHandle);        
    xTaskCreate(Process, "Tarefa de Processamento", 128, (void*)&ledTaskHandle, 2, NULL);  
    xTaskCreate(verifica_botao, "Tarefa do Botao", 128, NULL, 1, NULL);     

    // Iniciar o escalonador do FreeRTOS
    vTaskStartScheduler();

  
    return 0;
}
