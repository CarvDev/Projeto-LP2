## Principal:

### função main():
```c
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
            ...
        case 2:
            // Gerenciar doutores
            ...
        case 3:
            // Gerenciar agendamentos
            ...
        case 4:
            // Sair do sistema
            ...
        default:
            // Caso de erro (opção inválida)
            ...
        }
    } while (opcao != 4);
    // Se a opção 4 for selecionada quebramos o laço

    return 0;
}
```

**Função principal do sistema de gerenciamento.**
Executa o loop principal do programa, exibindo o menu, obtendo a opção do usuário
através da função segura `obter_opcao()` e direcionando o fluxo conforme a escolha.
O programa permanece em execução até que a opção de sair (4) seja selecionada.

**Tratamento de opções inválidas com `default` no `switch`**:
Qualquer valor inteiro retornado por `obter_opcao()` que não corresponda
aos casos 1, 2, 3 ou 4 é capturado pelo `default`. Isso inclui números fora
da faixa esperada (ex.: 5, -1, 999). Uma mensagem clara é exibida e o loop
continua, permitindo que o usuário tente novamente sem interromper o programa.

## Interação com o usuaŕio:

### função imprimir_menu():

```c
void imprimir_menu() {
    printf(...);
}
```

Essa função servirá para imprimir o menu de opções

### função imprimir_cursor():

```c
void imprimir_cursor() { printf("> "); }
```

Essa função imprime o caractere '>' para servir de cursor no terminal

### função limpar_buffer():

```c
void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
```

Essa função consome todos os caracteres do buffer, até encontrar o \n, deixando o buffer limpo

### função obter_opcao():
```c
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
```

Essa função lê e retorna a opção escolhida pelo usuário, com as seguintes medidas de segurança:

1. **Validação da entrada**  
   A função utiliza o retorno de `scanf()` para verificar se a conversão para inteiro foi bem-sucedida. Quando o usuário digita caracteres não numéricos (como letras), `scanf` retorna `0`, indicando falha. Isso evita que valores indefinidos sejam armazenados em `opc` e que o programa prossiga com dados inválidos.

2. **Limpeza do buffer de entrada**  
   A função `limpar_buffer()` é chamada em duas situações:
   - Após uma entrada inválida, para descartar todos os caracteres que causaram a falha (incluindo a nova linha). Sem essa limpeza, esses caracteres permaneceriam no buffer e fariam a próxima leitura falhar novamente, gerando um loop infinito.
   - Após uma leitura bem-sucedida, para remover o caractere de nova linha (`\n`) deixado pelo usuário ao pressionar Enter. Isso evita que esse caractere residual interfira em futuras operações de entrada no programa.

3. **Loop até entrada válida**  
   O programa fica em um loop infinito até que uma entrada numérica seja fornecida. Isso garante que a função só retorne quando tiver um valor inteiro válido, prevenindo que o restante do código trabalhe com dados inconsistentes ou não inicializados.

4. **Tratamento de erro amigável**  
   Quando uma entrada inválida é detectada, uma mensagem clara é exibida (`[Erro: Digite apenas números]`), orientando o usuário a corrigir a entrada. A função `imprimir_cursor()` é chamada para restaurar o prompt, melhorando a usabilidade e a clareza da interação.

5. **Uso adequado do retorno de scanf**  
   Em vez de ignorar possíveis falhas de leitura, o código verifica explicitamente o retorno de `scanf()`. Isso está alinhado com as boas práticas de programação defensiva, pois a função `scanf` pode falhar por diversos motivos (entrada inválida, EOF, erro de leitura). Embora o código atual só trate o caso `retorno == 0`, ele já cobre o cenário mais comum de erro de formato.
