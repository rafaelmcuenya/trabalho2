# Nome do executável
PROJ_NAME = ted
ALUNO = Rafael Matui Cuenya

# Compilador
CC = gcc

# Flags
CFLAGS = -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall -Wextra
LDFLAGS = -O0 -lm

OBJETOS = main.o leitor.o forma.o circulo.o retangulo.o linha.o texto.o \
          anteparo.o pintura.o ponto.o poligono.o geometria.o arvore.o \
          lista.o visibilidade.o sort.o criarTxt.o criarSvg.o \
          trataNomeArquivo.o

# Regra principal
$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(LDFLAGS) $(OBJETOS) -lm
	@echo "=== Compilação bem sucedida ==="

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Módulo principal
main.o: main.c leitor.h trataNomeArquivo.h sort.h visibilidade.h

# Módulo leitor
leitor.o: leitor.c leitor.h forma.h circulo.h retangulo.h linha.h texto.h \
          anteparo.h pintura.h strdupi.h criarTxt.h criarSvg.h \
          trataNomeArquivo.h lista.h visibilidade.h poligono.h ponto.h \
          arvore.h sort.h

# Módulo forma
forma.o: forma.c forma.h circulo.h retangulo.h linha.h texto.h anteparo.h strdupi.h

# Módulos de formas geométricas
circulo.o: circulo.c circulo.h strdupi.h anteparo.h
retangulo.o: retangulo.c retangulo.h strdupi.h
linha.o: linha.c linha.h strdupi.h
texto.o: texto.c texto.h strdupi.h

# Módulo anteparo
anteparo.o: anteparo.c anteparo.h strdupi.h

# Módulo pintura 
pintura.o: pintura.c pintura.h retangulo.h circulo.h texto.h linha.h anteparo.h lista.h

# Módulos geométricos
ponto.o: ponto.c ponto.h
poligono.o: poligono.c poligono.h lista.h ponto.h anteparo.h forma.h
geometria.o: geometria.c geometria.h ponto.h anteparo.h

# Módulos de estrutura de dados
arvore.o: arvore.c arvore.h
lista.o: lista.c lista.h

# Módulos de algoritmos
visibilidade.o: visibilidade.c visibilidade.h anteparo.h lista.h ponto.h arvore.h poligono.h
sort.o: sort.c sort.h

# Módulos de saída
criarTxt.o: criarTxt.c criarTxt.h forma.h anteparo.h visibilidade.h poligono.h
criarSvg.o: criarSvg.c criarSvg.h forma.h anteparo.h

# Módulo de tratamento de arquivos
trataNomeArquivo.o: trataNomeArquivo.c trataNomeArquivo.h strdupi.h

# Regras utilitárias
clean:
	rm -f $(OBJETOS) $(PROJ_NAME)
	@echo "=== Limpeza realizada ==="

valgrind: $(PROJ_NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(PROJ_NAME) -f teste.geo -o saida

.PHONY: clean valgrind
