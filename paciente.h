typedef struct {
    char id[5];
    char nome[50];
    int idade;
    char genero;
    float peso;
    char tipoSanguineo[5];
} Paciente;

/* Solicita ao usuário informações para o cadastro de um novo paciente,
 * calcula a ID do novo usuário, incrementa os registros de quantidade e
 * de última ID e retorna esse novo paciente
 */
Paciente cadastrar_paciente(int *qtdPacientes, int *ultimaIdPaciente);

// Imprime os dados do paciente passado como argumento
void imprimir_paciente(Paciente paciente);

// Agrupa a execução das principais funções de clientes
void modulo_pacientes();
