#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

/*
 * DEFINIÇÕES DOS PINOS:
 * - LED RGB usa 3 pinos (vermelho, verde, azul)
 * - Buzzer usa PWM para controle de tom
 * - Botões com pull-up interno
 */
#define RED_PIN     13   // LED Vermelho
#define GREEN_PIN   11   // LED Verde
#define BLUE_PIN    12   // LED Azul
#define BUZZER_PIN  21   // Buzzer (PWM)
#define BUTTON_A    5    // Botão A (controla LED)
#define BUTTON_B    6    // Botão B (controla buzzer)

// Handles para controle das tarefas
TaskHandle_t ledTaskHandle = NULL;      // Handle da tarefa do LED
TaskHandle_t buzzerTaskHandle = NULL;   // Handle da tarefa do buzzer

/* ======== FUNÇÕES DO LED RGB ======== */

/**
 * Define a cor do LED RGB
 * @param R Vermelho (1 = ligado, 0 = desligado)
 * @param G Verde
 * @param B Azul
 */
void set_led_color(bool R, bool G, bool B) {
    gpio_put(RED_PIN, R);
    gpio_put(GREEN_PIN, G);
    gpio_put(BLUE_PIN, B);
}

/**
 * Tarefa do LED RGB - Alterna entre cores a cada 500ms
 * @param pvParameters Parâmetros da tarefa (não usado)
 */
void task_led_rgb(void *pvParameters) {
    int cor = 0;  // 0 = Vermelho, 1 = Azul, 2 = Verde

    while (1) {
        switch (cor) {
            case 0:
                set_led_color(1, 0, 0);  // Vermelho
                break;
            case 1:
                set_led_color(0, 0, 1);  // Azul
                break;
            case 2:
                set_led_color(0, 1, 0);  // Verde
                break;
        }

        cor = (cor + 1) % 3;  // Cicla entre 0, 1, 2
        vTaskDelay(pdMS_TO_TICKS(500));  // Espera 500ms
    }
}

/* ======== FUNÇÕES DO BUZZER ======== */

/**
 * Inicializa o PWM para o buzzer
 */
void pwm_init_buzzer() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_config config = pwm_get_default_config();
    
    // Configura divisão de clock para gerar tom audível
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (100 * 4096));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(BUZZER_PIN, 0);  // Inicia desligado
}

/**
 * Emite um bip com duração específica
 * @param duration_ms Duração do bip em milissegundos
 */
void beep(uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_gpio_level(BUZZER_PIN, 2048);  // 50% duty cycle (tom constante)
    vTaskDelay(pdMS_TO_TICKS(duration_ms));
    pwm_set_gpio_level(BUZZER_PIN, 0);     // Silencia
    vTaskDelay(pdMS_TO_TICKS(100));        // Pequena pausa entre bips
}

/**
 * Tarefa do buzzer - Emite bips periódicos
 * @param pvParameters Parâmetros da tarefa (não usado)
 */
void task_buzzer(void *pvParameters) {
    while (1) {
        beep(100);  // Bip de 100ms
        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo
    }
}

/* ======== FUNÇÕES DOS BOTÕES ======== */

/**
 * Tarefa dos botões - Monitora e controla outras tarefas
 * @param pvParameters Parâmetros da tarefa (não usado)
 */
void task_buttons(void *pvParameters) {
    bool lastA = true, lastB = true;  // Estado anterior dos botões

    while (1) {
        bool currentA = gpio_get(BUTTON_A);
        bool currentB = gpio_get(BUTTON_B);

        // Verifica borda de descida (botão pressionado)
        if (currentA == false && lastA == true) {
            // Alterna estado da tarefa do LED
            if (eTaskGetState(ledTaskHandle) == eSuspended) {
                vTaskResume(ledTaskHandle);
            } else {
                vTaskSuspend(ledTaskHandle);
                set_led_color(0, 0, 0);  // Desliga LED quando suspenso
            }
        }

        if (currentB == false && lastB == true) {
            // Alterna estado da tarefa do buzzer
            if (eTaskGetState(buzzerTaskHandle) == eSuspended) {
                vTaskResume(buzzerTaskHandle);
            } else {
                vTaskSuspend(buzzerTaskHandle);
                pwm_set_gpio_level(BUZZER_PIN, 0);  // Silencia buzzer
            }
        }

        lastA = currentA;
        lastB = currentB;

        vTaskDelay(pdMS_TO_TICKS(100));  // Verifica botões a cada 100ms
    }
}

/* ======== FUNÇÃO PRINCIPAL ======== */

int main() {
    stdio_init_all();  // Inicializa comunicação serial (não usada aqui)

    // Configura GPIOs do LED RGB
    gpio_init(RED_PIN); gpio_set_dir(RED_PIN, GPIO_OUT);
    gpio_init(GREEN_PIN); gpio_set_dir(GREEN_PIN, GPIO_OUT);
    gpio_init(BLUE_PIN); gpio_set_dir(BLUE_PIN, GPIO_OUT);

    // Configura botões com pull-up interno
    gpio_init(BUTTON_A); 
    gpio_set_dir(BUTTON_A, GPIO_IN); 
    gpio_pull_up(BUTTON_A);
    
    gpio_init(BUTTON_B); 
    gpio_set_dir(BUTTON_B, GPIO_IN); 
    gpio_pull_up(BUTTON_B);

    pwm_init_buzzer();  // Inicializa buzzer

    // Cria as tarefas do FreeRTOS
    xTaskCreate(task_led_rgb,   "LED Task",    256, NULL, 1, &ledTaskHandle);
    xTaskCreate(task_buzzer,    "Buzzer Task", 256, NULL, 1, &buzzerTaskHandle);
    xTaskCreate(task_buttons,   "Button Task", 256, NULL, 2, NULL);  // Prioridade mais alta

    vTaskStartScheduler();  // Inicia o escalonador do FreeRTOS

    while (true);  // Nunca deverá chegar aqui
}