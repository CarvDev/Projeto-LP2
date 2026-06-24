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

/* Lê o cabeçalho (quantidade) e carrega o bloco de dados de um arquivo binário. 
   Grava a quantidade de dados via ponteiro.
   Define a quantidade como 0 caso o arquivo não exista ou esteja corrompido. */
void ler_arquivo(const char *nomeArquivo, int *ptrQuantidadeDados, void *vetorDados, size_t tamanhoDado);

// Grava a quantidade de elementos no cabeçalho e depois o bloco de dados em formato binário.
void gravar_arquivo(const char *nomeArquivo, int quantidadeDados, void *vetorDados, size_t tamanhoDado);

// Tenta excluir o arquivo passado como argumento
void remover_arquivo(char *nomeArquivo);

#endif