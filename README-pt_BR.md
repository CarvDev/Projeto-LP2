# C Hospital CRUD

Um sistema CRUD compacto e educacional para um cenário de gestão hospitalar, desenvolvido para a disciplina de "Linguagem de Programação II" do curso de Ciência da Computação da UESC.

English version 🇺🇸: [README.md](README.md).

![Screenshot](screenshot.png)

**Autores:**

* Arthur de Carvalho ([CarvDev](https://www.google.com/search?q=https://github.com/CarvDev))
* Rafael Mota ([rafaelmotafreitas](https://www.google.com/search?q=https://github.com/rafaelmotafreitas))

**Objetivo:**
Fornecer uma implementação clara e bem estruturada em C que demonstre habilidades essenciais de programação de sistemas: design modular, estruturas de dados dinâmicas, gerenciamento de memória e persistência baseada em arquivos. O projeto destina-se tanto a ser um exercício de aprendizado quanto uma demonstração da proficiência dos autores na linguagem C.

**Principais Funcionalidades:**
* Gerenciamento de pacientes: criar, visualizar, editar e remover registros.
* Gerenciamento de médicos: criar, visualizar, editar e remover registros.
* Agendamento de consultas: criar, visualizar, editar e remover agendamentos vinculando pacientes e médicos.
* Interface de usuário via console com código modularizado e separação clara entre a lógica de dados e a interface.

**Tecnologias e Habilidades Demonstradas:**
* Linguagem: C (ISO C, uso idiomático de ponteiros, structs e arrays).
* Build: `Makefile` com alvos (targets) simples para compilar e executar o programa usando `gcc`.
* Gerenciamento de memória: manipulação de memória de baixo nível com `memmove()` e alocação dinâmica de memória (`malloc`/`free`), com estratégia de duplicação e tratamento cuidadoso do ciclo de vida.
* Estruturas de dados: listas encadeadas customizadas e arrays dinâmicos para armazenar registros na memória.
* Recursão: implementação recursiva do algoritmo de Busca Binária.
* Argumentos de CLI: exportação/redefinição (reset) rápida de dados e menu de ajuda no terminal.
* Operações bitwise (bit a bit): conversão de caracteres minúsculos em maiúsculos sem o uso da função `toupper()`.
* Ponteiros: manipulação eficiente de dados, aritmética de ponteiros e funções genéricas utilizando o tipo de dado `void*`.
* Chamadas de sistema (System calls): função de criação de diretórios definida com compilação condicional para portabilidade tanto em sistemas Windows quanto POSIX.
* Gerenciamento via timestamps: para lidar de forma eficiente com operações orientadas ao tempo, como agendamentos.
* Tratamento de erros: limpeza do buffer de entrada, normalização de strings e segurança de memória.
* Entrada e Saída de Arquivos (File I/O): armazenamento persistente usando as APIs padrão de arquivos em C (`fopen`, `fread`, `fwrite`, `fclose`).
* Modularização: divisão entre cabeçalhos (headers) e códigos-fonte (`paciente.c/.h`, `medico.c/.h`, `agendamento.c/.h`, `auxiliar.c/.h`) para demonstrar o design de APIs em C.

**Estrutura do Repositório (alto nível):**
* `main.c` — ponto de entrada do programa e loop de menu
* `paciente.c` / `paciente.h` — lógica do CRUD de pacientes
* `medico.c` / `medico.h` — lógica do CRUD de médicos
* `agendamento.c` / `agendamento.h` — lógica de agendamentos vinculando pacientes e médicos
* `auxiliar.c` / `auxiliar.h` — utilitários auxiliares e funções de I/O
* `Makefile` — alvos de compilação (build) e execução
* `dados/` — diretório destinado aos arquivos de dados persistidos

**Compilação e Execução (Build & Run):**
1. Compilar (Build): execute `make` (requer `gcc`)
2. Executar: `./hospital`
3. Exibir menu de ajuda: `./hospital --help`
4. Redefinir dados (Reset): `./hospital --reset`
5. Exportar dados: `./hospital --export`

**Compatibilidade:**
O projeto foi desenvolvido para ser compatível com os sistemas operacionais Windows, GNU/Linux e macOS/BSD.

**Licença e Contribuição:**
Este projeto está licenciado sob a licença MIT, conforme declarado no arquivo `LICENSE`. No momento, não são esperadas contribuições e melhorias, pois este projeto é considerado finalizado.