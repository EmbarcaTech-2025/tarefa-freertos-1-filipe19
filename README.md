# Tarefa: Roteiro de FreeRTOS #1 - EmbarcaTech 2025

Autor: **Filipe Alves de Sousa**
Curso: Residência Tecnológica em Sistemas Embarcados
Instituição: EmbarcaTech - HBr
Brasília-DF, junho de 2025

---
Sistema Multitarefa com FreeRTOS na BitDogLab

📝 Descrição
Este projeto implementa um sistema multitarefa usando FreeRTOS no Raspberry Pi Pico (BitDogLab) para controlar:

Um LED RGB que alterna entre vermelho, verde e azul

Um buzzer que emite bips periódicos

Dois botões que suspendem/retomam as tarefas do LED e buzzer

🛠️ Pré-requisitos
VSCode com extensão C/C++

Toolchain ARM GCC

CMake (versão 3.13 ou superior)

Raspberry Pi Pico SDK

🚀 Como Compilar e Gravar
Clone este repositório:

´´´bash
git clone [URL_DO_SEU_REPOSITORIO]
cd [NOME_DO_REPOSITORIO]
Configure o ambiente de build:
´´´

´´´bash
mkdir build
cd build
cmake ..
Compile o projeto:
´´´

´´´bash
make
´´´

Grave o firmware na BitDogLab:
Pressione o botão BOOTSEL na placa e conecte via USB
Copie o arquivo Tarefa_FreeRTOS.uf2 para a unidade RPI-RP2 que aparecerá

🎯 Funcionamento
O LED RGB alterna entre vermelho, verde e azul a cada 500ms
O buzzer emite um bip curto a cada 1 segundo
Botão A: suspende/retoma o LED RGB
Botão B: suspende/retoma o buzzer

📂 Estrutura de Arquivos
text
├── CMakeLists.txt          # Configuração do projeto
├── FreeRTOS/               # Kernel do FreeRTOS
├── include/
│   └── FreeRTOSConfig.h    # Configurações do FreeRTOS
└── src/
    └── main.c              # Código principal


🔍 Explicação Detalhada
1. Fluxo do Programa
Inicialização: Configura os pinos do LED, buzzer e botões

Criação de Tarefas:
´´´bash
task_led_rgb: Controla o LED RGB em um loop infinito
task_buzzer: Controla o buzzer com bips periódicos
task_buttons: Monitora os botões e controla as outras tarefas
´´´

Escalonamento: O FreeRTOS gerencia a execução concorrente das tarefas

2. Prioridades
Botões: Prioridade 2 (mais alta) - para resposta rápida
LED e Buzzer: Prioridade 1

3. Controle de Tarefas

´´´bash
vTaskSuspend(): Pausa uma tarefa
vTaskResume(): Retoma uma tarefa pausada
eTaskGetState(): Verifica o estado atual de uma tarefa
´´´

4. Temporização
vTaskDelay(): Espera por um tempo especificado (não bloqueante)
O tick do FreeRTOS está configurado para 1ms (1000Hz)

5. Boas Práticas Implementadas
Debounce de botões por software (verificação a cada 100ms)
Desligamento seguro do LED e buzzer quando suspensos
Prioridades adequadas para garantir responsividade
Comentários detalhados para cada seção do código


📝 Reflexão e Entrega
Questões para Reflexão (inclua no seu relatório ou README.md):
1️⃣ O que acontece se todas as tarefas tiverem a mesma prioridade?
Quando todas as tarefas têm a mesma prioridade, o escalonador do FreeRTOS usa o algoritmo Round Robin (time slicing). Isso significa que:
Cada tarefa executa por um "time slice" (fatia de tempo) definido (normalmente 1 tick do sistema, ex: 1ms)
As tarefas são executadas em rodízio, compartilhando igualmente o tempo da CPU
Não há preempção por prioridade, podendo causar atrasos em tarefas críticas
No nosso caso específico, o botão poderia ter resposta mais lenta quando o LED ou buzzer estivessem executando

2️⃣ Qual tarefa consome mais tempo da CPU?
No nosso projeto:

Tarefa do Buzzer é a mais "gulosa" porque:
Executa operações de PWM (hardware_pwm) que consomem ciclos de CPU
Tem um delay curto (100ms entre bips) vs 500ms do LED
Quando ativa, fica ~10% do tempo gerando som (100ms bip a cada 1000ms)
Tarefa dos Botões é a mais eficiente:
Só verifica os GPIOs a cada 100ms
Consome pouquíssimos ciclos de CPU (apenas leitura digital)
Tarefa do LED fica no meio-termo:
Muda estado a cada 500ms
Apenas operações simples de GPIO

3️⃣ Riscos de usar polling sem prioridades:
Usar polling (verificação contínua) sem gerenciamento de prioridades pode causar:

Starvation: Tarefas menos importantes podem bloquear as críticas
Latência Imprevisível: Tempo de resposta varia conforme carga do sistema
Perda de Eventos: Se o polling for lento, pode perder pressionamentos rápidos de botão
Eficiência Energética: CPU fica 100% ocupada mesmo sem necessidade
Exemplo Prático: No nosso caso, sem prioridades:
Um bip longo do buzzer poderia atrasar a resposta do botão
O LED poderia piscar irregularmente sob carga do sistema

📌 Dica para Melhoria:
Para sistemas mais robustos, considere:

c
// Em vez de polling nos botões, usar interrupções:
gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_isr);

---

## 📜 Licença
GNU GPL-3.0.
