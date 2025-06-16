
---

````markdown
# 🎯 Tarefa: Roteiro de FreeRTOS #1 - EmbarcaTech 2025

**Autor:** Filipe Alves de Sousa  
**Curso:** Residência Tecnológica em Sistemas Embarcados  
**Instituição:** EmbarcaTech - HBr  
**Local e Data:** Brasília-DF, junho de 2025

---

## 🧠 Projeto: Sistema Multitarefa com FreeRTOS na BitDogLab

Este projeto implementa um sistema multitarefa utilizando FreeRTOS na plataforma Raspberry Pi Pico (BitDogLab), com os seguintes recursos:

- 🔴 Um **LED RGB** que alterna entre vermelho, verde e azul.
- 🔊 Um **buzzer** que emite bips periódicos.
- 🎮 Dois **botões** que suspendem/retomam as tarefas do LED e do buzzer.

---
## 🌟 Visão Geral do Projeto

### 🎥 Demonstração
[![vídeo de demonstração do projeto funcionando](https://img.youtube.com/vi/n9v3qVqzuhI/0.jpg)](https://www.youtube.com/shorts/n9v3qVqzuhI)

---
## ⚙️ Pré-requisitos

- VS Code com extensão C/C++
- Toolchain ARM GCC
- CMake (versão 3.13 ou superior)
- Raspberry Pi Pico SDK

---

## 🚀 Como Compilar e Gravar

Clone este repositório:

```bash
git clone [URL_DO_SEU_REPOSITORIO]
cd [NOME_DO_REPOSITORIO]
````

Configure o ambiente de build:

```bash
mkdir build
cd build
cmake ..
```

Compile o projeto:

```bash
make
```

Grave o firmware na BitDogLab:

1. Pressione o botão **BOOTSEL** da placa.
2. Conecte via USB ao computador.
3. Copie o arquivo `Tarefa_FreeRTOS.uf2` para a unidade `RPI-RP2` que aparecerá.

---

## ⚡ Funcionamento do Sistema

| Componente | Comportamento                                      |
| ---------- | -------------------------------------------------- |
| LED RGB    | Alterna entre vermelho, verde e azul a cada 500 ms |
| Buzzer     | Emite um bip curto a cada 1 segundo                |
| Botão A    | Suspende/retoma o LED RGB                          |
| Botão B    | Suspende/retoma o buzzer                           |

---

## 📁 Estrutura de Arquivos

```
project-root/
├── CMakeLists.txt           # Configuração do projeto
├── FreeRTOS/                # Kernel do FreeRTOS
├── include/
│   └── FreeRTOSConfig.h     # Configurações do FreeRTOS
└── src/
    └── main.c               # Código principal
```

---

## 🧩 Explicação Detalhada

### 🔄 Fluxo do Programa

* **Inicialização:** Configura pinos de LED, buzzer e botões.
* **Criação de Tarefas:**

  * `task_led_rgb`: Controla o LED RGB.
  * `task_buzzer`: Controla o buzzer.
  * `task_buttons`: Monitora os botões e controla as outras tarefas.
* **Escalonamento:** O FreeRTOS gerencia a execução concorrente.

### 📊 Prioridades

* `task_buttons`: **Prioridade 2** (alta) – resposta rápida aos botões.
* `task_led_rgb` e `task_buzzer`: **Prioridade 1**.

### 🧵 Controle de Tarefas

```c
vTaskSuspend();     // Pausa uma tarefa
vTaskResume();      // Retoma uma tarefa pausada
eTaskGetState();    // Verifica o estado da tarefa
```

### ⏲️ Temporização

* `vTaskDelay()` para delays não bloqueantes.
* Tick configurado para **1 ms** (1000 Hz).

### ✅ Boas Práticas Implementadas

* Debounce de botões por software (verificação a cada 100 ms).
* Desligamento seguro de LED e buzzer ao suspender.
* Prioridades ajustadas para melhor resposta.
* Comentários claros e detalhados no código.

---

## 📘 Questões para Reflexão

1️⃣ **O que acontece se todas as tarefas tiverem a mesma prioridade?**

* O FreeRTOS usa **Round Robin** (rodízio por time slice).
* As tarefas compartilham igualmente o tempo da CPU.
* Tarefas críticas podem sofrer atraso.
* Exemplo: botão pode responder mais lentamente se LED e buzzer estiverem ativos.

2️⃣ **Qual tarefa consome mais tempo da CPU?**

* **Buzzer** é a mais "pesada":

  * Usa PWM e opera a cada 1 segundo (100 ms de bip).
  * Delay menor = mais ativa.
* **Botões** são mais leves:

  * Apenas leem GPIO a cada 100 ms.
* **LED** tem consumo intermediário:

  * Altera cores a cada 500 ms.

3️⃣ **Riscos de usar polling sem prioridades:**

* Pode ocorrer:

  * Starvation (tarefas importantes ignoradas).
  * Latência imprevisível.
  * Perda de eventos (botões).
  * Alto consumo energético.

* No projeto:

  * Um bip longo pode atrasar botão.
  * LED pode piscar fora de tempo.

---

## 🛠️ Dica para Melhoria

Para sistemas mais robustos, substitua **polling** por **interrupções**:

```c
gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_isr);
```

---


## 📜 Licença
GNU GPL-3.0.
