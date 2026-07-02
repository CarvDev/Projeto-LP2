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
        "6. Limpar agendamentos antigos"
        "7. Voltar\n",
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

Horario solicitar_horario() {
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

    return horario;
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
    Horario horario = solicitar_horario();
    novo->timestamp = obter_timestamp(horario);

    while(1) {
        solicitar_id_inteligente(novo->idPaciente, 'P', sizeof(novo->idPaciente), "ID do Paciente: ");
        if (pesquisar_paciente(0, qtdPacientes - 1, novo->idPaciente) == -1) {
            fprintf(stderr, "[ERRO] O paciente não existe. Por favor, tente novamente...\n");
        } else break;
    }

    while (1) {
        solicitar_id_inteligente(novo->idMedico, 'M', sizeof(novo->idMedico), "ID do Médico: ");
        if (pesquisar_medico(0, qtdMedicos - 1, novo->idMedico) == -1) {
            fprintf(stderr, "[ERRO] O médico não existe. Por favor, tente novamente...\n");
        } else break;
    }

    // Imprimindo os dados lidos
    printf("\n");
    imprimir_agendamento(novo);
    printf("[Os dados conferem? (S/n)]\n");
    imprimir_cursor();
    char resposta = obter_opcao();
    if (maiusculo(resposta) == 'N') {
        free(novo);
        printf("[AVISO] Cadastro cancelado.\n");
        return; 
    }

    // Percorre a lista avaliando o 'timestamp' do próximo nó para encontrar
    // o ponto de inserção correto e manter a lista cronologicamente ordenada.
    Agendamento *atual = lista;
    while(atual->prox != NULL && atual->prox->timestamp <= novo->timestamp) {
        atual = atual->prox;
    } 

    // Inserindo o novo agendamento na posição ideal e refazendo as ligações
    novo->anterior = atual;
    novo->prox = atual->prox;
    
    // Garante que o nó seguinte (se existir) aponte de volta para o novo nó,
    // mantendo a integridade da lista duplamente encadeada.
    if (atual->prox != NULL) {
        atual->prox->anterior = novo;
    }
    atual->prox = novo;
    
    (qtdAgendamentos)++;
    (ultimaIdAgendamento)++;
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

Agendamento *remover_agendamento(Agendamento *agendamento) {
    // Garante que o nó recebido não é nulo
    if (agendamento == NULL) {
        return NULL;
    }

    // Isola o nó atual ajustando os ponteiros dos vizinhos
    agendamento->anterior->prox = agendamento->prox;
    if (agendamento->prox != NULL) {
        agendamento->prox->anterior = agendamento->anterior;
    }

    // Libera a memória do nó isolado
    free(agendamento);

    // Retorna NULL para indicar que o objeto não é mais válido
    return NULL;
}

static void editar_dados_agendamento(Agendamento *agendamento, Agendamento *lista) {
    char opcao;
    char novoId[6];
    int indice;

    do {
        printf("\n--- EDITANDO AGENDAMENTO ---\n");
        imprimir_agendamento(agendamento);

        printf("\n[O que deseja alterar?]\n");
        printf("1. Médico\n");
        printf("2. Paciente\n");
        printf("3. Horário\n");
        printf("0. Concluir edição\n");
        imprimir_cursor();

        opcao = obter_opcao();

        switch (opcao) {
            case '1':
                solicitar_id_inteligente(novoId, 'M', sizeof(novoId), "ID do novo Médico: ");
                indice = pesquisar_medico(0, qtdMedicos - 1, novoId);
                
                // Previne a associação de um médico inexistente
                if (indice == -1) {
                    fprintf(stderr, "[ERRO] O Médico não existe\n");
                    break;
                }

                strcpy(agendamento->idMedico, novoId);
                printf("[AVISO] Médico atualizado com sucesso.\n");
                break;
            
            case '2':
                solicitar_id_inteligente(novoId, 'P', sizeof(novoId), "ID do novo Paciente: ");
                indice = pesquisar_paciente(0, qtdPacientes - 1, novoId);

                // Previne a associação de um paciente inexistente
                if (indice == -1) {
                    fprintf(stderr, "[ERRO] O Paciente não existe\n");
                    break;
                }

                strcpy(agendamento->idPaciente, novoId);
                printf("[AVISO] Paciente atualizado com sucesso.\n");
                break;

            case '3':
                printf("-- Novo Horário --\n");
                Horario novoHorario = solicitar_horario();
                time_t novoTimestamp = obter_timestamp(novoHorario);

                // Otimização: Evita realocação e reordenação desnecessária
                if (novoTimestamp == agendamento->timestamp) {
                    printf("[AVISO] O horário é o mesmo. Nenhuma alteração foi feita.\n");
                    break;
                }

                // Isola o nó atual ajustando os ponteiros dos vizinhos
                agendamento->anterior->prox = agendamento->prox;
                if (agendamento->prox != NULL) {
                    agendamento->prox->anterior = agendamento->anterior;
                }

                // Atualiza o dado temporal
                agendamento->timestamp = novoTimestamp;

                // Percorre a lista avaliando o 'timestamp' do próximo nó para encontrar
                // o ponto de inserção correto e manter a lista cronologicamente ordenada.
                Agendamento *busca = lista;
                while(busca->prox != NULL && busca->prox->timestamp <= agendamento->timestamp) {
                    busca = busca->prox;
                } 

                // Refaz as ligações inserindo o nó isolado na nova posição descoberta
                agendamento->anterior = busca;
                agendamento->prox = busca->prox;
                
                if (busca->prox != NULL) {
                    busca->prox->anterior = agendamento;
                }
                busca->prox = agendamento;

                printf("[AVISO] Horário atualizado com sucesso.\n");
                break;
            
            case '0':
                printf("[Edição Concluída]\n");
                break;

            default:
                fprintf(stderr, "[ERRO] Opção inválida. tente novamente...\n");
                break;
        }
    } while (opcao != '0');
}

void modulo_agendamentos() {
    char opcao = 0;
    char idPesquisa[6];
    Agendamento *agendamentoEncontrado = NULL;
    Agendamento *atual = listaAgendamentos; 
    time_t limite = time(NULL) - 86400; // 24 horas atrás

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
            atual = listaAgendamentos; 
            while(atual->prox != NULL) {
                atual = atual->prox;
                imprimir_agendamento(atual);
            }
            printf("-- FIM DA LISTA --\n");
            break;

        case '3':
            // Pesquisa
            solicitar_id_inteligente(idPesquisa, 'A', sizeof(idPesquisa),
                "\n[Informe a ID do agendamento a ser pesquisado]\n> ");
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
            solicitar_id_inteligente(idPesquisa, 'A', sizeof(idPesquisa), 
                "\n[Informe a ID do agendamento a ser modificado]\n> ");
            agendamentoEncontrado = pesquisar_agendamento(listaAgendamentos, idPesquisa);

            if (agendamentoEncontrado == NULL) {
                fprintf(stderr, "[ERRO] O agendamento solicitado não existe\n");
                break;
            }

            editar_dados_agendamento(agendamentoEncontrado, listaAgendamentos);
            break;

        case '5':
            // Remover
            solicitar_id_inteligente(idPesquisa, 'A', sizeof(idPesquisa),
                "\n[Informe a ID do agendamento a ser removido]\n> ");
            agendamentoEncontrado = pesquisar_agendamento(listaAgendamentos, idPesquisa);
            
            if (agendamentoEncontrado == NULL) {
                fprintf(stderr, "[ERRO] O agendamento solicitado não existe\n");
                break;
            }

            agendamentoEncontrado = remover_agendamento(agendamentoEncontrado);
            (qtdAgendamentos)--;

            printf("[%s Removido]\n", idPesquisa);
            break;

        case '6':
            // Apagar antigos (24 horas ou mais)
            atual = listaAgendamentos->prox;
            Agendamento *proximo;
            int removidos = 0;

            // Iteramos sequencialmente e interrompemos o laço no momento em que encontrarmos 
            // um agendamento mais recente que o limite imposto.
            while(atual != NULL && atual->timestamp <= limite) {
                proximo = atual->prox; 
                
                remover_agendamento(atual);
                
                (qtdAgendamentos)--;
                (removidos)++;
                
                atual = proximo;
            }
            
            printf("[AVISO] Limpeza concluída: %d agendamento(s) antigo(s) removido(s).\n", removidos);
            break;

        case '7':
            // Voltar
            break;

        default:
            // Caso de erro (opção inválida)
            printf("[ERRO] Opção inválida. Tente novamente...\n\n");
            break;
        }
    } while (opcao != '7');
    // Se a opção 6 for selecionada quebramos o laço
}