#include "paciente.h"
#include "auxiliar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Paciente *pacientes = NULL;
int capacidadePacientes = 0;
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
    formatar_id(ultimaIdPaciente + 1, 'P', paciente.id, sizeof(paciente.id));

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
            paciente.genero = maiusculo(generoTemp[0]);

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
                *ptrChar = maiusculo(*ptrChar);
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
    if (maiusculo(resposta) == 'N') return cadastrar_paciente();

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

// Sub-menu específico para editar os dados de um paciente sem afetar a ID
static void editar_dados_paciente(int indice) {
    char opcao;

    do {
        printf("\n--- EDITANDO PACIENTE ---\n");
        imprimir_paciente(pacientes[indice]);

        printf("\n[O que deseja alterar?]\n");
        printf("1. Nome\n");
        printf("2. Idade\n");
        printf("3. Gênero\n");
        printf("4. Peso\n");
        printf("5. Tipo Sanguíneo\n");
        printf("0. Concluir edição\n");
        imprimir_cursor();

        opcao = obter_opcao();

        switch (opcao) {
            case '1':
                while (1) {
                    printf("Novo Nome: ");
                    if (fgets(pacientes[indice].nome, sizeof(pacientes[indice].nome), stdin)) {
                        remover_quebra_linha(pacientes[indice].nome);
                        break;
                    }
                    fprintf(stderr, "[ERRO] Tente novamente\n");
                }
                printf("[AVISO] Nome atualizado com sucesso.\n");
                break;

            case '2':
                while (1) {
                    printf("Nova Idade: ");
                    if (scanf(" %d", &pacientes[indice].idade) == 1) break;
                    limpar_buffer();
                    fprintf(stderr, "[ERRO] Tente novamente\n");
                }
                limpar_buffer();
                printf("[AVISO] Idade atualizada com sucesso.\n");
                break;

            case '3':
                while (1) {
                    char generoTemp[10];
                    printf("Novo Gênero (F/M): ");
                    if (fgets(generoTemp, sizeof(generoTemp), stdin)) {
                        remover_quebra_linha(generoTemp);
                        pacientes[indice].genero = maiusculo(generoTemp[0]);

                        if (pacientes[indice].genero != 'M' && pacientes[indice].genero != 'F') {
                            fprintf(stderr, "[ERRO] Gênero Inválido. Por favor, tente novamente\n");
                            continue;
                        }
                        break;
                    }
                    fprintf(stderr, "[ERRO] Tente novamente\n");
                }
                printf("[AVISO] Gênero atualizado com sucesso.\n");
                break;

            case '4':
                while (1) {
                    printf("Novo Peso (kg): ");
                    if (scanf(" %f", &pacientes[indice].peso) == 1) break;
                    limpar_buffer();
                    fprintf(stderr, "[ERRO] Tente novamente\n");
                }
                limpar_buffer();
                printf("[AVISO] Peso atualizado com sucesso.\n");
                break;

            case '5':
                while (1) {
                    printf("Novo Tipo Sanguíneo: ");
                    char tipoSanguineoTemp[5];
                    if (fgets(tipoSanguineoTemp, sizeof(tipoSanguineoTemp), stdin)) {
                        remover_quebra_linha(tipoSanguineoTemp);

                        int tamString = strlen(tipoSanguineoTemp);
                        for (int i = 0; i < tamString; i++) {
                            tipoSanguineoTemp[i] = maiusculo(tipoSanguineoTemp[i]);
                        }

                        switch (tipoSanguineoTemp[0]) {
                            case 'A': break;
                            case 'B': break;
                            case 'O': break;
                            default: goto erro;
                        }

                        switch (tipoSanguineoTemp[tamString - 1]) {
                            case '+': break;
                            case '-': break;
                            default: goto erro;
                        }

                        if (tamString > 2 && tipoSanguineoTemp[1] != 'B') goto erro;

                        strcpy(pacientes[indice].tipoSanguineo, tipoSanguineoTemp);
                        break;

                        erro:
                        fprintf(stderr, "[ERRO] Tipo Sanguíneo Inválido. Por favor, tente novamente\n");
                        continue;
                    }
                    fprintf(stderr, "[ERRO] Tente novamente\n");
                }
                printf("[AVISO] Tipo Sanguíneo atualizado com sucesso.\n");
                break;

            case '0':
                printf("\n[Edição concluída]\n");
                break;

            default:
                printf("[ERRO] Opção inválida. Tente novamente...\n");
                break;
        }
    } while (opcao != '0');
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
            if (qtdPacientes >= capacidadePacientes) {
                // Se for a primeira vez, aloca 2 espaços. Se não, dobra o tamanho atual.
                capacidadePacientes = (capacidadePacientes == 0) ? 2 : capacidadePacientes * 2;
                
                Paciente *temp = realloc(pacientes, capacidadePacientes * sizeof(Paciente));
                if (temp == NULL) {
                    fprintf(stderr, "[ERRO CRÍTICO] Falha ao alocar memória!\n");
                    capacidadePacientes /= 2; // Reverte a capacidade em caso de erro
                    break;
                }
                pacientes = temp; // Atualiza o ponteiro com a nova memória
            }
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
            solicitar_id_inteligente(idPesquisa, 'M', sizeof(idPesquisa), "\n[Informe a ID do paciente a ser pesquisado]\n>  ");
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
            solicitar_id_inteligente(idPesquisa, 'P', sizeof(Paciente), "\n[Informe a ID do paciente a ser modificado]\n> ");
            indicePaciente = pesquisar_paciente(0, qtdPacientes - 1, idPesquisa);

            if (indicePaciente < 0) {
                fprintf(stderr, "[ERRO] O paciente solicitado não existe\n");
                break;
            }

            editar_dados_paciente(indicePaciente);
            break;

        case '5':
            // Remover
            solicitar_id_inteligente(idPesquisa, 'P', sizeof(Paciente), "\n[Informe a ID do paciente a ser removido]\n> ");
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

