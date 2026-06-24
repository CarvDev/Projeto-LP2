#ifndef PACIENTE_H
#define PACIENTE_H

typedef struct {
    char id[5];
    char nome[50];
    int idade;
    char genero;
    float peso;
    char tipoSanguineo[5];
} Paciente;

// Exportando as variáveis globais
extern Paciente pacientes[];
extern int qtdPacientes;
extern int ultimaIdPaciente;
extern const char caminhoArqPacientes[];

/* Solicita ao usuário informações para o cadastro de um novo paciente,
 * calcula a ID do novo usuário, incrementa os registros de quantidade e
 * de última ID e retorna esse novo paciente
*/
Paciente cadastrar_paciente();

// Imprime os dados do paciente passado como argumento
void imprimir_paciente(Paciente paciente);

/* Retorna o índice do paciente correspondente a ID informada.
 * obs: utiliza busca binária
*/ 
int pesquisar_paciente(int inicio, int fim, char idDesejada[]);

// Agrupa a execução das principais funções de clientes
void modulo_pacientes();

#endif