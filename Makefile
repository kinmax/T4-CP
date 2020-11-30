MPICC = mpicc -o bubble_fases bubble_fases.c
LADCOMP = ladcomp -env mpicc -o bubble_fases bubble_fases.c
GCC = gcc -o bubble bubble.c

# targets:

all: Parser.class

run: Parser.class
        java Parser

build: clean Parser.class

clean:
        rm -f *~ *.class Yylex.java Parser.java y.output

Parser.class: TS_entry.java TabSimb.java Yylex.java Parser.java
        $(JAVAC) Parser.java

Yylex.java: exemploSem.flex
        $(JFLEX) exemploSem.flex

Parser.java: exemploSem.y
        $(BYACCJ) exemploSem.y