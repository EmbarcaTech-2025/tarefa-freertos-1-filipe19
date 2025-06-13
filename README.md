
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
---

## 📜 Licença
GNU GPL-3.0.
