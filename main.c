#include <stdio.h>
/*
 * Esse é o arquivo principal para o projeto
 * Inicialmente desenvolveremos tudo aqui
 * Posteriormente faremos a modularização
 */

// Essa será uma função simples de imprimir o menu
// Posteriormente podemos adicionar uma ASCII art aqui
void imprimir_menu()
{
    printf("------ HOSPITAL ------\n"
           "Digite a opção desejada:\n"
           // PLACEHOLDERS:
           "1. Gerenciar pacientes\n"
           "2. Gerenciar doutores\n"
           "3. Gerenciar agendamentos\n"
           "4. Sair do sistema\n");
}

// Imprime o caracatere '>' para servir de cursor no terminal
void imprimir_cursor() { printf("> "); }

// Função de limpeza de buffer, portável entre qualquer
// sistema operacional. Lê caracteres do buffer até
// consumir o \n, deixando o buffer limpo
void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int obter_opcao()
{
    int opc; // Variável para armazer a opção selecionada
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
            printf("[Opção selecionada: %d]\n\n", opcao);
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
