#include "paciente.h"
#include "auxiliar.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PACIENTES 100

Paciente pacientes[MAX_PACIENTES];
int qtdPacientes = 0;
int ultimaIdPaciente = 0; // Para controle numérico das IDs
const char caminhoArqPacientes[] = "dados/pacientes.bin";

static void imprimir_menu_pacientes() {
    printf("------ PACIENTES ------\n"
        "[Pacientes Cadastrados: %d]\n\n"

        "[Digite a opção desejada]\n"
        "1. Cadastrar paciente\n"
        "2. Visualizar todos os pacientes\n"
        "3. Pesquisar paciente\n"
        "4. Modificar paciente\n"
        "5. Remover paciente\n"
        "6. Voltar\n",
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

        if (scanf(" %d", &paciente.idade) == 1) break;
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
            remover_quebra_linha(generoTemp);
            paciente.genero = toupper(generoTemp[0]);

            if (paciente.genero != 'M' && paciente.genero != 'F') {
                fprintf(stderr, "[ERRO] Gênero Inválido. Por favor, tente novamente\n");
                continue;
            }
            break;
        }
        fprintf(stderr, "[ERRO] Por favor, tente novamente\n");
    }

    // Peso
    while (1) {
        printf("Peso (kg): ");
        
        if (scanf(" %f", &paciente.peso) == 1) break;
        limpar_buffer();
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
            fprintf(stderr, "[ERRO] Tipo Sanguíneo Inválido. Por favor, tente novamente\n");
            continue;
        }
        fprintf(stderr, "[ERRO] Por favor, tente novamente\n");
    }

    // Imprimindo dados lidos
    printf("\n");
    imprimir_paciente(paciente);
    printf("[Os dados conferem? (S/n)]\n");
    imprimir_cursor();
    char resposta = obter_opcao();
    if (toupper(resposta) == 'N') return cadastrar_paciente();

    (qtdPacientes)++;
    (ultimaIdPaciente)++;
    return paciente;
}

int pesquisar_paciente(int inicio, int fim, char idDesejada[]) {

    if (inicio <= fim) {
        int meio = inicio + (fim - inicio) /2;
        int resultadoComparacao = strcmp(pacientes[meio].id, idDesejada);

        // ID igual a do paciente do meio
        if (resultadoComparacao == 0) {
            return meio;
        }

        // ID procurada é menor que a do meio (busca na metade esquerda)
        if (resultadoComparacao > 0) {
            return pesquisar_paciente(inicio, meio - 1, idDesejada);
        }

        // ID procurada é maior que a do meio (busca na metade direita)
        return pesquisar_paciente(meio + 1, fim, idDesejada);
    }

    // Caso base: Elemento não encontrado (inicio > fim)
    return -1;
}

void modulo_pacientes() {
    char opcao = 0;
    char idPesquisa[6];
    int rawId;
    int indicePaciente;

    // Iniciando interação com usuário
    do {
        printf("\n");
        imprimir_menu_pacientes();
        imprimir_cursor();
        opcao = obter_opcao();

        switch (opcao) {
        case '1':
            // Cadastrar
            pacientes[qtdPacientes] = cadastrar_paciente();
            break;

        case '2':
            // Visualizar todos
            printf("\n-- INÍCIO DA LISTA --\n");
            for (int i = 0; i < qtdPacientes; i++) {
                imprimir_paciente(pacientes[i]);
            }
            printf("-- FIM DA LISTA --\n");
            break;

        case '3':
            // Pesquisar
            printf("\n[Informe a ID do paciente a ser pesquisado]\n");
            imprimir_cursor();
            if (!fgets(idPesquisa, sizeof(idPesquisa), stdin)) {
                fprintf(stderr, "[ERRO] Não foi possível ler a ID informada\n");
                break;
            }
            remover_quebra_linha(idPesquisa);

            // Tratamento inteligente da ID informada:
            // ignora o primeiro digito (P) e pega apenas o valor 
            // numérico, depois volta ao formato correto de string
            rawId = atoi(idPesquisa + (isalpha(idPesquisa[0]) ? 1 : 0));
            formatar_id_paciente(rawId, idPesquisa, sizeof(idPesquisa));

            // Faz a pesquisa com a ID informada
            indicePaciente = pesquisar_paciente(0, qtdPacientes - 1, idPesquisa);

            // Caso de erro
            if (indicePaciente < 0) {
                fprintf(stderr, "[AVISO] Paciente não encontrado\n");
                break; 
            }
            // Caso de sucesso
            printf("\n -- PACIENTE ENCONTRADO --\n");
            imprimir_paciente(pacientes[indicePaciente]);
            break;

        case '4':
            // Modificar
            printf("[Opção selecionada: %c]\n", opcao);
            break;

        case '5':
            // Remover
            printf("\n[Informe a ID do paciente a ser removido]\n");
            imprimir_cursor();
            if (!fgets(idPesquisa, sizeof(idPesquisa), stdin)) {
                fprintf(stderr, "[ERRO] Não foi possível ler a ID informada\n");
                break;
            }
            // Lógica de pesquisa
            remover_quebra_linha(idPesquisa);
            rawId = atoi(idPesquisa + (isalpha(idPesquisa[0]) ? 1 : 0));
            formatar_id_paciente(rawId, idPesquisa, sizeof(idPesquisa));
            indicePaciente = pesquisar_paciente(0, qtdPacientes - 1, idPesquisa);

            // Remove o paciente 
            if (remover_item(pacientes, &qtdPacientes, sizeof(Paciente), indicePaciente) == NULL) {
                fprintf(stderr, "[ERRO] O paciente solicitado não existe\n");
                break;
            }

            printf("[%s Removido]\n", idPesquisa);
            break;

        case '6':
            // Voltar
            break;

        default:
            // Caso de erro (opção inválida)
            printf("[ERRO] Opção inválida. Tente novamente...\n\n");
            break;
        }
    } while (opcao != '6');
    // Se a opção 6 for selecionada quebramos o laço
}

