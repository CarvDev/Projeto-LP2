#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stddef.h> // para o size_t

// Verifica o sistema operacional para incluir a biblioteca correta de diretórios
#ifdef _WIN32
    #include <direct.h>
    #define criar_pasta(path) _mkdir(path)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define criar_pasta(path) mkdir(path, 0777)
#endif

// Essa será uma função simples de imprimir o menu
// Posteriormente podemos adicionar uma ASCII art aqui
void imprimir_menu();

// Imprime o caracatere '>' para servir de cursor no terminal
void imprimir_cursor();

// Função de limpeza de buffer, portável entre qualquer
// sistema operacional. Lê caracteres do buffer até
// consumir o \n, deixando o buffer limpo
void limpar_buffer();

// Função auxiliar para remover o \n numa string
// Limpa o buffer caso necessário
void remover_quebra_linha(char *string);

// Essa função lê um caractere do input do usuário,
// limpa o buffer de forma segura e o retorna
char obter_opcao();

/* 
 * Converte um ID numérico para o formato padrão do sistema (Prefixo + 3 dígitos).
 * Exemplo: se o caractere for 'P' e rawId for 7, o resultado gravado será "P007".
 * Limita a gravação ao tamString para evitar buffer overflow.
 */
void formatar_id(int rawId, char caractere, char* stringId, size_t tamString);

/*
 * Lê o ID digitado pelo usuário e corrige a formatação automaticamente.
 * A lógica ignora se o usuário digitou a letra do prefixo ou não:
 * ela verifica se o primeiro caractere é uma letra, pula ele se for, 
 * extrai apenas o número com atoi() e reaplica o formato padrão no mesmo buffer.
 */
void solicitar_id_inteligente(char stringId[], char caractere, size_t tamString, char mensagem[]);

/* Lê o cabeçalho (quantidade) e carrega o bloco de dados de um arquivo binário. 
   Grava a quantidade de dados via ponteiro.
   Define a quantidade como 0 caso o arquivo não exista ou esteja corrompido. */
void ler_arquivo(const char *nomeArquivo, int *ptrQuantidadeDados, void *vetorDados, size_t tamanhoDado);

// Grava a quantidade de elementos no cabeçalho e depois o bloco de dados em formato binário.
void gravar_arquivo(const char *nomeArquivo, int quantidadeDados, void *vetorDados, size_t tamanhoDado);

// Tenta excluir o arquivo passado como argumento
void remover_arquivo(char *nomeArquivo);

/**
 * Remove um item de um vetor genérico mantendo a ordenação.
 * Retorna o ponteiro para o vetor em caso de sucesso, ou NULL se o índice for inválido.
 */
void *remover_item(void *vetor, int *tamanhoVetor, size_t tamanhoDado, int indiceItem);

/* Função biwise que torna o caratere recebido maiúsculo.
 * Funciona igual a função toupper() e retorna o mesmo caractere em
   caso de não ser uma letra minúscula
*/
char maiusculo (char c);

#endif