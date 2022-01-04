CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: pjC

pjC: server.o addPlace.o Login.o FuncConstant.o helper.o home.o
	${CC} server.o Login.o addPlace.o FuncConstant.o helper.o home.o -o pjC -I/usr/include/postgresql -lpq -std=c99


server.o: server.c
	${CC} ${CFLAGS} server.c -I/usr/include/postgresql -lpq -std=c99

addPlace.o: addPlace.c
	${CC} ${CFLAGS} addPlace.c -I/usr/include/postgresql -lpq -std=c99

Login.o: Login.c
	${CC} ${CFLAGS} Login.c -I/usr/include/postgresql -lpq -std=c99

FuncConstant.o: FuncConstant.c
	${CC} ${CFLAGS} FuncConstant.c -I/usr/include/postgresql -lpq -std=c99

helper.o: helper.c
	${CC} ${CFLAGS} helper.c -I/usr/include/postgresql -lpq -std=c99

home.o: home.c
	${CC} ${CFLAGS} home.c -I/usr/include/postgresql -lpq -std=c99

clean:
	rm -f *.o *~