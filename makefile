
project: projecttrial.o pieces.o main.o
	cc projjecttrial.o pieces.o main.o -lncurses
projecttrial.o: projecttrial.c
	cc -c projecttrial.c
pieces.o: pieces.c
	cc -c pieces.c
main.o: main.c
	cc -c main.c
projecttrial.c: project.h
	cc -c projecttrial.c
pieces.h: project.h
	cc -c pieces.h
main.c: project.h
	cc -c main.c
