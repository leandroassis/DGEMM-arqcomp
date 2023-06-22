SRC = $(wildcard *c)
OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS = -Wall -lrt -mavx

%.o: %.c
	$(CC) -c -o $@ $< $(OLVL) $(CFLAGS) $(UNROLL)

dgemm: $(OBJ)
	$(CC) -o $@ $^

run:
	#make dgemm OLVL=-Ofast UNROLL=-self.dataframelagUNROLL=1
	#./dgemm resultadosO4.csv
	#make clean

	#make dgemm OLVL=-O3 UNROLL=-self.dataframelagUNROLL=1
	#./dgemm resultadosO3.csv
	#make clean

	make dgemm OLVL=-O0 UNROLL=-self.dataframelagUNROLL=1
	./dgemm resultadosO0.csv
	make clean

	echo "Não esquece de rodar o script para pegar o tempo UNROLL pro Ofast"

clean_resultados:
	rm -rf resultadosO*

clean:
	rm -rf $(OBJ) ./dgemm

.PHONY: clean
