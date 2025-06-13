/*
 * FreeRTOS V202111.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

 #ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* ========= CONFIGURAÇÕES DO ESCALONADOR ========= */
#define configUSE_PREEMPTION            1   // Usa escalonamento preemptivo
#define configUSE_TICKLESS_IDLE         0   // Não usa modo de baixo consumo
#define configTICK_RATE_HZ              ( ( TickType_t ) 1000 )  // Tick a 1kHz (1ms)
#define configMAX_PRIORITIES            32  // Máximo de prioridades
#define configMINIMAL_STACK_SIZE        ( configSTACK_DEPTH_TYPE ) 256  // Tamanho mínimo da pilha

/* ========= SINCRONIZAÇÃO ========= */
#define configUSE_MUTEXES               1   // Habilita mutexes
#define configUSE_COUNTING_SEMAPHORES   1   // Habilita semáforos contadores
#define configQUEUE_REGISTRY_SIZE       8   // Número máximo de filas registradas

/* ========= GERENCIAMENTO DE MEMÓRIA ========= */
#define configSUPPORT_DYNAMIC_ALLOCATION 1  // Alocação dinâmica de memória
#define configTOTAL_HEAP_SIZE           (128*1024)  // 128KB de heap

/* ========= TIMERS ========= */
#define configUSE_TIMERS                1   // Habilita timers de software
#define configTIMER_TASK_PRIORITY       ( configMAX_PRIORITIES - 1 )  // Prioridade alta
#define configTIMER_TASK_STACK_DEPTH    1024  // Pilha para a tarefa de timers

/* ========= ESPECÍFICO PARA RP2040 ========= */
#define configSUPPORT_PICO_SYNC_INTEROP 1   // Compatibilidade com SDK do Pico
#define configSUPPORT_PICO_TIME_INTEROP 1

/* ========= FUNÇÕES DE DEPURAÇÃO ========= */
#include <assert.h>
#define configASSERT(x) assert(x)  // Para detecção de erros

/* ========= INCLUSÃO DE FUNÇÕES DA API ========= */
// Habilita várias funções úteis do FreeRTOS
#define INCLUDE_vTaskSuspend            1
#define INCLUDE_vTaskDelay              1
#define INCLUDE_eTaskGetState           1
// ... (outras definições de inclusão)

#endif /* FREERTOS_CONFIG_H */