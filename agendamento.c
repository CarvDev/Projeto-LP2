#include "agendamento.h"
#include "auxiliar.h"
#include "paciente.h"
#include "medico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ucontext.h>
#include <time.h>

int qtdAgendamentos = 0;
int ultimaIdAgendamento = 0;
const char caminhoArqAgendamentos[] = "dados/agendamentos.bin";
Agendamento *listaAgendamentos = NULL;
// A cabeça da lista será inicializada em main()

static void imprimir_menu_agendamentos() {
    printf("------ AGENDAMENTOS ------\n"
        "[Agendamentos Cadastrados: %d]\n\n"

        "[Digite a opção desejada]\n"
        "1. Cadastrar agendamento\n"
        "2. Visualizar todos os agendamentos\n"
        "3. Pesquisar agendamento\n"
        "4. Modificar agendamento\n"
        "5. Remover agendamento\n"
        "6. Voltar\n",
    qtdAgendamentos);
}

time_t obter_timestamp(Horario horario) {
    struct tm entrada = {0};
    time_t wTimestamp;

    // Preenche a estrutura respeitando as regras da biblioteca <time.h>
    entrada.tm_mday = horario.dia;
    entrada.tm_mon = horario.mes - 1;
    entrada.tm_year = horario.ano - 1900;
    entrada.tm_hour = horario.hora;
    entrada.tm_min = horario.minuto;
    
    // Define -1 para que o sistema descubra sozinho se naquela data operava horário de verão
    entrada.tm_isdst = -1;

    // Converte para Unix timestamp
    wTimestamp = mktime(&entrada);
    return wTimestamp;
}

Horario obter_horario(time_t timestamp) {
    Horario wHorario;
    struct tm *tempoLocal = localtime(&timestamp);

    wHorario.ano = tempoLocal->tm_year + 1900;
    wHorario.mes = tempoLocal->tm_mon + 1;
    wHorario.dia = tempoLocal->tm_mday;
    wHorario.hora = tempoLocal->tm_hour;
    wHorario.minuto = tempoLocal->tm_min;

    return wHorario;
}

void imprimir_agendamento(Agendamento *agendamento) {
    int indiceMedico = pesquisar_medico(0, qtdMedicos - 1, agendamento->idMedico);
    int indicePaciente = pesquisar_paciente(0, qtdPacientes - 1, agendamento->idPaciente);
    Horario horario = obter_horario(agendamento->timestamp);

    printf("%s:\n", agendamento->id);
    printf("  Data: %02d/%02d/%04d\n", horario.dia, horario.mes, horario.ano);
    printf("  Horário: %02d:%02d\n", horario.hora, horario.minuto);

    // Imprime ID e nome (ou ERRO se inválido) do médico e paciente 
    printf("  Médico: %s (%s)\n", agendamento->idMedico, (indiceMedico >= 0)? medicos[indiceMedico].nome : "ERRO");
    printf("  Paciente: %s (%s)\n", agendamento->idPaciente, (indicePaciente >= 0)? pacientes[indicePaciente].nome : "ERRO");
    return;
}

static void cadastrar_agendamento(Agendamento *lista) {
    Agendamento *novo = malloc(sizeof(Agendamento));
    if (novo == NULL) {
        fprintf(stderr, "[ERRO] Não foi possível alocar memória para o agendamento");
        return;
    }

    printf("---- NOVO AGENDAMENTO ----\n");

    // ID (definida automaticamente)
    formatar_id(qtdAgendamentos + 1, 'A', novo->id, sizeof(novo->id));

    // Data 
    Horario horario;
    while (1) {
        printf("Data (DD/MM/AAAA ou DD/MM): ");

        // Se leu 2 dados, usa o ano atual
        int retornoScanf = scanf("%d/%d/%d", &horario.dia, &horario.mes, &horario.ano);
        if (retornoScanf == 2) {
            // Obtém o ano atual e salva na variável de horário
            time_t tempoBruto;
            struct tm *tempoLocal;
            time(&tempoBruto);
            tempoLocal = localtime(&tempoBruto);
            horario.ano = tempoLocal->tm_year + 1900;
            break;
        }

        // Se leu 3, usa o ano informado
        if (retornoScanf == 3) break;

        // Caso de erro
        limpar_buffer();
        fprintf(stderr, "[ERRO] Por favor, tente novamente...\n");
    }
    limpar_buffer();

    // Horário
    while (1) {
        printf("Horário (HH:MM): ");

        if (scanf("%d:%d", &horario.hora, &horario.minuto) == 2) break; 
        limpar_buffer();
        fprintf(stderr, "[ERRO] Por favor, tente novamente...\n");
    }
    limpar_buffer();

    novo->timestamp = obter_timestamp(horario);

    solicitar_id_inteligente(novo->idPaciente, 'P', sizeof(novo->idPaciente), "ID do Paciente: ");
    if (pesquisar_paciente(0, qtdPacientes - 1, novo->idPaciente) == -1) {
        fprintf(stderr, "[ERRO] O paciente não existe\n");
        goto erro;
    }

    solicitar_id_inteligente(novo->idMedico, 'M', sizeof(novo->idMedico), "ID do Médico: ");
    if (pesquisar_medico(0, qtdMedicos - 1, novo->idMedico) == -1) {
        fprintf(stderr, "[ERRO] O médico não existe\n");
        goto erro;
    }

    // Imprimindo os dados lidos
    printf("\n");
    imprimir_agendamento(novo);
    printf("[Os dados conferem? (S/n)]\n");
    imprimir_cursor();
    char resposta = obter_opcao();
    if (maiusculo(resposta) == 'N') goto erro;

    // Varrendo a lista
    Agendamento *atual = lista;
    while(atual->prox != NULL && atual->timestamp <= novo->timestamp) {
        atual = atual->prox;
    } 

    // Inserindo o novo agendamento na posição ideal
    novo->anterior = atual;
    novo->prox = atual->prox;
    atual->prox = novo;
    (qtdAgendamentos)++;
    (ultimaIdAgendamento)++;
    return;

    // Libera a memória alocada e retorna sem alterar a lista original
    erro:
    if (novo != NULL) free(novo);
    return;
}

Agendamento *pesquisar_agendamento(Agendamento *lista, char idDesejada[]) {
    // Varrendo a lista
    Agendamento *atual = lista; 
    while (atual->prox != NULL) {
        atual = atual->prox;

        // Sucesso (retorna o endereço do encontrado)
        if (strcmp(atual->id, idDesejada) == 0) return atual;
        
        // Erro (repete a iteração)
        continue;
    }

    // Se não encontrar nenhum resultado, retorna nulo
    return NULL;
}

void modulo_agendamentos() {
    char opcao = 0;
    char idPesquisa[6];
    Agendamento *agendamentoEncontrado = NULL;

    // Iniciando interação com usuário
    do {
        printf("\n");
        imprimir_menu_agendamentos();
        imprimir_cursor();
        opcao = obter_opcao();

        switch (opcao) {
        case '1':
            // Cadastrar
            cadastrar_agendamento(listaAgendamentos);
            break;

        case '2':
            // Visualizar todos
            printf("\n-- INÍCIO DA LISTA --\n");
            Agendamento *atual = listaAgendamentos; 
            while(atual->prox != NULL) {
                atual = atual->prox;
                imprimir_agendamento(atual);
            }
            printf("-- FIM DA LISTA --\n");
            break;

        case '3':
            // Pesquisa
            solicitar_id_inteligente(idPesquisa, 'A', sizeof(idPesquisa),
                "[Informe a ID do atendimento a ser pesquisado]");
            agendamentoEncontrado = pesquisar_agendamento(listaAgendamentos, idPesquisa);

            // Caso de erro 
            if (agendamentoEncontrado == NULL) {
                fprintf(stderr, "[AVISO] Agendamento não encontrado\n");
                break;
            }
            // Caso de sucesso
            printf("\n -- AGENDAMENTO ENCONTRADO --\n");
            imprimir_agendamento(agendamentoEncontrado);
            break;

        case '4':
            // Modificar

            break;

        case '5':
            // Remover
            solicitar_id_inteligente(idPesquisa, 'A', sizeof(idPesquisa),
                "\n[Informe a ID do atendimento a ser removido]\n> ");
            agendamentoEncontrado = pesquisar_agendamento(listaAgendamentos, idPesquisa);
            
            if (agendamentoEncontrado == NULL) {
                fprintf(stderr, "[ERRO] O agendamento solicitado não existe\n");
                break;
            }

            // Lógica de remoção
            agendamentoEncontrado->anterior->prox = agendamentoEncontrado->prox;
            if (agendamentoEncontrado->prox != NULL) {
                agendamentoEncontrado->prox->anterior = agendamentoEncontrado->anterior;
            }
            free(agendamentoEncontrado);
            agendamentoEncontrado = NULL;

            (qtdAgendamentos)--;
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