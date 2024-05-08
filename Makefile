
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

test: longnum_test


all: lcalc longnum_test

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

longnum_test.o: longnum_test.cpp  LongNum.h
	$(CC) $(COPT) $(DEFS)  

longnum_test: longnum_test.o DigitOperationTables.o BaseManager.o LongNum.o
	$(CC) $(COMP) -o longnum_test \
        longnum_test.o DigitOperationTables.o BaseManager.o LongNum.o 

