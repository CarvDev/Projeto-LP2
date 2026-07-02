#include <stdio.h>
#include <stdlib.h>
#include "agendamento.h"
#include "auxiliar.h"
#include "paciente.h"
#include "medico.h"


int main()
{
    char opcao = 0;
    
    // Inicializando a cabeça da lista encadeada dos agendamentos
    listaAgendamentos = calloc(1, sizeof(Agendamento));
    if (listaAgendamentos == NULL) {
        fprintf(stderr, "[ERRO CRÍTICO] Não foi possível alocar memória para os agendamentos.\n"
            "[Saindo do sistema...]\n");
        return 1;
    }

    // Carregando dados dos arquivos
    printf("[Carregando Dados Anteriores...]\n");

    // TODO: implementar alocação dinâmica para os pacientes
    pacientes = ler_arquivo(caminhoArqPacientes, &qtdPacientes, &capacidadePacientes, sizeof(Paciente));
    medicos = ler_arquivo(caminhoArqMedicos, &qtdMedicos, &capacidadeMedicos, sizeof(Medico));
    ler_arquivo_agendamentos(); // A função de carrgeamento de agendamentos é exclusiva

    // Sincroniza os contadores de ID baseando-se no último registro recuperado
    if (qtdMedicos > 0) ultimaIdMedico = atoi(medicos[qtdMedicos - 1].id + 1);
    if (qtdPacientes > 0) ultimaIdPaciente = atoi(pacientes[qtdPacientes - 1].id + 1);

    // Iniciando interação com usuário
    do {
        printf("\n");
        imprimir_menu();
        imprimir_cursor();
        opcao = obter_opcao();

        switch (opcao) {
        case '1':
            // Gerenciar pacientes
            modulo_pacientes();
            break;

        case '2':
            // Gerenciar doutores
            modulo_medicos();
            break;

        case '3':
            // Gerenciar agendamentos
            modulo_agendamentos();
            break;

        case '4':
            // Sair do sistema
            break;

        default:
            // Caso de erro (opção inválida)
            printf("[ERRO] Opção inválida. Tente novamente...\n");
            break;
        }
    } while (opcao != '4');
    // Se a opção 4 for selecionada quebramos o laço

    // Salvando dados
    printf("\n[Salvar Alterações? (S/n)]\n");
    imprimir_cursor();
    opcao = obter_opcao();
    
    if (maiusculo(opcao) != 'N') {
        printf("[Salvando Dados...]\n");
        criar_pasta("dados");
        gravar_arquivo(caminhoArqMedicos, qtdMedicos, medicos, sizeof(Medico));
        gravar_arquivo(caminhoArqPacientes, qtdPacientes, pacientes, sizeof(Paciente));
        gravar_arquivo_agendamentos();
    }

    // Liberação de memória dinâmica antes do fim do programa
    if (medicos != NULL) {
        free(medicos);
        medicos = NULL; 
    }

    if (pacientes != NULL) {
        free(pacientes);
        medicos = NULL;
    }

    if (listaAgendamentos != NULL) {
        Agendamento *atual = listaAgendamentos -> prox;
        Agendamento *proximo;
        
        while (atual != NULL) {
            proximo = atual->prox;
            free(atual);
            atual = proximo;
        }

        free(listaAgendamentos);
        listaAgendamentos = NULL;
    }

    printf("[Saindo do sistema...]\n");
    return 0;
}