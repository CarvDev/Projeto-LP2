#ifndef MEDICO_H
#define MEDICO_H

typedef struct {
    char id[5];
    char nome[50];
    char crm[20];
    char especializacao[50];
} Medico;

// Usando 'extern' para que a main consiga enxergar a nossa alocação dinâmica
extern Medico *medicos; 
extern int qtdMedicos;
extern int capacidadeMedicos;
extern int ultimaIdMedico;
extern const char caminhoArqMedicos[];

/* Solicita ao usuário informações para o cadastro de um novo médico,
 * calcula a ID do novo usuário, incrementa os registros de quantidade e
 * de última ID e retorna esse novo médico
*/
Medico cadastrar_medico();

// Imprime os dados do médico passado como argumento
void imprimir_medico(Medico medico);

/* Retorna o índice do médico correspondente a ID informada.
 * obs: utiliza busca binária
*/ 
int pesquisar_medico(int inicio, int fim, char idDesejada[]);

// Agrupa a execução das principais funções de médicos
void modulo_medicos();

#endif