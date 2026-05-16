#include <stdio.h>

void imprimir_menu()
{
    printf("------ HOSPITAL ------\n"
           "Digite a opção desejada:\n"
           "1. Gerenciar pacientes\n"
           "2. Gerenciar doutores\n"
           "3. Gerenciar agendamentos\n"
           "4. Sair do sistema\n");
}

void imprimir_cursor() { printf("> "); }

void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int obter_opcao()
{
    int opc; // Variável para armazenar a opção selecionada
    int scanf_retorno; // Variável para checar o retorno do scanf

    while (1) {
        // Armazena o valor de retorno da função scanf
        scanf_retorno = scanf("%d", &opc);

        // Caso de erro para quando o input NÃO for um número (ex: 'a')
        // (scanf_retorno será 0)
        if (scanf_retorno == 0) {
            printf("[Erro: Digite apenas números]\nTente novamente...\n\n");
            limpar_buffer(); // Limpa o input inválido (ex: 'a\n')
            imprimir_cursor();
            continue; // Pula para a próxima iteração do loop

        } else
            break; // Quebra o loop
    }

    limpar_buffer();
    return opc; // Retorna a opção selecionada
}
