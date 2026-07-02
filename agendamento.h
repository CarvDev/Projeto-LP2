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

// Estrutura para gravação no arquivo: contém apenas os dados reais, eliminando os ponteiros da
// lista encadeada. Isso evita gravar lixo de memória no disco.
typedef struct {
    char id[5];
    char idMedico[5];
    char idPaciente[5];
    time_t timestamp;
} RegistroAgendamento;

// Agrupa a execução das principais funções de agendamentos
void modulo_agendamentos();

/*
 * Serializa a lista encadeada e o estado dos contadores globais para o disco.
 * Oculta os ponteiros (*prox, *anterior) usando uma struct espelho internamente, 
 * gravando apenas as chaves e o timestamp no binário para otimizar espaço
 * e impedir a gravação de lixo de memória irrecuperável.
 */
void gravar_arquivo_agendamentos(void);

/*
 * Lê o cabeçalho do arquivo binário para restaurar os contadores (qtd e última ID)
 * e reconstrói a lista duplamente encadeada na memória heap.
 * Diferentemente dos vetores, a estrutura de lista encadeada nos obriga
 * a ler de um por um.
 */
void ler_arquivo_agendamentos(void);

// Exportando as variáveis globais
extern int ultimaIdAgendamento;
extern int qtdAgendamentos;
extern const char caminhoArqAgendamentos[];
extern Agendamento *listaAgendamentos;
#endif