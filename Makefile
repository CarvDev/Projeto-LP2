# O Makefile suporta build cross-platform (Windows e Unix-like).
# O nome do binário final recebe a extensão .exe no Windows ou nenhuma no Unix.
TARGET = hospital

# Detecção do ambiente em tempo de execução usando a variável de ambiente OS.
# O Windows define a variável de ambiente OS como Windows_NT.
ifeq ($(OS),Windows_NT)
	TARGET_EXT = .exe
	RM_CMD = del /Q /F
else
	TARGET_EXT =
	RM_CMD = rm -f
endif

CC = gcc

# Flags configuradas para ambiente de produção:
# -O3: Otimização máxima de performance.
# -Wall -Wextra: Mantém o rigor de verificação de código.
# -DNDEBUG: Desativa macros de assert() que não devem rodar em ambiente produtivo.
CFLAGS = -O3 -Wall -Wextra -DNDEBUG

# Mapeamento estático dos arquivos de código fonte essenciais para a compilação.
SRCS = main.c agendamento.c auxiliar.c medico.c paciente.c

# Substituição de sufixo para gerar a lista de arquivos objeto (.o) correspondentes.
OBJS = $(SRCS:.c=.o)

# Target padrão executado ao rodar apenas 'make'.
all: $(TARGET)$(TARGET_EXT)

# O target final compila o executável a partir dos objetos gerados.
# $@ = nome do target (hospital ou hospital.exe)
# $^ = lista de todos os pré-requisitos (todos os .o)
$(TARGET)$(TARGET_EXT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regra genérica de inferência para gerar cada arquivo objeto a partir do seu respectivo .c.
# $< = primeiro pré-requisito (o arquivo .c correspondente)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target utilitário para limpar artefatos de build (útil para pipelines e rebuilds).
clean:
	$(RM_CMD) *.o $(TARGET)$(TARGET_EXT)

# Declaração de targets 'phony' para evitar conflitos caso arquivos reais com esses nomes sejam criados no diretório.
.PHONY: all clean