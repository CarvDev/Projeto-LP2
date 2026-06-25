#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliar.h"
#include "paciente.h"
#include "medico.h"

int main()
{
    char opcao = 0;

    // Carregando dados dos arquivos
    printf("[Carregando Dados Anteriores...]\n");
    ler_arquivo(caminhoArqMedicos, &qtdMedicos, medicos, sizeof(Medico));
    ler_arquivo(caminhoArqPacientes, &qtdPacientes, pacientes, sizeof(Paciente));

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
            printf("[Opção selecionada: %c]\n", opcao);
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

    // Salvado dados
    printf("\n[Salvar Alterações? (S/n)]\n");
    imprimir_cursor();
    opcao = obter_opcao();
    if (toupper(opcao) != 'N') {
        printf("[Salvando Dados...]\n");
        criar_pasta("dados");
        gravar_arquivo(caminhoArqMedicos, qtdMedicos, medicos, sizeof(Medico));
        gravar_arquivo(caminhoArqPacientes, qtdPacientes, pacientes, sizeof(Paciente));
    }

    printf("[Saindo do sistema...]\n");
    return 0;
}
