// Essa será uma função simples de imprimir o menu
// Posteriormente podemos adicionar uma ASCII art aqui
void imprimir_menu();

// Imprime o caracatere '>' para servir de cursor no terminal
void imprimir_cursor();

// Função de limpeza de buffer, portável entre qualquer
// sistema operacional. Lê caracteres do buffer até
// consumir o \n, deixando o buffer limpo
void limpar_buffer();

// Essa função lê e retorna a opção escolhida pelo usuário
// Possui medidas de segurança para verificar se o input é válido
// Se o input é inválido, ela permanece em loop, evitando continuação inadequada
// IMPORTANTE: ela sempre limpa o buffer após cada entrada
int obter_opcao();
