#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "auxiliar.h"
#include "agendamento.h"
#include "medico.h"
#include "paciente.h"

void imprimir_menu()
{
    printf("------ HOSPITAL ------\n"
           "Digite a opção desejada:\n"
           "1. Gerenciar pacientes (cadastros: %d)\n"
           "2. Gerenciar médicos (cadastros: %d)\n"
           "3. Gerenciar agendamentos (cadastros: %d)\n"
           "4. Sair do sistema\n",
        qtdPacientes, qtdMedicos, qtdAgendamentos);
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

void formatar_id(int rawId, char caractere, char* stringId, size_t tamString) {
    snprintf(stringId, tamString, "%c%03d", caractere, rawId);
}

void solicitar_id_inteligente(char stringId[], char caractere, size_t tamString, char mensagem[]) {
    while (1) {
        printf("%s", mensagem);

        if (!fgets(stringId, tamString, stdin)) {
            fprintf(stderr, "[ERRO] Por favor, tente novamente\n");
            remover_quebra_linha(stringId);
            continue;
        }
        break;
    }
    remover_quebra_linha(stringId);

    int rawId = atoi(stringId + (isalpha(stringId[0]) ? 1 : 0));
    formatar_id(rawId, caractere, stringId, tamString);
}

void* ler_arquivo(const char *nomeArquivo, int *ptrQuantidadeDados, int *ptrCapacidadeVetor, size_t tamanhoDado) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        // Em caso de falha de abertura, a quantidade é zerada e retorna-se NULL para indicar ausência de dados
        fprintf(stderr, "[AVISO] Não foi possível ler o arquivo '%s' (Começando sem dados deste Módulo)\n", nomeArquivo);
        *ptrQuantidadeDados = 0;
        return NULL;
    }

    // Leitura da quantidade de registros no cabeçalho
    if (fread(ptrQuantidadeDados, sizeof(*ptrQuantidadeDados), 1, arquivo) != 1) {
        fprintf(stderr, "[AVISO] Arquivo '%s' vazio ou cabeçalho ilegível. Assumindo 0 registros.\n", nomeArquivo);
        *ptrQuantidadeDados = 0;
        fclose(arquivo);
        return NULL;
    }

    if (*ptrQuantidadeDados <= 0) {
        fclose(arquivo);
        return NULL;
    }

    // Calcula a potência de 2 mais próxima para acomodar os dados no vetor dinâmico
    for (*ptrCapacidadeVetor = 2; *ptrCapacidadeVetor <= *ptrQuantidadeDados; *ptrCapacidadeVetor *= 2);

    // Aloca um bloco contíguo de memória para a capacidade calculada
    void *vetorDados = malloc(*(ptrCapacidadeVetor) * tamanhoDado);
    if (vetorDados == NULL) {
        fprintf(stderr, "[ERRO] Falha de alocação de memória para os dados do arquivo '%s'.\n", nomeArquivo);
        *ptrQuantidadeDados = 0;
        *ptrCapacidadeVetor = 0;
        fclose(arquivo);
        return NULL;
    }

    // Preenche o bloco alocado com os dados do arquivo
    fread(vetorDados, tamanhoDado, *ptrQuantidadeDados, arquivo);
    fclose(arquivo);

    return vetorDados;
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

void *remover_item(void *vetor, int *tamanhoVetor, size_t tamanhoDado, int indiceItem) {
    // Validação de segurança dos limites do vetor
    if (indiceItem < 0 || indiceItem >= *tamanhoVetor) {
        return NULL;
    }

    // Calculando quantos elementos precisam ser deslocados
    int itensParaMover = *tamanhoVetor - indiceItem - 1;

    if (itensParaMover > 0) {
        // Convertemos para uint8_t* (1 byte) para permitir aritmética de ponteiros 
        uint8_t *ptrGenerico = (uint8_t *)vetor;

        // Calculamos os endereços de memória exatos baseados no tamanho do dado
        uint8_t *destino = ptrGenerico + (indiceItem * tamanhoDado);
        uint8_t *origem  = destino + tamanhoDado;

        memmove(destino, origem, itensParaMover * tamanhoDado);
    }

    (*tamanhoVetor)--;
    return vetor;
}

char maiusculo (char c) {
	// Garante que o caractere está no intervalo das minúsculas 'a'..'z'
	if (c < 'a' || c > 'z') return c;

	// Máscara correspondente a 0b11011111 
	// As minúsculas possuem o bit 5 ativo (limpá-lo resulta na maiúscula)
	char filtro = 0xDF;

	// Aplica a máscara para zerar o bit 5
	c &= filtro;

	return c;
}