#include <stdio.h>
#include <string.h>
#include "auxiliar.h"

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

void remover_quebra_linha(char *string) {
	// Busca o caractere \n na string
	int posicao = strcspn(string, "\n");
	char c = string[posicao];

	// Se c for \n, o remove
	if (c == '\n') {
		string[posicao] = '\0';
		return;
	} 
	
	// Se não encontrou \n, significa que a entrada foi truncada
	// e o resto da linha (lixo) ainda está no buffer do teclado.
	limpar_buffer();
}

char obter_opcao()
{
    char caractereLido;

    // Captura o primeiro caractere digitado (pode ser um número ou letra)
    caractereLido = getchar();

    // Limpa o restante do buffer imediatamente (consome o '\n' e lixos)
    if (caractereLido != '\n') limpar_buffer();

    return caractereLido;
}

void ler_arquivo(const char *nomeArquivo, int *ptrQuantidadeDados, void *vetorDados, size_t tamanhoDado) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        // Se não foi possível abrir o arquivo, fprint avisa e perror informa o motivo
        fprintf(stderr, "[AVISO] Não foi possível ler o arquivo '%s'. ", nomeArquivo);
        perror("Motivo");
        *ptrQuantidadeDados = 0;
        return;
    }

    // Primeiro lemos a quantidade de dados no cabeçalho do arquivo
    // O retorno de fread deve ser exatamente 1. Caso contrário, o arquivo está vazio ou corrompido.
    if (fread(ptrQuantidadeDados, sizeof(*ptrQuantidadeDados), 1, arquivo) != 1) {
        fprintf(stderr, "[AVISO] Arquivo '%s' vazio ou cabeçalho ilegível. Assumindo 0 registros.\n", nomeArquivo);
        *ptrQuantidadeDados = 0;
        fclose(arquivo);
        return;
    }

    // Em seguida, lemos os dados
    if (*ptrQuantidadeDados > 0) {
        fread(vetorDados, tamanhoDado, *ptrQuantidadeDados, arquivo);
    }

    fclose(arquivo);
}

void gravar_arquivo(const char *nomeArquivo, int quantidadeDados, void *vetorDados, size_t tamanhoDado) {
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        // Se não foi possível abrir o arquivo, perror informa o motivo
        // e fprintf avisa que os dados não serão salvos
        perror(nomeArquivo);
        fprintf(stderr, "[AVISO] Dados não serão salvos\n");
        return;
    }

    // No cabeçalho do arquivo, gravamos a quantidade dos dados
    if (fwrite(&quantidadeDados, sizeof(int), 1, arquivo) != 1) {
        fprintf(stderr, "[ERRO] Falha ao gravar o cabeçalho no arquivo '%s'. O disco pode estar cheio.\n", nomeArquivo);
        fclose(arquivo);
        return;
    }

    // Em seguida, gravamos os dados
    if (quantidadeDados > 0) {
        size_t itensGravados = fwrite(vetorDados, tamanhoDado, quantidadeDados, arquivo);
        
        // O retorno do fwrite deve ser exatamente igual a 'quantidadeDados'.
        if (itensGravados != (size_t)quantidadeDados) {
            fprintf(stderr, "[ERRO] Gravação incompleta em '%s'. Gravados %zu de %d itens.\n",
                nomeArquivo, itensGravados, quantidadeDados);
        }
    }

    fclose(arquivo);
}

void remover_arquivo(char *nomeArquivo) {
    // Se o retorno de remove for diferente de zero, não foi possível remover o arquivo
    if (remove(nomeArquivo) != 0) {
        fprintf(stderr, "[ERRO] Não foi possivel deletar o arquivo.\n");
        perror(nomeArquivo);
        return;
    } 
    
    printf("[AVISO] Arquivo '%s' removido.\n", nomeArquivo);
}