# $Id: Makefile,v 1.6 2014-02-13 18:58:45-08 - - $

MKFILE    = Makefile

GCC       = gcc -g -O0 -Wall -Wextra -std=gnu99
SUBMIT	 = submit cmps012b-wm.w15 lab9

CSOURCE   = numsort.c inssort.c linesort.c
NUMSORT	 = inssort.o numsort.o
LINESORT  = linesort.o inssort.o
CHEADER   = inssort.h 
EXECBIN   = numsort linesort
SOURCES   = ${CHEADER} ${CSOURCE} ${MKFILE} README

all : ${EXECBIN}

numsort : ${NUMSORT}
	${GCC} -o $@ ${NUMSORT}

linesort : ${LINESORT}
	${GCC} -o $@ ${LINESORT} 

%.o : %.c
	cid + $<
	${GCC} -c $<

ci : ${SOURCES}
	cid + ${SOURCES} test*.data
submit : ${SOURCES}
		  ${SUBMIT} ${SOURCES}
