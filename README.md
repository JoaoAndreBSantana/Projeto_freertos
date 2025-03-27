# Projeto_freertos
O que faz:
Controla um LED usando um botão com 3 tarefas no FreeRTOS.

Tarefas:

verifica_botao - Lê o botão (100ms)

Process - Avisa quando apertar (prioridade média)

Led - Controla o LED (prioridade alta)

Como funciona:

Apertou o botão → LED acende

Soltou o botão → LED apaga
 
