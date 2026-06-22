#include "paciente.h"
#include "auxiliar.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_PACIENTES 100

Paciente pacientes[MAX_PACIENTES];
int qtdPacientes = 0;
int ultimaIdPaciente = 0; // Para controle numérico das IDs

static void imprimir_menu_pacientes() {
    printf("------ PACIENTES ------\n"
        "[Pacientes Cadastrados: %d]\n\n"
        "[Digite a opção desejada]\n"
        "1. Cadastrar paciente\n"
        "2. Visualizar paciente\n"
        "3. Modificar paciente\n"
        "4. Remover paciente\n"
        "5. Voltar\n",
    qtdPacientes);
}

// Formata a ID do paciente como String
static void formatar_id_paciente(int rawId, char* stringId, size_t tamString) {
    snprintf(stringId, tamString, "P%03d", rawId);
}

void imprimir_paciente(Paciente paciente) {
    printf("%s:\n", paciente.id);
    printf("  Nome: %s\n", paciente.nome);
    printf("  Idade: %d anos\n", paciente.idade);
    printf("  Gênero: %c\n", paciente.genero);
    printf("  Peso: %.2fkg\n", paciente.peso);
    printf("  Tipo Sanguíneo: %s\n", paciente.tipoSanguineo);
}

Paciente cadastrar_paciente() {
    Paciente paciente;
    int dadosLidos = 0;

    printf("---- NOVO PACIENTE ----\n");

    // Id (definida automaticamente)
    formatar_id_paciente(ultimaIdPaciente + 1, paciente.id, sizeof(paciente.id));

    // Nome
    while (1) {
        printf("Nome: ");
        
        if (fgets(paciente.nome, sizeof(paciente.nome), stdin)) {
            remover_quebra_linha(paciente.nome);
            break; 
        } 
        fprintf(stderr, "[ERRO] Por favor, tente novamente\n");
    } 

    // Idade
    while (1) {
        printf("Idade: ");

        if (scanf(" %d", &paciente.idade) == 1) {
            break;
        }
        limpar_buffer();
        fprintf(stderr, "[ERRO] Por favor, tente novamente\n");
    } 
    limpar_buffer();

    // Genero
    while (1) {
        // Será lido um buffer temporário primeiro
        char generoTemp[10]; 
        printf("Gênero (F/M): ");

        if (fgets(generoTemp, sizeof(generoTemp), stdin)) {
            paciente.genero = toupper(generoTemp[0]);
            remover_quebra_linha(generoTemp);
            break;
        }
        fprintf(stderr, "[ERRO] Por favor, tente novamente\n");
    }

    // Peso
    while (1) {
        printf("Peso (kg): ");
        
        if (scanf(" %f", &paciente.peso) == 1) break;
        fprintf(stderr, "[ERRO] Por favor, tente novamente\n");
    }
    limpar_buffer();

    // Tipo Sanguineo
    while (1) {
        printf("Tipo Sanguineo: ");
        char tipoSanguineoTemp[5];
        
        if (fgets(tipoSanguineoTemp, sizeof(tipoSanguineoTemp), stdin)) {
            remover_quebra_linha(tipoSanguineoTemp);

            // Formatando:
            int tamString = strlen(tipoSanguineoTemp);
            for (int i = 0; i < tamString; i++) {
                char *ptrChar = &tipoSanguineoTemp[i];
                // Deixando tudo maiúsculo
                *ptrChar = toupper(*ptrChar);
            }

            // Verificando se está no padrão ideal:
            // (primeiro deve haver A, B ou O) 
            switch (tipoSanguineoTemp[0]) {
                case 'A': break;
                case 'B': break;
                case 'O': break;
                default: goto erro;
            }

            // (no final deve haver o sinal de + ou -)
            switch (tipoSanguineoTemp[tamString - 1]) {
                case '+': break;
                case '-': break;
                default: goto erro;
            }

            // Se houver uma segunda letra, deve ser B
            if (tamString > 2 && tipoSanguineoTemp[1] != 'B') goto erro;

            // Copiando para o destino correto
            strcpy(paciente.tipoSanguineo, tipoSanguineoTemp);
            break;

            erro:
            fprintf(stderr, "[ERRO: Tipo Sanguíneo Inválido] Por favor, tente novamente\n");
            continue;
        }
        fprintf(stderr, "[ERRO] Por favor, tente novamente\n");
    }

    // Imprimindo dados lidos
    char resposta;
    printf("\n");
    imprimir_paciente(paciente);
    printf("Os dados conferem? (s/n) \n");
    imprimir_cursor();
    scanf(" %c", &resposta);
    limpar_buffer();
    if (toupper(resposta) == 'N') return cadastrar_paciente();

    (qtdPacientes)++;
    (ultimaIdPaciente)++;
    return paciente;
}

void modulo_pacientes() {
    int opcao = 0;

    // Iniciando interação com usuário
    do {
        imprimir_menu_pacientes();
        imprimir_cursor();
        opcao = obter_opcao();

        switch (opcao) {
        case 1:
            // Cadastrar
            pacientes[qtdPacientes] = cadastrar_paciente();
            break;

        case 2:
            // Visualizar
            printf("[Opção selecionada: %d]\n\n", opcao);
            break;

        case 3:
            // Modificar
            printf("[Opção selecionada: %d]\n\n", opcao);
            break;

        case 4:
            // Remover
            printf("[Opção selecionada: %d]\n\n", opcao);
            break;

        case 5:
            // Voltar
            printf("\n");
            break;

        default:
            // Caso de erro (opção inválida)
            printf("[Opção inválida]\nTente novamente...\n\n");
            break;
        }
    } while (opcao != 5);
    // Se a opção 5 for selecionada quebramos o laço
}
