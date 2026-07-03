#include "medico.h"
#include "auxiliar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Medico *medicos = NULL;
int capacidadeMedicos = 0; // Controla o tamanho total alocado na memória
int qtdMedicos = 0;        // Controla quantos médicos realmente existem
int ultimaIdMedico = 0;    // Para controle numérico das IDs
const char caminhoArqMedicos[] = "dados/medicos.bin";

static void imprimir_menu_medicos() {
    printf("------ MÉDICOS ------\n"
        "[Médicos Cadastrados: %d]\n\n"

        "[Digite a opção desejada]\n"
        "1. Cadastrar médico\n"
        "2. Visualizar todos os médicos\n"
        "3. Pesquisar médico\n"
        "4. Modificar médico\n"
        "5. Remover médico\n"
        "0. Voltar\n",
    qtdMedicos);
}

void imprimir_medico(Medico medico) {
    printf("%s:\n", medico.id);
    printf("  Nome: %s\n", medico.nome);
    printf("  CRM: %s\n", medico.crm);
    printf("  Especialização: %s\n", medico.especializacao);
}

// Protótipo para utilizar na função de cadastro
static void editar_dados_medico(Medico *medico);

Medico cadastrar_medico() {
    Medico medico;

    printf("---- NOVO MÉDICO ----\n");

    // Id (definida automaticamente usando a nova função do auxiliar)
    formatar_id(ultimaIdMedico + 1, 'M', medico.id, sizeof(medico.id));

    // Nome
    while (1) {
        printf("Nome completo: ");
        
        if (fgets(medico.nome, sizeof(medico.nome), stdin)) {
            remover_quebra_linha(medico.nome);
            break; 
        } 
        fprintf(stderr, "[ERRO] Por favor, tente novamente\n");
    } 

    // CRM
    while (1) {
        printf("CRM: ");
        
        if (fgets(medico.crm, sizeof(medico.crm), stdin)) {
            remover_quebra_linha(medico.crm);
            break; 
        } 
        fprintf(stderr, "[ERRO] Por favor, tente novamente\n");
    } 

    // Especializacao
    while (1) {
        printf("Especialização: ");
        
        if (fgets(medico.especializacao, sizeof(medico.especializacao), stdin)) {
            remover_quebra_linha(medico.especializacao);
            break; 
        } 
        fprintf(stderr, "[ERRO] Por favor, tente novamente\n");
    }

    // Imprimindo dados lidos
    printf("\n");
    imprimir_medico(medico);
    printf("[Os dados conferem? (S/n)]\n");
    imprimir_cursor();
    char resposta = obter_opcao();
    
    if (maiusculo(resposta) == 'N') editar_dados_medico(&medico); 

    (qtdMedicos)++;
    (ultimaIdMedico)++;
    return medico;
}

int pesquisar_medico(int inicio, int fim, char idDesejada[]) {

    if (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int resultadoComparacao = strcmp(medicos[meio].id, idDesejada);

        // ID igual a do medico do meio
        if (resultadoComparacao == 0) {
            return meio;
        }

        // ID procurada é menor que a do meio (busca na metade esquerda)
        if (resultadoComparacao > 0) {
            return pesquisar_medico(inicio, meio - 1, idDesejada);
        }

        // ID procurada é maior que a do meio (busca na metade direita)
        return pesquisar_medico(meio + 1, fim, idDesejada);
    }

    // Caso base: Elemento não encontrado (inicio > fim)
    return -1;
}

// Sub-menu específico para editar os dados de um médico sem afetar a ID
static void editar_dados_medico(Medico *medico) {
    char opcao;

    do {
        printf("\n--- EDITANDO MÉDICO ---\n");
        imprimir_medico(*medico);
        
        printf("\n[O que deseja alterar?]\n");
        printf("1. Nome\n");
        printf("2. CRM\n");
        printf("3. Especialização\n");
        printf("0. Concluir edição\n");
        imprimir_cursor();
        
        opcao = obter_opcao();

        switch (opcao) {
            case '1':
                while (1) {
                    printf("Novo Nome: ");
                    if (fgets(medico->nome, sizeof(medico->nome), stdin)) {
                        remover_quebra_linha(medico->nome);
                        break;
                    }
                    fprintf(stderr, "[ERRO] Tente novamente\n");
                }
                printf("[AVISO] Nome atualizado com sucesso.\n");
                break;
                
            case '2':
                while (1) {
                    printf("Novo CRM: ");
                    if (fgets(medico->crm, sizeof(medico->crm), stdin)) {
                        remover_quebra_linha(medico->crm);
                        break;
                    }
                    fprintf(stderr, "[ERRO] Tente novamente\n");
                }
                printf("[AVISO] CRM atualizado com sucesso.\n");
                break;
                
            case '3':
                while (1) {
                    printf("Nova Especialização: ");
                    if (fgets(medico->especializacao, sizeof(medico->especializacao), stdin)) {
                        remover_quebra_linha(medico->especializacao);
                        break;
                    }
                    fprintf(stderr, "[ERRO] Tente novamente\n");
                }
                printf("[AVISO] Especialização atualizada com sucesso.\n");
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

void modulo_medicos() {
    char opcao = 0;
    char idPesquisa[6];
    int indiceMedico;

    // Iniciando interação com usuário
    do {
        printf("\n");
        imprimir_menu_medicos();
        imprimir_cursor();
        opcao = obter_opcao();

        switch (opcao) {
        case '1':
            if (qtdMedicos >= capacidadeMedicos) {
                // Se for a primeira vez, aloca 2 espaços. Se não, dobra o tamanho atual.
                capacidadeMedicos = (capacidadeMedicos == 0) ? 2 : capacidadeMedicos * 2;
                
                Medico *temp = realloc(medicos, capacidadeMedicos * sizeof(Medico));
                if (temp == NULL) {
                    fprintf(stderr, "[ERRO CRÍTICO] Falha ao alocar memória!\n");
                    capacidadeMedicos /= 2; // Reverte a capacidade em caso de erro
                    break;
                }
                medicos = temp; // Atualiza o ponteiro com a nova memória
            }
            medicos[qtdMedicos] = cadastrar_medico();
            break;

        case '2':
            // Visualizar todos
            printf("\n-- INÍCIO DA LISTA --\n");
            for (int i = 0; i < qtdMedicos; i++) {
                imprimir_medico(medicos[i]);
            }
            printf("-- FIM DA LISTA --\n");
            break;

        case '3':
            // Pesquisar 
            solicitar_id_inteligente(idPesquisa, 'M', sizeof(idPesquisa), "\n[Informe a ID do médico a ser pesquisado]\n> ");
            indiceMedico = pesquisar_medico(0, qtdMedicos - 1, idPesquisa);

            // Caso de erro
            if (indiceMedico < 0) {
                fprintf(stderr, "[AVISO] Médico não encontrado\n");
                break; 
            }
            // Caso de sucesso
            printf("\n -- MÉDICO ENCONTRADO --\n");
            imprimir_medico(medicos[indiceMedico]);
            break;

        case '4':
            // Modificar 
            solicitar_id_inteligente(idPesquisa, 'M', sizeof(idPesquisa), "\n[Informe a ID do médico a ser modificado]\n> ");
            indiceMedico = pesquisar_medico(0, qtdMedicos - 1, idPesquisa);

            // Se não encontrou
            if (indiceMedico < 0) {
                fprintf(stderr, "[ERRO] O médico solicitado não existe\n");
                break;
            }

            // Se encontrou, chama o sub-menu passando o ponteiro para o médico
            editar_dados_medico(&medicos[indiceMedico]);
            break;

        case '5':
            // Remover
            solicitar_id_inteligente(idPesquisa, 'M', sizeof(idPesquisa), "\n[Informe a ID do médico a ser removido]\n> ");
            indiceMedico = pesquisar_medico(0, qtdMedicos - 1, idPesquisa);

            // Remove o médico
            if (remover_item(medicos, &qtdMedicos, sizeof(Medico), indiceMedico) == NULL) {
                fprintf(stderr, "[ERRO] O médico solicitado não existe\n");
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
    } while (opcao != '0');
}