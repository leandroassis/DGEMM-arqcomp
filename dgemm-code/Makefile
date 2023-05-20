SRC = $(wildcard *c)
OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS = -Wall -lrt -mavx

%.o: %.c
	$(CC) -c -o $@ $< $(OLVL) $(CFLAGS) $(UNROLL) $(ADD_TEMPO)

dgemm: $(OBJ)
	$(CC) -o $@ $^

compare: $(OBJ)
	$(CC) -o $@ $^

run:
	#make dgemm OLVL=-Ofast UNROLL=-DflagUNROLL=1 ADD_TEMPO=-DTEMPO=1
	#./dgemm resultadosO4.csv
	#make clean

	make dgemm OLVL=-O3 UNROLL=-DflagUNROLL=1 ADD_TEMPO=-DTEMPO=1
	./dgemm resultadosO3.csv
	make clean

	#make dgemm OLVL=-O2 UNROLL=-DflagUNROLL=0 ADD_TEMPO=-DTEMPO=1
	#./dgemm resultadosO2.csv
	#make clean

	#make dgemm OLVL=-O1 UNROLL=-DflagUNROLL=0 ADD_TEMPO=-DTEMPO=1
	#./dgemm resultadosO1.csv
	#make clean

	make dgemm OLVL=-O0 UNROLL=-DflagUNROLL=0 ADD_TEMPO=-DTEMPO=1
	./dgemm resultadosO0.csv
	make clean

clean_resultados:
	rm -rf resultadosO*

clean:
	rm -rf $(OBJ) ./dgemm

.PHONY: clean
