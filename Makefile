
CC=g++
export WARNINGS=-Wall

ifndef CFLAGS
  CFLAGS=-g $(WARNINGS)
endif

ifdef OPT
  CFLAGS=-O3 $(WARNINGS)
endif


COPT=-c $< -o $@ $(CADD) $(CFLAGS)

clean: 
	rm -f *.o lcalc

all: lcalc

DigitOperationTables.o: DigitOperationTables.cpp DigitOperationTables.h
	$(CC) $(COPT) $(DEFS) 

BaseManager.o: BaseManager.cpp BaseManager.h DigitOperationTables.h
	$(CC) $(COPT) $(DEFS)  

LongNum.o: LongNum.cpp LongNum.h BaseManager.h DigitOperationTables.h
	$(CC) $(COPT) $(DEFS)  

TokenScanner.o: TokenScanner.cpp TokenScanner.h
	$(CC) $(COPT) $(DEFS)  

Evaluator.o: Evaluator.cpp Evaluator.h TokenScanner.h LongNum.h BaseManager.h
	$(CC) $(COPT) $(DEFS)  

lcalc.o: lcalc.cpp Evaluator.h TokenScanner.h LongNum.h BaseManager.h
	$(CC) $(COPT) $(DEFS)  

lcalc: lcalc.o DigitOperationTables.o BaseManager.o LongNum.o TokenScanner.o Evaluator.o
	$(CC) $(COMP) -o lcalc \
        lcalc.o DigitOperationTables.o BaseManager.o LongNum.o TokenScanner.o Evaluator.o
