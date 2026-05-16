#include <stdio.h>
#include "auxiliar.h"
#include "paciente.h"

int main()
{
    int opcao = 0;

    // Iniciando interação com usuário
    do {
        imprimir_menu();
        imprimir_cursor();
        opcao = obter_opcao();

        switch (opcao) {
        case 1:
            // Gerenciar pacientes
            modulo_pacientes();
            break;

        case 2:
            // Gerenciar doutores
            printf("[Opção selecionada: %d]\n\n", opcao);
            break;

        case 3:
            // Gerenciar agendamentos
            printf("[Opção selecionada: %d]\n\n", opcao);
            break;

        case 4:
            // Sair do sistema
            // printf("[Opção selecionada: %d]\n\n", opcao);
            printf("[Saindo do sistema...]\n\n");
            break;

        default:
            // Caso de erro (opção inválida)
            printf("[Opção inválida]\nTente novamente...\n\n");
            break;
        }
    } while (opcao != 4);
    // Se a opção 4 for selecionada quebramos o laço

    return 0;
}
