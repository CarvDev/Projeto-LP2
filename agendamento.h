#ifndef AGENDAMENTO_H
#define AGENDAMENTO_H

#include <time.h>

typedef struct node {
    char id[5];
    char idMedico[5];
    char idPaciente[5];
    time_t timestamp;
    struct node *prox;
    struct node *anterior;
} Agendamento;

typedef struct {
    int minuto;
    int hora;
    int dia;
    int mes;
    int ano;
} Horario;

void modulo_agendamentos();

// Exportando as variáveis globais
extern int ultimaIdAgendamento;
extern int qtdAgendamentos;
extern const char caminhoArqAgendamentos[];
extern Agendamento *listaAgendamentos;
#endif