#include "paciente.h"
#include "auxiliar.h"
#include <stdio.h>
#include <string.h>

#define MAX_PACIENTES 100

Paciente pacientes[MAX_PACIENTES];
int qtdPacientes = 0;
int ultimaIdPaciente = 0;

static void imprimir_menu_pacientes() {
    printf("------ PACIENTES ------\n"
           "Digite a opção desejada:\n"
           "1. Cadastrar paciente\n"
           "2. Visualizar paciente\n"
           "3. Modificar paciente\n"
           "4. Remover paciente\n"
           "5. Voltar\n");
}

static void formatar_id_paciente(int rawId, char* stringId, size_t tamString) {
    snprintf(stringId, tamString, "P%03d", rawId);
}

Paciente cadastrar_paciente(int *qtdPacientes, int *ultimaIdPaciente) {
    Paciente paciente;

    printf("---- NOVO PACIENTE ----\n");

    // id
    formatar_id_paciente(*ultimaIdPaciente + 1, paciente.id, sizeof(paciente.id));

    // nome
    printf("Nome: ");
    fgets(paciente.nome, sizeof(paciente.nome), stdin);

    // idade
    printf("Idade: ");
    scanf(" %d", &paciente.idade);

    // genero
    printf("Gênero (F/M): ");
    scanf(" %c", &paciente.genero);

    // peso
    printf("Peso (kg): ");
    scanf(" %f", &paciente.peso);

    // tipo sanguineo
    printf("Tipo Sanguineo: ");
    scanf(" %s", paciente.tipoSanguineo);

    (*qtdPacientes)++;
    (*ultimaIdPaciente)++;
    return paciente;
}

void imprimir_paciente(Paciente paciente) {
    printf("Paciente %s:\n", paciente.id);
    printf("\tNome: %s", paciente.nome);
    printf("\tIdade: %d anos\n", paciente.idade);
    printf("\tGênero: %c\n", paciente.genero);
    printf("\tPeso: %.2fkg\n", paciente.peso);
    printf("\tTipo Sanguíneo: %s\n", paciente.tipoSanguineo);
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
            pacientes[qtdPacientes] = cadastrar_paciente(&qtdPacientes, &ultimaIdPaciente);
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
